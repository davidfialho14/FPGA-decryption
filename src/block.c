#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "block.h"

const uint8_t defaultKey[] =
{
  0x2b, 0x28, 0xab, 0x09,
  0x7e, 0xae, 0xf7, 0xcf,
  0x15, 0xd2, 0x15, 0x4f,
  0x16, 0xa6, 0x88, 0x3c
};

const uint8_t defaultState[] =
{
  0x32, 0x88, 0x31, 0xe0,
  0x43, 0x5a, 0x31, 0x37,
  0xf6, 0x30, 0x98, 0x07,
  0xa8, 0x8d, 0xa2, 0x34
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

void initKey(Block key) {
  memcpy(key, defaultKey, BLOCKSIZE);
}

void initState(Block state) {
  memcpy(state, defaultState, BLOCKSIZE);
}
