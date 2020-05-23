#include <stdio.h>
#include <string.h>
#include "aes.h"

int main(int argc, char **argv)
{
	aeskey_t key;
	RoundKeyType RoundKey;
	state_t buf;
	size_t size = sizeof(state_t);
	fread(key, sizeof(aeskey_t), 1, stdin);
	KeyExpansion(&RoundKey, key);
	while (fread(buf, size, 1, stdin) > 0) {
		Cipher(&buf, RoundKey);
		//InvCipher(&buf, RoundKey);
		fwrite(buf, size, 1, stdout);
		memset(&buf, 0, size);
	}
	return 0;
}
