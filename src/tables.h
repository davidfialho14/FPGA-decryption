#ifndef TABLES_H
#define TABLES_H

#ifndef BOARD
#include <stdint-gcc.h>
#else
#include <stdint.h>
#endif

#define RCONSIZE    256
#define SBOXSIZE    256

extern const uint8_t rcon[RCONSIZE];
extern const uint8_t sbox[SBOXSIZE];
extern const uint8_t invSbox[SBOXSIZE];

#endif // TABLES_H

