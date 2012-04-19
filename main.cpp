#include <iostream>
#include <unistd.h>
#include <vector>
#include <stdio.h>
#include "X11/Xlib.h"
#include "extras.h"
#include <opencv2/imgproc/imgproc.hpp>  // Gaussian Blur
#include <opencv2/core/core.hpp>        // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/highgui/highgui.hpp>  // OpenCV window I/O
#define BINARY_THRESHOLD 210

using namespace cv;
using namespace std;

Mat src, hsv_image, hsv_mask;

void NoiseReduce(Mat *src, Mat *dst) {
	int dilation_size = 0, erosion_size = 0;
	Mat element = getStructuringElement( MORPH_RECT,
					Size( 2*erosion_size + 1, 2*erosion_size+1 ),
					Point( erosion_size, erosion_size ) );
	/// Apply the erosion operation
	erode(*src, *dst, element);
	element = getStructuringElement( MORPH_RECT,
					Size( 2*dilation_size + 1, 2*dilation_size+1 ),
					Point( dilation_size, dilation_size ) );
	/// Apply the dilation operation
	dilate(*dst, *dst, element );
}

int main()
{
	char c;
	int t, flag=0, count=0;
	int s; //Screen	
	int cmx, cmy, _cmx, _cmy; //Center of mass, current and previous
	int screenWidth, screenHeight, captureWidth, captureHeight, pixel_count; //Image resolutions
	int heldr = 0, heldl = 0, heldu = 0, heldd = 0; //Keypress status
	Display *d;
	Window root_window;
	Moments m;
	
	_cmx = _cmy = cmx = cmy = 0;
	
	/* open connection with the server */
	d = XOpenDisplay(NULL);
	root_window = XRootWindow(d, 0);
	
	if (d == NULL) {
		printf("Cannot open display\n");
		exit(1);
   	}
   	
 	//namedWindow("src",CV_WINDOW_AUTOSIZE);
	//namedWindow("hsv-mask",CV_WINDOW_AUTOSIZE);
	
	s = DefaultScreen(d);
	
	screenWidth = XDisplayWidth(d, s);
	screenHeight = XDisplayHeight(d, s);

	XSelectInput(d, root_window, KeyReleaseMask);

	VideoCapture capture(1); //Default(0) or External(1) camera
	
	if(!capture.isOpened())
		return -1;
	
	captureWidth = capture.get(CV_CAP_PROP_FRAME_WIDTH);
	captureHeight = capture.get(CV_CAP_PROP_FRAME_HEIGHT);
		
	Scalar hsv_min = Scalar(0, 30, 80, 0);
	Scalar hsv_max = Scalar(20, 150, 255, 0);
	
	while(1) {
		capture >> src;
		NoiseReduce(&src, &src);
		imshow("src", src);
		
		cvtColor(src, hsv_image, CV_BGR2HSV);
		
		inRange(hsv_image, hsv_min, hsv_max, hsv_mask);
		imshow("hsv-mask", hsv_mask);
		
		m = moments(hsv_mask, true);

		for(int i=0; i < hsv_mask.rows; i++) 
			for(int j=0; j < hsv_mask.cols; j++)
			{
				if(hsv_mask.at<uchar>(i, j) == 255)
					pixel_count++;
			}
		
		if(pixel_count < 3000) {
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
