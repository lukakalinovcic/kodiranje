#include <cstdio>
#include <cstring>

char s[65536];

int main() {
  scanf("%s", s);
  char *p = s;
  int min_n0 = 25 * 6 + 1;
  int result = 0;
  while (*p) {    
    int n0 = 0, n1 = 0, n2 = 0;
    for (int r = 0; r < 6; ++r) {
      for (int c = 0; c < 25; ++c) {
        if (*p == '0') ++n0;
        if (*p == '1') ++n1;
        if (*p == '2') ++n2;
        ++p;
      }
    }
    if (n0 < min_n0) {
      min_n0 = n0;
      result = n1 * n2;
    }
  }
  printf("%d\n", result);
  return 0;
}
