#include "extras.h"
#include <iostream>
#include "X11/Xlib.h"
#include "X11/keysym.h"
#include "X11/extensions/XTest.h"

unsigned int keycode;
using namespace std; 

int _holdRight(Display *display) {
	keycode = XKeysymToKeycode(display, XK_KP_Right);
	XTestFakeKeyEvent(display, keycode, True, 0);
	XFlush(display);
	cout<<"Hold";
	return 1;
}
int _holdLeft(Display *display) {
	keycode = XKeysymToKeycode(display, XK_KP_Left);
	XTestFakeKeyEvent(display, keycode, True, 0);
	XFlush(display);
	cout<<"Hold";
	return 1;
}
int _releaseRight(Display *display) {
	keycode = XKeysymToKeycode(display, XK_KP_Right);
	XTestFakeKeyEvent(display, keycode, False, 0);
	XFlush(display);
	cout<<"Release";
	return 0;
}
int _releaseLeft(Display *display) {
	keycode = XKeysymToKeycode(display, XK_KP_Left);
	XTestFakeKeyEvent(display, keycode, False, 0);
	XFlush(display);
	cout<<"Release";
	return 0;
}
void _tapRight(Display *display) {
	keycode = XKeysymToKeycode(display, XK_KP_Right);
	XTestFakeKeyEvent(display, keycode, True, 0);
	XTestFakeKeyEvent(display, keycode, False, 0);
	XFlush(display);
	cout<<"Tap";
}
void _tapLeft(Display *display) {
	keycode = XKeysymToKeycode(display, XK_KP_Left);
	XTestFakeKeyEvent(display, keycode, True, 0);
	XTestFakeKeyEvent(display, keycode, False, 0);
	XFlush(display);
	cout<<"Tap";
}
