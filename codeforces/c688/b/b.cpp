#include <algorithm>
#include <cstring>
#include "lib/base/check.h"
#include "lib/base/integral_types.h"

using namespace std;

char s[100001];

int main() {
  CHECK(scanf("%s", s));
  printf("%s", s);
  reverse(s, s + strlen(s));
  printf("%s\n", s);
  return 0;
}
