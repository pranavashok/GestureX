#ifndef EXTRAS_H
#define EXTRAS_H
#include "X11/Xlib.h"

void _starttimer(void);
void _stoptimer(void);
void _printtimer(void);
int _getdiff(void);
void _pressRight(Display *);
void _pressLeft(Display *);

#endif
