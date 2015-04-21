#ifndef DECRYPTION_H
#define DECRYPTION_H

void decrypt(Block a, const Block key);
void addRoundKey(Block a, const Block key);

#endif
