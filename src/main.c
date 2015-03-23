#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

#include "block.h"
#include "encryption.h"
#include "decryption.h"

int main(int argc, char *argv[]) {

  if(argc != 2) {
    puts("é preciso uma linha até 16 caracteres para encriptar");
    exit(-1);
  }

  Block state;
  bzero(state, sizeof(state));
  strncpy((char*) state, argv[1], sizeof(state));
  puts("block a init");
  printBlock(state);

  char *password = getpass("Enter key: ");

  Block key;
  bzero(key, sizeof(key));
  strncpy((char*) key, password, sizeof(key));
  puts("block key init");
  printBlock(key);

  encrypt(state, key);
  puts("block encrypted");
  printBlock(state);

  bzero(key, sizeof(key));
  strncpy((char*) key, password, sizeof(key));
  puts("block key init");
  printBlock(key);
  decrypt(state, key);
  puts("block decrypted");
  printBlock(state);

  return 0;
}
