#include "extras.h"
#include "X11/Xlib.h"
#include "X11/keysym.h"
#include "X11/extensions/XTest.h"

void pressRight(Display *display) {
	unsigned int keycode;
	keycode = XKeysymToKeycode(display, XK_KP_Right);
	XTestFakeKeyEvent(display, keycode, True, 0);
	XTestFakeKeyEvent(display, keycode, False, 0);
	XFlush(display);
}
void pressLeft(Display *display) {
	unsigned int keycode;
	keycode = XKeysymToKeycode(display, XK_KP_Left);
	XTestFakeKeyEvent(display, keycode, True, 0);
	XTestFakeKeyEvent(display, keycode, False, 0);
	XFlush(display);
}
