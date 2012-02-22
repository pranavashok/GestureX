#include "math.h"
#include <stdio.h>
#include <sys/time.h>
#include <iostream>
#include <unistd.h>
#include "X11/Xlib.h"
#include "extras.h"
#include <vector>
#include <opencv2/imgproc/imgproc.hpp>  // Gaussian Blur
#include <opencv2/core/core.hpp>		// Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/highgui/highgui.hpp>  // OpenCV window I/O
#define BINARY_THRESHOLD 210
#define LOW_THRESHOLD 20

using namespace cv;
using namespace std;

int edgeThresh = 1;
int ratio = 3;
int kernel_size = 3, contouridx;

Mat frame, frame_gray, result, detected_edges;
vector<vector<Point> > contours;
vector<Vec4i> hierarchy;

void CannyThreshold(int, void*)
{
  /// Reduce noise with a kernel 3x3
	imshow("Before", frame_gray );
	equalizeHist(frame_gray,frame_gray);
  	GaussianBlur(frame_gray, detected_edges, Size(3,3), 5, 5);
	blur(detected_edges, detected_edges, Size(3,3) );

  /// Canny detector
	Canny(detected_edges, detected_edges, LOW_THRESHOLD, LOW_THRESHOLD*ratio, kernel_size );
 /// find Contour

  	//findContours( detected_edges, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE);
 //draw contour	
	//drawContours( frame_gray,  contours, contouridx, 240, 1, 8, hierarchy,1);

  /// Using Canny's output as a mask, we display our result
	result = Scalar::all(0);

	frame.copyTo(result, detected_edges);	
	imshow("Result", frame_gray );
}
void incre(int, void*) {
	contouridx++;
}
	

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
	
	//namedWindow("Original",CV_WINDOW_AUTOSIZE);
	namedWindow("Result",CV_WINDOW_AUTOSIZE);
	createTrackbar("contour_idx","Result", &contouridx , 100, incre);
	while(1) {
		_starttimer();
		
		capture >> frame;
		
		if( !frame.data ) return -1;
		
		_stoptimer();

		result.create( frame.size(), frame.type() );
		cvtColor(frame, frame_gray, CV_BGR2GRAY );
		CannyThreshold(0, 0);
				
		/*m = moments(red, true);
		
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
		*/
		_printtimer();
		//cout<<"cmx-_cmx: "<<cmx-_cmx<<"\t"<<"cmy-_cmy: "<<cmy-_cmy<<endl;

		//imshow( "Original", frame );
		//imshow( "Result", red );
		char c = waitKey(5);
		if( c == 27 ) break;
	}

	return 0;	
}
