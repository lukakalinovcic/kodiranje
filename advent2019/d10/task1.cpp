#include <algorithm>
#include <cmath>
#include <cstdio>

int R, C;
char a[100][101];

int gcd(int a, int b) { return b == 0 ? a : gcd(b, a % b); }

int main() {
  while (scanf("%s", a[R]) == 1) ++R;
  while (a[0][C]) ++C;

  int best_r = -1, best_c = -1;
  int max_cnt = 0;
  for (int r0 = 0; r0 < R; ++r0) {
    for (int c0 = 0; c0 < C; ++c0) {
      if (a[r0][c0] != '#') continue;
      int cnt = 0;
      for (int dr = -R; dr <= R; ++dr) {
        if (r0 + dr < 0 || r0 + dr >= R) continue;
        for (int dc = -C; dc <= C; ++dc) {
          if (c0 + dc < 0 || c0 + dc >= C) continue;
          if (dr == 0 && dc == 0) continue;
          if (gcd(abs(dr), abs(dc)) != 1) continue;

          int r = r0;
          int c = c0;
          for (;;) {
            r += dr;
            c += dc;
            if (r < 0 || r >= R) break;
            if (c < 0 || c >= C) break;
            if (a[r][c] == '#') {
              ++cnt;
              break;
            }
          }
        }
      }
      if (cnt > max_cnt) {
        max_cnt = cnt;
        best_r = r0;
        best_c = c0;
      }
    }
  }
  printf("%d\n", max_cnt);
  printf("%d %d\n", best_r, best_c);
  return 0;
}
