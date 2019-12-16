#include <cstring>
#include <cstdio>

int pattern(int row, int col) {
  int k = col % (4 * row);
  k /= row;
  if (k % 4 == 1) return 1;
  if (k % 4 == 3) return -1;
  return 0;
}

char a[1024];
char b[1024];

int main() {
  scanf("%s", a);
  int n = strlen(a);
  for (int iter = 0; iter < 100; ++iter) {
    for (int i = 0; i < n; ++i) {
      int sum = 0;
      for (int j = 0; j < n; ++j) sum += (a[j] - '0') * pattern(i + 1, j + 1);
      b[i] = sum % 10;
      if (b[i] < 0) b[i] = - b[i];
      b[i] += '0';
    }
    
    for (int i = 0; i < n; ++i) a[i] = b[i];
    printf("%s\n", a);
  }
  return 0;
}
