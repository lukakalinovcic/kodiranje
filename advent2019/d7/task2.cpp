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

struct State {
  vector<int> prog;
  int pc = 0;
};

bool Execute(State* state, vector<int> input, int* output) {
  vector<int>& prog = state->prog;
  int& pc = state->pc;
  int next_input = 0;
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
      assert(next_input < (int)input.size());
      prog[c] = input[next_input++];
    } else if (op == 4) {
      int c = ParamAddr();
      *output = prog[c];
      return true;
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
      return false;
    }
  }
}

int main() {
  vector<int> prog = Read();
  vector<int> perm = {5, 6, 7, 8, 9};
  int max_thrust = 0;
  do {
    int thrust = 0;
    State state[5];
    for (int i = 0; i < 5; ++i) {
      state[i].prog = prog;
      state[i].pc = 0;
      assert(Execute(&state[i], {perm[i], thrust}, &thrust));
    }
    bool done = false;
    while (!done) {
      for (int i = 0; i < 5; ++i) {
        if (!Execute(&state[i], {thrust}, &thrust)) {
          assert(i == 0);
          done = true;
          break;
        }
      }
    }
    max_thrust = max(max_thrust, thrust);
  } while (next_permutation(perm.begin(), perm.end()));
  printf("%d\n", max_thrust);
  return 0;
}
