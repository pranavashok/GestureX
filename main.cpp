#include "math.h"
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include "X11/Xlib.h"
#include "extras.h"
#include <vector>
#include <opencv2/imgproc/imgproc.hpp>  // Gaussian Blur
#include <opencv2/core/core.hpp>        // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/highgui/highgui.hpp>  // OpenCV window I/O
#define BINARY_THRESHOLD 210

using namespace cv;
using namespace std;

int main()
{
	int height,width,t,white,flag=0,count=0,xvel=0;
	char c;
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
	_starttimer();
	while(1) {

		capture >> frame;
		white = 0;
		cvtColor(frame, red, CV_BGR2GRAY);
		GaussianBlur(red, red, Size(7,7), 4, 4);
		split(frame, channels);
		flag = 0;
		count++;
		for(int i=0; i < red.rows; i++) 
			for(int j=0; j < red.cols; j++)
			{
				if((channels[0].at<uchar>(i, j) > 40+channels[2].at<uchar>(i, j)) && (channels[0].at<uchar>(i, j) > 40+channels[1].at<uchar>(i, j))) {
					red.at<uchar>(i, j) = 255;
					white++;
					if(white > 500)
						flag = 1;
				}
				else
					red.at<uchar>(i, j) = 0;
			}
		if(flag != 1){
			for(int i=0; i < red.rows; i++) 
				for(int j=0; j < red.cols; j++)
					red.at<uchar>(i, j) = 0;
			count=0;
		}
		threshold(red, red, BINARY_THRESHOLD, 255, THRESH_BINARY);
		
		m = moments(red, true);

		_stoptimer();
		t = _getdiff();
		_printtimer();
		_starttimer();
		_cmx = cmx;

		//imshow( "Result", red );

		if(m.m10/m.m00 >= 0 || m.m10/m.m00 <= captureWidth)
			cmx = (m.m10/m.m00)*4;
			
		_cmy = cmy;
		
		if(m.m01/m.m00 >= 0 || m.m01/m.m00 <= captureHeight)
			cmy = (m.m01/m.m00)*4;
		
//		if(cmx-_cmx < -300 || cmx-_cmx > 300 || cmy-_cmy < -300 || cmy-_cmy > 300) //Skip large movements
//			continue;
			
//		if((cmx-_cmx < 3 && cmx-_cmx > -3) || (cmy-_cmy < 3 && cmy-_cmy > -3)) //Skip small movements
//			continue;
		xvel=(_cmx-cmx)/t;
		//cout<<"\tx: "<<xvel<<endl;
		//cout<<"\tc: "<<count<<endl;
		//cout<<"\ty: "<<cmy<<endl;
		//flag++;
		//if(flag > 5) {
		if(count>1){
			//if(cmx - _cmx < -150 && cmx < 100) {
			if(xvel>7)
				_pressRight(d);
		//		flag = 0;
			//}
		//	else if(cmy - _cmy < -150 && cmy < 100 ) {
			else if(xvel<-7)
				_pressLeft(d);
		//		flag = 0;
		//	}
		}
		//}
		//q = XQueryPointer(d, root_window, &root_return, &child_return, &curx, &cury, &win_x_return, &win_y_return, &mask_return);

		/*if(_cmx - cmx > 0)
			for(int i = 1; i < _cmx - cmx; i++) {
				XWarpPointer(d, None, None, 0, 0, 0, 0, 1, (cmy-_cmy)/(_cmx-cmx));
				XFlush(d);
			}
		else
			for(int i = 1; i < cmx - _cmx; i++) {
				XWarpPointer(d, None, None, 0, 0, 0, 0, -1, (cmy-_cmy)/(cmx-_cmx));
				XFlush(d);
			}*/
		//_printtimer();
		//cout<<flag<<":\tcmx: "<<cmx<<"\t\t_cmx: "<<_cmx<<"\t";

		//imshow( "Original", frame );

		c = cvWaitKey(1);
		if( c == 27 ) break;
	}

	return 0;	
}
