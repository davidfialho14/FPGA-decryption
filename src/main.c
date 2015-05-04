#include <stdio.h>
#include <stdint.h>

#include "htimer.h"
#include "block.h"
#include "decryption.h"
#include "file_operations.h"

int main(int argc, char *argv[]) {

	//iniciar o contador
	if(init_timer(1) == XST_FAILURE) {
		puts("erro na inicialização do timer");
		exit(-1);
	}

	//começar contagem
	start_timer(1);

	//chave secreta
	char password[] = "password";
	Block key;
	initKey(key);

	Block state;
	int i = 0;
	while (readBlock(i, state)) {
		decrypt(state, key);

		//reiniciar chave
		initKey(key);
		i++;	// próximo bloco de 128 bits
	}

	//obter valor do contador
	u32 Val0, Val1;
	Val1 = get_timer64_val (&Val0);
	printf("time: %f\n", conv2_cycles_to_msecs(Val1, Val0));

	return 0;
}
