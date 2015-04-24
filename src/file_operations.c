#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "file_operations.h"
#include "block.h"

#ifndef BOARD

int inputFd = -1;
int outputFd = -1;

int readBlock(int blockPosition, Block block) {
  return (read(inputFd, block, BLOCKSIZE) > 0);
}

int writeBlock(int blockPosition, Block block) {
  return (write(outputFd, block, BLOCKSIZE) > 0);
}

#else

#define SIZE_ADDR       0xAC000000
#define FILE_BASE_ADDR  0xAC000004
#define MEMSIZE			0x08000000

/**
 * Nota:  É preciso colocar um teste para verificar se estamos a exceder o tamanho da memória, mas isto só pode ser
 * 				feito quando se souber o tamanho da memória que vai ser utilizada.
 * 				Além disso é preciso colocar aqui ainda o endereço base da memória.
 */

/**
 * Nota:  Ter em conta uma possível implementação em que não utilizamos o memcpy e o bloco é acedido directamente da
 * 				memória. A leitura do bloco seria feita da seguinte forma:
 * 									block = (Block) (FILE_BASE_ADDR + blockPosition * BLOCKSIZE);
 * 				Neste caso não seria necessária uma função de escrita uma vez que as operações eram feitas directamente na
 *     		posição de memória de destino.
 */

static int blockCount = -1;

int readBlock(int blockPosition, Block block) {
  int blockRead = 0;    // bloco ainda não foi lido

  if(blockCount == -1) {
    //obter tamanho do ficheiro
	int aux = *((int*) SIZE_ADDR);
    blockCount = aux / BLOCKSIZE + (aux % BLOCKSIZE > 0);
  }

  if(blockPosition >= 0 && blockPosition < blockCount) {
    //ler bloco
    memcpy((uint8_t*) block, (uint8_t*) (FILE_BASE_ADDR + blockPosition * BLOCKSIZE), BLOCKSIZE);
    blockRead = 1;
  }

  return blockRead;
}

int writeBlock(int blockPosition, Block block) {
  int blockWritten = 0;    // bloco ainda não foi escrito

  if(blockCount == -1) {
    //obter tamanho do ficheiro
    blockCount = *((int32_t*) SIZE_ADDR) / BLOCKSIZE + (*((int32_t*) SIZE_ADDR) % BLOCKSIZE > 0);
  }

  if(blockPosition >= 0 && blockPosition < blockCount) {
    //escrever bloco
    memcpy((uint8_t*) (FILE_BASE_ADDR + blockPosition * BLOCKSIZE), (uint8_t*) block, BLOCKSIZE);

    int i;
    for(i = 0; i < BLOCKSIZE; i++) {
    	putchar(block[i]);
    }

    blockWritten = 1;
  }

  return blockWritten;
}

#endif
