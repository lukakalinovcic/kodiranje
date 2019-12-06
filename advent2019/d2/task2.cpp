#include <cassert>
#include <cstdio>
#include <vector>

using namespace std;

char s[65536];

vector<int> Reset(int a, int b) {
  vector<int> prog;
  for (int i = 0, j; s[i]; i = j) {
    int x = 0;
    for (j = i; s[j] && s[j] != ','; ++j) {
      x = x * 10 + s[j] - '0';
    }
    if (s[j] == ',') ++j;
    prog.push_back(x);
  }
  prog[1] = a;
  prog[2] = b;
  return prog;
}

int Execute(vector<int> prog) {
  int pc = 0;
  for (;;) {
    int op = prog[pc];
    int a = prog[pc + 1];
    int b = prog[pc + 2];
    int c = prog[pc + 3];
    assert(a < prog.size());
    assert(b < prog.size());
    assert(c < prog.size());
    pc += 4;
    if (op == 1) {
      prog[c] = prog[a] + prog[b];
    } else if (op == 2) {
      prog[c] = prog[a] * prog[b];
    } else {
      break;
    }
  }
  return prog[0];
}

int main() {
  scanf("%s", s);
  int sz = Reset(12, 2).size(); 
  for (int a = 1; a < sz; ++a) {
    for (int b = 1; b < sz; ++b) {
      int c = Execute(Reset(a, b));
      if (c == 19690720) {
        printf("%d %d\n", a, b);
      }
    }
  }
    
  
  return 0;
}
