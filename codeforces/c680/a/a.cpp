#include <algorithm>
#include <cstdio>
#include <map>

#define CHECK(condition) if (!(condition)) { \
  printf("CHECK(%s) failed\n", #condition); \
  exit(0); \
}

typedef long long int64;

using namespace std;

int main() {
  int sum = 0;
  map<int, int> cnt;
  for (int i = 0; i < 5; ++i) {
    int x;
    CHECK(scanf("%d", &x));
    sum += x;
    ++cnt[x];
  }
  int result = sum;
  for (const auto& entry : cnt) {
    int x = entry.first;
    int freq = entry.second;
    if (freq >= 3) result = min(result, sum - x * 3);
    if (freq >= 2) result = min(result, sum - x * 2);
  }
  printf("%d\n", result);
  return 0;
}
