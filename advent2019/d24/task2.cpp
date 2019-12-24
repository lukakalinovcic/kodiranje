#include <cstdio>
#include <map>
#include <set>
#include <vector>

using namespace std;

struct Cell {
  int layer, r, c;
};
bool operator<(const Cell &a, const Cell &b) {
  if (a.layer != b.layer) return a.layer < b.layer;
  if (a.r != b.r) return a.r < b.r;
  if (a.c != b.c) return a.c < b.c;
  return 0;
}

vector<Cell> GetNeighbors(const Cell& a) {
  static int dr[4] = { -1, 0, 1, 0 };
  static int dc[4] = { 0, 1, 0, -1 };
  vector<Cell> neighbors;
  for (int d = 0; d < 4; ++d) {
    Cell b{a.layer, a.r + dr[d], a.c + dc[d]};
    if (b.r < 0 || b.r >= 5) continue;
    if (b.c < 0 || b.c >= 5) continue;
    if (b.r == 2 && b.c == 2) continue;
    neighbors.push_back(b);
  }
  if (a.r == 0) {
    neighbors.push_back(Cell{a.layer - 1, 1, 2});
  }
  if (a.r == 4) {
    neighbors.push_back(Cell{a.layer - 1, 3, 2});
  }
  if (a.c == 0) {
    neighbors.push_back(Cell{a.layer - 1, 2, 1});
  }
  if (a.c == 4) {
    neighbors.push_back(Cell{a.layer - 1, 2, 3});
  }
  if (a.r == 1 && a.c == 2) {
    for (int c = 0; c < 5; ++c) {
      neighbors.push_back(Cell{a.layer + 1, 0, c});
    }
  }
  if (a.r == 3 && a.c == 2) {
    for (int c = 0; c < 5; ++c) {
      neighbors.push_back(Cell{a.layer + 1, 4, c});
    }
  }
  if (a.r == 2 && a.c == 1) {
    for (int r = 0; r < 5; ++r) {
      neighbors.push_back(Cell{a.layer + 1, r, 0});
    }
  }
  if (a.r == 2 && a.c == 3) {
    for (int r = 0; r < 5; ++r) {
      neighbors.push_back(Cell{a.layer + 1, r, 4});
    }
  }
  return neighbors;
}

set<Cell> Step(const set<Cell>& prev) {
  map<Cell, int> neighbors;
  set<Cell> candidates = prev;
  for (const Cell& a : prev) {
    for (const Cell& b : GetNeighbors(a)) {
      neighbors[b] += 1;
      candidates.insert(b);
    }
  }
  set<Cell> next;
  for (const Cell& a : candidates) {
    bool is_prev = prev.count(a);
    int n_neighbors = neighbors[a];
    if (((is_prev && n_neighbors == 1) ||
         (!is_prev && (n_neighbors >= 1 && n_neighbors <= 2)))) {
      next.insert(a);
    }
  }
  return next;
}

int main() {
  set<Cell> bugs;
  char a[5][6];
  for (int r = 0; r < 5; ++r) {
    scanf("%s", a[r]);
    for (int c = 0; c < 5; ++c) {
      if (a[r][c] == '#') {
        bugs.insert(Cell{0, r, c});
      }
    }
  }

  for (int i = 0; i < 200; ++i) {
    bugs = Step(bugs);
  }
  printf("%d\n", bugs.size());
  
  return 0;
}
