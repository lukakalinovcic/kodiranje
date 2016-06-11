#include <algorithm>
#include <cstdio>

#define CHECK(condition) if (!(condition)) { \
  printf("CHECK(%s) failed\n", #condition); \
  exit(0); \
}

typedef long long int64;

using namespace std;

int main() {
  int n, h;
  CHECK(scanf("%d%d", &n, &h) == 2);
  int ret = 0;
  for (int i = 0; i < n; ++i) {
    int x;
    CHECK(scanf("%d", &x));
    ret += x <= h ? 1 : 2;
  }
  printf("%d\n", ret);
  return 0;
}
