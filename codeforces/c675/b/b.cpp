#include <algorithm>
#include <cstdio>

#define CHECK(condition) if (!(condition)) { \
  printf("CHECK(%s) failed\n", #condition); \
  exit(0); \
}

typedef long long int64;

using namespace std;

int main() {
  int n, a, b, c, d; 
  CHECK(scanf("%d%d%d%d%d", &n, &a, &b, &c, &d));
  int delta1 = b - c;
  int delta2 = a - d;
  int delta3 = a + b - c - d;
  int max_delta = max(max(0, delta1), max(delta2, delta3));
  int min_delta = min(min(0, delta1), min(delta2, delta3));
  int span = max_delta - min_delta + 1;
  printf("%lld\n", n * max<int64>(0, n - span + 1));
  return 0;
}
