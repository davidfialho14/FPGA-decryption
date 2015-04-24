#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "defines.h"

#ifndef BOARD
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "encryption.h"
#else
#include "htimer.h"
#endif

#include "block.h"
#include "decryption.h"
#include "file_operations.h"

#ifndef BOARD
extern int inputFd;
extern int outputFd;
#endif

int main(int argc, char *argv[]) {

#ifndef BOARD

	if (argc != 3) {
		puts("é preciso indicar o ficheiro para encriptar/desencriptar e a opção");
		exit(-1);
	}

	int toEncrypt = 1;

	if (strcmp(argv[1], "-d") == 0) {
		//decrypt
		toEncrypt = 0;
	} else if (strcmp(argv[1], "-e") == 0) {
		toEncrypt = 1;
	} else {
		puts("opção errada");
		exit(-1);
	}

	//abrir ficheiro original
	inputFd = open(argv[2], O_RDONLY);
	if (inputFd == -1) {
		puts("ficheiro não existe");
		exit(-1);
	}

	//definir nome do ficheiro encriptado
	char outputName[strlen(argv[2]) + 5];
	strcpy(outputName, argv[2]);

	if (toEncrypt)
		strcat(outputName, ".enc");
	else
		strcat(outputName, ".dec");

	//abrir ficheiro encriptadp
	outputFd = open(outputName, O_RDWR | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
	if (outputFd == -1) {
		puts("não foi possível criar ficheiro encriptado");
		exit(-1);
	}
#else
	if(init_timer(1) == XST_FAILURE) {
		puts("erro na inicializa��o do timer");
		exit(-1);
	}

	start_timer(1);
#endif

	//chave secreta
	char password[] = "password";
	Block key;
	bzero(key, sizeof(key));
	strncpy((char*) key, password, sizeof(key));

	Block state;
	bzero(state, sizeof(state));

	int i = 0;
	while (readBlock(i, state)) {

		#ifndef BOARD
		if (toEncrypt)
			encrypt(state, key);
		else
			decrypt(state, key);
		#else
		decrypt(state, key);
		#endif

		if (!writeBlock(i, state)) {
			puts("não foi possivel escrever no ficheiro");
		}

		bzero(state, sizeof(state));
		bzero(key, sizeof(key));
		strncpy((char*) key, password, sizeof(key));

		i++;	// próximo bloco de 128 bits
	}

#ifndef BOARD
	close(inputFd);
	close(outputFd);
#else
	u32 Val0, Val1;
	Val1 = get_timer64_val (&Val0);
	printf("time: %f\n", conv2_cycles_to_msecs(Val1, Val0));
#endif

	printf("processed blocks: %d\n", i);

	return 0;
}

//#include <stdio.h>
//#include <stdlib.h>
//#include <stdint.h>
//#include "fsl.h"
//#include "block.h"
//
//const uint8_t initBlock[] =
//{
//  0x32, 0x88, 0x31, 0xe0,
//  0x43, 0x5a, 0x31, 0x37,
//  0xf6, 0x30, 0x98, 0x07,
//  0xa8, 0x8d, 0xa2, 0x34
//};
//
//uint8_t resBlock[] =
//{
//  0xFF, 0xFF, 0xFF, 0xFF,
//  0xFF, 0xFF, 0xFF, 0xFF,
//  0x00, 0x00, 0x00, 0x00,
//  0xFF, 0xFF, 0xFF, 0xFF,
//};
//
//int main() {
//
//	int i, aux = -1;
//
//	for(i = 0; i < 12; i += 4) {
//		aux = *((int*) &(initBlock[i]));
//		putfsl(aux, 0);
//	}
//
//	aux = *((int*) &(initBlock[12]));
//	cputfsl(aux, 0);
//
//	for(i = 0; i < 16; i += 4) {
//		getfsl(aux, 0);
//		printf("get: 0x%08X\n", aux);
//		*((int*)&resBlock[i]) = aux;
//	}
//
//	printBlock(resBlock);
//
//	return 0;
//}
