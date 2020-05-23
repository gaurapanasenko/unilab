#include <string.h>
#include <stdio.h>
#include "aes.h"

#define ROTL8(x,shift) ((uint8_t) ((x) << (shift)) | ((x) >> (8 - (shift))))

static uint8_t sbox[256] = {0};
static uint8_t rsbox[256] = {0};

void initialize_aes_sbox() {
  uint8_t p = 1, q = 1;

  /* loop invariant: p * q == 1 in the Galois field */
  do {
    /* multiply p by 3 */
    p = (p ^ (p << 1) ^ (p & 0x80 ? 0x1B : 0)) & 0xFF;

    /* divide q by 3 (equals multiplication by 0xf6) */
    q ^= q << 1;
    q ^= q << 2;
    q ^= q << 4;
    q ^= (q & 0x80 ? 0x09 : 0) & 0xFF;

    /* compute the affine transformation */
    uint8_t xformed = q ^ ROTL8(q, 1) ^ ROTL8(q, 2) ^ ROTL8(q, 3) ^ ROTL8(q, 4) ^ 0x63;

    sbox[p] = xformed;
    rsbox[xformed] = p;
  } while (p != 1);

  /* 0 is a special case since it has no inverse */
  sbox[0] = 0x63;
}

// The round constant word array, Rcon[i], contains the values given by
// x to the power (i-1) being powers of x (x is denoted as {02}) in the field GF(2^8)
const uint8_t Rcon[11] = {
  0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36 };

const uint8_t MMC[4] = {0x03, 0x01, 0x01, 0x02};
const uint8_t INVMMC[4] = {0x0b, 0x0d, 0x09, 0x0e};

#define getSBoxValue(num) (sbox[(num)])
#define getSBoxInvert(num) (rsbox[(num)])

// This function produces Nb(Nr+1) round keys. The round keys are used in each round to decrypt the states.
void KeyExpansion(RoundKeyType* RoundKey, const aeskey_t key) {
  unsigned i, j;
  uint8_t tempa[4]; // Used for the column/row operations

  initialize_aes_sbox();

  // The first round key is the key itself.
  memcpy(RoundKey, key, sizeof(aeskey_t));

  // All other round keys are found from the previous round keys.
  for (i = Nk; i < Nb * (Nr + 1); ++i) {
    memcpy(tempa, (*RoundKey)[i - 1], 4);

    if (i % Nk == 0) {
      // This function shifts the 4 bytes in a word to the left once.
      // [a0,a1,a2,a3] becomes [a1,a2,a3,a0]

      // Function RotWord()
      {
        const uint8_t u8tmp = tempa[0];
        memmove(tempa, tempa + 1, 3);
        tempa[3] = u8tmp;
      }

      // SubWord() is a function that takes a four-byte input word and
      // applies the S-box to each of the four bytes to produce an output word.

      // Function Subword()
      for (j = 0; j < 4; j++) {
        tempa[j] = getSBoxValue(tempa[j]);
      }

      tempa[0] = tempa[0] ^ Rcon[i/Nk];
    }
#if defined(AES256) && (AES256 == 1)
    if (i % Nk == 4)
    {
      // Function Subword()
      for (j = 0; j < 4; j++) {
        tempa[j] = getSBoxValue(tempa[j]);
      }
    }
#endif
    for (j = 0; j < 4; j++) {
      (*RoundKey)[i][j] = (*RoundKey)[i - Nk][j] ^ tempa[j];
    }
  }
}

// This function adds the round key to state.
// The round key is added to the state by an XOR function.
void AddRoundKey(uint8_t round, state_t* state, const RoundKeyType RoundKey) {
  uint8_t i,j;
  for (i = 0; i < 4; ++i) {
    for (j = 0; j < 4; ++j) {
      (*state)[i][j] ^= RoundKey[round * Nb][(i * Nb) + j];
    }
  }
}

// The SubBytes Function Substitutes the values in the
// state matrix with values in an S-box.
void SubBytes(state_t* state) {
  uint8_t i, j;
  for (i = 0; i < 4; ++i) {
    for (j = 0; j < 4; ++j) {
      (*state)[j][i] = getSBoxValue((*state)[j][i]);
    }
  }
}

// The ShiftRows() function shifts the rows in the state to the left.
// Each row is shifted with different offset.
// Offset = Row number. So the first row is not shifted.
void ShiftRows(state_t* state) {
  uint8_t temp;

  // Rotate first row 1 columns to left
  temp           = (*state)[0][1];
  (*state)[0][1] = (*state)[1][1];
  (*state)[1][1] = (*state)[2][1];
  (*state)[2][1] = (*state)[3][1];
  (*state)[3][1] = temp;

  // Rotate second row 2 columns to left
  temp           = (*state)[0][2];
  (*state)[0][2] = (*state)[2][2];
  (*state)[2][2] = temp;

  temp           = (*state)[1][2];
  (*state)[1][2] = (*state)[3][2];
  (*state)[3][2] = temp;

  // Rotate third row 3 columns to left
  temp           = (*state)[0][3];
  (*state)[0][3] = (*state)[3][3];
  (*state)[3][3] = (*state)[2][3];
  (*state)[2][3] = (*state)[1][3];
  (*state)[1][3] = temp;
}

uint8_t xtime(uint8_t x) {
  return ((x<<1) ^ (((x>>7) & 1) * 0x1b));
}

// Multiply is used to multiply numbers in the field GF(2^8)
uint8_t Multiply_rec(uint8_t x, uint8_t y, uint8_t i) {
  if (i == 0) return (y & 1) * x;
  return ((y & 1) * x) ^ Multiply_rec(xtime(x), y>>1, i - 1);
}

uint8_t Multiply(uint8_t x, uint8_t y) {
  return Multiply_rec(x, y, 3);
}

uint8_t MixColumnsMultiply(uint8_t *col, uint8_t i, const uint8_t *arr) {
  uint8_t res = 0, j;
  for (j = 0; j < 4; ++j) {
    res ^= Multiply(col[j], arr[(3 - i + j) % 4]);
  }
  return res;
}

void MixColumnsCommon(state_t* state, const uint8_t *arr) {
  uint8_t i, j, t[4];
  for (i = 0; i < 4; ++i) {
    memcpy(t, (*state)[i], 4);
    for (j = 0; j < 4; ++j) {
      (*state)[i][j] = MixColumnsMultiply(t, j, arr);
    }
  }
}

// MixColumns function mixes the columns of the state matrix
void MixColumns(state_t* state) {
  MixColumnsCommon(state, MMC);
}

// MixColumns function mixes the columns of the state matrix.
// The method used to multiply may be difficult to understand for the inexperienced.
// Please use the references to gain more information.
void InvMixColumns(state_t* state) {
  MixColumnsCommon(state, INVMMC);
}


// The SubBytes Function Substitutes the values in the
// state matrix with values in an S-box.
void InvSubBytes(state_t* state) {
  uint8_t i, j;
  for (i = 0; i < 4; ++i) {
    for (j = 0; j < 4; ++j) {
      (*state)[j][i] = getSBoxInvert((*state)[j][i]);
    }
  }
}

void InvShiftRows(state_t* state)
{
  uint8_t temp;

  // Rotate first row 1 columns to right
  temp = (*state)[3][1];
  (*state)[3][1] = (*state)[2][1];
  (*state)[2][1] = (*state)[1][1];
  (*state)[1][1] = (*state)[0][1];
  (*state)[0][1] = temp;

  // Rotate second row 2 columns to right
  temp = (*state)[0][2];
  (*state)[0][2] = (*state)[2][2];
  (*state)[2][2] = temp;

  temp = (*state)[1][2];
  (*state)[1][2] = (*state)[3][2];
  (*state)[3][2] = temp;

  // Rotate third row 3 columns to right
  temp = (*state)[0][3];
  (*state)[0][3] = (*state)[1][3];
  (*state)[1][3] = (*state)[2][3];
  (*state)[2][3] = (*state)[3][3];
  (*state)[3][3] = temp;
}

// Cipher is the main function that encrypts the PlainText.
void Cipher(state_t* state, const RoundKeyType RoundKey) {
  uint8_t round = 0;

  // Add the First round key to the state before starting the rounds.
  AddRoundKey(0, state, RoundKey);

  // There will be Nr rounds.
  // The first Nr-1 rounds are identical.
  // These Nr rounds are executed in the loop below.
  // Last one without MixColumns()
  for (round = 1; ; ++round) {
    SubBytes(state);
    ShiftRows(state);
    if (round == Nr) {
      break;
    }
    MixColumns(state);
    AddRoundKey(round, state, RoundKey);
  }
  // Add round key to last round
  AddRoundKey(Nr, state, RoundKey);
}

void InvCipher(state_t* state, const RoundKeyType RoundKey) {
  uint8_t round = 0;

  initialize_aes_sbox();

  // Add the First round key to the state before starting the rounds.
  AddRoundKey(Nr, state, RoundKey);

  // There will be Nr rounds.
  // The first Nr-1 rounds are identical.
  // These Nr rounds are executed in the loop below.
  // Last one without InvMixColumn()
  for (round = (Nr - 1); ; --round)
  {
    InvShiftRows(state);
    InvSubBytes(state);
    AddRoundKey(round, state, RoundKey);
    if (round == 0) {
      break;
    }
    InvMixColumns(state);
  }

}
