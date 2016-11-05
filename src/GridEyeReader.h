/**@file GridEyeReader.h
 *
 * @date June 30, 2016
 * @author Hessam Mohammadmoradi <hmoradi@cs.uh.edu>
 */

#ifndef GRIDEYEREADER_H_
#define GRIDEYEREADER_H_
#include "buffer.h"

using namespace std;
class GridEyeReader {
public:
	GridEyeReader();
	void ReadFrameLive(int& frameN_read);
	int buffer_size();
	void read_frame_from_file(std::ifstream* infile);
	vector<int**> interprete_data(unsigned char *buf, int len);
	void Init();
	virtual ~GridEyeReader();
	int ** next_frame();
	int ** to_delete;
private:
	Buffer buffer_;
	struct ftdi_context *ftdi;
	unsigned char residue[1024] ;
	int exitRequested = 0;
	int frameNumber = 0;
	unsigned char buf[1024];
    int f ; 
    int i;
    int vid;
    int pid;
    int baudrate;
    int interface;
    int find_packet_head(unsigned char *buf, int index, int len);
	void array_copy(unsigned char* dest, unsigned char* source,int start_index, int len);
	int** interprete_packet(unsigned char* buf,int index);
	
};

#endif /* GRIDEYEREADER_H_ */
