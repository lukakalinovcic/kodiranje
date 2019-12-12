#include <algorithm>
#include <cassert>
#include <cstdio>
#include <map>
#include <vector>

using namespace std;

typedef long long int64;

vector<int64> Read() {
  static char s[65536];
  scanf("%s", s);
  char* p = s;
  int64 x;
  int k;
  vector<int64> prog;
  while (sscanf(p, "%lld%n", &x, &k) == 1) {
    prog.push_back(x);
    p += k;
    assert(k <= 18);
    if (*p == ',') ++p;
  }
  return prog;
}

struct Coords {
  int x, y;
};
bool operator<(const Coords &A, const Coords &B) {
  if (A.x != B.x) return A.x < B.x;
  if (A.y != B.y) return A.y < B.y;
  return 0;
}
 
map<Coords, int> color;
int GetColor(int x, int y) {
  Coords t = {x, y};
  if (!color.count(t)) return 0;
  return color[t];
}
void SetColor(int x, int y, int val) {
  color[Coords{x, y}] = val;
}
int x = 0;
int y = 0;
int d = 0;
const int dx[4] = { 0, 1, 0, -1 };
const int dy[4] = { 1, 0, -1, 0 };

bool Execute(const vector<int64>& start) {
  map<int64, int64> prog;
  for (int i = 0; i < (int)start.size(); ++i) {
    prog[i] = start[i];
  }
  int parity = 0;
  int64 pc = 0;
  int64 relative_base = 0;
  for (;;) {
    int64 value = prog[pc++];
    int op = value % 100; value /= 100;
    auto ParamAddr = [&]() {
      int mode = value % 10; value /= 10;
      assert(mode == 0 || mode == 1 || mode == 2);
      if (mode == 0) {
        return prog[pc++];
      } else if (mode == 1) {
        return pc++;
      } else {
        return relative_base + prog[pc++];
      }
    };
    
    if (op == 1) {
      int64 a = ParamAddr();
      int64 b = ParamAddr();
      int64 c = ParamAddr();
      prog[c] = prog[a] + prog[b];
    } else if (op == 2) {
      int64 a = ParamAddr();
      int64 b = ParamAddr();
      int64 c = ParamAddr();
      prog[c] = prog[a] * prog[b];
    } else if (op == 3) {
      int64 c = ParamAddr();
      prog[c] = GetColor(x, y);
    } else if (op == 4) {
      int64 c = ParamAddr();
      if (parity == 0) {
        SetColor(x, y, prog[c]);
      } else {
        d = (d + (prog[c] == 0 ? 3 : 1)) % 4;
        x += dx[d];
        y += dy[d];
      }
      parity ^= 1;
    } else if (op == 5) {
      int64 a = ParamAddr();
      int64 b = ParamAddr();
      if (prog[a] != 0) pc = prog[b];
    } else if (op == 6) {
      int64 a = ParamAddr();
      int64 b = ParamAddr();
      if (prog[a] == 0) pc = prog[b];
    } else if (op == 7) {
      int64 a = ParamAddr();
      int64 b = ParamAddr();
      int64 c = ParamAddr();
      prog[c] = prog[a] < prog[b];
    } else if (op == 8) {
      int64 a = ParamAddr();
      int64 b = ParamAddr();
      int64 c = ParamAddr();
      prog[c] = prog[a] == prog[b];
    } else if (op == 9) {
      int64 a = ParamAddr();
      relative_base += prog[a];
    } else {
      return true;
    }
  }
}

void Draw() {
  int minx = 0, maxx = 0;
  int miny = 0, maxy = 0;
  for (const auto& entry : color) {
    int x = entry.first.x;
    int y = entry.first.y;
    minx = min(minx, x);
    maxx = max(maxx, x);
    miny = min(miny, y);
    maxy = max(maxy, y);
  }
  for (int y = maxy; y >= miny; --y) {
    for (int x = minx; x <= maxx; ++x) {
      printf("%d", GetColor(x, y));
    }
    printf("\n");
  }
}

int main() {
  vector<int64> prog = Read();
  SetColor(0, 0, 1);
  Execute(prog);
  printf("%d\n", (int)color.size());
  Draw();
  return 0;
}
