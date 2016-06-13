#ifndef LIB_NUMBERS_FACTORS_H_
#define LIB_NUMBERS_FACTORS_H_

#include <algorithm>
#include <cmath>
#include <vector>

#include "primes.h"

template <typename T>
struct Factor {
  T prime;
  int power;
};

// Factorizes a number with k factors in O(k). Takes O(n) memory.
class OfflineFactorizator {
 public:
  OfflineFactorizator(int n) {
    lookup_.resize(n + 1);
    for (int i = 2; i <= n; ++i) {
      if (lookup_[i].first != 0) continue;
      lookup_[i] = std::pair<int, int>(i, 1);
      if ((long long) i * i > n) continue;
      for (int j = i * i, q = i; j <= n; j += i, ++q) {
        if (lookup_[j].first == 0) lookup_[j] = std::pair<int, int>(i, q);
      }
    }
  }

  std::vector<Factor<int>> GetFactors(int n) const {
    std::vector<Factor<int>> factors;
    while (n != 1) {
      factors.emplace_back();
      auto& f = factors.back();
      f.prime = lookup_[n].first;
      for (f.power = 0; lookup_[n].first == f.prime; ++f.power) {
        n = lookup_[n].second;
      }
    }
    return factors;
  }

  bool IsPrime(int n) const {
    return lookup_[n].second == 1;
  }

 private:
  std::vector<std::pair<int, int>> lookup_;
};

// Factorizes a number in O(sqrt(n) / ln(sqrt(n))).
// Takes O(sqrt(n) / ln(sqrt(n))) memory.
template <typename T>
class OnlineFactorizator {
 public:
  OnlineFactorizator(T n) {
    primes_ = PrimesUpTo(sqrt(n) + 1);
    primes_.push_back(2000000000);  // sentinel.
  }

  std::vector<Factor<T>> GetFactors(T n) const {
    std::vector<Factor<T>> factors;
    for (int i = 0; (long long)primes_[i] * primes_[i] <= n; ++i) {
      if (n % primes_[i]) continue;
      factors.emplace_back();
      auto& f = factors.back();
      f.prime = primes_[i];
      f.power = 1;
      n /= f.prime;
      for (;;) {
        T x = n / f.prime;
        if (x * f.prime != n) break;
        n = x;
        ++f.power;
      }
    }
    if (n > 1) {
      factors.emplace_back();
      factors.back().prime = n;
      factors.back().power = 1;
    }
    return factors;
  }

  bool IsPrime(T n) const {
    for (int i = 0; (long long)primes_[i] * primes_[i] <= n; ++i) {
      if (n % primes_[i] == 0) return false;
    }
    return true;
  }

  const std::vector<int>& primes() const { return primes_; }

 private:
  std::vector<int> primes_;
};

#endif  // LIB_NUMBERS_FACTORS_H_
