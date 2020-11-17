
//#ifdef _WIN32
#if 0

#ifndef CLOCK_GETTIME_H
#define CLOCK_GETTIME_H

#define CLOCK_REALTIME			0
#define CLOCK_MONOTONIC			1
#define CLOCK_PROCESS_CPUTIME_ID	2
#define CLOCK_THREAD_CPUTIME_ID		3


extern int clock_gettime(int X, struct timespec *tv);

#endif

#endif
