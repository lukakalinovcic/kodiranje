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
map<Coords, Coords> portal;
map<Coords, int> dist;

char Get(int r, int c) {
  return (r >= 0 && r < R) && (c >= 0 && c < C) ? a[r][c] : ' ';
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
    portal[a] = b;
    portal[b] = a;
  }

  Coords s = marks["AA"][0];
  Coords t = marks["ZZ"][0];
  queue<Coords> Q;
  Q.push(s);
  dist[s] = 0;
  while (!Q.empty()) {
    Coords a = Q.front();
    Q.pop();
    for (int d = 0; d < 4; ++d) {
      Coords b{a.r + dr[d], a.c + dc[d]};
      if (Get(b.r, b.c) != '.') continue;
      if (dist.count(b)) continue;
      dist[b] = dist[a] + 1;
      Q.push(b);
    }
    if (portal.count(a)) {
      Coords b = portal[a];
      if (dist.count(b)) continue;
      dist[b] = dist[a] + 1;
      Q.push(b);
    }
  }
  printf("%d\n", dist[t]);

  return 0;
}
