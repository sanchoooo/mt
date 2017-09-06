/*
 * common.h
 *
 *  Created on: Oct 25, 2013
 *      Author: akoster
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>

#include <math.h>

#include <time.h> //time sprintf
#include <sys/time.h> //gettimeofday

#include <unistd.h> //getopt
#include <stdbool.h>
#include "dSFMT-src-2.2/dSFMT.h"

 extern bool keepRunning;

#ifdef _USESSL
//for random data generation using ssh
#include <openssl/rand.h>
#endif

#ifdef _OPENMP
  #include <omp.h>
#else
  #define omp_get_thread_num() 0
#endif

typedef struct CONFIG_DATA
{
    int threads_per_name;
    char directory[512];  //Name of the device or file
    char name[256];  //Name of the device or file
    uint64_t counter;// Number of blocks to read/write
    uint32_t new_seed;
    uint32_t itterations;
    uint32_t seed;
    uint32_t array_size;//Defines block size. Basically number of elements in the dfsmt array
    bool WRITE_ONLY;
    bool READ_ONLY;
    FILE *fp;
    mode_t mode;
    int fd;
    dsfmt_t dsfmt;

} CONFIG_DATA;

#endif /* COMMON_H_ */

