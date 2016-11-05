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
#include "GridEyeReader.h"
#include "mlxd.h"
#include "XMPPWrapper.h"
#include "OccupancyCounter.h"


using namespace cv;
using namespace std;


int exitRequested = 0;
int frameN_read = 0;
int frameN_processed = 0;
int people_inside = 0;
int retval ;
int person_count_last = 0;
string input_file_name_;
std::ofstream outfile;
GridEyeReader grideyeReader;
XMPPWrapper XMPPWrapper_;
OccupancyCounter OccupancyCounter_;
MLXD mlxd_;

/*
*    Utility: Calculate the frame rate
*/
long lastTime = 0;
int lastFrame = 0;
void calc_frame_rate(){
    time_t now;
    time(&now);
    if((long)now - lastTime >=1){
        cout << "frame rate is  "<<(frameN_read - lastFrame) << endl;
        lastFrame = frameN_read;
        lastTime = (long)now;
    }
}

/*
*    Utility: Exit on Ctrl+ c.
*/
void sigintHandler(int signum){
	outfile.close();
	grideyeReader.~GridEyeReader();
    exitRequested = 1;
}

/*
*    Process Frames - This Function will be called in a separate thread
*    @ input: Video Stream Reference to save frames as a video
*/
void init_processing_frame(std::ofstream* outfile){
	int ** frame;
	vector<Person> people;
    
	while(true){ 
		if ((!LIVE and frameN_processed > END_FRAME) or (exitRequested))
			return;
		if(!LIVE and frameN_processed < START_FRAME ){
			frameN_processed++;
			continue;
		}
		
		if(SENSOR==0)
			frame = grideyeReader.next_frame();
		else
			frame = mlxd_.next_frame();
		
		
		Mat im = OccupancyCounter_.convert_to_Mat(frame); 
		
		OccupancyCounter_.process_frame(im,frameN_processed,people);
		people_inside = OccupancyCounter_.people_inside;
		if(ENABLE_XMPP_REPORTING){
			if(people_inside != person_count_last){
				XMPPWrapper_.occupancyChange(people_inside, people_inside - person_count_last);
			}
		}
		person_count_last = people_inside;
		frameN_processed++;
		delete[] frame;
	}
}

/*
*    Read Frames from File - This Function will be called in a separate thread
*    @ input: Input File Stream
*/

//This function reads from file and puts the frames into a buffer
void retrive_frame_from_file(std::ifstream* infile,int sensorType){
	while(true){
    	if(sensorType == 0)
    		grideyeReader.read_frame_from_file(infile);
    	else
    		mlxd_.read_frameMLX_from_file(infile);
        frameN_read++;
    	if (frameN_read >= END_FRAME){
			cout << "End of file" << endl;
			return;
    	}
	}
}

/*
*    Read Frames from GridEye - This Function will be called in a separate thread
*/	
void retrive_frame_live(){
	
	auto begin = chrono::high_resolution_clock::now(); 
	while(!exitRequested){
        if(SENSOR==0){ //GridEye
	        auto end = chrono::high_resolution_clock::now();
	        auto dur = end - begin;
	        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
	        if(ms < 100)
				usleep((100 - ms)*1000);
	        else
				cout << "reading frames ms passed is " << ms << endl;
	    	grideyeReader.ReadFrameLive(frameN_read);
	        begin = chrono::high_resolution_clock::now();
       }
       else{  //MLX
       	mlxd_.readFrameLive(frameN_read);
       	usleep(10*1000);
	   }
	}
}

/*
*    Read from File. Creates 2 threads for reading from file and Processing them
*    @input 1: Input file name
*    @input 2: Output Video Stream Reference 
*/	
void read_from_file(string file_name,std::ofstream* outfile){
    std::ifstream infile(file_name);
    int** frame = NULL;
   
    person_count_last = 0;
    OccupancyCounter_.people_inside = (people_inside);
    retrive_frame_from_file(&infile,SENSOR);
    init_processing_frame(outfile);
}
/*
*    Update number of people upon receving message from Sensorandrow
*    @input 1: number of poeple inside
*/	 
void update_people_count_handler(int param){
	
	if(ENABLE_XMPP_REPORTING){
       XMPPWrapper_.update_people_count(param,people_inside);
       OccupancyCounter_.people_inside = (people_inside);
   }
}
/*
*    Utitliy: Reset number of people inside
*    @input 1: Initial wait time
*    @input 2: Midnight timer
*/	
void reset_occupancy_count(unsigned int initial_wait_time, atomic<bool>& timer_keeps_running){
	if(ENABLE_XMPP_REPORTING){
        XMPPWrapper_.reset_occupancy_count(initial_wait_time, timer_keeps_running,people_inside);
        OccupancyCounter_.people_inside = (people_inside);
	}
}

/*
*    Utility: Computer remaining time to mid night
*/
int compute_remaining_time_of_today(){

	struct timeval tv;
	struct tm *tm;

	int current_hour, current_min, current_sec;
	gettimeofday(&tv, NULL);

	if ((tm = localtime(&tv.tv_sec)) != NULL) {
		current_hour = tm->tm_hour; //24 hr format
		current_min = tm->tm_min;
		current_sec = tm->tm_sec;
	}
	else
	{
		cout << "Could not compute local time for timer based restart." << endl;
		return 0;
	}
	int remaining_time = (24 - current_hour - 1)*60*60 + (60 - current_min - 1)*60 + (60 - current_sec -1) + 1;  //in seconds, assuming we will reset at 12:00 AM
	return remaining_time;
}
/*
*  Utility: Converty string to bool
*  Input 1: input string
*/
bool to_bool(string str){
	if(str.find("true")!=string::npos)
		return true;
	return false;
}

int main(int argc, char **argv){
	
	input_file_name_ = INPUT_FILE;
	int i = 1;
	while(i< argc){
		if(string(argv[i]) == "-p")
			people_inside = atoi(argv[i+1]);
		i += 2;
	}
	//init_params();
	//VideoWriter outputVideo;
	// if(SAVE_VIDEO){
	// 	Size S = Size(3*GRIDEYE_WIDTH ,GRIDEYE_HEIGHT);
	//     outputVideo.open("footprint.avi",  CV_FOURCC('M','J','P','G'), 10, S, true);
 //        if (!outputVideo.isOpened())
 //        {
 //            cout  << "Could not open the output video for write: " << endl;
 //            return 0;
 //        }
	// }
	
   
    /* Initiate a timer for resetting people count at midnight */
    atomic<bool> timer_keeps_running {true} ;
	if(reset_count_at_midnight) {
		int initial_wait_time = compute_remaining_time_of_today();
		thread( reset_occupancy_count, initial_wait_time, std::ref(timer_keeps_running) ).detach() ; 
	}
	
	if(ENABLE_XMPP_REPORTING)
	    XMPPWrapper_.report_updated_people_count(-200, people_inside, people_inside - person_count_last); //logging + reporting initial count
    if(ENABLE_ACTUATION)
    {
    	signal(SIGUSR1, update_people_count_handler);
    }
    signal(SIGINT, sigintHandler);

    if(LIVE == false){
		cout << "reading from "<< input_file_name_ << endl;
		read_from_file(input_file_name_,&outfile);
		return 0;
	}
  	
    vector<Person> people;
    if(SENSOR==0) //GridEYE
    	grideyeReader.Init();
    else	//MLX sensor
    	mlxd_.init();
    OccupancyCounter_.people_inside = (people_inside);
    person_count_last = 0;
       
    std::thread producer_thread(retrive_frame_live);
    signal(SIGINT, SIG_DFL);
    init_processing_frame(&outfile); //the main function is basically the receiver thread
    
    retval =  EXIT_SUCCESS;
    
    grideyeReader.~GridEyeReader();
    return retval;
}
