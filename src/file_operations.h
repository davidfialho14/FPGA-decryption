#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include "block.h"

#ifdef BOARD

#else
extern int inputFd;
extern int outputFd;
#endif

int readBlock(Block block, int);
int writeBlock(Block block, int);

#endif // FILE_OPERATIONS_H
