#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "block.h"

const uint8_t defaultBlock[] =
{
  0x2b, 0x28, 0xab, 0x09,
  0x7e, 0xae, 0xf7, 0xcf,
  0x15, 0xd2, 0x15, 0x4f,
  0x16, 0xa6, 0x88, 0x3c
};

void printBlock(Block block) {
  uint8_t i, j;
  for(i = 0; i < BLOCKLENGTH; ++i) {
    for(j = 0; j < BLOCKLENGTH; ++j) {
      printf("0x%02X\t", block[BPOS(i, j)]);
    }
    putchar('\n');
  }
}

void initBlock(Block block) {
  memcpy(block, defaultBlock, BLOCKSIZE);
}
