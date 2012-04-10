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
	int s, cmx, cmy, _cmx, _cmy, curx, cury, screenWidth, screenHeight, captureWidth, captureHeight, pixel_count, heldr = 0, heldl = 0, heldu = 0, heldd = 0;
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
 	namedWindow("src",CV_WINDOW_AUTOSIZE);
	namedWindow("hsv-mask",CV_WINDOW_AUTOSIZE);
	s = DefaultScreen(d);
	
	screenWidth = XDisplayWidth(d, s);
	screenHeight = XDisplayHeight(d, s);

	XSelectInput(d, root_window, KeyReleaseMask);

	VideoCapture capture(1);
	
	if(!capture.isOpened())  // check if we succeeded
        	return -1;
	
	captureWidth = capture.get(CV_CAP_PROP_FRAME_WIDTH);
	captureHeight = capture.get(CV_CAP_PROP_FRAME_HEIGHT);
	
	Mat src, hsv_image, hsv_mask;
	
	Scalar hsv_min = Scalar(0, 30, 80, 0);
	Scalar hsv_max = Scalar(20, 150, 255, 0);
	
	_starttimer();

	while(1) {
		capture >> src;
		
		imshow("src", src);
		cvtColor(src, hsv_image, CV_BGR2HSV);
		//imshow("hsv-image", hsv_image);
		inRange(hsv_image, hsv_min, hsv_max, hsv_mask);
		imshow("hsv-mask", hsv_mask);
		
		m = moments(hsv_mask, true);

		for(int i=0; i < hsv_mask.rows; i++) 
			for(int j=0; j < hsv_mask.cols; j++)
			{
				if(hsv_mask.at<uchar>(i, j) == 255)
					pixel_count++;
			}
		
		if(pixel_count < 2000) {
			pixel_count = 0;		
			continue;
		}
		
		pixel_count = 0;

		//_stoptimer();
		//t = _getdiff();
		//_printtimer();
		//_starttimer();
		
		_cmx = cmx;

		if(m.m10/m.m00 >= 0 || m.m10/m.m00 <= captureWidth)
			cmx = (m.m10/m.m00);
			
		_cmy = cmy;
		
		if(m.m01/m.m00 >= 0 || m.m01/m.m00 <= captureHeight)
			cmy = (m.m01/m.m00);
			
		cout<<cmx<<", "<<cmy<<endl;	
		
		if(cmx-_cmx < -300 || cmx-_cmx > 300 || cmy-_cmy < -300 || cmy-_cmy > 300)
			continue;
		
		if((cmx-_cmx < -3 && cmx-_cmx > 3) || (cmy-_cmy < -3 && cmy-_cmy > 3))
			continue;
		
		if(cmx > 0.75*captureWidth) {
			if(heldl==0) heldl = _holdLeft(d);
		}
		else if(cmx < 0.25*captureWidth) {
			if(heldr==0) heldr = _holdRight(d);
		}
		else {
			if(heldr==1) heldr = _releaseRight(d);
			if(heldl==1) heldl = _releaseLeft(d);
			if(cmx < 0.35*captureWidth && cmx > 0.25*captureWidth)
				_tapRight(d);
			if(cmx > 0.65*captureWidth && cmx < 0.75*captureWidth)
				_tapLeft(d);
		}

		if(cmx > 0.3*captureHeight) {
			if(heldu==0) heldu = _holdUp(d);
		}		
		else if(cmx < 0.1*captureHeight) {
			if(heldd==0) heldd = _holdDown(d);
		}
		else {
			if(heldu==1) heldu = _releaseUp(d);
			if(heldd==1) heldd = _releaseDown(d);
			if(cmx > 0.65*captureHeight && cmx < 0.75*captureHeight)
				_tapUp(d);
			if(cmx < 0.35*captureHeight && cmx > 0.25*captureHeight)
				_tapDown(d);
		}
		
		c = cvWaitKey(10);
		if( c == 27 ) break;
	}

	return 0;	
}
