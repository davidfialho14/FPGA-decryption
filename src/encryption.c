#include "defines.h"

#ifndef BOARD

#include <stdint.h>
#include "block.h"
#include "encryption.h"
#include "decryption.h"
#include "tables.h"

void subBytes(Block a) {
  uint8_t i, j;
  for(i = 0; i < BLOCKLENGTH; ++i) {
    for(j = 0; j < BLOCKLENGTH; ++j) {
      a[BPOS(i, j)] = sbox[a[BPOS(i, j)]];
    }
  }
}

void shiftRows(Block a) {
/* On Big Endian CPU's, use RotateRowL */
  ROW(a, 1) = ROWROR(ROW(a, 1),8);
  ROW(a, 2) = ROWROR(ROW(a, 2),16);
  ROW(a, 3) = ROWROR(ROW(a, 3),24);
}

#define MULTBY2(x) ((x & 0x80) ? ((x) << 1) ^ 0x1B : ((x) << 1))

void mixColumns(Block a) {
  Block auxBlock;

  for(int j = 0; j < BLOCKLENGTH; ++j) {
    auxBlock[BPOS(0, j)] = MULTBY2(a[BPOS(0, j)]) ^ MULTBY2(a[BPOS(1, j)]) ^
                          a[BPOS(1, j)] ^ a[BPOS(2, j)] ^ a[BPOS(3, j)];
    auxBlock[BPOS(1, j)] = a[BPOS(0, j)] ^ MULTBY2(a[BPOS(1, j)]) ^
                          MULTBY2(a[BPOS(2, j)]) ^ a[BPOS(2, j)] ^ a[BPOS(3, j)];
    auxBlock[BPOS(2, j)] = a[BPOS(0, j)] ^ a[BPOS(1, j)] ^ MULTBY2(a[BPOS(2, j)]) ^
                          MULTBY2(a[BPOS(3, j)]) ^ a[BPOS(3, j)];
    auxBlock[BPOS(3, j)] = MULTBY2(a[BPOS(0, j)]) ^ a[BPOS(0, j)] ^ a[BPOS(1, j)] ^
                          a[BPOS(2, j)] ^ MULTBY2(a[BPOS(3, j)]);
  }

  memcpy(a, auxBlock, BLOCKSIZE);
}

void roundKey(Block key, uint8_t round) {
  uint8_t i;
  uint8_t col[BLOCKLENGTH]; // coluna auxiliar

  // rodar coluna
  for(i = 0; i < BLOCKLENGTH - 1; ++i) {
    col[i] = key[BPOS(i + 1, 3)];
  }
  col[3] = key[BPOS(0, 3)];

  // aplicar subbyte a cada byte da coluna
  for(i = 0; i < BLOCKLENGTH; ++i) {
    col[i] = sbox[col[i]];
  }

  // fazer XOR entre a ultima coluna e a primeira
  for(i = 0; i < BLOCKLENGTH; ++i) {
    col[i] = key[BPOS(i, 0)] ^ col[i];
  }

  // fazer xor entre o resultado anterior e a coluna de rcon e guardar resultado na primeira coluna
  key[BPOS(0, 0)] = col[0] ^ rcon[round];
  for(i = 1; i < BLOCKLENGTH; ++i) {
    key[BPOS(i, 0)] = col[i] ^ 0x00;
  }

  // fazer round das proximas 3 colunas
  uint8_t j;
  for(j = 1; j < BLOCKLENGTH; ++j) {
    for(i = 0; i < BLOCKLENGTH; ++i) {
      key[BPOS(i, j)] ^= key[BPOS(i, j - 1)];
    }
  }
}

void encrypt(Block a, Block key) {

  // ronda 0
  addRoundKey(a, key);

  // fazer 10 rondas
  uint8_t n;
  for(n = 1; n < 10; ++n) {
    roundKey(key, n);

    subBytes(a);
    shiftRows(a);
    mixColumns(a);

    addRoundKey(a, key);
  }

  // ultima ronda
  subBytes(a);
  shiftRows(a);
  roundKey(key, n);
  addRoundKey(a, key);
}

#endif
