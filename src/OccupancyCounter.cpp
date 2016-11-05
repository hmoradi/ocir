/**@file OccupancyCounter.cpp
 *
 * @date July 3, 2016
 * @author Hessam Mohammadmoradi <hmoradi@cs.uh.edu>
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <signal.h>
#include "ftdi.h"
#include <time.h>
#include <string.h>
#include <atomic>  
#include <opencv2/opencv.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include "params.h"
#include <chrono>
#include <thread>
#include <opencv2/core/core.hpp>        
#include <opencv2/highgui/highgui.hpp>  
#include "OccupancyCounter.h"
#include <queue>
#include "opencv2/imgproc/imgproc.hpp"

void OccupancyCounter::log_(string msg){
  if(PRINT_DEBUG_MSG)
    cout << msg << endl;
}

bool OccupancyCounter::fft_(Mat im, Mat raw_im, int frameN, string window){
    Mat padded; 
    Mat I;
    I = raw_im.clone();        
                       //expand input image to optimal size
    int m = getOptimalDFTSize( I.rows );
    int n = getOptimalDFTSize( I.cols ); // on the border add zero values
    copyMakeBorder(I, padded, 0, m - I.rows, 0, n - I.cols, BORDER_CONSTANT, Scalar::all(0));

    Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
    Mat complexI;
    merge(planes, 2, complexI);         // Add to the expanded another plane with zeros

    dft(complexI, complexI);            // this way the result may fit in the source matrix
    split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
    magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude
    Mat magI = planes[0];

    magI += Scalar::all(1);                    // switch to log_arithmic scale
    log(magI, magI);

    // crop the spectrum, if it has an odd number of rows or columns
    magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));

    // rearrange the quadrants of Fourier image  so that the origin is at the image center
    int cx = magI.cols/2;
    int cy = magI.rows/2;

    Mat q0(magI, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
    Mat q1(magI, Rect(cx, 0, cx, cy));  // Top-Right
    Mat q2(magI, Rect(0, cy, cx, cy));  // Bottom-Left
    Mat q3(magI, Rect(cx, cy, cx, cy)); // Bottom-Right

    Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
    q2.copyTo(q1);
    tmp.copyTo(q2);
    Mat scaled_magI;
    Mat filtered;
    double minx,maxx;
    minMaxIdx(magI,&minx,&maxx);
    float thresh = 0.3* maxx ;
    threshold( magI, filtered, thresh, 1,THRESH_TOZERO );   

    int nonzero = countNonZero(filtered);
    log_("None zeros : "+ to_string(nonzero));
    if (nonzero <= 35)
      return false;
   //convertScaleAbs(filtered,scaled_magI,255);
   // Mat resized = resize_frame(scaled_magI,256);
   // namedWindow(window, CV_WINDOW_AUTOSIZE );
   // imshow(window, resized );
    return true;
}

bool OccupancyCounter::check_by_histogram(Mat diffs, string window, int frameN,int people_size){

  int histSize = 50;
  float range[] = { -10, 40 } ;
  const float* histRange = { range };
  bool uniform = true; bool accumulate = false;
  Mat hist_result;
  /// Compute the histograms:
  calcHist( &diffs, 1, 0, Mat(), hist_result, 1, &histSize, &histRange, uniform, accumulate );
  Mat hist_show = hist_result.clone();
  if(SHOW_HIST){
    // Draw the histograms for B, G and R
    int hist_w = 256; int hist_h = 256;
    int bin_w = cvRound( (double) hist_w/histSize );

    Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );
    for (int i = 0;i<histSize;i++){
      hist_show.at<float>(i)= (hist_show.at<float>(i) * histImage.rows) / 32.0; 
    }
    /// Normalize the result to [ 0, histImage.rows ]
    //normalize(hist_result, hist_result, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    /// Draw for each channel
    for( int i = 1; i < histSize; i++ )
    {
         
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(hist_show.at<float>(i-1)) ) ,
                         Point( bin_w*(i), hist_h - cvRound(hist_show.at<float>(i)) ),
                         Scalar( 255, 0, 0), 2, 8, 0  );
    }

    namedWindow(window, CV_WINDOW_AUTOSIZE );
    imshow(window, histImage );
    if(SAVE_IMAGE)
      imwrite("./imgs/hist/"+to_string(frameN)+".png",histImage);
    //waitKey(100);
  }
  vector<Pulse> peaks ;
  int i = 0;
  float val;
  float global_max_amplitude = -10;
  
  while(i < histSize)
  {
    //continue till see the first non-zero value 
    do{
      val = hist_result.at<float>(i);
      if(val != 0)
        break;
      else
        i++;
    }while( i < histSize);

    if(i == histSize) break;

    //now we have the starting nonzero number

    Pulse ps;
    ps.start = i;
    float max_amplitude = -10;
    do{
      val = hist_result.at<float>(i);
      if(val > max_amplitude)
          max_amplitude = val;
      if(val == 0)
        break;
      else
        i++;
    }while( i < histSize);
    ps.end = i -1;
    ps.amp = max_amplitude;
    peaks.push_back(ps);

    if(global_max_amplitude < max_amplitude)
      global_max_amplitude = max_amplitude;
  }

  int status = 0;
  val = 0;
  int inc_dec_pos = -1;
  int dec_inc_pos = -1;
  float pre_val = hist_result.at<float>(0);
  int last_change_pos = 0;
  float max_local_amp = -1;
  float new_max_global_amp = 0;
  vector<Pulse> new_peaks;
  for(int i =1;i<histSize;i++){
    
    val = hist_result.at<float>(i);
    if(val > pre_val){
      if(status == 0)
        last_change_pos = i;
      else if(status == -1){
        Pulse ps;
        ps.start = last_change_pos;
        ps.end = i - 1;
        ps.amp = max_local_amp;
        new_peaks.push_back(ps);
        last_change_pos = i;
        max_local_amp = -1;
      }
      status = 1;
    }else if(val < pre_val){
      if(status == 0)
        last_change_pos = i;
      
      status = -1;
    }
    if(val > max_local_amp)
      max_local_amp = val;
    if(max_local_amp > new_max_global_amp)
      new_max_global_amp = max_local_amp;
    pre_val = val;
  }
  if(status == -1){
    Pulse ps;
    ps.start = last_change_pos;
    ps.end = i;
    ps.amp = max_local_amp;
    new_peaks.push_back(ps);
  }
  
  int total_spikes = 0;
  int distribution_width = 0;
  int max_pulse_index = -1;
  for(int i=0;i<peaks.size() ;i++){
    if(peaks.at(i).amp == global_max_amplitude){
      max_pulse_index = i;
      total_spikes = peaks.size() - i - 1;
      int len = peaks.size() - 1;
      distribution_width = peaks.at(len).end - peaks.at(i).end - 1;
      break;
    }
  }

  int new_total_spikes = 0;
  int new_distribution_width = 0;
  int new_max_pulse_index = -1;
  for(int i=0;i<new_peaks.size() ;i++){
    if(new_peaks.at(i).amp == new_max_global_amp){
      new_max_pulse_index = i;
      new_total_spikes = peaks.size() - i - 1;
      int len = new_peaks.size() - 1;
      new_distribution_width = new_peaks.at(len).end - new_peaks.at(i).end - 1;
      break;
    }
  }
  cout << "peaks size " << peaks.size() <<  " new peaks size " << new_peaks.size() << endl;
  //if the overall amplitude is < 50% of the amplitude computed below, it is a human
    //Imagine you are using 250th frame now; subtract background from this; compute histogram of the diff; compute the amplitude of the large distribution (noise).  
    //look at the amplitude of the current distribution

  //if there is **just** one extra spike after the large distribution
    //if the amplitude <= 2, and width < 2, it is a noise

  int offset = START_FRAME;
  if(LIVE)
    offset = 0;
  if(frameN < BACKGROUND_FRAMES+offset){
    if(background_amp < global_max_amplitude and global_max_amplitude != 64)
      background_amp = global_max_amplitude;
    return false; 
  }
  
  bool hist_passed = false;
  cout << "current amp  " << global_max_amplitude << " background_ amp " << background_amp << endl;
  int second_max_pulse = -1;
  if(new_distribution_width > 0){
    for(int i=new_max_pulse_index+1;i<new_peaks.size() ;i++){
      if(new_peaks.at(i).amp >= second_max_pulse){
        second_max_pulse = new_peaks.at(i).amp;
      }
    }
    if(second_max_pulse >= 2)
      hist_passed = true;
    else
      cout << "second max pulse " << second_max_pulse  << endl;
  }else if(new_max_global_amp <= (0.30*background_amp)){
    hist_passed = true;
  }

  peaks.clear();
  
  return hist_passed;
}
/*
*  OccupancyCounter Constructor
*/
OccupancyCounter::OccupancyCounter( ){
  people_inside = 0;
  total_people = 0;
}

/*
*   Utility: Convert 2 dimensional array to OpenCv Mat 
*   @Input 1: Pointer to 2 Dimensional Matrix
*/
Mat OccupancyCounter::convert_to_Mat(int** frame){
    Mat image;
    if(SENSOR==0){
      image = Mat::zeros(GRIDEYE_RAW_ROWS,GRIDEYE_RAW_COLS,CV_8UC1);
      for(int i=0;i<GRIDEYE_RAW_ROWS;i++){
        for (int j=0;j<GRIDEYE_RAW_COLS;j++){ 

          image.at<uchar>(j,i) = frame[i][j];
          //cout << i << " :" << j << ":" <<frame[i][j] << ":" << (int)image.at<uchar>(i,j) << endl;
        }
      } 
      //delete[] frame;
      return image;
    }else{
      image = Mat::zeros(MLX_RAW_ROWS,MLX_RAW_COLS,CV_8UC1);
      for(int i=0;i<MLX_RAW_ROWS;i++)
      for (int j=0;j<MLX_RAW_COLS;j++)
        image.at<uchar>(j,i) = frame[i][j]; 
      //delete[] frame;
      return image;
   }   
}

/*
*  Utility: Resize Mat to Desired Size Using CUBIC Interpolation
*  @Input 1: Input Mat
*/
Mat OccupancyCounter::resize_frame(Mat im , int width,int height){
  Size size(width,height);
  Mat result = Mat::zeros(width,height,CV_8UC1);
  resize(im,result,size,INTER_LINEAR);
  return result;
}

/*
*  Utility: Display Mat 
*  Input 1: Mat to display
*  Input 2: Video stream to save images as video
*  Input 3: Frame Number
*  Input 4: Boolean indicates to save image as separate image
*/

void OccupancyCounter::show_image(Mat im,int frameN, bool save, Mat diffs){
 if(!SHOW_IMAGE)
   return;
  Mat flipped = Mat::zeros(im.cols,im.rows,CV_8UC1);;
  for(int i = 0;i<im.cols;i++){
    for(int j=0;j<im.rows;j++){
      flipped.at<uchar>(j,i) = im.at<uchar>(i,j);
    }
  }
  Mat flipped_diffs = Mat::zeros(diffs.cols,diffs.rows,CV_8UC1);;
  for(int i = 0;i<diffs.cols;i++){
    for(int j=0;j < diffs.rows;j++){
      flipped_diffs.at<uchar>(j,i) = diffs.at<uchar>(i,j);
    }
  }
  if(SENSOR==0)
    im = resize_frame(flipped,GRIDEYE_WIDTH,GRIDEYE_HEIGHT);
  else
    im = resize_frame(flipped,MLX_WIDTH,MLX_HEIGHT);
  int xpos_offset = -5;
  if(people_inside < 0)
    xpos_offset = -15;
  Point textOrg;
  if(SENSOR==0)
    textOrg = Point(GRIDEYE_WIDTH/2 + xpos_offset ,GRIDEYE_HEIGHT/8 + 10);
  else
    textOrg = Point(MLX_WIDTH/2 + xpos_offset ,MLX_HEIGHT/8 + 10);
  Mat im_color;
  Mat display;
  Mat scaled_image;

  if(SENSOR==0)
    convertScaleAbs(im,scaled_image,255 / 120);
  else
    convertScaleAbs(im,scaled_image,255 / 40);
  applyColorMap(scaled_image,im_color,COLORMAP_HOT);
  flip(im_color,display,0);
  put_text(display,textOrg,to_string(people_inside) );
  namedWindow("frame",CV_WINDOW_AUTOSIZE);
  
  if(save){
    imwrite("./imgs/debug/"+to_string(frameN)+".png",display);
  }
  if(SHOW_DIFF){
    Mat im_color_diff;
    Mat display_diff;
    Mat scaled_image_diff; 
    
    if(SENSOR == 0){
      diffs = resize_frame(flipped_diffs,GRIDEYE_WIDTH,GRIDEYE_HEIGHT);
      convertScaleAbs(diffs,scaled_image_diff,255 / 40);
    }else{
      diffs = resize_frame(flipped_diffs,MLX_WIDTH,MLX_HEIGHT); 
      convertScaleAbs(diffs,scaled_image_diff,255 / 4);
    }
    applyColorMap(scaled_image_diff,im_color_diff,COLORMAP_HOT);
    flip(im_color_diff,display_diff,0); 
    namedWindow("differences",CV_WINDOW_AUTOSIZE);
    imshow("frame",display);
    imshow("differences",display_diff); 
  }else{
      imshow("frame",display);
  }
  waitKey(WAIT_TIME);
}

/*
*  Quantify crosspondence of person to body 
*  Input 1: Person
*  Input 2: Body
*  Input 3: frame Number
*/
bool OccupancyCounter::is_match(Person person_ , Body body, int frameN){

  if(person_.direction_ == UNKNOWNDIR){
    // if(person_.trajectory.begin()->second.side == LEFTSIDE and body.side == RIGHTSIDE){
    //   return true;
    // }
    // if(person_.trajectory.begin()->second.side == RIGHTSIDE and body.side == LEFTSIDE){
    //   return true;
    // }
    return true;
  }else if(person_.direction_ == LtoR){
    if(body.side == RIGHTSIDE)
      return true;
  }else if(person_.direction_ == RtoL){
    if(body.side == LEFTSIDE)
      return true;
  }
  //Look at the direction of the person..this means save his direction based on last matching

  //if the direction is unknown,
      //if the person starts from left, current side is right
        //match
      //if the person starts from right, current side is left
        //match
  //if the direction is known: Left to right
    //if the body is right
      //match
    //if the body is at left
      //new person
  //if the direction is known: Right to left
    //if the body is left
      //match
    //if the body is right
      //new person

  //after doing the matching
      //update direction
        //if the person starts from left, current side is right
          //update Left to Right
        //if the person starts from right, current side is left
          //update right to left
        //ignore current frame if the side can't be determined
  return false;  
}

/*
*  Check persons trajectory to see if it a valid walk
*  Input: Person
*  Return : True if person walks a valid walk, return false otherwise
*/
bool OccupancyCounter::check_trajectory(Person& person_){
	//direction dir = find_direction(person_);
  direction dir = person_.direction_;
  if((dir == LtoR) and (person_.counted_ != LR)){
    person_.counted_ = LR;
    log_("previous count " + to_string(people_inside) );
    log_("person id is " + to_string(person_.static_id) );
    log_("left to right" );
    if(OPOSITE_DIRECTION)
     people_inside ++;
   else
     people_inside --;
   log_("current count " + to_string(people_inside) );
   return true;
  }
  else if((dir == RtoL) and (person_.counted_ != RL)){
    person_.counted_ = RL;
    log_("previous count " + to_string(people_inside) );
    log_("person id is " + to_string(person_.static_id) );
    log_("right to left ");
    if(OPOSITE_DIRECTION)
      people_inside --;
    else
      people_inside ++;
    log_("current count " + to_string(people_inside) );
    return true;
  }else{
    log_("rejecting person "+to_string(person_.static_id)+" dir is "+to_string(person_.direction_));
  }
  return false;
}

/*
*  at the end of each frames processing, update status of current people
*  Input 1: Vector of Curretnly Detected Persons
*  Input 2: Frame Number
*  Return: True if this frame shoude be saved, return false otherwise
*/
bool OccupancyCounter::update_people_status(vector<Person>& people, int frameN){
  bool save = false;
  if(people.empty() == false) {
    for(int i = people.size() - 1; i >= 0; i--) {
      if(people.at(i).status_ == LEFT) {
          people.erase( people.begin() + i ); 
      }else{
        if(people.at(i).trajectory.size() >= TRAJECTORY_LENGTH ){
          log_("size thresh passed " );
          if(check_trajectory(people.at(i)))
            save = true;
          
          people.at(i).trajectory.erase(people.at(i).trajectory.begin(), --(people.at(i).trajectory.end()));
        }else if(frameN - people.at(i).trajectory.rbegin()->first - 5 >= DETECTION_DELAY){
          log_("time thresh passed " + to_string(frameN) + " " + to_string(people.at(i).trajectory.rbegin()->first )); 
          if(check_trajectory(people.at(i)))
            save = true;
          people.at(i).status_ = LEFT;
          people.erase( people.begin() + i );
        }else if(people.size() > 2){
          log_("people size big " + to_string(frameN) + " " + to_string(people.at(i).trajectory.rbegin()->first )); 
          if(check_trajectory(people.at(i))){
            save = true;
            people.at(i).status_ = LEFT;
            people.erase( people.begin() + i );
          }
        }
      }
    }
  }
  return save;
}

direction OccupancyCounter::find_direction(Person person_){
  
  if(person_.trajectory.begin()->second.side == LEFTSIDE and  person_.trajectory.rbegin()->second.side == RIGHTSIDE )
    return LtoR;
  else if(person_.trajectory.begin()->second.side == RIGHTSIDE and  person_.trajectory.rbegin()->second.side == LEFTSIDE)
    return RtoL;
 
  int start_frame = person_.trajectory.begin()->first % HISTORY_LENGTH;
  int end_frame = person_.trajectory.rbegin()->first % HISTORY_LENGTH;
  int start_side = UNKNOWNSIDE;
  int end_side = UNKNOWNSIDE;
  int index = 1;
  int pre_index = 0;
  int post_index = 0;
  while(index <= 4 and (start_side == 2 or end_side == 2)){
    pre_index = ((start_frame - index)+HISTORY_LENGTH) % HISTORY_LENGTH;
    post_index = (end_frame+index) % HISTORY_LENGTH; 
    if((start_side == 2) and frames[pre_index].valid and frames[pre_index].side != 2)
      start_side = frames[pre_index].side;
    if((end_side == 2) and frames[post_index].valid and frames[post_index].side != 2)
      end_side = frames[post_index].side;
    index++;
  }
  
  if(start_side == LEFTSIDE and end_side == RIGHTSIDE)
    return LtoR;
  else if(start_side == RIGHTSIDE and end_side == LEFTSIDE)
    return RtoL;
  
  return UNKNOWNDIR;
}

void OccupancyCounter::create_new_person(Body body_,vector<Person>& people,int frameN){
    if(body_.side == UNKNOWNSIDE){
      int count =0;
      int index;
      int frame_index = frameN % HISTORY_LENGTH;
      while(count <= 10){
        count++;
        index = (frame_index - count + HISTORY_LENGTH) % HISTORY_LENGTH;
        if(frames[index].valid == true and frames[index].side != UNKNOWNSIDE){
          body_.side = frames[index].side;
          break;
        }
      }
    }
    if(body_.side == UNKNOWNSIDE)
      return;
    Person new_one ;
    new_one.trajectory[frameN]= body_;
    
    new_one.status_ = UNKNOWN;          //It can be UNKNOWN, ENTERED, LEFT
    new_one.direction_ = UNKNOWNDIR;   //It can be JUSTENERED, LEFT_TO_RIGHT(LtoR), RIGHT_TO_LEFT (RtoL), UNKNOWNDIR
    new_one.counted_ = NOTYET;

    new_one.static_id = total_people;
    total_people++;
    
    people.push_back(new_one);
    log_("new person created  with id " + to_string(new_one.static_id)  +  " with body " + body_.getID() );
    return;
}
bool OccupancyCounter::correct_frame(Frame frame){
  int index = frame.frameN % HISTORY_LENGTH;
 
  if(frames[index].valid == false)
    return false;
  
  int previous = -1;
  int count = 0;
  while(!frames[(index+previous+HISTORY_LENGTH)%HISTORY_LENGTH].valid and count < WINDOW_SIZE){
    previous--;
    count++;
  }
  int next = 1;
  
  //if current frame's number of people is same as the next frame's number of people, then we are good!
  //otherwise, if number of people in the previous frame is the same as the next frame, then current frame is invalid

  int prev_index = (index+previous+HISTORY_LENGTH)%HISTORY_LENGTH;
  int next_index = (index+next)%HISTORY_LENGTH;
  if(frames[index].found_bodies.size() != frames[next_index].found_bodies.size()){
    if(frames[prev_index].found_bodies.size() == frames[next_index].found_bodies.size() ){
        frames[index].valid = false;
        log_("invalid frame: " + to_string(frame.frameN));
        return false;
    }
  }
  return true;
}
void OccupancyCounter::match_body_person(vector<Person>& people, int frameN){
  log_("matching at frame " + to_string((frameN-5+HISTORY_LENGTH)%HISTORY_LENGTH));
  Frame frame_ = frames[(frameN-5+HISTORY_LENGTH)%HISTORY_LENGTH];
  
  if(!correct_frame(frame_)) //current frame is invalid
    return; 
  
  if(frame_.found_bodies.size()==0)
    return;
  
  if(frame_.found_bodies.size()==1){
    if(people.size() == 0){   //people contains people in the scene (computed from the past frames); we are trying to match them with the bodies at the current frame
      log_("case 1 - creating new person ");
      create_new_person(frame_.found_bodies.at(0),people,frame_.frameN);
    }
    else if(people.size()==1){
      if(is_match(people.at(0),frame_.found_bodies.at(0),frameN)){
        people.at(0).trajectory[frame_.frameN] = frame_.found_bodies.at(0);
        log_("Person "+to_string(people.at(0).static_id)+" matched to body "+(frame_.found_bodies.at(0).getID()));
      }else{
        if(frame_.found_bodies.at(0).side != UNKNOWNSIDE)
          log_("case 2 - creating new person ");
          create_new_person(frame_.found_bodies.at(0),people,frame_.frameN);
      }
    }
    else if(people.size()==2){

      //we have two people (in past frames) and one body (in this frame)

      //if the directions of two people are unknown, match with the same side person
      //if we know the direction of one person (P1)
        //if current body is in the same direction -> match
        //if not -> do not match
      //if we know the direction of both people
        //match with anyone with the same direction
      Person& p1 = people.at(0);
      Person& p2 = people.at(1);
      Body& b = frame_.found_bodies.at(0);
      Body& first_seen_body_p1 = p1.trajectory.begin()->second;
      Body& first_seen_body_p2 = p2.trajectory.begin()->second;
      if(p1.direction_ == UNKNOWNDIR and p2.direction_ == UNKNOWNDIR){
        
        if(first_seen_body_p1.side != b.side){
          p1.trajectory[frameN] = b;
          log_("Person " + to_string(p1.static_id)+" matched with body "+ b.getID());
        }
        else if(first_seen_body_p2.side != b.side ){
          p2.trajectory[frameN] = b;
          log_("Person " + to_string(p2.static_id)+" matched with body "+ b.getID());
        }

      }else if(p1.direction_ != UNKNOWNDIR and p2.direction_ != UNKNOWNDIR){
          
          if(p1.direction_ == RtoL and b.side == LEFTSIDE){
            p1.trajectory[frameN] = b;
            log_("Person " + to_string(p1.static_id)+" matched with body "+ b.getID());
          }
          else if(p2.direction_ == RtoL and b.side == LEFTSIDE){
            p2.trajectory[frameN] = b;
            log_("Person " + to_string(p2.static_id)+" matched with body "+ b.getID());
          }
          else if(p1.direction_ == LtoR and b.side == RIGHTSIDE){
            p1.trajectory[frameN] = b;
            log_("Person " + to_string(p1.static_id)+" matched with body "+ b.getID());
          }
          else if(p2.direction_ == LtoR and b.side == RIGHTSIDE){
            p2.trajectory[frameN] = b;
            log_("Person " + to_string(p2.static_id)+" matched with body "+ b.getID());
          }
          else{
            log_("case 3 - creating new person ");
            create_new_person(b,people,frameN);
          }
      }else{
          if(p1.direction_ == UNKNOWNDIR){
            p1.trajectory[frameN] = b;
            log_("Person 1" + to_string(p1.static_id)+" matched with body "+ b.getID());
          }else if(p2.direction_ == UNKNOWNDIR){
            p2.trajectory[frameN] = b;
            log_("Person " + to_string(p2.static_id)+" matched with body "+ b.getID());
          }
      }
      
    }else{
      cout << "people size 3 "<< endl;
    }
  }else if(frame_.found_bodies.size()==2){
    if(people.size() ==0){
      log_("case 4 - creating new person ");
      create_new_person(frame_.found_bodies.at(0),people,frame_.frameN);
      create_new_person(frame_.found_bodies.at(1),people,frame_.frameN);
    }
    if(people.size()==1){
      //we have one people, but two bodies (in this frame)
      //if the direction is unknown, match with the opposite side person
      //if the direction is known, match with the same direction person
      Person& p = people.at(0);
      Body& b1 = frame_.found_bodies.at(0);
      Body& b2 = frame_.found_bodies.at(1);
      Body& first_seen_body_p = p.trajectory.begin()->second;
      if(p.direction_ == UNKNOWNDIR){
        if(b1.side == LEFTSIDE and first_seen_body_p.side == RIGHTSIDE){
          p.trajectory[frameN] = b1;
          log_("Person " + to_string(p.static_id)+" matched with body "+ b1.getID());
          create_new_person(b2,people,frameN);
        }
        else if(b1.side == RIGHTSIDE and first_seen_body_p.side == LEFTSIDE){
          p.trajectory[frameN] = b1;
          log_("Person " + to_string(p.static_id)+" matched with body "+ b1.getID());
          create_new_person(b2,people,frameN);
        }
        else if(b2.side == LEFTSIDE and first_seen_body_p.side == RIGHTSIDE){
          p.trajectory[frameN] = b2;
          log_("Person " + to_string(p.static_id)+" matched with body "+ b2.getID());
          create_new_person(b1,people,frameN);
        }
        else if(b2.side == RIGHTSIDE and first_seen_body_p.side == LEFTSIDE){
          p.trajectory[frameN] = b2;
          log_("Person " + to_string(p.static_id)+" matched with body "+ b2.getID());
          create_new_person(b1,people,frameN);
        }
      }else{
        if(p.direction_ == LtoR){
          if(b2.side == LEFTSIDE){
            p.trajectory[frameN] = b1;
            log_("case 5 creating new person ");
            create_new_person(b2,people,frameN);
          }else if(b1.side == LEFTSIDE){
            p.trajectory[frameN] = b2;
             log_("c6-Person " + to_string(p.static_id)+" matched with body "+ b2.getID());
            log_("case 6 creating new person ");
            create_new_person(b1,people,frameN);
             
          }
        }
        else if(p.direction_ == RtoL){
          if(b2.side == RIGHTSIDE){
            p.trajectory[frameN] = b1;
            log_("case 7 creating new person");
            create_new_person(b2,people,frameN);
          }else if(b1.side == RIGHTSIDE){
            p.trajectory[frameN] = b2;
            log_("case 8 creating new person");
            create_new_person(b1,people,frameN);
          }
        }
      }
    }else if(people.size()==2){
      Person& p1 = people.at(0);
      Person& p2 = people.at(1);
      Body& b1 = frame_.found_bodies.at(0);
      Body& b2 = frame_.found_bodies.at(1);
      Body& first_seen_body_p1 = p1.trajectory.begin()->second;
      Body& first_seen_body_p2 = p2.trajectory.begin()->second;
      int b1_p1_dist = b1.cx - p1.trajectory.rbegin()->second.cx; 
      int b1_p2_dist = b1.cx - p2.trajectory.rbegin()->second.cx; 
      int b2_p1_dist = b2.cx - p1.trajectory.rbegin()->second.cx; 
      int b2_p2_dist = b2.cx - p2.trajectory.rbegin()->second.cx; 
      if(p1.direction_ == UNKNOWNDIR and p2.direction_ == UNKNOWNDIR){
        if(b1_p1_dist <= b2_p1_dist){
            p1.trajectory[frameN] = b1;
            p2.trajectory[frameN] = b2;
            log_("2211-Person " + to_string(p1.static_id)+" matched with body "+ b1.getID());
        }else {
            p1.trajectory[frameN] = b2;
            p2.trajectory[frameN] = b1;
            log_("2212-Person " + to_string(p1.static_id)+" matched with body "+ b1.getID());
        }
        // if(b1.side != b2.side){
        //   if(first_seen_body_p1.side == LEFTSIDE and b1.side == RIGHTSIDE){
        //     p1.trajectory[frameN] = b1;
        //     p2.trajectory[frameN] = b2;
        //     log_("221-Person " + to_string(p1.static_id)+" matched with body "+ b1.getID());
        //   }
        //   else if(first_seen_body_p1.side == RIGHTSIDE and b2.side == LEFTSIDE){
        //     p1.trajectory[frameN] = b2;
        //     p2.trajectory[frameN] = b1;
        //     log_("222-Person " + to_string(p1.static_id)+" matched with body "+ b2.getID());
        //   }
        //   else if(first_seen_body_p1.side == LEFTSIDE and b2.side == RIGHTSIDE){
        //     p1.trajectory[frameN] = b2;
        //     p2.trajectory[frameN] = b1;
        //     log_("223-Person " + to_string(p1.static_id)+" matched with body "+ b2.getID());
        //   }
        //   else if(first_seen_body_p1.side == RIGHTSIDE and b1.side == LEFTSIDE){
        //     p1.trajectory[frameN] = b1;
        //     p2.trajectory[frameN] = b2;
        //     log_("224-Person " + to_string(p1.static_id)+" matched with body "+ b1.getID());
        //   }

        // }else{
        //     // There are 2 persons in the scene with unknown direction, and found 2 bodies in one side, just assign randomly
        //     p1.trajectory[frameN] = b1;
        //     log_("225-Person " + to_string(p1.static_id)+" matched with body "+ b1.getID());
        //     p2.trajectory[frameN] = b2;
        //     log_("226-Person " + to_string(p2.static_id)+" matched with body "+ b2.getID());
        // }
      }
      else if(p1.direction_ != UNKNOWNDIR and p2.direction_ != UNKNOWNDIR){
        if(p1.direction_ == LtoR and p2.direction_ == LtoR){
          if(b1.side == LEFTSIDE){
            create_new_person(b1,people,frameN);
            log_("case 9 creating new person");
          }
          if(b2.side == LEFTSIDE){
            create_new_person(b2,people,frameN);
            log_("case 10 creating new person");
          }
        }
        if(p1.direction_ == RtoL and p2.direction_ == RtoL){
          if(b1.side == RIGHTSIDE){
            create_new_person(b1,people,frameN);
            log_("case 11 creating new person");
          }
          if(b2.side == RIGHTSIDE){
            create_new_person(b2,people,frameN);
            log_("case 12 creating new person");
          }
        }
      }else{
         if(b1_p1_dist <= b2_p1_dist){
            p1.trajectory[frameN] = b1;
            p2.trajectory[frameN] = b2;
            log_("2213-Person " + to_string(p1.static_id)+" matched with body "+ b1.getID());
            log_("2213-Person " + to_string(p2.static_id)+" matched with body "+ b2.getID());
          }else {
            p1.trajectory[frameN] = b2;
            p2.trajectory[frameN] = b1;
            log_("2214-Person " + to_string(p1.static_id)+" matched with body "+ b2.getID());
            log_("2214-Person " + to_string(p2.static_id)+" matched with body "+ b1.getID());
          }
          // if(p1.direction_ == UNKNOWNDIR){
          //   if(first_seen_body_p1.side != b1.side){
          //     p1.trajectory[frameN] = b1;

          //     log_("227-Person " + to_string(p1.static_id)+" matched with body "+ b1.getID());
          //   }
          //   else if (first_seen_body_p1.side != b2.side){
          //     p1.trajectory[frameN] = b2;
          //     log_("228-Person " + to_string(p1.static_id)+" matched with body "+ b2.getID());
          //   }
          // }
          // else if(p2.direction_ == UNKNOWNDIR){
          //   if(first_seen_body_p2.side != b1.side){
          //     p2.trajectory[frameN] = b1;
          //     log_("229-Person " + to_string(p2.static_id)+" matched with body "+ b1.getID());
          //   }
          //   else if (first_seen_body_p2.side != b2.side){
          //     p2.trajectory[frameN] = b2;
          //     log_("2210Person " + to_string(p2.static_id)+" matched with body "+ b2.getID());
          //   }
          // }
        //}
      }
    }
  }
  update_people_direction(people);
  return;
}
void OccupancyCounter::update_people_direction(vector<Person>& people){
  for(int i=0;i<people.size();i++){
    log_("updating direction of person " + to_string(people.at(i).direction_));
    if(people.at(i).direction_ == UNKNOWNDIR){ 
      if(people.at(i).trajectory.begin()->second.side == LEFTSIDE and people.at(i).trajectory.rbegin()->second.side == RIGHTSIDE){ //trajectory is a map between frame number and bodies...so, ->second means the body
          people.at(i).direction_ = LtoR;
          cout << " LtoR : start at " << people.at(i).trajectory.begin()->second.cx << " end at " << people.at(i).trajectory.rbegin()->second.cx << endl;
      }
      else if(people.at(i).trajectory.begin()->second.side == RIGHTSIDE and people.at(i).trajectory.rbegin()->second.side == LEFTSIDE){
          people.at(i).direction_ = RtoL;
          cout << "R to L: start at " << people.at(i).trajectory.begin()->second.cx << " end at " << people.at(i).trajectory.rbegin()->second.cx << endl;
      }
    }
  }
}
/*
*  Utility: Print string on specifed location
*  Input 1: Original Img
*  Input 2: Position Point
*  Input 3: Desired String
*/
void OccupancyCounter::put_text(Mat& Img,Point textOrg, string txt){
  int fontFace = CV_FONT_HERSHEY_COMPLEX_SMALL;
  double fontScale = 1;
  int thickness = 1; 
  putText(Img,txt, textOrg, fontFace, fontScale, Scalar::all(40),thickness,8);
}

void OccupancyCounter::save_image(Mat img, int frameN, string posfix, int scale){
  Mat im1,im2,im3;
  if(scale != 0){
    convertScaleAbs(img,im1,255 / scale);
    applyColorMap(im1,im2,COLORMAP_HOT);
    flip(im2,im3,0);
    imwrite("./imgs/frame/"+to_string(frameN)+posfix+".png",im3);
  }else{
    Mat filtered;
    threshold( img, filtered, 70, 1,THRESH_TOZERO ); 
    convertScaleAbs(filtered,im1,255 / 120);
    applyColorMap(im1,im2,COLORMAP_HOT);
    flip(im2,im3,0);
     
    imwrite("./imgs/frame/"+to_string(frameN)+posfix+".png",im3);
  }
}
void OccupancyCounter::subtract_(Mat a, Mat b, Mat& result){
  Mat dst;
  if(!b.data)
    dst = a.clone();
  else
    subtract(a,b,dst);
  threshold( dst, result, -1, 1 ,THRESH_TOZERO ); //resetting negative values to zero
}

bool OccupancyCounter::is_someone_there(Mat frame_, int frameN,vector<Person>people){
  
  Mat diffs_;
  subtract(frame_,background_,diffs_);

  return (check_by_histogram(diffs_,"main hist",frameN,people.size()));

}

FrameSide OccupancyCounter::find_frame_side(Mat diff_im,int frameN){
  int left_heat = 0;
  int right_heat = 0;
  FrameSide side = UNKNOWNSIDE;  

  int check_len = 4;

  if(SENSOR == 0) //GridEye
    check_len = 4;
  else
    check_len = 8;

  for(int k=1;k<=check_len;k++){  //k will run longer for MLX
    left_heat = 0;
    right_heat = 0;
    for(int i=0;i< diff_im.cols;i++){
      for (int j=0;j<diff_im.rows;j++){ 
        if( i < k)
          left_heat += (int)diff_im.at<uchar>(i,j);
        else if(i >= (diff_im.cols - k))
          right_heat += (int)diff_im.at<uchar>(i,j);
      }
    }
    //log_("K " + to_string(k)+" left heat " + to_string(left_heat) + " right  heat " + to_string(right_heat));
    float avg_left_heat = left_heat/(k*diff_im.rows);
    float avg_right_heat = right_heat/(k*diff_im.rows);

    //log_("left heat " + to_string(left_heat) + "right heat " + to_string(right_heat) );
    if(avg_left_heat - avg_right_heat >= 1.25){ //*10/8 = 1.25 
      side = LEFTSIDE ;  
      break;
    }
    else if( avg_right_heat - avg_left_heat >= 1.25){
      side = RIGHTSIDE; 
      break;
    }   
  }
  // if(side == UNKNOWNSIDE){
    
  //   if(left_heat < right_heat ){
  //     side = RIGHTSIDE;
  //   }else{
  //     side = LEFTSIDE;
  //   }
  //   log_("not sure side but assigned "+to_string(side));
  // }
  return side;
}
void OccupancyCounter::assign_side_to_bodies(vector<Body>& found_bodies, Mat diff_im,FrameSide frame_side,int frameN){
  
  if(found_bodies.size() == 1){
    int side = frame_side;
    found_bodies.at(0).side = side;
    log_("body side is " +  to_string(found_bodies.at(0).side) + " and center is " + to_string(found_bodies.at(0).cx));
  }else if(found_bodies.size() == 2){
    if(found_bodies.at(0).centerC().x <= (diff_im.cols/ 2))
      found_bodies.at(0).side = 0; //Change to enum: @hessam
    else
      found_bodies.at(0).side = 1;
    if(found_bodies.at(1).centerC().x <= (diff_im.cols/ 2))
     found_bodies.at(1).side = 0;
    else
      found_bodies.at(1).side = 1;
  }
}

//It computes the center of the body contour by finding the min_x and max_x of the contour
void initBody(vector<Point> contour,Body& body){
  int minx = 8;             //fix for MLX: @Hessam
  int maxx = 0;
  for(Point p:contour){
   if(p.x < minx)
    minx = p.x;
   if(p.x > maxx)
    maxx = p.x; 
  }  
  body.cx = (minx + maxx)/2;
  body.contour_ = contour;
  return;
}
//Find 2 biggest counters which are larger than minimum threshold
vector<Body> OccupancyCounter::convert_contour_to_body(vector<vector<Point>> contours,int frameN, Mat im){
  vector<Point> biggest_contour;
  vector<Point> second_biggest_contour;
  
  //Find top 2 contours
  for(int i=0;i<contours.size();i++){
    
    if(contours.at(i).size() < ((MIN_BODY_AREA*im.rows*im.cols)/100.0))
      continue;
    if(contours.at(i).size() > biggest_contour.size()){

      second_biggest_contour = biggest_contour;
      biggest_contour = contours.at(i);
    }else if(contours.at(i).size() > second_biggest_contour.size()){
      second_biggest_contour = contours.at(i); 
    }
  }
  //create bodies based on found contours
  vector<Body> found_bodies;
  if(biggest_contour.size()>0){
      Body body;
      initBody(biggest_contour,body);
      found_bodies.push_back(body);
  }
  if(second_biggest_contour.size()>0){
      Body body;
      initBody(second_biggest_contour,body);
      found_bodies.push_back(body);
  }
  return found_bodies;
}

//Find each bodies temperature
void OccupancyCounter::filter_by_body_temperature(Mat image, vector<Body>& found_bodies){
	vector<vector<Point>> cont;
  for(Body& body : found_bodies){
    int sum = 0;
    int maxT = 0;
    for(Point p: body.contour_){
      sum += (int)image.at<uchar>(p.x,p.y);
      if((int)image.at<uchar>(p.x,p.y) > maxT)
        maxT = (int)image.at<uchar>(p.x,p.y);
    }
    body.maxT = maxT;
    body.T = sum / max((int)body.contour_.size(),1);
    log_( "T " + to_string(body.T) + " max T " + to_string(body.maxT ));
  }
 
  if(found_bodies.empty() == false) {
    for(int i = found_bodies.size() - 1; i >= 0; i--) {
        if(found_bodies.at(i).T <= min_body_temp_) {
            log_("erasing body with temp " + to_string(found_bodies.at(i).T) + " max temp " + to_string(found_bodies.at(i).maxT));
            found_bodies.erase( found_bodies.begin() + i ); 
        }
    }
  }
}
Mat threshold_(Mat raw_im, int thresh_val){
  int rows , cols;
  if(SENSOR==0){
    rows = GRIDEYE_RAW_ROWS;
    cols = GRIDEYE_RAW_COLS;
  }else{
    rows = MLX_RAW_ROWS;
    cols = MLX_RAW_COLS;
  }
  Mat result = Mat::zeros(cols,rows,CV_8UC1);
  for(int i=0;i<raw_im.rows;i++){
    for(int j=0;j<raw_im.cols;j++){
      if((int)raw_im.at<uchar>(i,j) >= thresh_val){
         result.at<uchar>(i,j) = 1;
      }
    }
  }
  return result;
}
vector<Point> bfs(Mat im, int** status,int col,int row){
  //cout << "bfs at " << row <<" " << col << endl;
  int col_margin;
  int row_margin;
  
  if(SENSOR==0){
    col_margin = GRIDEYE_RAW_COLS;
    row_margin = GRIDEYE_RAW_ROWS;
  }else{
    col_margin = MLX_RAW_COLS;
    row_margin = MLX_RAW_ROWS;
  }
  vector<Point> result;
  queue<Point> q;
  q.push(Point(col,row));
  status[col][row] = 1;
  while(!q.empty()){
    Point p = q.front();
    q.pop();
    if(status[p.x][p.y] == 1){
      status[p.x][p.y] = 2;
      result.push_back(p);
      for(int i=-1;i<=1;i++){
        for(int j=-1;j<=1;j++){
          int xx = p.x + i;
          int yy = p.y + j;
          if((xx  < 0) or (xx >= col_margin) or (yy < 0 ) or (yy>=row_margin) ) 
            continue; 
          if(((int)im.at<uchar>(xx,yy) == 1) and (status[xx][yy] == 0)){
            status[xx][yy] = 1;
            q.push(Point(xx,yy));
          }
        }
      }
    } 
  }
  //cout << "end of bfs component size is " << (int)result.size() << endl;
  return result;
}
vector<vector<Point>> connectedC(Mat im){
  int rows , cols;
  if(SENSOR==0){
    rows = GRIDEYE_RAW_ROWS;
    cols = GRIDEYE_RAW_COLS;
  }else{
    rows = MLX_RAW_ROWS;
    cols = MLX_RAW_COLS;
  }
  int** status = new int*[rows];
  for(int i=0;i<rows;i++)
    status[i] = new int[cols];
  for(int i=0;i<rows;i++)
    for(int j=0;j<cols;j++)
      status[i][j] = 0;
  vector<vector<Point>> contours ;
  
  for(int i=0;i<cols;i++){
    for(int j=0;j<rows;j++){
      if(((int)im.at<uchar>(i,j) == 1) and status[i][j] == 0){
        vector<Point > res = bfs(im,status,i,j);
        if(res.size()>0)
          contours.push_back(res);
      }
    }
  }
  delete[] status;
  return contours;
}
void print_frame(Mat im){
   for(int i=0;i<im.rows;i++){
        for(int j=0;j<im.cols;j++){
          cout <<  (int)im.at<uchar>(j,i) << " ";
        }
        cout << endl;
      }
      cout << endl;
}
void draw_contours(vector<vector<Point>> contours){
  int rows,cols;
  if(SENSOR==0){
    rows = GRIDEYE_RAW_ROWS;
    cols = GRIDEYE_RAW_COLS;
  }
  else{
    rows = MLX_RAW_ROWS;
    cols = MLX_RAW_COLS;
  }
  Mat result = Mat::zeros(cols,rows,CV_8UC1);
  int label = 2;
  for(int i=0;i<contours.size();i++){
    for(int j=0;j<contours.at(i).size();j++){
      Point p = contours.at(i).at(j);
      result.at<uchar>(p.x,p.y) = label;
    }
    label++;
  }
  print_frame(result);
}
//Extract bodies
vector<Body> OccupancyCounter::extract_body(Mat diffs, int frameN){
  int index = 0;
  vector<Body> found_bodies;
  vector<vector<Point>> contours;
  Mat masked;

  //check for index = 0, then 10, then try in between, if needed. @Hessam

  while(index <= 10){   //Threshold frame up to 10 temperatrue levels to find 2 bodies
      
    masked = threshold_(diffs,MIN_BODY_DIFF  + index);
    
    //print_frame(masked);
    contours.clear();
    contours = connectedC(masked); 
   //draw_contours(contours);
   // if(frameN==405)
    //  exit(0);
    found_bodies = convert_contour_to_body(contours,frameN,diffs);
    if(found_bodies.size() == 0 or found_bodies.size() == 2){ // did not found any contours or just fuond 2
      return found_bodies;
    }else if(found_bodies[0].area() <= ((MAX_BODY_AREA*diffs.rows*diffs.cols) / 100.0 ))
      return found_bodies;  
    index ++;
  } 

  return found_bodies;
}

void OccupancyCounter::plot_contours(Mat& im,vector<Body>& found_bodies){
  vector<vector<Point>> cons ;
  for(Body body:found_bodies){
     for(Point p:body.contour_){
       im.at<uchar>(p.y,p.x) = 25;
     }
  }
}

void OccupancyCounter::find_body_center(Mat frame,vector<Body>& found_bodies,int frameN){
  int sum = 0;
  cout << "at frame " << frameN << endl;
  vector<float> hist_result;
  for (int j= 0;j<frame.cols;j++){
    sum = 0;
    for (int i=0;i<frame.rows;i++){
      sum += frame.at<uchar>(j,i); 
    }
    hist_result.push_back(sum/4.0);
    cout << "col " << j << " sum " << sum << endl;
  }
  int status = 0;
  float pre_val = hist_result[0];
  
  vector<int> dec_incs;

  
  vector<int>  inc_decs ;
  for(int i=1;i<hist_result.size();i++){
    float  current_val = hist_result[i];
    if(current_val > pre_val){
      if(status == -1){
        dec_incs.push_back(i);
        status = 1;
      }else if(status == 0){
        status = 1;
      }
    }else if(current_val < pre_val){
      if(status == 1){
        inc_decs.push_back(i);
        status = -1;
      }else if(status == 0){
        status = -1;
      }
    }
    pre_val = current_val;
  }
  cout <<  "inc_dec " << inc_decs.size() << " dec_inc " << dec_incs.size() << endl; 
  if(found_bodies.size()== 2){
    if(dec_incs.size()==0){
      
      cout <<  "body centers not found  1 " << endl;
    }else if(dec_incs.size() == 1){
      found_bodies.at(0).cx = dec_incs[0] - 1;
      found_bodies.at(1).cx = dec_incs[0] + 1;
      cout << "body centers are " << dec_incs[0] - 1 <<" "<< dec_incs[0] + 1 << endl;
      found_bodies.at(0).gap = dec_incs[0];
      found_bodies.at(1).gap = dec_incs[0];
    }else{
      cout << "2 bodies found but " << dec_incs.size() << " local minimum found " << endl;
    }
  }else if(found_bodies.size() == 1){
    if(inc_decs.size()== 0){
      if(status == 1){
        inc_decs.push_back(hist_result.size()-1);
      }else if(status == -1){
        inc_decs.push_back(0);
      }
      found_bodies.at(0).cx = inc_decs[0];
      cout << "body center  " << inc_decs[0] << endl;
    }else if(inc_decs.size() == 1){
      found_bodies.at(0).cx = inc_decs[0];
      cout << "body center is " << inc_decs[0];
    }else{
     cout << "1 bodies found but " << inc_decs.size() << " local maximum found " << endl; 
    }
  }
  // Draw the histograms for B, G and R
   if(SHOW_HIST){
    int hist_w = 256; int hist_h = 256;
    int histSize = hist_result.size();
    int bin_w = cvRound( (double) hist_w/histSize );

    Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );
    
    for( int i = 1; i < hist_result.size(); i++ )
    {
         
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(hist_result[i-1]) ) ,
                         Point( bin_w*(i), hist_h - cvRound(hist_result[i]) ),
                         Scalar( 255, 0, 0), 2, 8, 0  );
    }

    namedWindow("col_hist", CV_WINDOW_AUTOSIZE );
    imshow("col_hist", histImage );
    if(SAVE_IMAGE)
      imwrite("./imgs/hist_col/"+to_string(frameN)+".png",histImage);
  }
}
/*
* Process frame 
* @Input 1: Original Image
* @Input 2: Vector of Persons
* @Input 3: Video Stream Reference
* @Input 4: Original Raw Frame
*/
void OccupancyCounter::process_frame(Mat frame, int frameN, vector<Person>& people ){
  Mat diffs;
  bool save =false;
  vector<Body> found_bodies;
  subtract_(frame,background_,diffs);
 
  FrameSide frame_side = find_frame_side(diffs,frameN);
  //Update background using the initial frames
  int offset = START_FRAME;
  if(LIVE)
    offset = 0;
  if(frameN <= BACKGROUND_FRAMES + offset){
    add_to_background(frame);
    min_body_temp_ = mean(background_)[0] + 4;
  }else if(frameN == BACKGROUND_FRAMES + offset+1){
    cout << "Background Calculation is Done !! " << endl;
    cout << " threshold is " <<  min_body_temp_ << endl;
  }

  if(is_someone_there(frame,frameN,people)){
    log_("some one in frame " + to_string(frameN));
    //
      Mat threshed;
      Mat flipped_diffs = Mat::zeros(diffs.cols,diffs.rows,CV_8UC1);;
      for(int i = 0;i<diffs.cols;i++){
        for(int j=0;j < diffs.rows;j++){
          flipped_diffs.at<uchar>(j,i) = diffs.at<uchar>(i,j);
        }
      }
      threshold(flipped_diffs, threshed, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
      Mat resized = resize_frame(threshed,64,64);
      Mat display;
      flip(resized,display,0);
      imwrite("./imgs/otsu/"+to_string(frameN)+".png",display);
    //
    found_bodies =  extract_body(diffs,frameN); //Extract Potentional Bodies
    
    if(found_bodies.size()>= 1){   //If we have found at least 1 body
      find_body_center(frame,found_bodies,frameN);
      filter_by_body_temperature(frame,found_bodies);  //Assign //cout << "body temp done " << endl;
        
      assign_side_to_bodies(found_bodies,diffs,frame_side,frameN);
      //plot_contours(diffs,found_bodies,im);   //plot contours for visualization
      if(SAVE_IMAGE) {
        save = true;
        //save_image(diffs,frameN,"",40);
      }
    }
  } 
  Frame frame_;
  frame_.frameN = frameN;
  frame_.valid = true;
  frame_.found_bodies = found_bodies;
  frame_.side = frame_side;
  
  frames[frameN % HISTORY_LENGTH].found_bodies.clear();
  frames[frameN % HISTORY_LENGTH]= frame_;
  
  match_body_person(people,frameN);
  update_people_status(people,frameN);
    
  show_image(frame,frameN,save,diffs);
  
}

void OccupancyCounter::add_to_background(Mat frame){
  double alpha = 0.5; double beta; double input;
  Mat dst;
  beta = ( 1.0 - alpha );
  if( !background_.data ){ //The very first background is empty
    background_ =  Mat::zeros(frame.rows,frame.cols,CV_8UC1);
    background_ = frame.clone();
  }
  else    
    addWeighted(frame , alpha, background_, beta, 0.0, background_);
  
}


