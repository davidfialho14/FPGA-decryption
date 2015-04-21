#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include "block.h"

extern int inputFd;
extern int outputFd;

int readBlock(int blockPosition, Block block);
int writeBlock(int blockPosition, Block block);

#endif
