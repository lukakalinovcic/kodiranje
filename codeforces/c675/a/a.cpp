#include <algorithm>
#include <cstdio>

#define CHECK(condition) if (!(condition)) { \
  printf("CHECK(%s) failed\n", #condition); \
  exit(0); \
}

using namespace std;

bool Solve(int a, int b, int c) {
  if (c == 0) return a == b;
  if (c > 0 && b < a) return false;
  if (c < 0 && b > a) return false;
  return (b - a) % c == 0;
}

int main() {
  int a, b, c;
  CHECK(scanf("%d%d%d", &a, &b, &c));
  printf(Solve(a, b, c) ? "YES\n" : "NO\n");
  return 0;
}
