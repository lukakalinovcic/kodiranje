#include <algorithm>
#include <cstdio>

#define CHECK(condition) if (!(condition)) { \
  printf("CHECK(%s) failed\n", #condition); \
  exit(0); \
}

using namespace std;

typedef long long int64;

int n, k;
bool known[100001];
int a[100001];
bool my_turn = false;
bool all_known = true;

bool Solve() {
  if (k == 0) {
    if (known[0]) {
      return a[0] == 0;
    } else {
      return my_turn;
    }
  } else {
    if (all_known) {
      for (int i = 0; i < n; ++i) {
        if (a[i] % k) return false;
        a[i + 1] += a[i] / k;
      }
      return a[n] == 0;
    } else {
      return n % 2 == 1;
    }
  }
}

int main() {
  CHECK(scanf("%d%d", &n, &k));
  for (int i = 0; i <= n; ++i) {
    static char buff[16];
    CHECK(scanf("%s", buff));
    known[i] = buff[0] != '?';
    if (known[i]) {
      sscanf(buff, "%d", &a[i]);
      my_turn = !my_turn;
    } else {
      all_known = false;
    }
  }
  if (Solve()) printf("Yes\n");
  else printf("No\n");

  return 0;
}
