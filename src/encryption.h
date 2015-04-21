#ifndef ENCRYPTION_H
#define ENCRYPTION_H

void encrypt(Block a, Block key);
void addRoundKey(Block a, const Block key);

#endif
