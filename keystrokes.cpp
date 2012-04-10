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
	cout<<"R(1)";
	return 1;
}
int _holdUp(Display *display) {
	keycode = XKeysymToKeycode(display, XK_KP_Up);
	XTestFakeKeyEvent(display, keycode, True, 0);
	XFlush(display);
	cout<<"U(1)";
	return 1;
}
int _holdDown(Display *display) {
	keycode = XKeysymToKeycode(display, XK_KP_Down);
	XTestFakeKeyEvent(display, keycode, True, 0);
	XFlush(display);
	cout<<"D(1)";
	return 1;
}
int _holdLeft(Display *display) {
	keycode = XKeysymToKeycode(display, XK_KP_Left);
	XTestFakeKeyEvent(display, keycode, True, 0);
	XFlush(display);
	cout<<"L(1)";
	return 1;
}
int _releaseRight(Display *display) {
	keycode = XKeysymToKeycode(display, XK_KP_Right);
	XTestFakeKeyEvent(display, keycode, False, 0);
	XFlush(display);
	cout<<"R(0)";
	return 0;
}
int _releaseUp(Display *display) {
	keycode = XKeysymToKeycode(display, XK_KP_Up);
	XTestFakeKeyEvent(display, keycode, False, 0);
	XFlush(display);
	cout<<"U(0)";
	return 0;
}
int _releaseDown(Display *display) {
	keycode = XKeysymToKeycode(display, XK_KP_Down);
	XTestFakeKeyEvent(display, keycode, False, 0);
	XFlush(display);
	cout<<"D(0)";
	return 0;
}
int _releaseLeft(Display *display) {
	keycode = XKeysymToKeycode(display, XK_KP_Left);
	XTestFakeKeyEvent(display, keycode, False, 0);
	XFlush(display);
	cout<<"L(0)";
	return 0;
}
void _tapRight(Display *display) {
	keycode = XKeysymToKeycode(display, XK_KP_Right);
	XTestFakeKeyEvent(display, keycode, True, 0);
	XTestFakeKeyEvent(display, keycode, False, 0);
	XFlush(display);
	cout<<"TR()";
}
void _tapUp(Display *display) {
	keycode = XKeysymToKeycode(display, XK_KP_Up);
	XTestFakeKeyEvent(display, keycode, True, 0);
	XTestFakeKeyEvent(display, keycode, False, 0);
	XFlush(display);
	cout<<"TU()";
}
void _tapDown(Display *display) {
	keycode = XKeysymToKeycode(display, XK_KP_Down);
	XTestFakeKeyEvent(display, keycode, True, 0);
	XTestFakeKeyEvent(display, keycode, False, 0);
	XFlush(display);
	cout<<"TD()";
}

void _tapLeft(Display *display) {
	keycode = XKeysymToKeycode(display, XK_KP_Left);
	XTestFakeKeyEvent(display, keycode, True, 0);
	XTestFakeKeyEvent(display, keycode, False, 0);
	XFlush(display);
	cout<<"TL()";
}
