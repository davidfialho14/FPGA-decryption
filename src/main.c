#include <stdio.h>
#include <stdint.h>

#include "htimer.h"
#include "block.h"
#include "decryption.h"
#include "file_operations.h"

int main(int argc, char *argv[]) {

	//iniciar o contador
	if(init_timer(1) == XST_FAILURE) {
		return 1;
	}

	//começar contagem
	start_timer(1);

	//chave secreta
	Block key;
	initKey(key);

	uint8_t *state;
	int i = 0;
	int j;
	while (readBlock(i, &state)) {
		decrypt(state, key);

		for(j = 0; j < BLOCKSIZE; j++) {
			xil_printf("%c", (state[j]));
		}

		i++;	// próximo bloco de 128 bits
	}

	//obter valor do contador
	u32 Val0, Val1;
	Val1 = get_timer64_val (&Val0);
	xil_printf("time: %d\n", (int) conv2_cycles_to_msecs(Val1, Val0));

	return 0;
}
