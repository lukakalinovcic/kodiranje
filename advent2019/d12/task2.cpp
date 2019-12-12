#include <cmath>
#include <cstdio>
#include <vector>

using namespace std;

struct Moon {
  int p, v;
};
bool operator==(const Moon& a, const Moon& b) {
  return a.p == b.p && a.v == b.v;
}

vector<vector<Moon>> Read() {
  static char s[256];
  vector<vector<Moon>> moons(3);
  while (fgets(s, sizeof(s), stdin)) {
    Moon x, y, z;
    sscanf(s, "<x=%d, y=%d, z=%d>", &x.p, &y.p, &z.p);
    x.v = y.v = z.v = 0;
    moons[0].push_back(x);
    moons[1].push_back(y);
    moons[2].push_back(z);
  }
  return moons;
}

int sgn(int x) {
  if (x < 0) return -1;
  if (x > 0) return 1;
  return 0;
}

typedef long long int64;
int64 gcd(int64 a, int64 b) { return b == 0 ? a : gcd(b, a%b); }
int64 lcm(int64 a, int64 b) { return a * b / gcd(a, b); }

int main() {
  vector<vector<Moon>> components = Read();
  int64 period = 1;
  for (const vector<Moon>& start : components) {
    int cnt = 0;
    vector<Moon> curr = start;
    do {
      ++cnt;
      for (Moon& a : curr) {
        for (Moon& b : curr) {
          a.v += sgn(b.p - a.p);
        }
      }
      for (Moon& a : curr) {
        a.p += a.v;
      }
    } while (curr != start);
    period = lcm(period, cnt);
  }
  printf("%lld\n", period);
  return 0;
}
