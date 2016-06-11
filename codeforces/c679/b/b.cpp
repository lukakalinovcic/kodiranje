#include <algorithm>
#include <cstdio>
#include <map>

#define CHECK(condition) if (!(condition)) { \
  printf("CHECK(%s) failed\n", #condition); \
  exit(0); \
}

typedef long long int64;

using namespace std;

typedef pair<int, int64> par;

int64 ClosestCube(int64 x) {
  int64 lo = 0, hi = 1000000;
  while (lo < hi) {
    int64 mid = (lo + hi + 1) / 2;
    if (mid * mid * mid <= x) lo = mid;
    else hi = mid - 1;
  }
  return lo * lo * lo;
}

map<int64, par> memo;

par Solve(int64 limit) {
  if (limit < 0) throw 1;
  if (limit == 0) return par(0, 0);
  
  if (memo.count(limit)) return memo[limit];

  int64 x = ClosestCube(limit);
  int64 k = limit / x;
  par a = Solve(limit - k * x);
  a.first += k; a.second += k * x;

  par b(0, 0);
  if (k > 0) {
    b = Solve(x - 1);
    b.first += k - 1;
    b.second += (k - 1) * x;
  }
  return memo[limit] = (a > b ? a : b);
}

int main() {
  int64 m;
  CHECK(scanf("%lld", &m));  
  par ret = Solve(m);
  printf("%d %lld\n", ret.first, ret.second);
  return 0;
}
