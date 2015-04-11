#ifndef BLOCK_H
#define BLOCK_H

#define BLOCKLENGTH 4
#define BLOCKSIZE   16

#define BPOS(i, j) ((i) * BLOCKLENGTH + (j))

// Rotates by n bits a given uint_32 l
#define RotateRowR(l,n)	((l>>n)|(l<<(32-n)))
#define RotateRowL(l,n)	((l<<n)|(l>>(32-n)))

// Converts 4 consecutive bytes into 1 uint_32
#define byteToInt(block)	(*((uint32_t*)block))

typedef uint8_t Block[BLOCKSIZE];

void printBlock(Block block);
void initKey(Block key);
void initState(Block state);

#endif
