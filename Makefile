# @file  Makefile
# @brief Makefile
#
# @author Mutsuo Saito (Hiroshima University)
# @author Makoto Matsumoto (Hiroshima University)
#
# Copyright (C) 2007, 2008 Mutsuo Saito, Makoto Matsumoto and
# Hiroshima University. All rights reserved.
#
# The new BSD License is applied to this software.
# see LICENSE.txt



WARN = -Wmissing-prototypes -Wall -Wextra #-v -Winline 
OPTI = -O3 -finline-functions -fomit-frame-pointer -DNDEBUG \
-fno-strict-aliasing --param max-inline-insns-single=1800 -s
TUNE = -mtune=native 
STD = -std=gnu99 -D_GNU_SOURCE
DEBUG = -g
CC = gcc
CCFLAGS = $(OPTI) $(WARN) $(STD) -D DSFMT_MEXP=19937 ${USESSL} ${DEBUG} -fopenmp
LDFLAGS = ${LIBS} ${LDADD}
LIBS = -lgomp 
LIBPATH = -L/opt/freeware/lib64/ -L/opt/freeware/lib/
SSE2FLAGS = -msse2 -DHAVE_SSE2 ${VECTOR_MATH} 
AVX_MATH =# -mavx #AVX for sandy bridge processors
VECTOR_MATH =# -ftree-vectorize -ftree-vectorizer-verbose=2 ${AVX_MATH} #-fdump-tree-vect-details
LDADD=-lrt -lm -lgomp -lpthread  -lcrypt
PPC_VECTOR_MATH = -maltivec ${VECTOR_MATH} -mpowerpc-gpopt -mpowerpc-gfxopt -mpowerpc64 -maix64 -mvsx -mcpu=power7
USESSL = #-D_USESSL

SRCS := $(wildcard *.c)
DEPS := $(wildcard *.h)

mt: $(DEPS)
	${CC} ${CCFLAGS} ${TUNE} ${SSE2FLAGS} -o $@ $(SRCS) ./dSFMT-src-2.2/dSFMT.c ${LDFLAGS}
#-DSINGLETHREADED
mt-ppc: $(DEPS)
	${CC} ${CCFLAGS} -maix64 ${LIBPATH} -static -o $@ *.c ./dSFMT-src-2.2/dSFMT.c ${LIBS} ${LDFLAGS}
mt-perf-ppc:
	${CC} ${CCFLAGS} -maix64 ${LIBPATH} -static  ${PPC_VECTOR_MATH} -o $@ *.c ./dSFMT-src-2.2/dSFMT.c ${LDFLAGS}

#mt-perf: $(SRC)
#	${CC} ${CCFLAGS} ${SSE2FLAGS} ${X86}  -g -o $@ mt-perf.c ./dSFMT-src-2.2/dSFMT.c
#	${CC} ${CCFLAGS} ${SSE2FLAGS} ${AVX_MATH} ${VECTOR_MATH} ${X86}  -g -o mt-perf-vec mt-perf.c ./dSFMT-src-2.2/dSFMT.c
#	${CC} ${CCFLAGS} ${X86}  -o mt-perf-nonsse.exe mt-perf.c ./dSFMT-src-2.2/dSFMT.c
#	${CC} ${CCFLAGS} ${AVX_MATH} ${VECTOR_MATH} ${X86}  -o mt-perf-nonsse-vec.exe mt-perf.c ./dSFMT-src-2.2/dSFMT.c

#mt-perf-ppc:
#	${CC} ${CCFLAGS} ${DEBUG} ${LDADD} ${PPC_VECTOR_MATH} -o $@ mt-perf.c ./dSFMT-src-2.2/dSFMT.c


clean:
	rm -f *.o *.exe* ./dSFMT-src-2.2/*.o mt mt-ppc mt-perf-ppc data*.hex
