#ifndef EXTRAS_H
#define EXTRAS_H
#include "X11/Xlib.h"

void _starttimer(void);
void _stoptimer(void);
void _printtimer(void);
int _getdiff(void);
int _holdRight(Display *);
int _holdUp(Display *);
int _holdDown(Display *);
int _holdLeft(Display *);
int _releaseRight(Display *);
int _releaseUp(Display *);
int _releaseDown(Display *);
int _releaseLeft(Display *);
void _tapRight(Display *);
void _tapUp(Display *);
void _tapDown(Display *);
void _tapLeft(Display *);

#endif
