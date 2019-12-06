#include <cstdio>

int main() {
  int sum = 0, x;
  while (scanf("%d", &x) == 1) {
    for(;;) {
      x = x / 3 - 2;
      if (x > 0) {
        sum += x;
      } else {
        break;
      }
    }
  }
  printf("%d\n", sum);
  return 0;
}
