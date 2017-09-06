/*
 * mt_time.h
 *
 *  Created on: Oct 25, 2013
 *      Author: akoster
 */

#ifndef MT_TIME_H_
#define MT_TIME_H_

uint64_t timeval_subtract(struct timespec *result, struct timespec *end, struct timespec *begin);
void timeval_print(struct timespec *tv);

typedef struct timing
{
    struct timespec tvBegin,
                     tvEnd,
                     tvDiff,
                     tvStart;
    unsigned long long diff_in_nano;

} TIMING;

#endif /* MT_TIME_H_ */
