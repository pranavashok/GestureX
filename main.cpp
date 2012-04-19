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
float lR, rR, uR, dR;
Scalar hsv_min, hsv_max;
Moments m;
int cmx, cmy, _cmx, _cmy; //Center of mass, current and previous
int screenWidth, screenHeight, captureWidth, captureHeight, pixel_count; //Image resolutions

void _noiseReduce(Mat *, Mat *);
int _getPixelCount(Mat);
void _setLeft();
void _setRight();
void _setUp();
void _setDown();

int main()
{
	char c;
	int t, flag=0, count=0;
	int s; //Screen	
	int heldr = 0, heldl = 0, heldu = 0, heldd = 0; //Keypress status
	Display *d;
	Window root_window;
	
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

	VideoCapture capture(1); //Default(0) or External(1) camera
	
	if(!capture.isOpened())
		return -1;
	
	captureWidth = capture.get(CV_CAP_PROP_FRAME_WIDTH);
	captureHeight = capture.get(CV_CAP_PROP_FRAME_HEIGHT);
		
	hsv_min = Scalar(0, 30, 80, 0);
	hsv_max = Scalar(20, 150, 255, 0);
	
	char wait[1];
	
	for(int i = 0; i!=-1; i) {
		capture >> src;
		_noiseReduce(&src, &src);
		imshow("src", src);
		
		if(i == 0) {
			cout<<"Calibration\n===============\n\n";
			cout<<"Gesture for left keypress\n";
			while(1) {
				capture>>src;
				imshow("src",src);
				c = cvWaitKey(20);
				if( c == ' ' ) break;
			}
			_setLeft(); 
			cout<<"Gesture for right keypress\n"; 
			while(1) {
				capture>>src;
				imshow("src",src);
				c = cvWaitKey(20);
				if( c == ' ' ) break;
			}
			_setRight(); 
			cout<<"Gesture for up keypress\n";
			while(1) {
				capture>>src;
				imshow("src",src);
				c = cvWaitKey(20);
				if( c == ' ' ) break;
			}
			_setUp();
			cout<<"Gesture for down keypress\n";
			while(1) {
				capture>>src;
				imshow("src",src);
				c = cvWaitKey(20);
				if( c == ' ' ) break;
			}
			_setDown();
			cout<<"("<<lR<<", "<<rR<<", "<<uR<<", "<<dR<<")\n";
			i++;
		}
		
		cvtColor(src, hsv_image, CV_BGR2HSV);
		
		inRange(hsv_image, hsv_min, hsv_max, hsv_mask);
		imshow("hsv-mask", hsv_mask);
		
		m = moments(hsv_mask, true);

		pixel_count = _getPixelCount(hsv_mask);
		
		if(pixel_count < 3000) {
			pixel_count = 0;		
			continue;
		}
		
		pixel_count = 0;
		
		_cmx = cmx; //Store previous CM(x) in _cmx

		if(m.m10/m.m00 >= 0 || m.m10/m.m00 <= captureWidth)
			cmx = (m.m10/m.m00);
			
		_cmy = cmy; //Store previous CM(y) in _cmy
		
		if(m.m01/m.m00 >= 0 || m.m01/m.m00 <= captureHeight)
			cmy = (m.m01/m.m00);
			
		//cout<<cmx<<", "<<cmy<<endl;	
		
		if(cmx-_cmx < -300 || cmx-_cmx > 300 || cmy-_cmy < -300 || cmy-_cmy > 300)
			continue;
		
		if((cmx-_cmx < -3 && cmx-_cmx > 3) || (cmy-_cmy < -3 && cmy-_cmy > 3))
			continue;
		
		if(cmx > (lR+0.1)*captureWidth) { //0.75
			if(heldl==0) heldl = _holdLeft(d);
		}
		else if(cmx < (rR-0.1)*captureWidth) { //0.25
			if(heldr==0) heldr = _holdRight(d);
		}
		else {
			if(heldr==1) heldr = _releaseRight(d);
			if(heldl==1) heldl = _releaseLeft(d);
			if(cmx < (rR+0.1)*captureWidth && cmx > (rR-0.1)*captureWidth)
				_tapRight(d);
			if(cmx > (lR-0.1)*captureWidth && cmx < (lR+0.1)*captureWidth)
				_tapLeft(d);
		}

		if(cmy < uR*captureHeight) { //0.3
			if(heldu==0) heldu = _holdUp(d);
		}		
		else if(cmy > dR*captureHeight) {
			if(heldd==0) heldd = _holdDown(d);
		}
		else {
			if(heldu==1) heldu = _releaseUp(d);
			if(heldd==1) heldd = _releaseDown(d);
			if(cmy > uR*captureHeight && cmy < (uR+0.1)*captureHeight)
				_tapUp(d);
			if(cmy > (dR-0.1)*captureHeight && cmy < dR*captureHeight)
				_tapDown(d);
		}
		
		c = cvWaitKey(10);
		if( c == 27 ) break;
	}

	return 0;
}

void _noiseReduce(Mat *src, Mat *dst) {
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

int _getPixelCount(Mat img) {
	int pixels = 0;
	for(int i=0; i < img.rows; i++) 
	for(int j=0; j < img.cols; j++)
	{
		if(img.at<uchar>(i, j) == 255)
			pixels++;
	}
	return pixels;
}

void _setLeft() {
	cvtColor(src, hsv_image, CV_BGR2HSV);
		
	inRange(hsv_image, hsv_min, hsv_max, hsv_mask);
	
	m = moments(hsv_mask, true);

	pixel_count = _getPixelCount(hsv_mask);
	
	if(pixel_count > 3000) {
		if(m.m10/m.m00 >= 0 || m.m10/m.m00 <= captureWidth)
			cmx = (m.m10/m.m00);
		lR = float(cmx)/float(captureWidth);
	}
	else
		lR = 0.75;
}
void _setRight() {
	cvtColor(src, hsv_image, CV_BGR2HSV);
		
	inRange(hsv_image, hsv_min, hsv_max, hsv_mask);
	
	m = moments(hsv_mask, true);

	pixel_count = _getPixelCount(hsv_mask);
	
	if(pixel_count > 3000) {
		if(m.m10/m.m00 >= 0 || m.m10/m.m00 <= captureWidth)
			cmx = (m.m10/m.m00);
		rR = float(cmx)/float(captureWidth);
	}
	else
		rR = 0.25;
}
void _setUp() {
	cvtColor(src, hsv_image, CV_BGR2HSV);
		
	inRange(hsv_image, hsv_min, hsv_max, hsv_mask);
	
	m = moments(hsv_mask, true);

	pixel_count = _getPixelCount(hsv_mask);
	
	if(pixel_count > 3000) {
		if(m.m01/m.m00 >= 0 || m.m01/m.m00 <= captureHeight)
			cmy = (m.m01/m.m00);
		uR = float(cmy)/float(captureHeight);
	}
	else
		uR = 0.5;
}
void _setDown() {
	cvtColor(src, hsv_image, CV_BGR2HSV);
		
	inRange(hsv_image, hsv_min, hsv_max, hsv_mask);
	
	m = moments(hsv_mask, true);

	pixel_count = _getPixelCount(hsv_mask);
	
	if(pixel_count > 3000) {
		if(m.m01/m.m00 >= 0 || m.m01/m.m00 <= captureHeight)
			cmy = (m.m01/m.m00);
		dR = float(cmy)/float(captureHeight);
	}
	else
		dR = 0.75;
}
