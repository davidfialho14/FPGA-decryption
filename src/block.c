#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "block.h"

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
  uint8_t i, j;
  for(i = 0; i < BLOCKLENGTH; ++i) {
    for(j = 0; j < BLOCKLENGTH; ++j) {
      block[BPOS(i, j)] = (uint8_t) (rand() % 256);
    }
  }
}
