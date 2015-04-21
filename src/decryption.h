#ifndef DECRYPTION_H
#define DECRYPTION_H

#include "block.h"

void decrypt(Block a, const Block key);

#ifndef BOARD
void addRoundKey(Block a, const Block key);
#endif

#endif
