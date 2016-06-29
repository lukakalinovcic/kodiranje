#include <cstring>
#include "lib/base/check.h"
#include "lib/base/integral_types.h"

using namespace std;

int n, k;
int c[500];
char dp[2][501][501];

int main() {
  CHECK(scanf("%d%d", &n, &k));
  for (int i = 0; i < n; ++i) {
    CHECK(scanf("%d", &c[i]));
  }
  
  int curr = 0, next = 1;
  dp[curr][0][0] = 1;
  for (int i = 0; i < n; ++i) {
    memset(dp[next], 0, sizeof dp[next]);
    for (int x = 0; x <= k; ++x) {
      for (int y = 0; y <= x; ++y) {
        if (!dp[curr][x][y]) continue;
        dp[next][x][y] = 1;
        if (x + c[i] <= k) {
          dp[next][x + c[i]][y] = 1;
          dp[next][x + c[i]][y + c[i]] = 1;
        }
      } 
    } 
    curr ^= 1;
    next ^= 1;
  }

  int cnt = 0;
  for (int i = 0; i <= k; ++i) { 
    if (dp[curr][k][i]) {
      ++cnt;
    }
  }
  printf("%d\n", cnt);
  for (int i = 0; i <= k; ++i) { 
    if (dp[curr][k][i]) {
      printf("%d%c", i, --cnt ? ' ' : '\n');
    }
  }

  return 0;
}
