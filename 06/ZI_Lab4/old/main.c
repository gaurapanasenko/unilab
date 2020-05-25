#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

struct public_key_t{
  long long n;
  long long K;
};

struct private_key_t{
  long long n;
  long long k;
};


uint32_t primes[UINT16_MAX] = {1,2,0};
uint16_t primes_size = 2;

uint16_t binarySearch(uint32_t *arr, uint32_t l, uint32_t r, uint32_t x)  {
  while (r >= l) {
    uint32_t mid = l + (r - l) / 2;
    if (arr[mid] == x)
      return mid;

    if (arr[mid] > x) r = mid - 1;
    else l = mid + 1;
  }
  return 0;
}

uint8_t is_prime(uint32_t num) {
  return num == primes[binarySearch(primes, 0, primes_size - 1, num)];
}

void generate_primes(uint16_t up_to) {
  uint64_t i;
  uint8_t prime, j;
  for (i = 3; primes_size <= up_to; i++) {
    prime = 1;
    for (j = 1; j < primes_size && primes[j] * primes[j] <= i; j++)
      if (i % primes[j] == 0) {
        prime = 0;
        break;
      }
    if (prime) {
      primes[primes_size++] = i;
    }
  }
}

uint64_t get_prime(uint16_t num) {
  generate_primes(num);
  return primes[num];
}

uint16_t rand_range(uint16_t a, uint16_t b) { // please a <= b
  return rand() % (b - a) + a;
}

uint32_t get_random_prime_range(uint16_t a, uint16_t b) {
  return get_prime(rand_range(a, b)); // or 1230 to 10000
}

uint32_t get_random_prime() {
  return get_random_prime_range(1230, UINT16_MAX - 2000); // or 1230 to 10000
}

uint64_t gcd(uint64_t a, uint64_t b) {
  if (!b) return a;
  return gcd(b, a % b);
}

uint64_t gcd_ext(uint64_t a, uint64_t b, uint64_t* x, uint64_t* y) {
  if (a == 0) {
    *x = 0; *y = 1;
    return b;
  }
  uint64_t x1, y1, d;
  d = gcd_ext(b%a, a, &x1, &y1);
  *x = y1 - (b / a) * x1;
  *y = x1;
  return d;
}

int main(int argc, char **argv) {
  srand(time(NULL));
  uint64_t p = get_random_prime(), q = get_random_prime(), n, phi_n, K, tmp, k;
  n = p * q;
  phi_n = (p - 1) * (q - 1); // phi(n)
  K = get_random_prime_range(2, phi_n); // K - public key
  gcd_ext(phi_n, K, &tmp, &k); // k - private key
  printf("%lu %lu %lu %lu %lu %lu %lu %lu\n", p, q, n, phi_n, k, tmp, k, (phi_n * tmp + k * K));
  return 0;
}
