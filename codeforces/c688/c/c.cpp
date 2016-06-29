#include <queue>
#include <vector>

#include "lib/base/check.h"
#include "lib/base/integral_types.h"

using namespace std;

#define MAXN 100000

int n, m;
vector<int> adj[MAXN];

int color[MAXN];

bool Solve() {
  queue<int> Q;
  for (int i = 0; i < n; ++i) {
    if (color[i]) continue;  
    color[i] = 1;
    Q.push(i);
    while(!Q.empty()) {
      int u = Q.front();
      Q.pop();
      for (int v : adj[u]) {
        if (color[v] == 0) {
          color[v] = 3 - color[u];
          Q.push(v);
        } else if (color[v] != 3 - color[u]) {
          return false;
        }
      }
    }
  }
  return true;
}

int main() {
  CHECK(scanf("%d%d", &n, &m));
  for (int i = 0; i < m; ++i) {
    int a, b;
    CHECK(scanf("%d%d", &a, &b)); --a; --b;
    adj[a].push_back(b);
    adj[b].push_back(a);
  }
  if (!Solve()) {
    printf("-1\n");
  } else {
    for (int c = 1; c <= 2; ++c) {
      int cnt = 0;
      for (int i = 0; i < n; ++i) {
        if (color[i] == c) ++cnt;
      }
      printf("%d\n", cnt);
      for (int i = 0; i < n; ++i) {
        if (color[i] == c) {
          printf("%d%c", i + 1, --cnt ? ' ' : '\n');
        }
      }
    }
  }
  return 0;
}
