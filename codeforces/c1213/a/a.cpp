#include <algorithm>

#include "lib/base/check.h"
#include "lib/base/integral_types.h"

using namespace std;

int main() {
  int n;
  CHECK(scanf("%d", &n));
  int odd = 0;
  int even = 0;
  for (int i = 0; i < n; ++i) {
    int x;
    CHECK(scanf("%d", &x));
    if (x % 2 == 0) ++even;
    else ++odd;
  }
  printf("%d\n", std::min(odd, even));
  return 0;
}
