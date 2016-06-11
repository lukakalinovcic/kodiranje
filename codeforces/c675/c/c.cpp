#include <algorithm>
#include <cstdio>
#include <map>

#define CHECK(condition) if (!(condition)) { \
  printf("CHECK(%s) failed\n", #condition); \
  exit(0); \
}

typedef long long int64;

using namespace std;

int n;

int main() {
  CHECK(scanf("%d", &n));
  int64 sum = 0;
  map<int64, int> cnt;
  int mx = 0;
  for (int i = 0; i < n; ++i) {
    int64 x;
    CHECK(scanf("%lld", &x));
    sum += x;
    cnt[sum] += 1;
    mx = max(mx, cnt[sum]);
  }
  printf("%d\n", n - mx);

  return 0;
}
