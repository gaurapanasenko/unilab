#ifndef _AES_H_
#define _AES_H_

#include <stdint.h>

#define AES128 1
//#define AES192 1
//#define AES256 1

#define AES_BLOCKLEN 16 // Block length in bytes - AES is 128b block only
#define Nb 4

#if defined(AES256) && (AES256 == 1)
    #define Nk 8
    #define Nr 14
#elif defined(AES192) && (AES192 == 1)
    #define Nk 6
    #define Nr 12
#else
    #define Nk 4        // The number of 32 bit words in a key.
    #define Nr 10       // The number of rounds in AES Cipher.
#endif
#define AES_KEYLEN Nk * 4   // Key length in bytes
#define AES_keyExpSize Nb * (Nr + 1)

typedef uint8_t RoundKeyType[AES_keyExpSize][Nb];
// state - array holding the intermediate results during ecryption.
typedef uint8_t state_t[4][Nb];
typedef uint8_t aeskey_t[Nk][Nb];


void KeyExpansion(RoundKeyType* RoundKey, const aeskey_t key);
void Cipher(state_t* buf, const RoundKeyType RoundKey);
void InvCipher(state_t* buf, const RoundKeyType RoundKey);


#endif // _AES_H_
