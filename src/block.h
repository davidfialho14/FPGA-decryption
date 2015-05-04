#ifndef BLOCK_H
#define BLOCK_H

#include <stdint.h>

#define BLOCKLENGTH 4
#define BLOCKSIZE   16

#define BPOS(i, j) ((i) * BLOCKLENGTH + (j))

// Rotates by n bits a given uint_32 number
#define ROWROR(l,n)	((l>>n)|(l<<(32-n)))
#define ROWROL(l,n)	((l<<n)|(l>>(32-n)))

// Converts a line of the block into a uint_32 number
#define ROW(block, i)	(*((uint32_t*) &block[BPOS((i),0)]))

typedef uint8_t Block[BLOCKSIZE];

void printBlock(Block block);
void initKey(Block key);
void initState(Block state);

#endif
