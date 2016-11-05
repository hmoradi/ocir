/**@file GridEyeReader.cpp
 *
 * @date June 30, 2016
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
#include "GridEyeReader.h"
#include "buffer.h"
using namespace std;
using namespace cv;
using std::deque;

/*
* Print current frame
*/
void print_frame(int ** frame){
    for(int i=0;i<GRIDEYE_RAW_ROWS;i++){
        for(int j=0;j<GRIDEYE_RAW_COLS;j++){
            cout << frame[i][j] << " ";
        }
        cout << endl;
    }  
    cout << endl;
    cout << endl;     
}
/*
* GridEye Reader Constructor
*/
GridEyeReader::GridEyeReader(){

    f = 0;
    i = 0;
    vid = 0x403;
    pid = 0x6015;
    baudrate = 115200;
    interface = INTERFACE_A; //INTERFACE_ANY;    
    
}
/*
* Each serail packet starts with ***
* This function tries to find this pattern on recieved data
*/
int GridEyeReader::find_packet_head(unsigned char *buf, int index, int len){
    
    int count = 0;
    int i=0;
    for(i=index; i< len; i++){
        if(buf[i]=='*'){
            count++;
            if(count ==3){
                return i -2;
            }
        }else{
            count = 0;
        }
    }
    return i;
}

int GridEyeReader::buffer_size(){
    return buffer_.size();
}
/*
*Prints content of serial packet received from FTDI module
*/
int** GridEyeReader::interprete_packet(unsigned char* buf,int index){
	int** frame = new int*[GRIDEYE_RAW_ROWS];
    for (int i=0;i<GRIDEYE_RAW_ROWS;i++)
        frame[i] = new int[GRIDEYE_RAW_COLS];
	
    int thermistor_data = ((int)buf[index + 4]<<8) | buf[index +3]; //Next 2 characters [index: 3, 4] are thermistor bytes	
    int k = index + 5; 
    int i, j;
    int high_byte;
    unsigned char low_byte;
    int temperature_reading;
    int invalid_counter = 0;
    for(i = 0; i < GRIDEYE_RAW_ROWS; i++) {
        for(j = 0; j < GRIDEYE_RAW_COLS; j++) {
            high_byte = buf[k+1];
            low_byte = buf[k];
            temperature_reading = (high_byte << 8) | low_byte;
            if (temperature_reading == 0)
                invalid_counter++;
            frame[i][j] = temperature_reading ;
            if(frame[i][j] < 1 or frame[i][j] >= 150)
				invalid_counter ++;
            k+=2;
        }
    }    
    if(PRINT_DEBUG_MSG) {
        cout << "T " << thermistor_data / 16.0<< endl;
        print_frame(frame);
    }
    if(invalid_counter < 2){
		frameNumber++;
		return frame;
	}

	return NULL;
}



/*
* Interperet recieved data to extract frames
* @Input 1: Recieved data
* @Input 2: Length of Recieved data
*/
vector<int**> GridEyeReader::interprete_data(unsigned char *buf, int len){
    vector<int**> frames;
	int** frame;
	int index = find_packet_head(buf,0,len);
	
    while(index < len){
        if(len - index >= 131){
            frame = interprete_packet(buf,index);
            if(frame != NULL)
                frames.push_back(frame);
        }
        index+= PacketLength;
        index = find_packet_head(buf,index,len);
    }
    return frames;	
}

//Reads line by line starting with 8 numbers separated by space
//It will check for 8 such lines

void GridEyeReader::read_frame_from_file(std::ifstream* infile){
    int** frame = new int*[GRIDEYE_RAW_ROWS];
    for (int i=0;i<GRIDEYE_RAW_ROWS;i++)
        frame[i] = new int[GRIDEYE_RAW_COLS];
    int index = 0;
    string sLine;
    while (!infile->eof())
    {
        getline(*infile, sLine);         
        std::istringstream iss(sLine);
        
        if ((iss >> frame[index][0] >> frame[index][1] >> frame[index][2] >> frame[index][3] >> frame[index][4] >> frame[index][5] >> frame[index][6] >> frame[index][7])) 
        { 
            for(int j = 0;j<8;j++){
                frame[index][j] = min(max(frame[index][j],14),255);
                //cout << frame[index][j] << " " ;
            }
           
            index ++;
            if (index >=8){
                buffer_.add(frame);
                index = 0; 
                frame = new int*[GRIDEYE_RAW_ROWS];
                for (int i=0;i<GRIDEYE_RAW_ROWS;i++)
                    frame[i] = new int[GRIDEYE_RAW_COLS];
            } 
        }
        //cout << endl;
    }
}

int** GridEyeReader::next_frame(){
    
	return buffer_.remove();
}

/*
* Initialize GridEye reader
*/
void GridEyeReader::Init(){
	// Init
    if ((ftdi = ftdi_new()) == 0)
    {
        fprintf(stderr, "ftdi_new failed\n");
        exit(-1);
        //return EXIT_FAILURE;
    }
    fprintf(stderr,"Selecting interface.\n");
    // Select interface
    int debug_res = ftdi_set_interface(ftdi, INTERFACE_A);
       
    // Open device 
    f = ftdi_usb_open(ftdi, this->vid, this->pid);
    if (f < 0)
    {
        fprintf(stderr, "unable to open ftdi device: %d (%s)\n", this->f, ftdi_get_error_string(this->ftdi));
        exit(-1);
    }
    fprintf(stderr,"FTDI device is open now.\n");
	fprintf(stderr,"Setting up baudrate.\n");
    // Set baudrate
    f = ftdi_set_baudrate(this->ftdi, this->baudrate);
    if (f < 0)
    {
        fprintf(stderr, "unable to set baudrate: %d (%s)\n", f, ftdi_get_error_string(ftdi));
        exit(-1);
    }
    fprintf(stderr,"Baudrate (%d) set up done.\n",baudrate);
}

/*
* Read frame from Sensor
*/    
void GridEyeReader::ReadFrameLive(int& frameN_read){
	unsigned char buffer[1024] = {0};
	//map<int,int**> frames;
	unsigned char buf2[1];
    buf2[0] = '*';
    f = ftdi_write_data(ftdi, buf2, 1); //Ask PIC24F04KA200 microcontroller to start sending data
    memset(buf, 0, sizeof buf);
    //usleep(1 * 500);
   	f = ftdi_read_data(ftdi, buffer, 1024);
    if (f<0){
        fprintf(stderr, "Something is wrong. %d bytes read\n",f);
        usleep(1 * 1000000);
    }else{
		if(f == 0){
			//cout << "abnormal reading by " << f << endl;
			return;
		}
		//if(f!= 134)
		//	return;
		vector<int**>frames = interprete_data(buffer,f);
		for(int i=0;i<frames.size();i++){
			frameN_read++;
			buffer_.add(frames[i]);
		}
		memset(buffer,0,sizeof buffer);
        frames.clear();
	}
    return;
}

/*
*  GridEye Reader Destructor
*/
GridEyeReader::~GridEyeReader(){
    if(!LIVE)
        return;
    unsigned char buf2[1];
    buf2[0] = '~';
    int f = ftdi_write_data(ftdi, buf2, 1); // Asking PIC24F04KA200 microcontroller to stop sending data
    if( f < 0)
        cout<< "Error in writing data: " << endl;
	ftdi_usb_close(this->ftdi);
    ftdi_free(this->ftdi);
}
     
    
	
