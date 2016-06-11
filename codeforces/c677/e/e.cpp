#include <algorithm>
#include <cmath>
#include <cstdio>

#define CHECK(condition) if (!(condition)) { \
  printf("CHECK(%s) failed\n", #condition); \
  exit(0); \
}

typedef long long int64;

using namespace std;

#define MAX 1000

int n;
char num[MAX + 2][MAX + 2];
short zero_dist[MAX + 2][MAX + 2][8];
short prefix2[MAX + 2][MAX + 2][8];
short prefix3[MAX + 2][MAX + 2][8];

const int dr[8] = { -1, -1, 0, 1, 1, 1, 0, -1 };
const int dc[8] = { 0, 1, 1, 1, 0, -1, -1, -1 }; 

const double log_2 = log(2);
const double log_3 = log(3);

bool IsBetter(int a2, int a3, int b2, int b3) {
  if (a2 <= b2 && a3 <= b3) return false;
  return log_2 * a2 + log_3 * a3 >= log_2 * b2 + log_3 * b3;
}

int main() {
  CHECK(scanf("%d", &n));
  bool only_zeroes = true;
  for (int r = 1; r <= n; ++r) {
    static char buff[MAX + 1];
    CHECK(scanf("%s", buff));
    for (int c = 1; c <= n; ++c) {
      num[r][c] = buff[c - 1] - '0';
      only_zeroes &= (num[r][c] == 0);
    }
  }
  if (only_zeroes) {
    printf("0\n");
    return 0;
  }

  for (int r0 = 0; r0 <= n + 1; ++r0) {
    for (int c0 = 0; c0 <= n + 1; ++c0) {
      if (r0 != 0 && r0 != n + 1 && 
          c0 != 0 && c0 != n + 1) {
        continue;
      }
      for (int d = 0; d < 8; ++d) {
        int inv_d = d ^ 4;
        zero_dist[r0][c0][inv_d] = 0;
        prefix2[r0][c0][inv_d] = 0;
        prefix3[r0][c0][inv_d] = 0;
        int r = r0;
        int c = c0;
        for (;;) {
          int rr = r + dr[d];
          int cc = c + dc[d];
          if (rr < 0 || rr > n + 1 || 
              cc < 0 || cc > n + 1) {
            break;
          }
          zero_dist[rr][cc][inv_d] =
              num[rr][cc] == 0 ? 0 : 1 + zero_dist[r][c][inv_d];
          prefix2[rr][cc][inv_d] = (num[rr][cc] == 2) + prefix2[r][c][inv_d];
          prefix3[rr][cc][inv_d] = (num[rr][cc] == 3) + prefix3[r][c][inv_d];
          r = rr;
          c = cc;
        }
      }
    }
  }

  int p2 = 0;
  int p3 = 0;
  for (int r = 1; r <= n; ++r) {
    for (int c = 1; c <= n; ++c) {
      if (num[r][c] == 0) continue;
      for (int d0 = 0; d0 < 2; ++d0) {
        int reach = n;
        for (int d = d0; d < 8; d += 2) {
          reach = min(reach, (int)zero_dist[r][c][d]);
        }
        int c2 = num[r][c] == 2;
        int c3 = num[r][c] == 3;
        for (int d = d0; d < 8; d += 2) {
          c2 += prefix2[r + dr[d]][c + dc[d]][d] -
                prefix2[r + reach * dr[d]][c + reach * dc[d]][d];
          c3 += prefix3[r + dr[d]][c + dc[d]][d] -
                prefix3[r + reach * dr[d]][c + reach * dc[d]][d];
        }
        if (IsBetter(c2, c3, p2, p3)) {
          p2 = c2;
          p3 = c3;
        }
      }
    }
  }
  int ret = 1;
  for (int i = 0; i < p2; ++i) {
    ret = ret * 2LL % 1000000007;
  }
  for (int i = 0; i < p3; ++i) {
    ret = ret * 3LL % 1000000007;
  }
  printf("%d\n", ret);

  return 0;
}
