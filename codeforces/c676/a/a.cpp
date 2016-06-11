#include <algorithm>
#include <cstdio>

#define CHECK(condition) if (!(condition)) { \
  printf("CHECK(%s) failed\n", #condition); \
  exit(0); \
}

using namespace std;

int n;
int a[100];

int main() {
  CHECK(scanf("%d", &n));
  int x = -1, y = -1;
  for (int i = 0; i < n; ++i) {
    CHECK(scanf("%d", &a[i]));
    if (a[i] == 1) x = i;
    if (a[i] == n) y = i;
  }
  printf("%d\n", max(abs(x - y), max(max(x, y), n - 1 - min(x, y))));
  return 0;
}
