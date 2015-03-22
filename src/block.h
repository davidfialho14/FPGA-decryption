#ifndef BLOCK_H
#define BLOCK_H

#define BLOCKLENGTH 4
#define BLOCKSIZE   16

#define BPOS(i, j) ((i) * BLOCKLENGTH + (j))

typedef uint8_t Block[BLOCKSIZE];

void printBlock(Block block);
void initKey(Block key);
void initState(Block state);

#endif
