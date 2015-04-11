#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define RotateRow(l,n)	((l>>(n<<3))|(l<<(32-(n<<3)))) // Rotates to the right by n bytes a given uint_32 l
#define byteToInt(block)       (*((uint32_t*)block))  // Converts 4 bytes into 1 uint_32
//typedef uint8_t Block[BLOCKSIZE];

void LtoB(uint8_t * line) // converts a line of 4 bytes from LE to BE
{
	uint8_t aux = line[0];
	line[0] = line[3];
	line[3] = aux;
	aux = line[1];
	line[1] = line[2];
	line[2] = aux;
}

int main()
{
  //uint8_t line[4] = {0x04, 0x05, 0x06, 0x07};
  uint8_t block[4][4] = { {0x00, 0x01, 0x02, 0x03}, 
                          {0x04, 0x05, 0x06, 0x07},
                          {0x08, 0x09, 0x0a, 0x0b},
                          {0x0c, 0x0d, 0x0e, 0x0f}};     
  
  LtoB(block[0]);
  LtoB(block[1]);
  LtoB(block[2]);
  LtoB(block[3]); 
  
  printf("%08x\n", byteToInt(&block[0]));
  printf("%08x\n", byteToInt(&block[1]));
  printf("%08x\n", byteToInt(&block[2]));
  printf("%08x\n\n", byteToInt(&block[3])); 

  byteToInt(&block[1]) = RotateRow(byteToInt(&block[1]),1);
  byteToInt(&block[2]) = RotateRow(byteToInt(&block[2]),2);
  byteToInt(&block[3]) = RotateRow(byteToInt(&block[3]),3);  
  
  printf("%08x\n", byteToInt(&block[0]));
  printf("%08x\n", byteToInt(&block[1]));
  printf("%08x\n", byteToInt(&block[2]));
  printf("%08x\n", byteToInt(&block[3]));
  
  exit(1);
}
