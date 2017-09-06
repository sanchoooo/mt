# mt
IO generator using OMP

This is a test app I have put together to play with the GCC OpenMP framework.
The goal of the app is to use OMP for the creation of multiple threads of IO

The secondary goal of this app is to use the SSE/AVX engine to generate the random
data for the threads.  Moving the data generation to the SSE/AVX engine allows for
more CPU availability for IO operations.  I've used the
Double precision SIMD-oriented Fast Mersenne Twister (dSFMT) from the
University of Tokyo (http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/SFMT/#dSFMT)

Utilizing the dSFMT which has been optimized for SSE/AVX I've been able to achieve
10GBytes/sec of random data generation.

Initially written for centos, ported to AIX 6

$ ./mt -?
./mt options
-c      number times to write/read -n elements
-f      file or device to run against
-i      number of itterations to run of -c write/reads of -n size
-n      number of MT19937 array elements(8 byte values[uint64_t]) to generate min is 394, default is 8192
-r      read(verify) the device and exit
-s      starting seed, default is to read urandom for a value
-t      number of threads >= 1 to run
-w      write only.
-x      specify config file for input.  If you use this, all other options are negated.
-?      this help

config file options:
COUNT        same as -c
PATH         /path/to/file/creation/
FILE         filename to write to
ITTER        same as -i
SEED         same as -s
ARRAY_SIZE   same as -n
THREADS      number of threads to create per path.  Will automatically create filenames.

The concept behind this app is to use the SIMD(x86) or ALTIVEC(power) to generate
the random data the application will use to write.  This free's up more processor time
for the io calls and more instances to be run.  The initial seed starts the sequence
and each following seed(uint32_t) is derived from the first set of random keys generated.
Using a Mersanne twister implementation for the random data, the data written is
double(8 bytes) in size.  So the data is self verifying.  If the seed is written incorrectly
the data won't verify.

The seed is a long type but only uint32_t is used
