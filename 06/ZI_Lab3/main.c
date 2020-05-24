#include <stdio.h>
#include <string.h>
#include "aes.h"

int main(int argc, char **argv)
{
  aeskey_t key;
  RoundKeyType RoundKey;
  state_t buf;
  size_t size = sizeof(state_t);
  fread(key, 1, sizeof(aeskey_t), stdin);
  KeyExpansion(&RoundKey, key);
  while (fread(buf, 1, size, stdin) > 0) {
    if (argv[0][strlen(argv[0]) - 1] != 'd') Cipher(&buf, RoundKey);
    else InvCipher(&buf, RoundKey);
    fwrite(buf, 1, size, stdout);
    memset(&buf, 0, size);
  }
  return 0;
}
