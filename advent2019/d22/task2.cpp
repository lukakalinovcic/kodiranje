#include <cassert>
#include <cstdio>
#include <vector>

using namespace std;

typedef long long int64;
typedef __int128 int128;

const int64 n = 119315717514047LL;
const int64 kCycles = 101741582076661LL;

void Identity(int64 a[3][3]) {
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      a[i][j] = (i == j);
    }
  }
}

void Mult(int64 a[3][3], int64 b[3][3], int64 c[3][3]) {
  int128 tmp[3][3];
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      tmp[i][j] = 0;
      for (int k = 0; k < 3; ++k) {
        tmp[i][j] += int128(a[i][k]) * int128(b[k][j]);
      }
    }
  }
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      c[i][j] = tmp[i][j] % n;
    }
  }
}

void Pow(int64 a[3][3], int64 n, int64 b[3][3]) {
  if (n == 0) {
    Identity(b);
  } else if (n % 2 == 0) {
    int64 t[3][3];
    Pow(a, n / 2, t);
    Mult(t, t, b);
  } else {
    int64 t[3][3];
    Pow(a, n - 1, t);
    Mult(a, t, b);
  }
}

// a * x + b * y = g
void Euclid(int128 a, int128 b, int128& x, int128& y, int128& g) {
  if (b == 0) {
    x = 1; y = 0; g = a;
  } else {
    Euclid(b, a % b, y, x, g);
    y -= x * (a / b);
  }
}

struct Operation {
  enum Type { kCut, kDealIntoNewStack, kDealWithIncrement };
  Type type;
  int64 k;

  void ToMatrix(int64 a[3][3]) {
    for (int i = 0; i < 3; ++i) {
      for (int j = 0; j < 3; ++j) {
        a[i][j] = 0;
      }
    }
    a[2][2] = 1;
    if (type == kCut) {
      a[0][0] = 1;
      a[1][1] = 1;
      a[1][2] = -k;
    } else if (type == kDealIntoNewStack) {
      a[0][0] = -1;
      a[1][1] = -1;
      a[1][2] = -1;
    } else {
      a[0][0] = k;
      a[1][1] = k;
    }
  }
};

int64 m[3][3];
int64 o[3][3];

int128 Mod(int128 x) { return (x % n + n) % n; }

int main() {
  Identity(m);
  char s[1024];
  while (fgets(s, sizeof s, stdin)) {
    Operation op;
    if (s[0] == 'c') {
      op.type = Operation::kCut;
      assert(sscanf(s + 4, "%lld", &op.k) == 1);
    } else if (s[5] == 'i') {
      op.type = Operation::kDealIntoNewStack;
      op.k = 0;
    } else {
      assert(s[5] == 'w');
      op.type = Operation::kDealWithIncrement;
      assert(sscanf(s + 20, "%lld", &op.k) == 1);
    }
    op.ToMatrix(o);
    Mult(o, m, m);
  }
  Pow(m, kCycles, m);
  int64 pos = 2020;
  int128 a = Mod(m[0][0]);
  int128 b = Mod(pos - m[1][2]);
  int128 i, j, g;
  Euclid(a, n, i, j, g);
  assert(g == 1);
  int64 result = Mod(i * b);
  printf("%lld\n", result);
  return 0;
}
