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

int R = 0, C = 0;
char a[256][257];

void Execute(const vector<int64>& start) {
  map<int64, int64> prog;
  for (int i = 0; i < (int)start.size(); ++i) {
    prog[i] = start[i];
  }
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
      int val = prog[c];
      if (val == 10) {
        a[R][C] = 0;
        ++R;
        C = 0;
      } else {
        a[R][C++] = val;
      }
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
      return;
    }
  }
}

const int dr[4] = { -1, 0, 1, 0 };
const int dc[4] = { 0, 1, 0, -1 }; 

int main() {
  vector<int64> prog = Read();
  Execute(prog);
  while (a[0][C] > 0) ++C;
  --R;
  int result = 0;
  printf("%d %d\n", R, C);
  for (int r = 0; r < R; ++r) {
    for (int c = 0; c < C; ++c) {
      if (a[r][c] == '.') continue;
      int paths = 0;
      for (int d = 0; d < 4; ++d) {
        int rr = r + dr[d];
        int cc = c + dc[d];
        if (rr < 0 || rr >= R) continue; 
        if (cc < 0 || cc >= C) continue; 
        if (a[rr][cc] == '.') continue;
        ++paths;
      }
      if (paths == 4) {
        result += r * c;
        a[r][c] = 'O';
      }
    }
    printf("%s\n", a[r]);
  }
  printf("Result: %d\n", result);
  return 0;
}
