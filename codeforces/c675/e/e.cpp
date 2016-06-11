#include <algorithm>
#include <cstdio>

#define CHECK(condition) if (!(condition)) { \
  printf("CHECK(%s) failed\n", #condition); \
  exit(0); \
}

typedef long long int64;

using namespace std;

int n;
int a[100001];
int loga[100001];
int64 dp[100001];

int main() {
  CHECK(scanf("%d", &n));
  a[0] = 0;
  for (int i = 1; i < n; ++i) {
    CHECK(scanf("%d", &a[i]));
  }
  a[n] = n;
  loga[n] = n;
  dp[n] = 0;

  int64 ret = 0;
  for (int i = n - 1; i > 0; --i) {
    int x = i + 1;
    for (int j = a[i]; j > 0; j -= j & -j) {
      if (a[loga[j]] > a[x]) x = loga[j];
    }
    for (int j = i; j <= n; j += j & -j) {
      if (a[i] > a[loga[j]]) loga[j] = i;
    }
    dp[i] = dp[x] + n - i - (a[i] - x);
    ret += dp[i];
  }
  printf("%lld\n", ret);

  return 0;
}
