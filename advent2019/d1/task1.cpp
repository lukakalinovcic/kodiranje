#include <cstdio>

int main() {
  int sum = 0, x;
  while (scanf("%d", &x) == 1) {
    sum += x / 3 - 2;
  }
  printf("%d\n", sum);
  return 0;
}
