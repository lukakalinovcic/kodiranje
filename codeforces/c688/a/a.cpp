#include "lib/base/check.h"
#include "lib/base/integral_types.h"

using namespace std;

int main() {
  int n, d;
  CHECK(scanf("%d%d", &n, &d));
  int curr = 0;
  int best = 0;
  for (int i = 0; i < d; ++i) {
    static char buff[128];
    CHECK(scanf("%s", buff));
    bool allones = true;
    for (int j = 0; j < n; ++j) {
      allones &= (buff[j] == '1');
    }
    if (allones) {
      curr = 0;
    } else {
      ++curr;
      if (curr > best) best = curr;
    }
  }
  printf("%d\n", best);
  return 0;
}
