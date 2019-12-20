#include <cassert>
#include <cctype>
#include <cstdio>
#include <cstring>
#include <map>
#include <queue>
#include <string>
#include <vector>

using namespace std;

int R, C;
char a[200][201];

struct Coords {
  int r, c;
};
bool operator<(const Coords &A, const Coords &B) {
  if (A.r != B.r) return A.r < B.r;
  if (A.c != B.c) return A.c < B.c;
  return 0;
}
const int dr[4] = { -1, 0, 1, 0 };
const int dc[4] = { 0, 1, 0, -1 };

map<string, vector<Coords>> marks;
map<Coords, pair<Coords, int>> portal;
map<pair<Coords, int>, int> dist;

char Get(int r, int c) {
  return (r >= 0 && r < R) && (c >= 0 && c < C) ? a[r][c] : ' ';
}

bool IsInner(int r, int c) {
  int d = min(min(r, R - r), min(c, C - c));
  return d > 4;
}

int main() {
  while (fgets(a[R], sizeof a[R], stdin)) ++R;
  C = strlen(a[0]);

  for (int r = 0; r < R; ++r) {
    for (int c = 0; c < C; ++c) {
      if (isupper(a[r][c])) {
        if (isupper(Get(r + 1, c))) {
          string key = string(1, a[r][c]) + string(1, a[r + 1][c]);
          if (Get(r + 2, c) == '.') {
            marks[key].push_back(Coords{r + 2, c});
          } else {
            assert(Get(r - 1, c) == '.');
            marks[key].push_back(Coords{r - 1, c});
          }
        } else if (isupper(Get(r, c + 1))) {
          string key = string(1, a[r][c]) + string(1, a[r][c + 1]);
          if (Get(r, c + 2) == '.') {
            marks[key].push_back(Coords{r, c + 2});
          } else {
            assert(Get(r, c - 1) == '.');
            marks[key].push_back(Coords{r, c - 1});
          }
        }
      }
    }
  }

  assert(marks["AA"].size() == 1);
  assert(marks["ZZ"].size() == 1);
  for (const auto& entry : marks) {
    if (entry.second.size() < 2) continue;
    assert(entry.second.size() == 2);
    Coords a = entry.second[0];
    Coords b = entry.second[1];
    portal[a] = make_pair(b, IsInner(a.r, a.c) ? 1 : -1);
    portal[b] = make_pair(a, IsInner(b.r, b.c) ? 1 : -1);
  }

  Coords s = marks["AA"][0];
  Coords t = marks["ZZ"][0];
  queue<pair<Coords, int>> Q;
  Q.push(make_pair(s, 0));
  dist[make_pair(s, 0)] = 0;
  while (!Q.empty()) {
    pair<Coords, int> a = Q.front();
    Q.pop();
    if (a.second >= 50) continue;
    for (int d = 0; d < 4; ++d) {
      pair<Coords, int> b(Coords{a.first.r + dr[d], a.first.c + dc[d]}, a.second);
      if (Get(b.first.r, b.first.c) != '.') continue;
      if (dist.count(b)) continue;
      dist[b] = dist[a] + 1;
      Q.push(b);
    }
    if (portal.count(a.first)) {
      pair<Coords, int> b(portal[a.first].first, a.second + portal[a.first].second);
      if (b.second < 0) continue;
      if (dist.count(b)) continue;
      dist[b] = dist[a] + 1;
      Q.push(b);
    }
  }
  printf("%d\n", dist[make_pair(t, 0)]);

  return 0;
}
