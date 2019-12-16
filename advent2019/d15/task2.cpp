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

map<int64, int64> prog;
int64 pc = 0;
int64 relative_base = 0;

void Init(const vector<int64>& start) {
  for (int i = 0; i < (int)start.size(); ++i) {
    prog[i] = start[i];
  }
}

int64 Execute(int64 input) {
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
      prog[c] = input;
    } else if (op == 4) {
      int64 c = ParamAddr();
      return prog[c];
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
      assert(false);
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

void Draw() {
  int minx = 1000, maxx = -1000;
  int miny = 1000, maxy = -1000;
  for (const auto& entry : tile) {
    int x = entry.first.x;
    int y = entry.first.y;
    minx = min(minx, x);
    maxx = max(maxx, x);
    miny = min(miny, y);
    maxy = max(maxy, y);
  }
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

const int dx[4] = { 0, 0, -1, 1 };
const int dy[4] = { 1, -1, 0, 0 };

void Dfs(int x, int y) {
  for (int d = 0; d < 4; ++d) {
    int xx = x + dx[d];
    int yy = y + dy[d];
    Coords C{xx, yy};
    if (tile.count(C)) continue;
    int result = Execute(d + 1);
    if (result == 0) continue;
    if (result == 2) {
      printf("::: %d %d\n", xx, yy);
    }
    tile[C] = result;
    Dfs(xx, yy);
    Execute((d ^ 1) + 1);
  }
}

int Bfs(int x, int y) {
  map<Coords, int> dist;
  vector<Coords> Q;
  Coords C{x, y};
  Q.push_back(C);
  dist[C] = 0;
  for (int i = 0; i < Q.size(); ++i) {
    C = Q[i];
    for (int d = 0; d < 4; ++d) {
      Coords D{C.x + dx[d], C.y + dy[d]};
      if (dist.count(D)) continue;
      if (tile[D] == 0) continue;
      dist[D] = dist[C] + 1;
      Q.push_back(D);
    }
  }
  return dist[Q.back()];
}

int main() {
  vector<int64> prog = Read();
  Init(prog);
  tile[Coords{0, 0}] = 1;
  Dfs(0, 0);
  printf("%d\n", Bfs(18, -18));
  
  return 0;
}
