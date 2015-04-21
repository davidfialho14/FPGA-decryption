#include <unistd.h>
#include "file_operations.h"

int inputFd = -1;
int outputFd = -1;

int readBlock(int blockPosition, Block block) {
  return (read(inputFd, block, sizeof(block)) > 0);
}

int writeBlock(int blockPosition, Block block) {
  return (write(outputFd, block, sizeof(block)) > 0);
}
