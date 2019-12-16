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

vector<int64> Execute(const vector<int64>& start) {
  map<int64, int64> prog;
  for (int i = 0; i < (int)start.size(); ++i) {
    prog[i] = start[i];
  }
  vector<int64> out;
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
      assert(false);
      int64 c = ParamAddr();
      prog[c] = 1;
    } else if (op == 4) {
      int64 c = ParamAddr();
      out.push_back(prog[c]);
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
      return out;
    }
  }
}

struct Coords {
  int x, y;
};
bool operator<(const Coords &A, const Coords &B) {
  if (A.x != B.x) return A.x < B.x;
  if (A.y != B.y) return A.y < B.y;
  return 0;
}

map<Coords, int> tile;

void Draw(const map<Coords, int>& tile) {
  int minx = 1000, maxx = -1000;
  int miny = 1000, maxy = -1000;
  for (const auto& entry : tile) {
    int x = entry.first.x;
    int y = entry.first.x;
    minx = min(minx, x);
    maxx = max(maxx, x);
    miny = min(miny, y);
    maxy = max(maxy, y);
  }
  printf("(%d %d) -- (%d %d)\n", minx, miny, maxx, maxy);
  for (int y = maxy; y >= miny; --y) {
    for (int x = minx; x <= maxx; ++x) {
      Coords C{x, y};
      int t = 0;
      const auto it = tile.find(C);
      if (it != tile.end()) {
        t = it->second;
      }
      printf("%d", t);
    }
    printf("\n");
  }
}

int main() {
  vector<int64> prog = Read();
  vector<int64> data = Execute(prog);
  assert(data.size() % 3 == 0);
  map<Coords, int> tile;
  for (int i = 0; i < (int)data.size(); i += 3) {
    Coords C;
    C.x = data[i];
    C.y = data[i + 1];
    tile[C] = data[i + 2];
  }
  Draw(tile);
  int cnt = 0;
  for (const auto& entry : tile) {
    if (entry.second == 2) ++cnt;
  }
  printf("blocks: %d\n", cnt);
  return 0;
}
