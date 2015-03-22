#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "block.h"
#include "encryption.h"
#include "decryption.h"

int main(int argc, char *argv[]) {

  Block a;
  initState(a);
  puts("block a init");
  printBlock(a);

  Block key;
  initKey(key);
  puts("block key init");
  printBlock(key);

  encrypt(a, key);
  puts("block encrypted");
  printBlock(a);

  invShiftRows(a);
  puts("block invShiftRows");
  printBlock(a);

  return 0;
}
