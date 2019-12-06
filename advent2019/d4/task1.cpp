#include <cstdio>

bool Ok(int x) {
  bool same2 = false;
  int prev = 10;
  for (int i = 0; i < 6; ++i) {
    int curr = x % 10;
    x /= 10;
    if (prev < curr) return 0;
    if (prev == curr) same2 = true;
    prev = curr;
  }
  return same2;
}

int main() {
  int a = 128392;
  int b = 643281;
  int c = 0;
  for (int x = a; x <= b; ++x) {
    c += Ok(x);
  }
  printf("%d\n", c);
  return 0;
}
