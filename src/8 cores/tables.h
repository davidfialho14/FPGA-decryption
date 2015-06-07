#ifndef TABLES_H
#define TABLES_H

#include <stdint.h>

#define RCONSIZE    256
#define SBOXSIZE    256

extern const uint8_t rcon[RCONSIZE];
extern const uint8_t sbox[SBOXSIZE];
extern const uint8_t invSbox[SBOXSIZE];

#endif // TABLES_H
