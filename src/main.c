#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "block.h"
#include "encryption.h"
#include "decryption.h"

int main(int argc, char *argv[]) {

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
	int fdIn = open(argv[2], O_RDONLY);
	if (fdIn == -1) {
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
	int fdOut = open(outputName, O_RDWR | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
	if (fdOut == -1) {
		puts("não foi possível criar ficheiro encriptado");
		exit(-1);
	}

	// ler chave secreta
	char password[] = "password";
	Block key;
	bzero(key, sizeof(key));
	strncpy((char*) key, password, sizeof(key));

	Block state;
	bzero(state, sizeof(state));

	puts("a encriptar...");
	while (read(fdIn, state, sizeof(state)) > 0) {
		if (toEncrypt)
			encrypt(state, key);
		else
			decrypt(state, key);

		if (write(fdOut, state, sizeof(state)) <= 0) {
			perror("write");
		}

		bzero(state, sizeof(state));
		bzero(key, sizeof(key));
		strncpy((char*) key, password, sizeof(key));
	}
	puts("encriptado!");

	close(fdIn);
	close(fdOut);

	return 0;
}
