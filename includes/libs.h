#ifndef LIBS_H_INC
#define LIBS_H_INC



////////

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>

#include <assert.h>

#include <bitset>


///////

#ifndef _GNU_SOURCE
    #define _GNU_SOURCE
#endif 

#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include <limits.h>
#include <errno.h>

#include <fcntl.h>
#include <unistd.h>

#endif