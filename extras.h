#ifndef EXTRAS_H
#define EXTRAS_H
#include "X11/Xlib.h"

void _starttimer(void);
void _stoptimer(void);
void _printtimer(void);
int getdiff(void);
void pressRight(Display *);
void pressLeft(Display *);

#endif
