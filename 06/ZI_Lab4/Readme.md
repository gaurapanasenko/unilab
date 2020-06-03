# Звіт з лабораторної роботи №4<br/>за дисципліною "Захист інформації"<br/>студента групи ПА-17-2<br/>Панасенка Єгора Сергійовича<br/>Кафедра комп'ютерних технологій, фпм, дну <br/>2018-2019 навч.р.

Переглянути можна [https://github.com/gaurapanasenko/unilab/blob/master/06/ZI_Lab4/](https://github.com/gaurapanasenko/unilab/blob/master/06/ZI_Lab4/)

## Постановка задачі

Програмно реалізувати алгоритм RSA.

## Приклад роботи

### Консольний приклад

Створимо деяку пару ключів та деяке повідомлення, та зашифруємо повідомлення:
```shell
./keygen > keypair.bin
dd if=keypair.bin of=pub.bin count=1 bs=256
dd if=keypair.bin of=priv.bin count=1 bs=256 skip=1
echo "Hello, world!" > data.txt
cat priv.bin data.txt | ./encrypt > out.bin
```

Дешифруємо повідомлення та отримаємо наше повідомлення:
```shell
cat pub.bin out.bin | ./decrypt
```

## Код роботи

### Файл `main.py`

```python
#!/usr/bin/env python

import sys
import math
import random

SQRT_MAX_PRIME = 45
MAX_PRIME = SQRT_MAX_PRIME ** 2

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
    while n.bit_length() != 1024:
        p = get_random_prime(512)
        q = get_random_prime(512)
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

def parse_block(data, length):
    for i in range(64 - len(data)):
        data += b'\x00'
    return int.from_bytes(data, byteorder='big')

def read_block(length):
    return sys.stdin.buffer.read(length)

def main(args):
    if 'key' in args[0]:
        for i in generate_keys():
            sys.stdout.buffer.write(i.to_bytes(128, byteorder='big'))
    elif 'encrypt' in args[0]:
        n = parse_block(read_block(128), 128)
        K = parse_block(read_block(128), 128)
        while True:
            data = read_block(64)
            if not data:
                return 0
            out = any_crypt(parse_block(data, 64), n, K)
            sys.stdout.buffer.write(out.to_bytes(128, byteorder='big'))
    elif 'decrypt' in args[0]:
        n = parse_block(read_block(128), 128)
        K = parse_block(read_block(128), 128)
        while True:
            data = read_block(128)
            if not data:
                return 0
            out = any_crypt(parse_block(data, 128), n, K)
            sys.stdout.buffer.write(out.to_bytes(64, byteorder='big'))
    else:
        print(args)
    return 0

if __name__ == '__main__':
    sys.exit(main(sys.argv))

```
