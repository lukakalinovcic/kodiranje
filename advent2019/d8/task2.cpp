#include <cstdio>
#include <cstring>

char s[65536];

int main() {
  scanf("%s", s);
  char *p = s;
  char out[6][26];
  for (int r = 0; r < 6; ++r) {
    for (int c = 0; c < 25; ++c) {
      out[r][c] = '2';
    }
    out[r][25] = 0;
  }
  
  while (*p) {    
    for (int r = 0; r < 6; ++r) {
      for (int c = 0; c < 25; ++c) {
        if (out[r][c] == '2') out[r][c] = *p;
        ++p;
      }
    }
  }
  for (int r = 0; r < 6; ++r) {
    printf("%s\n", out[r]);
  }
  return 0;
}
