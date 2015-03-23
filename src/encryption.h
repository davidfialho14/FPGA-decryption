#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#define SBOXSIZE    256

extern const uint8_t sbox[SBOXSIZE];

void encrypt(Block a, Block key);
void addRoundKey(Block a, const Block key);

#endif
