#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include "block.h"

#ifndef BOARD
extern int inputFd;
extern int outputFd;
#endif

int readBlock(int blockPosition, Block block);
int writeBlock(int blockPosition, Block block);

#endif
