#include <cstdio>
#include <cstring>
#include <stack>
#include <vector>

#include "lib/base/check.h"
#include "lib/base/integral_types.h"

using namespace std;

#define MAX 5000

int n, m;
vector<int> adj[MAX];
int Time;
int Discover[MAX];
int Finish[MAX];
enum { White, Gray, Black } Color[MAX];
int LowLink[MAX];
int Components;
int Component[MAX];
stack<int> S;
int InStack[MAX];

void dfs_visit( int x ) {
  Color[x] = Gray;
  Discover[x] = ++Time;
  LowLink[x] = Discover[x];
  S.push( x );
  InStack[x] = 1;
  
  for( vector<int>::iterator it = adj[x].begin(); it != adj[x].end(); ++it ) {
    switch( Color[*it] ) {
    case White:
      dfs_visit( *it );
      LowLink[x] = min(LowLink[x], LowLink[*it]);
      break;
    case Gray:
      break;
    case Black:
      break;
    }
    if( InStack[*it] )
      LowLink[x] = min(LowLink[x], Discover[*it]);
  }
  if( LowLink[x] == Discover[x] ) {
    for( ++Components; !S.empty() && Discover[S.top()] >= Discover[x]; S.pop() ) {
      Component[S.top()] = Components;
      InStack[S.top()] = 0;
    }
  }
  Color[x] = Black;
  Finish[x] = ++Time;
}

bool terminal[MAX];

int dist[MAX];
char seen[MAX];
int Q[MAX];
int qhead, qtail;

int Solve(const vector<int>& nodes) {
  int n = nodes.size();
  if (n == 1) return 1;

  int min_cycle = n;
  for (int i = 0; i < n; ++i) {
    memset(seen, 0, sizeof seen);
    int x = nodes[i];
    seen[x] = true;
    dist[x] = 0;
    qhead = qtail = 0;
    Q[qtail++] = x;
    while (qhead != qtail) {
      int u = Q[qhead++];
      for (int v : adj[u]) {
        if (v == x) {
          min_cycle = min(min_cycle, dist[u] + 1);
          qhead = qtail;
          break;
        }
        if (!seen[v]) {
          seen[v] = true;
          dist[v] = dist[u] + 1;
          Q[qtail++] = v;
        }
      }
    }
  }
  return n + min_cycle * 998 + 1;
}

int main() {
  CHECK(scanf("%d%d", &n, &m));
  for (int i = 0; i < m; ++i) {
    int a, b;
    CHECK(scanf("%d%d", &a, &b)); --a; --b;
    adj[a].push_back(b);
  }
  Components = 0;
  Time = 0;
  for (int i = 0; i < n; ++i) Color[i] = White;
  for (int i = 0; i < n; ++i) InStack[i] = 0;
  for (int i = 0; i < n; ++i) {
    if (Color[i] == White) {
      dfs_visit(i);
    }
  }
  
  vector<int> terminal(Components, true);
  vector<vector<int>> nodes(Components);
  for (int i = 0; i < n; ++i) {
    nodes[Component[i] - 1].push_back(i);
    for (int j : adj[i]) {
      if (Component[i] != Component[j]) terminal[Component[i] - 1] = false;
    }
  }

  int ret = 0;
  for (int i = 0; i < Components; ++i) {
    if (!terminal[i]) ret += nodes[i].size();
    else ret += Solve(nodes[i]);
  }
  printf("%d\n", ret);
  return 0;
}
