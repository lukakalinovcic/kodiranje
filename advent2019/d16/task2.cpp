#include <cstring>
#include <cstdio>

int n;
char a[65000001];
char b[65000001];
int sum[65000001];

inline int GetSum(int lo, int hi) {
  if (hi >= n) hi = n;
  return sum[hi] - sum[lo];
}

int main() {
  scanf("%s", a);
  n = strlen(a);
  for (int i = 0; i < n; ++i) {
    for (int j = 1; j < 10000; ++j) {
      a[j * n + i] = a[(j - 1) * n + i];
    }
  }
  n *= 10000;
  a[n] = 0;
  int offset = 0;
  for (int i = 0; i < 7; ++i) {
    offset = offset * 10 + a[i] - '0';
  }
  for (int iter = 0; iter < 100; ++iter) {
    printf("Iter %d\n", iter);
    sum[0] = 0;
    for (int i = 1; i <= n; ++i) sum[i] = sum[i - 1] + a[i - 1] - '0';

    for (int i = 0; i < n; ++i) {
      int sum = 0;
      int k = (i + 1);
      int mode = 0;
      for (int j = -1 + k; j < n; j += 2 * k) {
        if (mode == 0) sum += GetSum(j, j + k);
        if (mode == 1) sum -= GetSum(j, j + k);
        mode = mode ^ 1;
      }
      a[i] = sum % 10;
      if (a[i] < 0) a[i] = -a[i];
      a[i] += '0';
    }
  }
  a[offset + 8] = 0;
  printf("%s\n", a + offset);
  return 0;
}
