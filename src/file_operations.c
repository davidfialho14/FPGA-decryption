#include <stdint.h>
#include <stdio.h>
#include "file_operations.h"
#include "xparameters.h"

#define SIZE_ADDR       0xB8000000
#define FILE_BASE_ADDR  0xB8000004
#define MEMSIZE			0x08000000

#define FILESIZE        *((int*) SIZE_ADDR)

#define CPU_COUNT	1
#define SYNC_ADDR 	0x43410000

int readBlock(int blockPosition, uint8_t **block) {
	int blockCount = FILESIZE / BLOCKSIZE + (FILESIZE % BLOCKSIZE > 0);

	if(blockPosition >= blockCount - 1) {
		return 0;
	}

  *block = (uint8_t*) (FILE_BASE_ADDR + blockPosition * BLOCKSIZE);
  return 1;
}

int readMyBlock(int blockPosition, uint8_t **block) {
	int cpuPosition = blockPosition * 4 + XPAR_CPU_ID;
	return readBlock(cpuPosition, block);
}

void startCPU() {
//	uint8_t *sync = (uint8_t*) (SYNC_ADDR);
//	*sync = 0;
	uint8_t *m = (uint8_t*) (SYNC_ADDR + XPAR_CPU_ID + 1);
	*m = 1;
}

int allStarted() {
	int i;
	for(i = 0; i < CPU_COUNT; i++) {
		uint8_t *done = (uint8_t*) (SYNC_ADDR + i + 1);

		if(*done != 1) {
//#if XPAR_CPU_ID == 1
//			for(i = 0; i < CPU_COUNT + 1; i++) {
//				done = (uint8_t*) (SYNC_ADDR + i);
//				xil_printf("%d ", *done);
//			}
//			xil_printf("\n");
//#endif
			return 0;
		}
	}

//#if XPAR_CPU_ID == 1
//
//	xil_printf("all started\n");
//	for(i = 0; i < CPU_COUNT + 1; i++) {
//		uint8_t *done = (uint8_t*) (SYNC_ADDR + i);
//		xil_printf("%d ", *done);
//	}
//	xil_printf("\n");
//#endif

	return 1;
}

void toStart() {
	uint8_t *sync = (uint8_t*) (SYNC_ADDR);
#if XPAR_CPU_ID == 1
	while(!allStarted());
	*sync = 1;
//	xil_printf("sync : %d\n", *sync);
#else
	while((*sync) != 1);
#endif
}

void finishCPU() {
	uint8_t *m = (uint8_t*) (SYNC_ADDR + XPAR_CPU_ID + 1);
	*m = 2;
}

int allFinished() {

	int i;
	for(i = 0; i < CPU_COUNT; i++) {
		uint8_t *done = (uint8_t*) (SYNC_ADDR + i + 1);
		if(*done != 2) {

//#if XPAR_CPU_ID == 1
//			for(i = 0; i < CPU_COUNT + 1; i++) {
//				done = (uint8_t*) (SYNC_ADDR + i);
//				xil_printf("%d ", *done);
//			}
//			xil_printf("\n");
//#endif

			return 0;
		}
	}

	return 1;
}

void cleanSync() {
	int i;
	for(i = 0; i < CPU_COUNT+1; i++) {
		uint8_t *m = (uint8_t*) (SYNC_ADDR + i);
		*m = 0;
	}
}
