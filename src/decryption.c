#include <stdio.h>
#include "block.h"
#include "tables.h"
#include "decryption.h"

// xtime is a macro that finds the product of {02} and the argument to xtime modulo {1b}
#define xtime(x)   ((x<<1) ^ (((x>>7) & 1) * 0x1b))

// Multiplty is a macro used to multiply numbers in the field GF(2^8)
#define Multiply(x,y) (((y & 1) * x) ^ ((y>>1 & 1) * xtime(x)) ^ ((y>>2 & 1) * xtime(xtime(x))) ^ ((y>>3 & 1) * xtime(xtime(xtime(x)))) ^ ((y>>4 & 1) * xtime(xtime(xtime(xtime(x))))))

void invSubBytes(Block a) {
  uint8_t i, j;
  for(i = 0; i < BLOCKLENGTH; ++i) {
    for(j = 0; j < BLOCKLENGTH; ++j) {
      a[BPOS(i, j)] = invSbox[a[BPOS(i, j)]];
    }
  }
}

void invShiftRows(Block a) {
/* On Big Endian CPU's, use RotateRowR */
  ROW(a, 1) = ROWROL(ROW(a, 1),8);
  ROW(a, 2) = ROWROL(ROW(a, 2),16);
  ROW(a, 3) = ROWROL(ROW(a, 3),24);
}

void invMixColumns(Block a) {
  uint8_t aux[BLOCKLENGTH];
  uint8_t i, j;
  for(j = 0; j < BLOCKLENGTH; ++j) {
    for(i = 0; i < BLOCKLENGTH; ++i) {
      aux[i] = a[BPOS(i, j)];
    }

    a[BPOS(0, j)] = Multiply(aux[0], 0x0e) ^ Multiply(aux[1], 0x0b) ^ Multiply(aux[2], 0x0d) ^ Multiply(aux[3], 0x09);
    a[BPOS(1, j)] = Multiply(aux[0], 0x09) ^ Multiply(aux[1], 0x0e) ^ Multiply(aux[2], 0x0b) ^ Multiply(aux[3], 0x0d);
    a[BPOS(2, j)] = Multiply(aux[0], 0x0d) ^ Multiply(aux[1], 0x09) ^ Multiply(aux[2], 0x0e) ^ Multiply(aux[3], 0x0b);
    a[BPOS(3, j)] = Multiply(aux[0], 0x0b) ^ Multiply(aux[1], 0x0d) ^ Multiply(aux[2], 0x09) ^ Multiply(aux[3], 0x0e);
  }
}

void invRoundKey(const Block key, Block roundKey, uint8_t round) {
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
  roundKey[BPOS(0, 0)] = col[0] ^ rcon[round];
  for(i = 1; i < BLOCKLENGTH; ++i) {
    roundKey[BPOS(i, 0)] = col[i] ^ 0x00;
  }

  // fazer round das proximas 3 colunas
  uint8_t j;
  for(j = 1; j < BLOCKLENGTH; ++j) {
    for(i = 0; i < BLOCKLENGTH; ++i) {
      roundKey[BPOS(i, j)] = key[BPOS(i, j)] ^ roundKey[BPOS(i, j - 1)];
    }
  }
}

void expansionKey(const Block key, Block roundKeys[10]) {
  uint8_t n;

  // primeira ronda
  invRoundKey(key, roundKeys[0], 1);

  // restantes rondas
  for(n = 1; n < 10; ++n) {
    invRoundKey(roundKeys[n - 1], roundKeys[n], n+1);
  }
}

void addRoundKey(Block a, const Block key) {
  uint8_t i, j;
  for(i = 0; i < BLOCKLENGTH; ++i) {
    for(j = 0; j < BLOCKLENGTH; ++j) {
      a[BPOS(i, j)] ^= key[(BPOS(i, j))];
    }
  }
}

void decrypt(Block a, const Block key) {

  Block roundKeys[10];
  expansionKey(key, roundKeys);

  // ronda 0
  addRoundKey(a, roundKeys[9]);

  // fazer 9 rondas
  int8_t n;
  for(n = 8; n >= 0; --n) {
    invShiftRows(a);
    invSubBytes(a);
    addRoundKey(a, roundKeys[n]);
    invMixColumns(a);
  }

  // ultima ronda
  invShiftRows(a);
  invSubBytes(a);
  addRoundKey(a, key);
}
