#include "math.h"
#include <stdio.h>
#include <sys/time.h>
#include <iostream>
#include <unistd.h>
#include "X11/Xlib.h"
#include <vector>
#include <opencv2/imgproc/imgproc.hpp>  // Gaussian Blur
#include <opencv2/core/core.hpp>        // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/highgui/highgui.hpp>  // OpenCV window I/O
#define BINARY_THRESHOLD 210

using namespace cv;
using namespace std;

timeval start, end;
long mtime, seconds, useconds;

int main()
{
	int height,width;
	Display *d;
	Window root_window;
	Moments m;
	Window root_return, child_return;
	int win_x_return, win_y_return;
	unsigned int mask_return;
	int s, cmx, cmy, _cmx, _cmy, curx, cury, screenWidth, screenHeight, captureWidth, captureHeight;
	bool q;
	_cmx = 0;
	_cmy = 0;
	cmx = 0;
	cmy = 0;
	
	/* open connection with the server */
	d = XOpenDisplay(NULL);
	root_window = XRootWindow(d, 0);
	
	if (d == NULL) {
		printf("Cannot open display\n");
		exit(1);
   	}
 	
	s = DefaultScreen(d);
	
	screenWidth = XDisplayWidth(d, s);
	screenHeight = XDisplayHeight(d, s);

	XSelectInput(d, root_window, KeyReleaseMask);

	VideoCapture capture(0);
	
	if(!capture.isOpened())  // check if we succeeded
        	return -1;
	
	captureWidth = capture.get(CV_CAP_PROP_FRAME_WIDTH);
	captureHeight = capture.get(CV_CAP_PROP_FRAME_HEIGHT);
	Mat frame;
	Mat red;
	
	vector<Mat> channels;
	//namedWindow("Original",CV_WINDOW_AUTOSIZE);
	//namedWindow("Result",CV_WINDOW_AUTOSIZE);

	while(1) {
		gettimeofday(&start, NULL);
		capture >> frame;
		gettimeofday(&end, NULL);

		cvtColor(frame, red, CV_BGR2GRAY);
		GaussianBlur(red, red, Size(7,7), 4, 4);
		split(frame, channels);
		for(int i=0; i < red.rows; i++) 
			for(int j=0; j < red.cols; j++)
			{
				if((channels[2].at<uchar>(i, j) > 50+channels[1].at<uchar>(i, j)) && (channels[2].at<uchar>(i, j) > 50+channels[0].at<uchar>(i, j)))
					red.at<uchar>(i, j) = 255;
				else
					red.at<uchar>(i, j) = 0;
			}

		threshold(red, red, BINARY_THRESHOLD, 255, THRESH_BINARY);
		
		m = moments(red, true);
		
		_cmx = cmx;
		
		if(m.m10/m.m00 >= 0 || m.m10/m.m00 <= captureWidth)
			cmx = (m.m10/m.m00)*4;
		else
			continue;
			
		_cmy = cmy;
		
		if(m.m01/m.m00 >= 0 || m.m01/m.m00 <= captureHeight)
			cmy = (m.m01/m.m00)*4;
		else
			continue;
		
		if(cmx-_cmx < -300 || cmx-_cmx > 300 || cmy-_cmy < -300 || cmy-_cmy > 300) //Skip large movements
			continue;
			
		if((cmx-_cmx < 3 && cmx-_cmx > -3) || (cmy-_cmy < 3 && cmy-_cmy > -3)) //Skip small movements
			continue;
		
		//q = XQueryPointer(d, root_window, &root_return, &child_return, &curx, &cury, &win_x_return, &win_y_return, &mask_return);

		if(_cmx - cmx > 0)
			for(int i = 1; i < _cmx - cmx; i++) {
				XWarpPointer(d, None, None, 0, 0, 0, 0, 1, (cmy-_cmy)/(_cmx-cmx));
				XFlush(d);
			}
		else
			for(int i = 1; i < cmx - _cmx; i++) {
				XWarpPointer(d, None, None, 0, 0, 0, 0, -1, (cmy-_cmy)/(cmx-_cmx));
				XFlush(d);
			}

		seconds  = end.tv_sec  - start.tv_sec;
		useconds = end.tv_usec - start.tv_usec;
		mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
		cout<<"Time: "<<mtime<<" ms\n";
		//cout<<"cmx-_cmx: "<<cmx-_cmx<<"\t"<<"cmy-_cmy: "<<cmy-_cmy<<endl;

		//imshow( "Original", frame );
		//imshow( "Result", red );
		char c = cvWaitKey(1);
		if( c == 27 ) break;
	}

	return 0;	
}
