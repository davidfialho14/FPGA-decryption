#include <unistd.h>
#include "file_operations.h"
#include "block.h"

int inputFd = -1;
int outputFd = -1;

int readBlock(int blockPosition, Block block) {
  return (read(inputFd, block, BLOCKSIZE) > 0);
}

int writeBlock(int blockPosition, Block block) {
  return (write(outputFd, block, BLOCKSIZE) > 0);
}
