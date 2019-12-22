#include <cassert>
#include <cstdio>
#include <vector>

using namespace std;

const int n = 10007;

struct Operation {
  enum Type { kCut, kDealIntoNewStack, kDealWithIncrement };
  Type type;
  int k;

  int Do(int pos) const {
    if (type == kCut) {
      return (pos - k + n) % n;
    } else if (type == kDealIntoNewStack) {
      return n - pos - 1;
    } else {
      return pos * k % n;
    }
  }
  
  int Undo(int pos) const {
    if (type == kCut) {
      return (pos + k + n) % n;
    } else if (type == kDealIntoNewStack) {
      return n - pos - 1;
    } else {
      for (int i = 0; i < n; ++i) {
        if (i * k % n == pos) return i;
      }
      assert(false);
      return 0;
    }
  }
};

int main() {
  vector<Operation> ops;
  char s[1024];
  while (fgets(s, sizeof s, stdin)) {
    Operation op;
    if (s[0] == 'c') {
      op.type = Operation::kCut;
      assert(sscanf(s + 4, "%d", &op.k) == 1);
    } else if (s[5] == 'i') {
      op.type = Operation::kDealIntoNewStack;
      op.k = 0;
    } else {
      assert(s[5] == 'w');
      op.type = Operation::kDealWithIncrement;
      assert(sscanf(s + 20, "%d", &op.k) == 1);
    }
    ops.push_back(op);
  }  

  // for (int i = 0; i < n; ++i) {
  //   int pos = i;
  //   for (int j = ops.size() - 1; j >= 0; --j) {
  //     pos = ops[j].Undo(pos);
  //   }
  //   printf("%d ", pos);
  // }
  // printf("\n");

  int pos = 2019;
  for (const Operation& op : ops) {
    pos = op.Do(pos);
  }
  printf("%d\n", pos);
  
  return 0;
}
