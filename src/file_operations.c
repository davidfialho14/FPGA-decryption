#include "file_operations.h"

#ifdef BOARD

int readNextBlock(Block block, int /*ignorado*/) {
	return (read(fdIn, block, sizeof(block)) > 0);
}

int writeBlock(Block block, int blockPosition) {
	return (write(fdOut, state, sizeof(state)) <= 0);
}

#else

#include <unistd.h>

int inputFd;		// fd do ficheiro de leitura
int outputFd;		// fd do ficheiro de escrita

int readBlock(Block block, int blockPosition /*ignorado*/) {
	return (read(inputFd, block, sizeof(block)) > 0);
}

int writeBlock(Block block, int blockPosition /*ignorado*/) {
	return (write(outputFd, block, sizeof(block)) > 0);
}

#endif

