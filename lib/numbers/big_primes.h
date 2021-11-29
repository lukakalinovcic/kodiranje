#ifndef LIB_NUMBERS_BIG_PRIMES_H_
#define LIB_NUMBERS_BIG_PRIMES_H_
// Works up to 10^18.
#include "lib/base/integral_types.h"

int64 BigMult(int64 a, int64 b, int64 mod) {
  return int64(__int128(a) * __int128(b) % __int128(mod));
}

int64 BigPow(int64 a, int64 n, int64 mod) {
  if (n == 0) return 1 % mod;
  if (n % 2) return BigMult(a, BigPow(a, n - 1, mod), mod);
  int64 x = BigPow(a, n >> 1, mod);
  return BigMult(x, x, mod);
}

bool IsStrongProbablePrime(int64 n, int64 a) {
  if (n <= 1) return false;
  if (n == a) return true;

  int s = 0;
  while ((n - 1) % (1LL << (s + 1)) == 0 ) ++s;

  int64 d = (n - 1) / (1LL << s);
  if (BigPow( a, d, n) == 1) return true;
  for (int r = 0; r < s; ++r) {
    if (BigPow(a, d * (1LL << r), n) == n - 1) return true;
  }
  return false;
}

bool IsPrime(int64 n) {
  if (n == 1) return false;
  if (n != 2 && n % 2 == 0) return false;
  if (n == 46856248255981LL) return false;
  if (IsStrongProbablePrime(n, 2) == 0) return false;
  if (IsStrongProbablePrime(n, 3) == 0) return false;
  if (IsStrongProbablePrime(n, 7) == 0) return false;
  if (IsStrongProbablePrime(n, 61) == 0) return false;
  if (IsStrongProbablePrime(n, 24251) == 0) return false;
  return true;
}

#endif  // LIB_NUMBERS_BIG_PRIMES_H_
