/**@file OccupancyCounter.h
 *
 * @date June 30, 2016
 * @author Hessam Mohammadmoradi <hmoradi@cs.uh.edu>
 */

#ifndef OCCUPANCYCOUNTER_H_
#define OCCUPANCYCOUNTER_H_
using namespace std;
using namespace cv;
enum status {UNKNOWN, ENTERED,  LEFT};
enum direction {UNKNOWNDIR, LtoR , RtoL, JUSTENTERED};
enum counted {NOTYET, LR,RL};
enum FrameSide {LEFTSIDE,RIGHTSIDE,UNKNOWNSIDE};
struct Pulse {
	int start;
    int end;
    float amp;    
};
class Body {
public:
	vector<Point> contour_;
	float T;
	float maxT;
	int order_id;
    int side;
	int cx;
	int cy;
	int gap;
	float area(){
		return contour_.size();
	}

	Point centerC(){
	    return Point(cx,cy);
	}
	string getID(){
		return to_string(T)+"_"+to_string(centerC().x)+"_"+to_string(centerC().y)+"_"+to_string(area());
	}
};
struct Person {
    map<int,Body> trajectory;
    status status_;
    direction direction_;
    counted counted_;
    int static_id ;
};
struct Frame {
	int frameN;
	int side;
	bool valid;
	vector<Body> found_bodies;
};
class OccupancyCounter {
public:
	int total_people;
	int people_inside;
	int min_body_temp_;
	Mat background_;
	Frame frames[HISTORY_LENGTH];
	int frame_index;
	float background_amp;
	OccupancyCounter();
	void process_frame(Mat im, int frameN,vector<Person>& people);
	Mat convert_to_Mat(int** frame);
	Mat resize_frame(Mat im , int width, int height);
	
private:
	
	bool is_match (Person person_ , Body body, int frameN);
	bool update_people_status(vector<Person>& people, int frameN);
	void update_people_direction(vector<Person>& people);
	void show_image(Mat im, int frameN, bool save, Mat diffs);
	bool check_trajectory(Person& person_);
	void put_text(Mat& Img,Point textOrg, string txt);
	void add_to_background(Mat raw_frame);
	
	void save_image(Mat img , int frameN,string posfix,int scale);
	bool is_someone_there(Mat diffs ,int frameN,vector<Person>people);
	vector<Body> extract_body(Mat raw_im, int frameN);
	direction find_direction(Person person);
	vector<Body> convert_contour_to_body(vector<vector<Point>> contours,int frameN,Mat im);
	void filter_by_body_temperature(Mat image, vector<Body>& baseRects);
	void subtract_(Mat a, Mat b , Mat& result);
	bool fft_(Mat im, Mat raw_im, int frameN, string window);
	bool check_by_histogram(Mat im, string window,int frameN,int peoplesize);
	void create_new_person(Body body_,vector<Person>& people,int frameN);
	void plot_contours(Mat& im,vector<Body>& found_bodies);
	void log_(string msg);
	bool correct_frame(Frame frame);
	void match_body_person(vector<Person>& people, int frameN);
	FrameSide find_frame_side(Mat im,int frameN);
	void assign_side_to_bodies(vector<Body>& found_bodies, Mat raw_im,FrameSide frame_side,int frameN);
	void find_body_center(Mat frame,vector<Body>& found_bodies,int frameN);
};

#endif /* OCCUPANCYCOUNTER_H_ */
