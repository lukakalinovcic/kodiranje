#include <vector>

#include "lib/base/check.h"
#include "lib/base/integral_types.h"

using namespace std;

int main() {
  int T;
  CHECK(scanf("%d", &T));
  for (int tt = 0; tt < T; ++tt) {
    int64 n, m;
    CHECK(scanf("%lld%lld", &n, &m));

    int64 sum = 0;
    vector<int64> seen_step(10, 0);
    vector<int64> seen_sum(10, 0);
    for (int64 i = 0; i <= n; i += m) {
      const int digit = i % 10;
      if (seen_step[digit]) {
        const int64 cycle_len = i - seen_step[digit];
        const int64 cycle_sum = sum - seen_sum[digit];
        const int64 k = (n - i) / cycle_len;
        i += k * cycle_len;
        sum += k * cycle_sum;
      }
      seen_step[digit] = i;
      seen_sum[digit] = sum;
      sum += digit;
    }
    printf("%lld\n", sum);
  }
  return 0;
}
