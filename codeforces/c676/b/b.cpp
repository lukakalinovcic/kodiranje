#include <algorithm>
#include <cstdio>

#define CHECK(condition) if (!(condition)) { \
  printf("CHECK(%s) failed\n", #condition); \
  exit(0); \
}

using namespace std;

int n, t;
double x[11][11];

int main() {
  CHECK(scanf("%d%d", &n, &t));
  for (int i = 0; i < t; ++i) {
    x[0][0] += 1;
    for (int r = 0; r < n; ++r) {
      for(int c = 0; c <= r; ++c) {
        if (x[r][c] > 1) {
          double spill = x[r][c] - 1;
          x[r + 1][c] += spill / 2;
          x[r + 1][c + 1] += spill / 2;
          x[r][c] = 1;
        }
      }
    }
  }
  int ret = 0;
  for (int r = 0; r < n; ++r) {
    for(int c = 0; c <= r; ++c) {
      ret += (x[r][c] + 1e-9 >= 1.0);
    }
  }
  printf("%d\n", ret);
  return 0;
}
