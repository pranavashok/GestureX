#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include "extras.h"

timeval start, end;
long mtime, seconds, useconds;

void _starttimer() {
	gettimeofday(&start, NULL);
}
void _stoptimer() {
	gettimeofday(&end, NULL);
}
void _printtimer() {
	seconds  = end.tv_sec  - start.tv_sec;
	useconds = end.tv_usec - start.tv_usec;
	mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
	printf("Time: %lu ms\n", mtime);
}
int _getdiff() {
	seconds  = end.tv_sec  - start.tv_sec;
	useconds = end.tv_usec - start.tv_usec;
	mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
	return (int) mtime;
}
