/*
 * io.c
 *
 *  Created on: Oct 25, 2013
 *      Author: akoster
 */
#ifndef COMMON_H_
	#include "common.h"
#endif

#include "io.h"

/*
 * write_data
 *  fp - file pointer
dsfmt - pointer to array
counter - how many times to itterate
element_length - size of the array
data_size - size of a single element
x pointer to the array to compare to
 */
size_t write_data(dsfmt_t * restrict dsfmt, unsigned long long counter, unsigned int num_elements, unsigned int data_size, uint64_t * restrict x, FILE * fp)
{
    uint64_t rc,y;
    size_t bytes=0;
    for (y=0; y < counter; y++)
    {
        dsfmt_fill_array_close1_open2(dsfmt,(double*)x,num_elements);
        rc=fwrite(x,data_size,num_elements,fp);
        bytes+=rc;
        if ((rc != num_elements) || (!keepRunning)) {
            printf("Breaking at %llu for rc:%llu, keepRunning:%d\n",y,rc,keepRunning);
            break;
        }
    }
return bytes * data_size;
}

/* read_data
fp - file pointer
dsfmt - pointer to array
counter - how many times to itterate
element_length - size of the array
data_size - size of a single element
x pointer to the array to compare to
*/
size_t read_data(dsfmt_t * restrict dsfmt,unsigned long long counter, unsigned int num_elements, unsigned int data_size, uint64_t * restrict x,  FILE * fp)
{
    uint64_t  rc,y;
    uint64_t z[num_elements]; //This is what the data is read into


    size_t bytes=0;
    for (y=0; y < counter; y++)
    {
        dsfmt_fill_array_close1_open2(dsfmt,(double*)x,num_elements);
        rc=fread(z,data_size,num_elements,fp);
        if ((rc == num_elements) && (keepRunning)) {
            bytes+=rc;
            if ( memcmp(x, z, sizeof(z))) {
                printf("Failed on compare %llu on rc:%llu, keepRunning:%d\n",y,rc,keepRunning);
                rc=fclose(fp);
                goto exit;
            }
        } else {
            printf("Read rc %llu counter: %llu, keepRunning:%d\n",rc,y,keepRunning);
            bytes+=rc;
            goto exit;
        }
    }
    exit:
    return bytes * data_size;
}
