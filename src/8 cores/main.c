#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "htimer.h"
#include "block.h"
#include "decryption.h"
#include "file_operations.h"
#include "xparameters.h"

#define MASTER_ID	7

int main(int argc, char *argv[]) {

//#if XPAR_CPU_ID == MASTER_ID
//	//iniciar o contador
//	if(init_timer(1) == XST_FAILURE) {
//		xil_printf("erro timer\n");
//		return 1;
//	}
//
//	xil_printf("timer working\n");
//#endif

	startCPU();

	//esperar que todos o cpus tenham arrancado
	while(!allStarted());

//#if XPAR_CPU_ID == MASTER_ID
//	xil_printf("todos começaram\n");
//	//começar contagem
//	start_timer(1);
//#endif

	//chave secreta
	Block key;
	initKey(key);

	uint8_t *state;
	int i = 0;
	while (readMyBlock(i, &state)) {
		decrypt(state, key);
		i++;	// prÃ³ximo bloco de 128 bits
	}

//#if XPAR_CPU_ID == MASTER_ID
//	//obter valor do contador
//	u32 Val0, Val1;
//	Val1 = get_timer64_val (&Val0);
//	xil_printf("time: %d\n", (int) conv2_cycles_to_msecs(Val1, Val0));
//#endif

	finishCPU();

#if XPAR_CPU_ID == MASTER_ID

	while(!allFinished());

	i = 0;
	while (readBlock(i, &state)) {
		int j;
		for(j = 0; j < BLOCKSIZE; j++) {
			xil_printf("%c", state[j]);
		}

		i++;	// prÃ³ximo bloco de 128 bits
	}

#endif

	return 0;
}
