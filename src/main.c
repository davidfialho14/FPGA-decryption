#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "htimer.h"
#include "block.h"
#include "decryption.h"
#include "file_operations.h"
#include "xparameters.h"

int main(int argc, char *argv[]) {

//	startCPU();

#if XPAR_CPU_ID == 1

	//iniciar o contador
	if(init_timer(1) == XST_FAILURE) {
		xil_printf("erro na inicializacao do timer\n");
		return 1;
	}

#endif

//	toStart();

#if XPAR_CPU_ID == 1
	start_timer(1);
#endif

	//chave secreta
	Block key;
	initKey(key);

	uint8_t *state;
	int i = 0;
	while (readBlock(i, &state)) {
		decrypt(state, key);
		i++;	// próximo bloco de 128 bits
	}

//	finishCPU();

#if XPAR_CPU_ID == 1

//	xil_printf("espera terminar\n");
//	while(!allFinished());
//	xil_printf("terminou tudo\n");

	//obter valor do contador
	u32 Val0, Val1;
	Val1 = get_timer64_val (&Val0);
	xil_printf("time: %d %d %d\n", (int) conv2_cycles_to_msecs(Val1, Val0), Val1, Val0);

	i = 0;
	while (readBlock(i, &state)) {
		int j;
		for(j = 0; j < BLOCKSIZE; j++) {
			xil_printf("%c", state[j]);
		}

		i++;	// próximo bloco de 128 bits
	}

//	cleanSync();

#endif

	return 0;
}
