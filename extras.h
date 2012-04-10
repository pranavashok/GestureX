#ifndef EXTRAS_H
#define EXTRAS_H
#include "X11/Xlib.h"

void _starttimer(void);
void _stoptimer(void);
void _printtimer(void);
int _getdiff(void);
int _holdRight(Display *);
int _holdLeft(Display *);
int _releaseRight(Display *);
int _releaseLeft(Display *);
void _tapRight(Display *);
void _tapLeft(Display *);

#endif
