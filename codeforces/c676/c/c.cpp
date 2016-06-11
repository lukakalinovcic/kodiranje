#include <algorithm>
#include <cstdio>

#define CHECK(condition) if (!(condition)) { \
  printf("CHECK(%s) failed\n", #condition); \
  exit(0); \
}

using namespace std;

int n, k;
char s[100001];
int cnt[2][100001];

int Cnt(int a, int b, int x) {
  return cnt[x][b] - cnt[x][a];
}

int main() {
  CHECK(scanf("%d%d", &n, &k));
  CHECK(scanf("%s", s));
  for (int x = 0; x < 2; ++x) {
    for (int i = 0; i < n; ++i) {
      cnt[x][i + 1] = cnt[x][i] + (s[i] == 'a' + x);
    }
  }
  int ret = 0;
  for (int x = 0; x < 2; ++x) {
    for (int i = 0; i < n; ++i) {
      int lo = i, hi = n;
      while (lo < hi) {
        int mid = (lo + hi + 1) / 2;
        if (Cnt(i, mid, x) + k >= mid - i) lo = mid;
        else hi = mid - 1;
      }
      ret = max(ret, lo - i);
    }
  }
  printf("%d\n", ret);
  return 0;
}
