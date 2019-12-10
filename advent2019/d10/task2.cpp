#include <algorithm>
#include <cmath>
#include <cstdio>
#include <vector>

using namespace std;

int R, C;
char a[100][101];

int gcd(int a, int b) { return b == 0 ? a : gcd(b, a % b); }

struct vec2d {
  int x;
  int y;
  int q;
  int d;

  static int Quadrant(int x, int y) {
    if (x >= 0 && y > 0) return 0;
    if (x > 0 && y <= 0) return 1;
    if (x <= 0 && y < 0) return 2;
    return 3;
  }
  
  vec2d(int x, int y) : x(x), y(y) {
    q = Quadrant(x, y);
    d = x * x + y * y;
  }
};

int ccw(const vec2d& a, const vec2d& b) {
  int t = a.x * b.y - b.x * a.y;
  if (t < 0) return -1;
  if (t > 0) return 1;
  return 0;
}

bool operator<(const vec2d& a, const vec2d& b) {
  if (a.q != b.q) return a.q < b.q;
  int t = ccw(a, b);
  if (t != 0) return t < 0;
  return a.d < b.d;
}

int main() {
  while (scanf("%s", a[R]) == 1) ++R;
  while (a[0][C]) ++C;

  // From task1.
  int r0 = 23;
  int c0 = 17;
  a[r0][c0] = '.';

  vector<vec2d> out;
  for (;;) {
    vector<vec2d> in;
    for (int r = 0; r < R; ++r) {
      for (int c = 0; c < C; ++c) {
        if (a[r][c] == '#') {
          in.push_back(vec2d(c - c0, r0 - r));
        }
      }
    }
    if (in.empty()) break;
    sort(in.begin(), in.end());
    for (int i = 0, j; i < (int)in.size(); i = j) {
      int r = r0 - in[i].y;
      int c = c0 + in[i].x;
      a[r][c] = '.';
      out.push_back(in[i]);
      printf("out #%d:: %d %d\n", out.size(), c, r);
      for (j = i; j < (int)in.size() && ccw(in[i], in[j]) == 0; ++j);
    }
  }
  return 0;
}
