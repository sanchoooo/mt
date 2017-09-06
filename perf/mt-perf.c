#include "dSFMT-src-2.2/dSFMT.h"
#include <math.h>
#include <sys/time.h>
#include <stdlib.h> //strtol
#include <unistd.h> //getopt
long  int timeval_subtract(struct timeval *result, struct timeval *t2, struct timeval *t1);
void timeval_print(struct timeval *tv);


/* Return 1 if the difference is negative, otherwise 0.  */
long  int timeval_subtract(struct timeval *result, struct timeval *t2, struct timeval *t1)
{
    long  int diff = (t2->tv_usec + 1000000 * t2->tv_sec) - (t1->tv_usec + 1000000 * t1->tv_sec);
    result->tv_sec = diff / 1000000;
    result->tv_usec = diff % 1000000;

    return (diff<0);
}

void timeval_print(struct timeval *tv)
{
    char buffer[30];
    time_t curtime;

    printf("%ld.%06ld", tv->tv_sec, tv->tv_usec);
    curtime = tv->tv_sec;
    strftime(buffer, 30, "%m-%d-%Y  %T", localtime(&curtime));
    printf(" = %s.%06ld\n", buffer, tv->tv_usec);
}

int main(int argc, char* argv[]) {
    /*394 is the smallest amount possible*/
    unsigned int array_size=394;
    uint32_t seed=0;

    unsigned long counter=1000000, loop=1;

    dsfmt_t dsfmt;
    struct timeval tvBegin, tvEnd, tvDiff;
    int c;

    while ((c = getopt(argc, argv, "c:n:s:?")) != -1) {
        switch(c) {
        case 'c':
            counter=strtoul(optarg, NULL, 10);
            break;
        case 'n':
            array_size=strtoul(optarg, NULL, 10);
            if (array_size < 394){
                printf("number of elements must be >=394\n");
                return 1;
            }
            break;
        case 's':
            seed=strtoul(optarg, NULL, 10);
            break;
        case '?':
            printf( "%s options\n"
                    "-c      number of times to run\n"
                    "-n      number of MT19937 array elements to generate\n"
                    "-s      starting seed\n"
                    "-?      this help\n",argv[0]);
            return 1;
            break;
        default:
            return 1;

        }
    }//while

    printf("counter: %lu, array_size: %u, seed: %u\n",counter,array_size,seed);
    double x[array_size];


    dsfmt_init_gen_rand(&dsfmt, seed);
    printf("Start: ");
    // begin
    gettimeofday(&tvBegin, NULL);
    timeval_print(&tvBegin);
    while( loop < counter )
    {
        dsfmt_fill_array_close1_open2(&dsfmt,x,array_size);
        loop++;
    }
    printf("End:   ");
    gettimeofday(&tvEnd, NULL);
    timeval_print(&tvEnd);

    printf("Preformed %lu operations in ",loop);
    // diff
    timeval_subtract(&tvDiff, &tvEnd, &tvBegin);
    printf("%ld.%06ld\n", tvDiff.tv_sec, tvDiff.tv_usec);

    if (1 > tvDiff.tv_sec ) tvDiff.tv_sec=1;
    printf("%lu operations/sec at %u bytes/operation\n",(loop/tvDiff.tv_sec),sizeof(x));
    printf("%lu MiB/sec\n",((((loop / tvDiff.tv_sec)/1024) * (unsigned long)sizeof(x) ) / 1024));


   return 0;
}
