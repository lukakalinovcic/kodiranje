#include <algorithm>
#include <cmath>
#include <cstdio>
#include <vector>

using namespace std;

typedef pair<char, int> par;

vector<par> ReadLine() {
  static char s[65536];
  scanf("%s", s);
  char c;
  int n;
  int k;
  char *p = s;
  vector<par> ret;
  while (sscanf(p, "%c%d%n", &c, &n, &k) == 2) {
    ret.emplace_back(c, n);
    p += k;
    if (*p == ',') ++p;
  }
  return ret;
}

struct Coords {
  int x, y, dist;
};

bool Cmp(const Coords& a, const Coords& b) {
  if (a.x != b.x) return a.x < b.x;
  return a.y < b.y;
};

vector<Coords> Walk(const vector<par>& path) {
  vector<Coords> ret;
  Coords c = {0, 0, 0};
  ret.push_back(c);
  for (const par& p : path) {
    for (int k = 0; k < p.second; ++k) {
      if (p.first == 'L') --c.x;
      if (p.first == 'R') ++c.x;
      if (p.first == 'U') --c.y;
      if (p.first == 'D') ++c.y;
      c.dist += 1;
      ret.push_back(c);
    }
  }
  sort(ret.begin(), ret.end(), Cmp);
  return ret;
}

int main() {
  vector<Coords> a = Walk(ReadLine());
  vector<Coords> b = Walk(ReadLine());
  vector<Coords>::iterator i = a.begin();
  vector<Coords>::iterator j = b.begin();
  int ret = 1000000000;
  for (; i != a.end(); ++i) {
    for (; j != b.end() && Cmp(*j, *i); ++j);
    if (i->x == j->x && i->y == j->y && (i->x != 0 || i->y != 0)) {
      ret = min(ret, i->dist + j->dist);
    }
  }
  printf("%d\n", ret);
  return 0;
}
