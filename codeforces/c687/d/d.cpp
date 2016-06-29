#include <algorithm>
#include <cctype>
#include "lib/base/check.h"
#include "lib/base/integral_types.h"

using namespace std;


namespace IO {
#define MAXBUFF (1<<19)
  char buffer[MAXBUFF], *p = buffer+MAXBUFF;
  inline char read_char() {
    if( p == buffer+MAXBUFF ) {
      fread( buffer, 1, MAXBUFF, stdin );
      p = buffer;
    }
    return *p++;
  }
  
  inline int read_int() {
    char c;
    while( !isdigit(c = read_char()) );
    int ret = c-'0';
    while( isdigit(c = read_char()) ) ret = ret * 10 + c-'0';
    return ret;
  }
};


int n, m, q;
struct Edge {
  int a, b, w;
  int index;
} E[500000];
bool operator<(const Edge &A, const Edge &B) {
  return A.w > B.w;
}

int component[1000];
int color[1000];
int size[1000];
int link[1000];
int tail[1000];

void Join(int u, int v) {
  bool swap_colors = (color[u] == color[v]);
  u = component[u];
  v = component[v];
  if (size[u] > size[v]) swap(u, v);
  if (swap_colors) {
    for (int x = u; x != -1; x = link[x]) color[x] ^= 1;
  }
  size[v] += size[u];
  for (int x = u; x != -1; x = link[x]) component[x] = v;
  link[tail[v]] = u;
  tail[v] = tail[u];
}

int Solve(int a, int b) {
  for (int i = 0; i < n; ++i) {
    component[i] = i;
    color[i] = 0;
    size[i] = 1;
    link[i] = -1;
    tail[i] = i;
  }
  for (int i = 0; i < m; ++i) {
    if (E[i].index < a || E[i].index > b) continue;
    if (component[E[i].a] == component[E[i].b]) {
      if (color[E[i].a] == color[E[i].b]) return E[i].w;
    } else {
      Join(E[i].a, E[i].b);
    }
  }
  return -1;
}

int main() {
  n = IO::read_int();
  m = IO::read_int();
  q = IO::read_int();
  for (int i = 0; i < m; ++i) {
    E[i].a = IO::read_int() - 1;
    E[i].b = IO::read_int() - 1;
    E[i].w = IO::read_int();
    E[i].index = i;
  }
  sort(E, E + m);

  for (int i = 0; i < q; ++i) {
    int a = IO::read_int() - 1;
    int b = IO::read_int() - 1;
    printf("%d\n", Solve(a, b));
  }

  return 0;
}
