#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <strings.h>

#ifndef BOARD
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "encryption.h"
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

#endif

	//chave secreta
	char password[] = "password";
	Block key;
	bzero(key, sizeof(key));
	strncpy((char*) key, password, sizeof(key));

	Block state;
	bzero(state, sizeof(state));


	puts("a executar...");

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
	puts("terminado!");

#ifndef BOARD
	close(inputFd);
	close(outputFd);
#endif

	return 0;
}
