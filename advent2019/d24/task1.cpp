#include <cstdio>
#include <set>

using namespace std;

char a[5][6];

void Step() {
  static char b[5][5];
  static int dr[4] = { -1, 0, 1, 0 };
  static int dc[4] = { 0, 1, 0, -1 };
  for (int r = 0; r < 5; ++r) {
    for (int c = 0; c < 5; ++c) {
      b[r][c] = a[r][c];
      int neighbors = 0;
      for (int d = 0; d < 4; ++d) {
        int rr = r + dr[d];
        int cc = c + dc[d];
        if (rr < 0 || rr >= 5) continue;
        if (cc < 0 || cc >= 5) continue;
        neighbors += a[rr][cc] == '#';
      }
      if (b[r][c] == '#' && neighbors != 1) {
        b[r][c] = '.';
      } else if (b[r][c] == '.' && neighbors >= 1 && neighbors <= 2) {
        b[r][c] = '#';
      }
    }
  }
  for (int r = 0; r < 5; ++r) {
    for (int c = 0; c < 5; ++c) {
      a[r][c] = b[r][c];
    }
  }
}

int GetMask() {
  int mask = 0;
  for (int r = 0; r < 5; ++r) {
    for (int c = 0; c < 5; ++c) {
      mask |= (a[r][c] == '#') << (r * 5 + c);
    }
  }
  return mask;
}

int main() {
  for (int r = 0; r < 5; ++r) {
    scanf("%s", a[r]);
  }

  set<int> seen;
  seen.insert(GetMask());
  for (;;) {
    Step();
    if (seen.count(GetMask())) break;
    seen.insert(GetMask());
  }
  for (int r = 0; r < 5; ++r) {
    printf("%s\n", a[r]);
  }
  printf("%d\n", GetMask());
  
  return 0;
}
