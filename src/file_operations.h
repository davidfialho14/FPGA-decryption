#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include "block.h"

int readBlock(int blockPosition, uint8_t **block);
int readMyBlock(int blockPosition, uint8_t **block);

void startCPU();
int allStarted();
void toStart();
void finishCPU();
int allFinished();
void cleanSync();

#endif
