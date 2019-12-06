#include <cassert>
#include <cstdio>
#include <vector>

using namespace std;

char s[65536];

vector<int> Read() {
  vector<int> prog;
  char* p = s;
  int x, k;
  while (sscanf(p, "%d%n", &x, &k) == 1) {
    prog.push_back(x);
    p += k;
    if (*p == ',') ++p;
  }
  return prog;
}

void Execute(vector<int> prog) {
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
      prog[c] = 5;
    } else if (op == 4) {
      int c = ParamAddr();
      printf("Output %d\n", prog[c]);
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
      break;
    }
  }
}

int main() {
  scanf("%s", s);
  vector<int> prog = Read();
  Execute(prog);  
  return 0;
}
