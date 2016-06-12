#include <cmath>
#include <cstring>
#include <vector>

std::vector<int> PrimesUpTo(int n) {
  int data_len = (n >> 5) + 1;
  int* data = new int[data_len];
  memset(data, 0, data_len * sizeof(int));

  std::vector<int> primes;
  if (n >= 10000) {
    primes.reserve(1.01 * n / (log(n) - 1));
  }
  for (int i = 2; i <= n; ++i) {
    if ((data[(i >> 5)] >> (i & 31)) & 1) continue;
    primes.push_back(i);
    if ((long long) i * i > n) continue;
    for (int j = i * i; j <= n; j += i) {
      data[(j >> 5)] |= 1 << (j & 31);
    }
  }
  delete[] data;
  return primes;
}
