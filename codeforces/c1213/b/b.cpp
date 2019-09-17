#include <algorithm>

#include "lib/base/check.h"

using namespace std;

int a[150000];

int main() {
  int T;
  CHECK(scanf("%d", &T));
  for (int tt = 0; tt < T; ++tt) {
    int n;
    CHECK(scanf("%d", &n));
    for (int i = 0; i < n; ++i) {
      CHECK(scanf("%d", &a[i]));
    }
    int mini = 1000000000;
    int bad = 0;
    for (int i = n - 1; i >= 0; --i) {
      bad += (a[i] > mini);
      mini = std::min(mini, a[i]);
    }
    printf("%d\n", bad);
  }
  return 0;
}
