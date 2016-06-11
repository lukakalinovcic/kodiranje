#include <algorithm>
#include <cstdio>
#include <vector>

#define CHECK(condition) if (!(condition)) { \
  printf("CHECK(%s) failed\n", #condition); \
  exit(0); \
}

typedef long long int64;

using namespace std;

int main() {
  int n, a;
  CHECK(scanf("%d%d", &n, &a) == 2);
  vector<int> x(n + 1);
  for (int i = 1; i <= n; ++i) {
    CHECK(scanf("%d", &x[i]));
  }
  int ret = x[a];
  for (int d = 1; d < n; ++d) {
    int options = 0;
    int criminals = 0;
    if (a - d >= 1) {
      options += 1;
      criminals += x[a - d];
    }
    if (a + d <= n) {
      options += 1;
      criminals += x[a + d];
    }
    if (criminals == options) ret += criminals;
  }
  printf("%d\n", ret);
  return 0;
}
