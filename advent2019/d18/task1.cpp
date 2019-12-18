#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstdio>
#include <cstring>
#include <map>
#include <vector>

using namespace std;

int R, C;
char a[100][100];
int parent[100][100];
const int dr[4] = { -1, 0, 1, 0 };
const int dc[4] = { 0, 1, 0, -1 };

bool BuildTree(int r, int c) {
  char tmp = a[r][c];
  a[r][c] = '!';
  for (int d = 0; d < 4; ++d) {
    if (d == parent[r][c]) continue;
    int rr = r + dr[d];
    int cc = c + dc[d];
    if (rr < 0 || rr >= R) continue;
    if (cc < 0 || cc >= C) continue;
    if (a[rr][cc] == '#') continue;
    if (a[rr][cc] == '@') continue;
    if (a[rr][cc] == '&') continue;
    if (a[rr][cc] == '!') return false;
    parent[rr][cc] = d ^ 2;
    if (!BuildTree(rr, cc)) return false;
  }
  a[r][c] = tmp;
  return true;
}

struct Coords {
  int r, c;
};
bool Equal(Coords a, Coords b) {
  return a.r == b.r && a.c == b.c;
}

Coords key[27];

vector<Coords> PathToRoot(Coords a) {
  vector<Coords> aa = {a};
  while (parent[a.r][a.c] != -1) {
    int d = parent[a.r][a.c];
    a.r += dr[d];
    a.c += dc[d];
    aa.push_back(a);
  }
  return aa;
}

int DoorMask(const vector<Coords>& path) {
  int door_mask = 0;
  for (Coords cell : path) {
    if (isupper(a[cell.r][cell.c])) {
      door_mask |= (1 << (a[cell.r][cell.c] - 'A'));
    }
  }
  return door_mask;
}

void Path(Coords a, Coords b, int* path_len, int* door_mask) {
  vector<Coords> aa = PathToRoot(a);
  vector<Coords> bb = PathToRoot(b);
  reverse(aa.begin(), aa.end());
  reverse(bb.begin(), bb.end());
  *door_mask = DoorMask(aa) | DoorMask(bb);
  *path_len = 0;
  if (!Equal(aa[0], bb[0])) {
    *path_len = -2 + abs(aa[0].r - bb[0].r) + abs(aa[0].c - bb[0].c);
  }
  int i = 0;
  while (i < aa.size() && i < bb.size() && Equal(aa[i], bb[i])) ++i;
  *path_len += aa.size() - i;
  *path_len += bb.size() - i;
}

int path_len[27][27];
int door_mask[27][27];

map<pair<int, int>, int> memo;

int Rec(int i, int key_mask) {
  pair<int, int> memo_key(i, key_mask);
  int& result = memo[memo_key];
  if (result > 0) return result;
  result = 1000000;
  bool done = true;
  for (int j = 0; j < 26; ++j) {
    if (key[j].r == 0) continue;
    if ((key_mask >> j) & 1) continue;
    done = false;
    if ((key_mask & door_mask[i][j]) != door_mask[i][j]) continue;
    result = min(result, path_len[i][j] + Rec(j, key_mask | (1 << j)));
  }
  if (done) { result = 0; }
  return result;
}

int main() {
  while (scanf("%s", a[R]) == 1) ++R;
  C = strlen(a[0]);

  memset(parent, -1, sizeof parent);
  for (int r = 0; r < R; ++r) {
    for (int c = 0; c < C; ++c) {
      if (a[r][c] == '@' || a[r][c] == '&') {
        assert(BuildTree(r, c));
      }
      if (a[r][c] == '@') {
        key[26] = Coords{r, c};
      }
      if (islower(a[r][c])) {
        key[a[r][c] - 'a'] = Coords{r, c};
      }
    }
  }
  for (int i = 0; i <= 26; ++i) {
    if (key[i].r == 0) continue;
    for (int j = 0; j <= 26; ++j) {
      if (key[j].r == 0) continue;
      Path(key[i], key[j], &path_len[i][j], &door_mask[i][j]);
    }
  }
  printf("%d\n", Rec(26, 0));
  return 0;
}
