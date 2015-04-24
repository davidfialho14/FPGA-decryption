#include <stdio.h>
#include "block.h"
#include "tables.h"
#include "decryption.h"
#include "fsl.h"

#define MULTBY2(x) ((x & 0x80) ? ((x) << 1) ^ 0x1B : ((x) << 1))
#define MULTBY4(x) (MULTBY2(MULTBY2(x)))
#define MULTBY8(x) (MULTBY2(MULTBY4(x)))
#define MULTBYE(x) (MULTBY8(x) ^ MULTBY4(x) ^ MULTBY2(x))
#define MULTBYB(x) (MULTBY8(x) ^ MULTBY2(x) ^ (x))
#define MULTBYD(x) (MULTBY8(x) ^ MULTBY4(x) ^ (x))
#define MULTBY9(x) (MULTBY8(x) ^ (x))

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

#ifdef BOARD
void invMixColumns(Block a) {
	int i, aux = -1;
	for(i = 0; i < 12; i += 4) {
		aux = *((int*) &(a[i]));
		putfsl(aux, 0);
	}

	aux = *((int*) &(a[12]));
	cputfsl(aux, 0);

	for(i = 0; i < 16; i += 4) {
		getfsl(aux, 0);
		*((int*)&a[i]) = aux;
	}
}
#else
void invMixColumns(Block a) {
  Block aux;
  int j;
  for( j = 0; j < BLOCKLENGTH; ++j) {
    aux[BPOS(0, j)] = MULTBYE(a[BPOS(0, j)]) ^ MULTBYB(a[BPOS(1, j)]) ^ MULTBYD(a[BPOS(2, j)]) ^ MULTBY9(a[BPOS(3, j)]);
    aux[BPOS(1, j)] = MULTBY9(a[BPOS(0, j)]) ^ MULTBYE(a[BPOS(1, j)]) ^ MULTBYB(a[BPOS(2, j)]) ^ MULTBYD(a[BPOS(3, j)]);
    aux[BPOS(2, j)] = MULTBYD(a[BPOS(0, j)]) ^ MULTBY9(a[BPOS(1, j)]) ^ MULTBYE(a[BPOS(2, j)]) ^ MULTBYB(a[BPOS(3, j)]);
    aux[BPOS(3, j)] = MULTBYB(a[BPOS(0, j)]) ^ MULTBYD(a[BPOS(1, j)]) ^ MULTBY9(a[BPOS(2, j)]) ^ MULTBYE(a[BPOS(3, j)]);
  }

  memcpy(a, aux, BLOCKSIZE);
}
#endif

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
