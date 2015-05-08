#include <stdint.h>
#include "block.h"

const uint8_t defaultKey[] =
{
  'p', 'a', 's', 's',
  'w', 'o', 'r', 'd',
  0, 0, 0, 0,
  0, 0, 0, 0
};

void initKey(Block key) {
  int i;
	for(i = 0; i < BLOCKSIZE; i++) {
    key[i] = defaultKey[i];
  }
}
