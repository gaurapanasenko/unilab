import numpy as np
import sys

def bit_shift(value, count): return (value << count) & 255 | value >> (8 - count)

def generate_sboxes():
    sbox = np.zeros(256, dtype=np.uint8)
    invsbox = np.zeros(256, dtype=np.uint8)
    p = 1
    q = 1
    sbox[0] = 0x63
    while True:
        p = (p ^ (p << 1) ^ (0x1B if p & 0x80 else 0)) & 255
        q ^= q << 1
        q ^= q << 2
        q ^= q << 4
        q ^= 0x09 if q & 0x80 else 0
        q &= 255
        val = q ^ bit_shift(q, 1) ^ bit_shift(q, 2) ^ bit_shift(q, 3) ^ bit_shift(q, 4) ^ 0x63
        sbox[p] = val
        invsbox[val] = p
        if p == 1:
            break
    return sbox, invsbox

SBOX, INVSBOX = generate_sboxes()



def SubBytes(cell):
    return np.array(([[SBOX[j] for j in i] for i in cell), dtype=np.uint8)

def InvSubBytes(cell):
    return np.array(([[INVSBOX[j] for j in i] for i in cell), dtype=np.uint8)

def ShiftRows(cell):
    return np.array([np.append(row[i:], row[:i]) for i, row in enumerate(cell)], dtype=np.uint8)

def InvShiftRows(cell):
    return np.array([np.append(row[4-i:], row[:4-i]) for i, row in enumerate(cell)], dtype=np.uint8)

def main():
    data = sys.stdin.buffer.read(-1)
    np_data = np.frombuffer(data, dtype=np.uint8)
    np_data = np.append(np_data, np.zeros(abs(- np_data.shape[0] % 16), dtype=np.uint8))
    np_data = np_data.reshape(int(np_data.shape[0] / 16), 4, 4)
    print(np_data)
    sb_data = SubBytes(np_data)
    print((sb_data))

main()
