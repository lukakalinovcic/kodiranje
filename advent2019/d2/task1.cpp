#include <cassert>
#include <cstdio>
#include <vector>

using namespace std;

char s[65536];

int main() {
  scanf("%s", s);
  vector<int> prog;
  for (int i = 0, j; s[i]; i = j) {
    int x = 0;
    for (j = i; s[j] && s[j] != ','; ++j) {
      x = x * 10 + s[j] - '0';
    }
    if (s[j] == ',') ++j;
    prog.push_back(x);
  }
  prog[1] = 12;
  prog[2] = 2;

  int pc = 0;
  for (;;) {
    int op = prog[pc];
    int a = prog[pc + 1];
    int b = prog[pc + 2];
    int c = prog[pc + 3];
    assert(a < prog.size());
    assert(b < prog.size());
    assert(c < prog.size());
    printf(":: %d %d %d %d\n", op, a, b, c);
    pc += 4;
    if (op == 1) {
      prog[c] = prog[a] + prog[b];
    } else if (op == 2) {
      prog[c] = prog[a] * prog[b];
    } else {
      break;
    }
  }
  printf("%d\n", prog[0]);
  
  return 0;
}
