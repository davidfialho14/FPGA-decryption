#include <stdint.h>
#include "file_operations.h"

#define SIZE_ADDR       0xAC000000
#define FILE_BASE_ADDR  0xAC000004
#define MEMSIZE			    0x08000000

#define FILESIZE        *((int*) SIZE_ADDR)

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

static int blockCount = FILESIZE / BLOCKSIZE + (FILESIZE % BLOCKSIZE > 0);

int readBlock(int blockPosition, Block block) {
  block = (uint8_t*) (FILE_BASE_ADDR + blockPosition * BLOCKSIZE);
  return blockRead;
}
