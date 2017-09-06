/*
 * help.c
 *
 *  Created on: Oct 25, 2013
 *      Author: akoster
 */
#ifndef COMMON_H_
	#include "common.h"
#endif
#include "help.h"


void print_help(char* argv)
{
    printf( "%s options\n"
            "-c      number times to write/read -n elements\n"
            "-f      file or device to run against\n"
            "-i      number of itterations to run of -c write/reads of -n size\n"
            "-n      number of MT19937 array elements(8 byte values[uint64_t]) to generate min is 394, default is 8192\n"
            "-r      read(verify) the device and exit\n"
            "-s      starting seed, default is to read urandom for a value\n"
            "-t      number of threads >= 1 to run\n"
            "-w      write only.\n"
            "-x      specify config file for input.  If you use this, all other options are negated.\n"
            "-?      this help\n"
            "\n"
            "config file options:\n"
            "COUNT        same as -c\n"
            "PATH         /path/to/file/creation/\n"
            "FILE         filename to write to\n"
            "ITTER        same as -i\n"
            "SEED         same as -s\n"
            "ARRAY_SIZE   same as -n\n"
            "THREADS      number of threads to create per path.  Will automatically create filenames.\n"
            "\n"
            "The concept behind this app is to use the SIMD(x86) or ALTIVEC(power) to generate\n"
            "the random data the application will use to write.  This free's up more processor time\n"
            "for the io calls and more instances to be run.  The initial seed starts the sequence\n"
            "and each following seed(uint32_t) is derived from the first set of random keys generated.\n"
            "Using a Mersanne twister implementation for the random data, the data written is\n"
            "double(8 bytes) in size.  So the data is self verifying.  If the seed is written incorrectly\n"
            "the data won't verify.\n\n"
            "The seed is a long type but only uint32_t is used\n",argv);
}
