#ifndef LIB_NUMBERS_DIVISORS_H_
#define LIB_NUMBERS_DIVISORS_H_

#include <vector>

#include "lib/numbers/factors.h"

template <typename T, typename F>
class DivisorGenerator {
 public:
  DivisorGenerator(T n) : factorizator_(n) {}

  std::vector<T> GetDivisors(T n) const {
    std::vector<Factor<T>> factors = factorizator_.GetFactors(n);
    int k = 1;
    for (const auto& factor : factors) k *= factor.power + 1;
    std::vector<T> result(k);
    result[0] = 1;
    k = 1;
    for (const auto& factor : factors) {
      const int hi = k * (factor.power + 1);
      for (int d = k; k < hi; ++k) {
        result[k] = result[k - d] * factor.prime;
      }
    }
    return result;
  }

  const F& factorizator() const { return factorizator_; }

 private:
  F factorizator_;
};

#endif  // LIB_NUMBERS_DIVISORS_H_
