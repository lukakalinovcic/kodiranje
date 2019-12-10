#include <algorithm>
#include <cassert>
#include <cstdio>
#include <vector>

using namespace std;

vector<int> Read() {
  static char s[65536];
  scanf("%s", s);
  char* p = s;
  int x, k;
  vector<int> prog;
  while (sscanf(p, "%d%n", &x, &k) == 1) {
    prog.push_back(x);
    p += k;
    if (*p == ',') ++p;
  }
  return prog;
}

int Execute(vector<int> prog, vector<int> inputs) {
  int next_input = 0;
  int pc = 0;
  for (;;) {
    int value = prog[pc++];
    int op = value % 100; value /= 100;
    auto ParamAddr = [&]() {
      int mode = value % 10; value /= 10;
      assert(mode == 0 || mode == 1);
      if (mode == 0) {
        return prog[pc++];
      } else {
        return pc++;
      }
    };
    
    if (op == 1) {
      int a = ParamAddr();
      int b = ParamAddr();
      int c = ParamAddr();
      prog[c] = prog[a] + prog[b];
    } else if (op == 2) {
      int a = ParamAddr();
      int b = ParamAddr();
      int c = ParamAddr();
      prog[c] = prog[a] * prog[b];
    } else if (op == 3) {
      int c = ParamAddr();
      assert(next_input < (int)inputs.size());
      prog[c] = inputs[next_input++];
    } else if (op == 4) {
      int c = ParamAddr();
      return prog[c];
    } else if (op == 5) {
      int a = ParamAddr();
      int b = ParamAddr();
      if (prog[a] != 0) pc = prog[b];
    } else if (op == 6) {
      int a = ParamAddr();
      int b = ParamAddr();
      if (prog[a] == 0) pc = prog[b];
    } else if (op == 7) {
      int a = ParamAddr();
      int b = ParamAddr();
      int c = ParamAddr();
      prog[c] = prog[a] < prog[b];
    } else if (op == 8) {
      int a = ParamAddr();
      int b = ParamAddr();
      int c = ParamAddr();
      prog[c] = prog[a] == prog[b];
    } else {
      assert(false);
      break;
    }
  }
}

int main() {
  vector<int> prog = Read();
  vector<int> perm = {0, 1, 2, 3, 4};
  int max_thrust = 0;
  do {
    int thrust = 0;
    for (int x : perm) {
      thrust = Execute(prog, {x, thrust});
    }
    max_thrust = max(max_thrust, thrust);
  } while (next_permutation(perm.begin(), perm.end()));
  printf("%d\n", max_thrust);
  return 0;
}
