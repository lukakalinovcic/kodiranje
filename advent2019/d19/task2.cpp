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

int64 Execute(const vector<int64>& start, int64 x, int64 y) {
  map<int64, int64> prog;
  for (int i = 0; i < (int)start.size(); ++i) {
    prog[i] = start[i];
  }
  int64 pc = 0;
  int64 relative_base = 0;

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

//char a[1000];

int main() {
  vector<int64> prog = Read();

  int r0 = 100, c0 = 0;
  while (!Execute(prog, r0, c0)) ++c0;
  int r_down = r0;
  int c_right = c0;
  while (Execute(prog, r_down + 1, c0)) ++r_down;
  while (Execute(prog, r0, c_right + 1)) ++c_right;

  for (;;) {
    int height = r_down - r0 + 1;
    int width = c_right - c0 + 1;
    printf("%d %d    (%d %d - %d %d)\n", height, width, r0, c0, r_down, c_right);
    if (height >= 100 && width >= 100) break;
    if (height < width) {
      ++c0;
    } else {
      ++r0;
    }
    while (Execute(prog, r_down + 1, c0)) ++r_down;
    while (Execute(prog, r0, c_right + 1)) ++c_right;
  }
  printf("%d\n", r0 * 10000 + c0);

  return 0;
}
