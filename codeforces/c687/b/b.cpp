#include "lib/base/check.h"
#include "lib/base/integral_types.h"
#include "lib/numbers/factors.h"

using namespace std;

int main() {
  OfflineFactorizator factorizator(1000000);
  
  int n, k;
  CHECK(scanf("%d%d", &n, &k));
  vector<Factor<int>> target = factorizator.GetFactors(k);
  vector<int> have(target.size(), 0);
  for (int i = 0; i < n; ++i) {
    int x;
    CHECK(scanf("%d", &x));
    vector<Factor<int>> factors = factorizator.GetFactors(x);
  
    for (int i = 0, j = 0; i < (int)factors.size(); ++i) {
      while (j < (int)target.size() && target[j].prime < factors[i].prime) ++j;
      if (j < (int)target.size() && target[j].prime == factors[i].prime) {
        have[j] = max(have[j], factors[i].power);
      }
    }
  }
  bool ok = true;
  for (int i = 0; i < (int)target.size(); ++i) {
    if (have[i] < target[i].power) ok = false;
  }
  printf(ok ? "Yes\n" : "No\n");

  return 0;
}
