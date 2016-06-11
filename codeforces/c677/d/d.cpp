#include <algorithm>
#include <cstdio>
#include <queue>
#include <vector>

#define CHECK(condition) if (!(condition)) { \
  printf("CHECK(%s) failed\n", #condition); \
  exit(0); \
}

typedef long long int64;

using namespace std;

struct Coords {
  int r, c;
  Coords(int r, int c) : r(r), c(c) {}
};
struct CoordsWithDist {
  Coords coords;
  int dist;
  CoordsWithDist(Coords coords, int dist) : coords(coords), dist(dist) {}
};
bool operator<(const CoordsWithDist &A, const CoordsWithDist &B) {
  if (A.dist != B.dist) return A.dist > B.dist;
  return 0;
}

int R, C;
const int oo = 2000000000;
int dist[300][300];
const int dr[4] = { -1, 0, 1, 0 };
const int dc[4] = { 0, 1, 0, -1 }; 

vector<int> Solve(const vector<Coords>& prev,
                  const vector<int>& prev_dist,
                  const vector<Coords>& next) {
  vector<int> next_dist(next.size(), oo);
  if ((int)prev.size() * (int)next.size() > 10 * R * C) {
    for (int r = 0; r < R; ++r) {
      for (int c = 0; c < C; ++c) {
        dist[r][c] = oo;
      }
    }
    priority_queue<CoordsWithDist> PQ;
    for (int i = 0; i < (int)prev.size(); ++i) {
      dist[prev[i].r][prev[i].c] = prev_dist[i];
      PQ.push(CoordsWithDist(prev[i], prev_dist[i]));
    }
    while (!PQ.empty()) {
      Coords u = PQ.top().coords;
      PQ.pop();
      for (int d = 0; d < 4; ++d) {
        Coords v(u.r + dr[d], u.c + dc[d]);
        int dst = dist[u.r][u.c] + 1;
        if (v.r < 0 || v.r >= R) continue;
        if (v.c < 0 || v.c >= C) continue;
        if (dist[v.r][v.c] <= dst) continue;
        dist[v.r][v.c] = dst;
        PQ.push(CoordsWithDist(v, dst));
      }
    }
    for (int i = 0; i < (int)next.size(); ++i) {
      next_dist[i] = dist[next[i].r][next[i].c];
    }    
  } else {
    for (int i = 0; i < (int)next.size(); ++i) {
      for (int j = 0; j < (int)prev.size(); ++j) {
        next_dist[i] = min(next_dist[i], 
                           prev_dist[j] + 
                           abs(prev[j].r - next[i].r) + 
                           abs(prev[j].c - next[i].c));
      }
    }
  }
  return next_dist;
}

int main() {
  int L;
  CHECK(scanf("%d%d%d", &R, &C, &L));
  vector<vector<Coords>> levels(L + 1);
  levels[0].push_back(Coords(0, 0));
  for (int r = 0; r < R; ++r) {
    for (int c = 0; c < C; ++c) {
      int level;
      CHECK(scanf("%d", &level));
      levels[level].push_back(Coords(r, c));
    }
  }

  vector<int> dist(1, 0);
  for (int level = 1; level <= L; ++level) {
    dist = Solve(levels[level - 1], dist, levels[level]);
  }
  printf("%d\n", dist[0]);
  return 0;
}
