/*
 * io.h
 *
 *  Created on: Oct 25, 2013
 *      Author: akoster
 */

#ifndef IO_H_
#define IO_H_

size_t read_data(dsfmt_t * restrict dsfmt,unsigned long long counter, unsigned int num_elements, unsigned int data_size, uint64_t * restrict x,  FILE * fp);
size_t write_data(dsfmt_t * restrict dsfmt, unsigned long long counter, unsigned int num_elements, unsigned int data_size, uint64_t * restrict x, FILE * fp);


#endif /* IO_H_ */
