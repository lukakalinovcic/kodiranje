#include <algorithm>
#include <cassert>
#include <cstdio>
#include <map>
#include <queue>
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

struct State {
  map<int64, int64> prog;
  int64 pc = 0;
  int64 relative_base = 0;
  int address;
  queue<vector<int64>> in;
  queue<vector<int64>> out;
};

void Init(State& state, const vector<int64>& start, int address) {
  for (int i = 0; i < (int)start.size(); ++i) {
    state.prog[i] = start[i];
  }
  state.address = address;
  state.pc = 0;
  state.relative_base = 0;
  state.in.push({address});
}

void Execute(State& state) {
  map<int64, int64>& prog = state.prog;
  int64& pc = state.pc;
  int64& relative_base = state.relative_base;

  bool input_done = false;
  int next_input = 0;
  vector<int64> curr_out;
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
      if (input_done) {
        --pc;
        return;
      }
      int64 c = ParamAddr();
      if (state.in.empty()) {
        prog[c] = -1;
        //printf("%d inputed -1\n", state.address);
        input_done = true;
      } else {
        prog[c] = state.in.front()[next_input++];
        if (next_input == (int)state.in.front().size()) {
          //printf("%d inputed ", state.address);
          //for (int64 x : state.in.front()) {
          //  printf(" %lld", x);
          //}
          //printf("\n");
          state.in.pop();
          input_done = true;
        }
      }
    } else if (op == 4) {
      int64 c = ParamAddr();
      curr_out.push_back(prog[c]);
      if (curr_out.size() == 3) {
        //printf("%d outputed (%lld %lld %lld)\n", state.address, curr_out[0], curr_out[1], curr_out[2]);
        state.out.push(curr_out);
        curr_out.clear();
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
      assert(false);
    }
  }
}

int main() {
  const int n = 50;
  vector<int64> prog = Read();
  vector<State> state(n);
  for (int i = 0; i < n; ++i) {
    Init(state[i], prog, i);
    Execute(state[i]);
  }
  for (int iter = 0;; ++iter) {
    for (int i = 0; i < n; ++i) {
      Execute(state[i]);
    }
    for (int i = 0; i < n; ++i) {
      while (!state[i].out.empty()) {
        vector<int64> data = state[i].out.front();
        state[i].out.pop();
        int address = data[0];
        data.erase(data.begin());
        if (address < n) {
          state[address].in.push(data);
        } else {
          printf("%d output to %d ", i, address);
          for (int64 x : data) {
            printf(" %lld", x);
          }
          printf("\n");
        }
      }
    }
    int buffer_cnt = 0;
    for (int i = 0; i < n; ++i) {
      buffer_cnt += state[i].in.size();
    }
    printf("Iter %d: total buffer size %d\n", iter, buffer_cnt);
    if (buffer_cnt == 0) break;
  }
  return 0;
}
