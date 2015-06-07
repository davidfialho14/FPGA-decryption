#include <stdint.h>
#include "file_operations.h"

#define SIZE_ADDR       0xAC000000
#define FILE_BASE_ADDR  0xAC000004
#define MEMSIZE			    0x08000000

#define FILESIZE        *((int*) SIZE_ADDR)

int readBlock(int blockPosition, uint8_t **block) {
	int blockCount = FILESIZE / BLOCKSIZE + (FILESIZE % BLOCKSIZE > 0);

	if(blockPosition >= blockCount) {
		return 0;
	}

  *block = (uint8_t*) (FILE_BASE_ADDR + blockPosition * BLOCKSIZE);
  return 1;
}
