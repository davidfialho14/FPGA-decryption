#include <stdint.h>
#include <stdio.h>
#include "block.h"
#include "rcon.h"
#include "encryption.h"

// xtime is a macro that finds the product of {02} and the argument to xtime modulo {1b}
#define xtime(x)   ((x<<1) ^ (((x>>7) & 1) * 0x1b))

// Multiplty is a macro used to multiply numbers in the field GF(2^8)
#define Multiply(x,y) (((y & 1) * x) ^ ((y>>1 & 1) * xtime(x)) ^ ((y>>2 & 1) * xtime(xtime(x))) ^ ((y>>3 & 1) * xtime(xtime(xtime(x)))) ^ ((y>>4 & 1) * xtime(xtime(xtime(xtime(x))))))

// Rotates by n bytes a given uint_32 l
#define RotateRowR(l,n)	((l>>(n<<3))|(l<<(32-(n<<3))))
#define RotateRowL(l,n)	((l<<(n<<3))|(l>>(32-(n<<3))))

// Converts 4 bytes into 1 uint_32
#define byteToInt(block)	(*((uint32_t*)block))

extern const uint8_t rcon[RCONSIZE];
extern const uint8_t sbox[SBOXSIZE];

const uint8_t invSbox[256] =
{
   0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB,
   0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87, 0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB,
   0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D, 0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E,
   0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2, 0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25,
   0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92,
   0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA, 0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84,
   0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A, 0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06,
   0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02, 0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B,
   0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA, 0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73,
   0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85, 0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E,
   0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89, 0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B,
   0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20, 0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4,
   0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31, 0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F,
   0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D, 0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF,
   0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0, 0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61,
   0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26, 0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D
};

/* UNNECESSARY ON BIG ENDIAN CPU's!!! */
void LtoB(uint8_t * line) // converts a line of 4 bytes from LE to BE
{
	uint8_t aux = line[0];
	line[0] = line[3];
	line[3] = aux;
	aux = line[1];
	line[1] = line[2];
	line[2] = aux;
}

void BtoL(uint8_t * line) // converts a line of 4 bytes from LE to BE
{
	uint8_t aux = line[0];
	line[0] = line[3];
	line[3] = aux;
	aux = line[1];
	line[1] = line[2];
	line[2] = aux;
}

void invShiftRows(Block a) {
  printf("\n\n\n\n");
  printf("Rotated L0:\t%x %x %x %x\n", a[BPOS(0,0)], a[BPOS(0,1)], a[BPOS(0,2)], a[BPOS(0,3)]);
  printf("Rotated L1:\t%x %x %x %x\n", a[BPOS(1,0)], a[BPOS(1,1)], a[BPOS(1,2)], a[BPOS(1,3)]);
  printf("Rotated L2:\t%x %x %x %x\n", a[BPOS(2,0)], a[BPOS(2,1)], a[BPOS(2,2)], a[BPOS(2,3)]);
  printf("Rotated L3:\t%x %x %x %x\n", a[BPOS(3,0)], a[BPOS(3,1)], a[BPOS(3,2)], a[BPOS(3,3)]);
  printf("\nLittle Endian:\t%08x\n", byteToInt(&a[BPOS(1,0)]));
/*
  LtoB(&a[BPOS(1,0)]);
  LtoB(&a[BPOS(2,0)]);
  LtoB(&a[BPOS(3,0)]);
*/
  printf("Big Endian:\t%08x\n", byteToInt(&a[BPOS(1,0)]));

  byteToInt(&a[BPOS(1,0)]) = RotateRowR(byteToInt(&a[BPOS(1,0)]),1);
  byteToInt(&a[BPOS(2,0)]) = RotateRowR(byteToInt(&a[BPOS(2,0)]),2);
  byteToInt(&a[BPOS(3,0)]) = RotateRowR(byteToInt(&a[BPOS(3,0)]),3);

  printf("Rotated (BE):\t%08x\n", byteToInt(&a[BPOS(1,0)]));
/*
  BtoL(&a[BPOS(1,0)]);
  BtoL(&a[BPOS(2,0)]);
  BtoL(&a[BPOS(3,0)]);
*/
  printf("Rotated (LE):\t%08x\n\n", byteToInt(&a[BPOS(1,0)]));

  printf("Rotated L0:\t%x %x %x %x\n", a[BPOS(0,0)], a[BPOS(0,1)], a[BPOS(0,2)], a[BPOS(0,3)]);
  printf("Rotated L1:\t%x %x %x %x\n", a[BPOS(1,0)], a[BPOS(1,1)], a[BPOS(1,2)], a[BPOS(1,3)]);
  printf("Rotated L2:\t%x %x %x %x\n", a[BPOS(2,0)], a[BPOS(2,1)], a[BPOS(2,2)], a[BPOS(2,3)]);
  printf("Rotated L3:\t%x %x %x %x\n", a[BPOS(3,0)], a[BPOS(3,1)], a[BPOS(3,2)], a[BPOS(3,3)]);
  

}

void invSubBytes(Block a) {
  uint8_t i, j;
  for(i = 0; i < BLOCKLENGTH; ++i) {
    for(j = 0; j < BLOCKLENGTH; ++j) {
      a[BPOS(i, j)] = invSbox[a[BPOS(i, j)]];
    }
  }
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
