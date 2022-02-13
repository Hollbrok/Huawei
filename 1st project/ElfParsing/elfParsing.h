#ifndef ELFPARSING_H
#define ELFPARSING_H

#include <err.h>
#include <fcntl.h>

#include <stdio.h>
#include <gelf.h>
#include <stdint.h>
#include <stdlib.h>
#include <sysexits.h>
#include <unistd.h>
#include <gelf.h>

#include <stdlib.h>


#include "../includes/debug.h"
#include "../includes/common.h"

void elfPars(char *fileName);



#endif