#include <algorithm>
#include <cassert>
#include <cstdio>
#include <map>
#include <string>
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


int64 Execute(const vector<int64>& start, const char* input) {
  map<int64, int64> prog;
  int64 pc = 0;
  int64 relative_base = 0;
  for (int i = 0; i < (int)start.size(); ++i) {
    prog[i] = start[i];
  }
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
      prog[c] = *input++;
    } else if (op == 4) {
      int64 c = ParamAddr();
      if (prog[c] > 256) return prog[c];
      printf("%c", (char)prog[c]);
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
      return -1;
    }
  }
}


int main() {
  vector<int64> prog = Read();
  vector<string> instructions = {
    "OR F J",
    "OR I J",
    "OR E T",
    "AND T J",  // J = E AND (F OR I)
    "NOT H T",
    "NOT T T",
    "OR T J",   // J = E AND (F OR I) OR H
    "AND D J",  // J = D AND (E AND (F OR I) OR H)
    "NOT B T",
    "NOT T T",
    "AND C T",
    "NOT T T",  // T = NOT B OR NOT C
    "AND T J",
    "NOT A T",
    "OR T J",   // J = NOT A OR (NOT B OR NOR C) AND D AND (E AND (F OR I) OR H)
    "RUN",
  };
  string joined;
  for (const string& s : instructions) {
    joined += s + string("\n");
  }
  printf("%lld\n", Execute(prog, joined.c_str()));
  return 0;
}
