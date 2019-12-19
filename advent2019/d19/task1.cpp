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
  prog.clear();
  for (int i = 0; i < (int)start.size(); ++i) {
    prog[i] = start[i];
  }
  pc = 0;
  relative_base = 0;
}

int64 Execute(int64 x, int64 y) {
  bool first = true;
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
      if (first) {
        prog[c] = x;
        first = false;
      } else {
        prog[c] = y;
      }
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

char a[50][51];

int main() {
  vector<int64> prog = Read();

  int cnt = 0;
  for (int r = 0; r < 50; ++r) {
    for (int c = 0; c < 50; ++c) {
      Init(prog);
      a[r][c] = Execute(r, c) ? '#' : '.';
      cnt += a[r][c] == '#';
    }
    printf("%s\n", a[r]);
  }
  printf("%d\n", cnt);

  return 0;
}
