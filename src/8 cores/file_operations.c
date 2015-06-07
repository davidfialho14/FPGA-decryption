#include <stdint.h>
#include <stdio.h>
#include "file_operations.h"
#include "xparameters.h"

#define SIZE_ADDR       0xB8000000
#define FILE_BASE_ADDR  0xB8000004
#define MEMSIZE			0x08000000

#define FILESIZE        *((int*) SIZE_ADDR)

#define CPU_COUNT	8
#define SYNC_ADDR 	0x43410000

int readBlock(int blockPosition, uint8_t **block) {
	int blockCount = FILESIZE / BLOCKSIZE + (FILESIZE % BLOCKSIZE > 0);

	if(blockPosition >= blockCount) {
		return 0;
	}

  *block = (uint8_t*) (FILE_BASE_ADDR + blockPosition * BLOCKSIZE);
  return 1;
}

int readMyBlock(int blockPosition, uint8_t **block) {
	int cpuPosition = blockPosition * CPU_COUNT + XPAR_CPU_ID;
	return readBlock(cpuPosition, block);
}

void startCPU() {
	uint8_t *m = (uint8_t*) (SYNC_ADDR + XPAR_CPU_ID);
	*m = 1;
}

int allStarted() {
	int i;
	for(i = 0; i < CPU_COUNT; i++) {
		uint8_t *done = (uint8_t*) (SYNC_ADDR + i);
		if(*done != 1) {
#if XPAR_CPU_ID == 7
			xil_printf("i:%d\n", i);
#endif
			return 0;
		}
	}

	return 1;
}

void finishCPU() {
	uint8_t *m = (uint8_t*) (SYNC_ADDR + XPAR_CPU_ID);
	*m = 2;
}

int allFinished() {

	int i;
	for(i = 0; i < CPU_COUNT; i++) {
		uint8_t *done = (uint8_t*) (SYNC_ADDR + i);
		if(*done != 2) {
			return 0;
		}
	}

	return 1;
}
