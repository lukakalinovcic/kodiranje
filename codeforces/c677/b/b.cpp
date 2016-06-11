#include <algorithm>
#include <cstdio>

#define CHECK(condition) if (!(condition)) { \
  printf("CHECK(%s) failed\n", #condition); \
  exit(0); \
}

typedef long long int64;

using namespace std;

int main() {
  int n, h, k;
  CHECK(scanf("%d%d%d", &n, &h, &k));
  int64 ret = 0;
  int sum = 0;
  for (int i = 0; i < n; ++i) {
    int x;
    CHECK(scanf("%d", &x));
    int need = sum + x - h;
    if (need > 0) {
      int t = (need + k - 1) / k;
      ret += t;
      sum = max(0, sum - t * k);
    }
    sum += x;
  }
  ret += (sum + k - 1) / k;
  printf("%lld\n", ret);
  return 0;
}
