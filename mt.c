#ifndef COMMON_H_
	#include "common.h"
#endif
#include "defines.h"
#include "io.h"
#include "mt_time.h"
#include "help.h"
#include "config_file.h"


static bool open_file(CONFIG_DATA cd[], unsigned int);
void intHandler(int);
bool keepRunning = true;

void intHandler(int dummy) {
    keepRunning = false;
    printf("SIGNAL caught:%d.  Waiting for threads to exit\n",dummy);
}

static bool open_file(CONFIG_DATA cd[], unsigned int element)
{
    char file_path[1024] = { 0 };

    if (NULL != cd[element].name){
       memcpy(&file_path,cd[element].directory, sizeof(file_path));
       strncat(file_path,cd[element].name, sizeof(cd[element].name));
    }
    printf("Attempting to open element:%d, to path %s\n",element,file_path);

    if (-1 != (cd[element].fd = open(file_path,O_RDWR  | O_CREAT  | O_NONBLOCK , cd[element].mode)))
    {
        printf("fd for open is :%d\n",cd[element].fd);
/*        if ((cd[element].fp=fdopen(cd[element].fd,"r+b")) != NULL)
        {
//           int rc=fseek(cd[element].fp,SEEK_END,0);
//           int device_size=ftell(cd[element].fp);
           rc=fseek(cd[element].fp,SEEK_SET,0);
        } else
        {
           printf("I couldn't fdopen %s \n",cd[element].name);
           printf("\n fdopen() failed with error [%s]\n",strerror(errno));
           return false;
        }*/
    } else {
        printf("I couldn't open %s \n",cd[element].name);
        printf("\n open() failed with error [%s]\n",strerror(errno));
        return false;
    }
    return true;
}

int main(int argc, char* argv[]) {
    /*394 is the smallest amount possible*/
    uint64_t loop=0,
             rc;
    size_t device_size=0,
           bytes_written=0,
           bytes_read=0;
    //double time_in_mill ;
    int c;
    unsigned int element=0;
    unsigned int number_threads=1;
//    extern char *optarg;

    TIMING times;
    CONFIG_DATA cd[MAX_THREADS];

    signal(SIGINT, intHandler);

/*openmp vars*/
    unsigned long nthreads, tid;
     while ((c = getopt(argc, argv, "c:f:i:n:rs:t:wx:?")) != -1) {
        switch(c) {
        case 'c':
            cd[element].counter=strtoul(optarg, NULL, 10);
            break;
        case 'f':
            strncpy(cd[element].directory, optarg, sizeof(cd[element].directory));
            break;
        case 'i':
            cd[element].itterations=strtoul(optarg, NULL, 10);
            break;
        case 'n':
            cd[element].array_size=strtoul(optarg, NULL, 10);
            if (cd[element].array_size < 394){
                printf("number of elements must be >=394\n");
                return 1;
            }
            break;
        case 'r':
            if (cd[element].WRITE_ONLY) {
                printf("read only and write only are mutually exclusive.  Can't set read only when write only is set\n");
                return 1;//TODO: Should not be return. Need to close files and free memory
            }
            cd[element].READ_ONLY=true;
            break;
        case 's':
            cd[element].new_seed=strtoul(optarg, NULL, 10);
            break;
        case 't':
            cd[element].threads_per_name=strtoul(optarg, NULL, 10);
            if (cd[element].threads_per_name < 1) { cd[element].threads_per_name = 1;}
            break;
        case 'w':
            if (cd[element].READ_ONLY) {
                printf("read only and write only are mutually exclusive.  Can't set write only when read only is set\n");
                return 1;
            }
            cd[element].WRITE_ONLY=true;
            break;
        case 'x':
            printf("config file specified. Options are reset to default\n");
            number_threads=get_config(cd, optarg);
            break;
        default:
        case '?':
            print_help(argv[0]);
            return 1;//TODO: Should not be return. Need to close files and free memory
            break;
        }
    }//while

     uint64_t x[cd[element].array_size];

#pragma omp parallel shared(cd,keepRunning) private(nthreads, tid, times, element, loop, rc, c, device_size, bytes_written, bytes_read, x  ) num_threads (number_threads)
  {
      /* Obtain thread number */
      tid = omp_get_thread_num();
      if (tid == 0)
        {
            nthreads = omp_get_num_threads();
            printf("Number of threads = %lu\n", nthreads);
        }
      printf("Thread %lu starting...\n",tid);

      /*set element to thread id*/
      element=tid;
      bytes_written=0;
      bytes_read=0;
      loop=0;

    #ifdef _USESSL
        char seed_buffer[sizeof(new_seed)];
        RAND_bytes(&seed_buffer,sizeof(seed_buffer));
        new_seed=strtoul(seed_buffer,NULL,10);
    #else
         if ((cd[element].fp=fopen( "/dev/urandom","r")) != NULL )
         {
             fread(&cd[element].new_seed,1,sizeof(cd[element].new_seed), cd[element].fp); //put random data into seed
             fclose(cd[element].fp);
             cd[element].fp=NULL;
         }
    #endif

    printf("counter: %llu, array_size: %u, seed: %u, device_size: %zu, WO:%d, RO:%d\n",cd[element].counter,cd[element].array_size,cd[element].new_seed,device_size,cd[element].WRITE_ONLY,cd[element].READ_ONLY);

    clock_gettime(CLOCK_REALTIME, &times.tvStart);
    timeval_print(&times.tvStart);

    while (( loop < cd[element].itterations ) && (open_file(cd,element)) && keepRunning)
    {
        bytes_written=0;
        bytes_read=0;
        loop++;
        printf("Starting loop: %llu of %u, seed: %u\n",loop,cd[element].itterations, cd[element].new_seed);

        if ((!cd[element].READ_ONLY) && (keepRunning)){
            /*Write new_seed to offet 0 */
            rc=fseek(cd[element].fp,SEEK_SET,0);

            if (rc) {
                printf("problem with seek 0\n");
            }
            rc=fwrite(&cd[element].new_seed,1,sizeof(uint32_t),cd[element].fp);
            dsfmt_init_gen_rand(&cd[element].dsfmt, cd[element].new_seed);
            clock_gettime(CLOCK_MONOTONIC, &times.tvBegin);
            bytes_written+=write_data(&cd[element].dsfmt, cd[element].counter, cd[element].array_size, sizeof(x[0]), x, cd[element].fp);
            clock_gettime(CLOCK_MONOTONIC, &times.tvEnd);

            times.diff_in_nano =  timeval_subtract(&times.tvDiff, &times.tvEnd, &times.tvBegin);
            printf("wrote: %zu bytes @ %.5lf MiB/sec in %llu nano seconds\n", bytes_written,
                    (bytes_written/((double)times.diff_in_nano/NANO_SEC))/MB_CONVERSTION,times.diff_in_nano);
        }

        if ((!cd[element].WRITE_ONLY)  && (keepRunning))
        {
            /*first long bytes are used for seeding the read*/
            rc=fseek(cd[element].fp,SEEK_SET,0);
            rc=fread(&cd[element].seed,1,sizeof(uint32_t),cd[element].fp);
            dsfmt_init_gen_rand(&cd[element].dsfmt, (uint32_t)cd[element].seed);

            /*Read seed for next itteration*/
            rc=fread(&cd[element].new_seed,1,sizeof(uint32_t),cd[element].fp);

            /*reset to 0 and read first long bytes to reset pointer*/
            rc=fseek(cd[element].fp,SEEK_SET,0);
            rc=fread(&cd[element].seed,1,sizeof(uint32_t),cd[element].fp);

            clock_gettime(CLOCK_MONOTONIC, &times.tvBegin);
            bytes_read+=read_data(&cd[element].dsfmt,  cd[element].counter, cd[element].array_size, sizeof(x[0]), x, cd[element].fp);
            clock_gettime(CLOCK_MONOTONIC, &times.tvEnd);

            times.diff_in_nano = timeval_subtract(&times.tvDiff, &times.tvEnd, &times.tvBegin);

            printf("read: %zu bytes @ %.5lf MiB/sec in %llu nano seconds\n",bytes_read,(bytes_read/((double)times.diff_in_nano/NANO_SEC))/MB_CONVERSTION,times.diff_in_nano);
        }
        printf("closing device\n");
       if (fclose(cd[element].fp))
       { printf("There was a problem closing the file %s for thread %d\n",cd[element].name,element);
       }
    }
    clock_gettime(CLOCK_REALTIME, &times.tvEnd);
    timeval_print(&times.tvEnd);

    printf("Preformed %llu operations in ",loop);
    // diff
    times.diff_in_nano = timeval_subtract(&times.tvDiff, &times.tvEnd, &times.tvStart);
    printf("%.5lf seconds\n",(double)times.diff_in_nano/NANO_SEC);

   printf("closing device\n");
   fclose(cd[element].fp);
  }/*OPENMP pragma*/

   return 0;//TODO: Should not be return. Need to close files and free memory

}
