/*
 * mt_time.c
 *
 *  Created on: Oct 25, 2013
 *      Author: akoster
 */
#ifndef COMMON_H_
	#include "common.h"
#endif
#include "defines.h"
#include "mt_time.h"



/* Return 1 if the difference is negative, otherwise 0.  */
uint64_t timeval_subtract(struct timespec *result, struct timespec *end, struct timespec *begin)
{
    uint64_t diff = (end->tv_nsec + (end->tv_sec * NANO_SEC )) - (begin->tv_nsec + (begin->tv_sec * NANO_SEC ));
    result->tv_sec = diff / NANO_SEC;
    result->tv_nsec = diff % NANO_SEC;

    return (diff);
}

void timeval_print(struct timespec *tv)
{
    char buffer[30];
    time_t curtime;
    long  ms; // Milliseconds
//    double time_in_mill ;

    ms = round(tv->tv_nsec / 1.0e6); // Convert nanoseconds to milliseconds

//    time_in_mill = (tv.tv_sec) * 1000 + ms;

    printf("%ld.%06ld", tv->tv_sec, ms);
    curtime = tv->tv_sec;
    strftime(buffer, 30, "%m-%d-%Y  %T", localtime(&curtime));
    printf(" = %s.%06ld\n", buffer, ms);
}

