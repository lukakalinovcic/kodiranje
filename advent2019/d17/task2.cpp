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

void Execute(const vector<int64>& start, const char* input, int64* output) {
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
      int64 c = ParamAddr();
      printf("%c", *input);
      if (*input == 10) printf("\n");
      prog[c] = *input++;
    } else if (op == 4) {
      int64 c = ParamAddr();
      if (output != nullptr) {
        if (prog[c] > 256) {
          *output = prog[c];
        } else {
          printf("%c", (char)prog[c]);
        }
      } else {
        int val = prog[c];
        if (val == 10) {
          a[R][C] = 0;
          ++R;
          C = 0;
        } else {
          a[R][C++] = val;
        }
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

int r0, c0, d;

bool Inside(int r, int c) {
  return (r >= 0 && r < R) && (c >= 0 && c < C);
}

bool Scaffold(int r, int c) {
  return Inside(r, c) && a[r][c] != '.';
}

struct Move {
  bool left = false;
  bool right = false;
  int straight = 0;
};
vector<Move> moves;
bool Walk() {
  int rr = r0 + dr[d];
  int cc = c0 + dc[d];
  if (!Scaffold(rr, cc)) {
    bool turned = false;
    for (int dd = d - 1; dd <= d + 1; dd += 2) {
      rr = r0 + dr[(dd + 4) % 4];
      cc = c0 + dc[(dd + 4) % 4];
      if (Scaffold(rr, cc)) {
        turned = true;
        moves.emplace_back();
        if (dd == d - 1) moves.back().left = true;
        else moves.back().right = true;
        d = (dd + 4) % 4;
        break;
      }
    }
    if (!turned) return false;
  }
  if (a[r0][c0] == '#') a[r0][c0] = '.';
  else a[r0][c0] = '#';
  r0 = rr;
  c0 = cc;
  moves.back().straight += 1;
  return true;
}

int main() {
  vector<int64> prog = Read();
  Execute(prog, "", nullptr);
  while (a[0][C] > 0) ++C;
  --R;
  printf("%d %d\n", R, C);
  for (int r = 0; r < R; ++r) {
    for (int c = 0; c < C; ++c) {
      if (a[r][c] == '.') continue;
      if (a[r][c] != '.' && a[r][c] != '#') {
        printf("%d", a[r][c]);
        r0 = r;
        c0 = c;
        if (a[r][c] == '^') d = 0;
        if (a[r][c] == '>') d = 1;
        if (a[r][c] == 'v') d = 2;
        if (a[r][c] == '<') d = 3;
      }
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
        a[r][c] = 'O';
      }

    }
    printf("%s\n", a[r]);
  }
  a[r0][c0] = '#';
  printf("%d %d %d\n", r0, c0, d);
  while (Walk());
  for (const Move& move : moves) {
    if (move.left) printf("L,");
    if (move.right) printf("R,");
    printf("%d,", move.straight);
  }
  printf("\n");
  // L,10,L,10,R,6,L,10,L,10,R,6,R,12,L,12,L,12,R,12,L,12,L,12,L,6,L,10,R,12,R,12,R,12,L,12,L,12,L,6,L,10,R,12,R,12,R,12,L,12,L,12,L,6,L,10,R,12,R,12,L,10,L,10,R,6
  
  // A=L,10,L,10,R,6
  // B=R,12,L,12,L,12
  // C=L,6,L,10,R,12,R,12
  // A,A,B,B,C,B,C,B,C,A

  int64 result = 0;
  prog[0] = 2;
  Execute(prog, "A,A,B,B,C,B,C,B,C,A\nL,10,L,10,R,6\nR,12,L,12,L,12\nL,6,L,10,R,12,R,12\ny\n", &result);
  printf("%lld\n", result);
  return 0;
}
  
