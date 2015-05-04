#include <stdint.h>
#include "file_operations.h"

#define SIZE_ADDR       0xAC000000
#define FILE_BASE_ADDR  0xAC000004
#define MEMSIZE			    0x08000000

#define FILESIZE        *((int*) SIZE_ADDR)

static int blockCount = FILESIZE / BLOCKSIZE + (FILESIZE % BLOCKSIZE > 0);

int readBlock(int blockPosition, Block block) {
  block = (uint8_t*) (FILE_BASE_ADDR + blockPosition * BLOCKSIZE);
  return blockRead;
}
