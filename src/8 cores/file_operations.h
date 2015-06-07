#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include "block.h"

int readBlock(int blockPosition, uint8_t **block);
int readMyBlock(int blockPosition, uint8_t **block);

void startCPU();
int allStarted();
void finishCPU();
int allFinished();

#endif
