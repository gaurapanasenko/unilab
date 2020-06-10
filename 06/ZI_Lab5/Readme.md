# Звіт з лабораторної роботи №5<br/>за дисципліною "Захист інформації"<br/>студента групи ПА-17-2<br/>Панасенка Єгора Сергійовича<br/>Кафедра комп'ютерних технологій, фпм, дну <br/>2018-2019 навч.р.

Переглянути можна [https://github.com/gaurapanasenko/unilab/blob/master/06/ZI_Lab5/](https://github.com/gaurapanasenko/unilab/blob/master/06/ZI_Lab6/)

## Постановка задачі

Програмно реалізувати ЕЦП.

## Приклад роботи

### Консольний приклад

Створимо деяку пару ключів та деяке повідомлення, та підпишемо повідомлення:
```shell
./keygen > keypair.bin
dd if=keypair.bin of=pub.bin count=1 bs=80
dd if=keypair.bin of=priv.bin count=1 bs=80 skip=1
echo "Hello, world!" > data.txt
cat pub.bin data.txt | ./sign > sign.bin
```

Перевірено повідомлення та чи правильний підпис:
```shell
cat priv.bin sign.bin data.txt | ./check
```

## Код роботи

### Файл `main.py`

```python
#!/usr/bin/env python

import sys
import math
import random
import hashlib

SQRT_MAX_PRIME = 45
MAX_PRIME = SQRT_MAX_PRIME ** 2
HASH_SIZE = 20
STRING_SIZE = HASH_SIZE * 2
PRIME_SIZE = STRING_SIZE * 8 / 2

# https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes
def generate_primes():
    arr: list = [True for i in range(MAX_PRIME)]
    for i in range(2, SQRT_MAX_PRIME):
        if arr[i]:
            j = i ** 2
            while j < MAX_PRIME:
                arr[j] = False
                j += i
    return [num for num, i in enumerate(arr) if i and num > 1]

MAIN_PRIMES = generate_primes()

def power(x, y, p):
    res = 1
    x = x % p
    while (y > 0):
        if (y & 1):
            res = (res * x) % p
        y = y>>1
        x = (x * x) % p
    return res;


# https://www.geeksforgeeks.org/primality-test-set-3-miller-rabin/
def miiller_test(d, n):
    a = 2 + random.randint(1, n - 4)
    x = power(a, d, n)

    if x == 1 or x == n - 1:
        return True

    while (d != n - 1):
        x = (x * x) % n
        d *= 2

        if (x == 1):
            return False
        if (x == n - 1):
            return True

    return False;

def test_is_prime(n, k):
    if (n <= 1 or n == 4):
        return False
    if (n <= 3):
        return True

    d = n - 1;
    while (d % 2 == 0):
        d //= 2

    for _ in range(k):
        if (miiller_test(d, n) == False):
            return False

    return True

def get_random_prime_range(start, end):
    x = random.randrange(start, end)
    while True:
        is_prime = True
        for i in MAIN_PRIMES:
            if x % i == 0:
                is_prime = False
                break
        if is_prime and test_is_prime(x, 5):
            return x
        x += 1


def get_random_prime(length):
    return get_random_prime_range(2 ** (length - 1) + 1, 2 ** length)

def gcd_ext(a, b):
  if a == 0:
    return b, 0, 1
  d, x1, y1 = gcd_ext(b % a, a)
  return d, y1 - (b // a) * x1, x1

def generate_keys():
    n = 0
    while n.bit_length() != PRIME_SIZE * 2:
        p = get_random_prime(PRIME_SIZE)
        q = get_random_prime(PRIME_SIZE)
        n = p * q
    phi_n = (p - 1) * (q - 1)
    K = 1
    k = -1
    while phi_n % K == 0 or k < 0:
        K = random.choice(MAIN_PRIMES)
        _, k, __ = gcd_ext(K, phi_n)
    return (n, K, n, k)

def any_crypt(data, n, k):
    return power(data, k, n)

def parse_block(data):
    return int.from_bytes(data, byteorder='big')

def read_block(length):
    return sys.stdin.buffer.read(length)

def main(args):
    if 'key' in args[0]:
        for i in generate_keys():
            sys.stdout.buffer.write(i.to_bytes(STRING_SIZE, byteorder='big'))
    elif 'sign' in args[0]:
        n = parse_block(read_block(STRING_SIZE))
        K = parse_block(read_block(STRING_SIZE))
        data = read_block(-1)
        h = hashlib.sha1(data).digest()
        int_hash = parse_block(h)
        sign = any_crypt(int_hash, n, K)
        sign_bytes = sign.to_bytes(STRING_SIZE, byteorder='big')
        sys.stdout.buffer.write(sign_bytes)
    elif 'check' in args[0]:
        n = parse_block(read_block(STRING_SIZE))
        k = parse_block(read_block(STRING_SIZE))
        sign = parse_block(read_block(STRING_SIZE))
        data = read_block(-1)
        h = hashlib.sha1(data).digest()
        out = any_crypt(sign, n, k)
        out = out.to_bytes(HASH_SIZE, byteorder='big')
        # sys.stdout.buffer.write(b'\1' if out == h else b'\0')
        print(out == h)
    else:
        print(args)
    return 0

if __name__ == '__main__':
    sys.exit(main(sys.argv))

```
