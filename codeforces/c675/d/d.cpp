#include <algorithm>
#include <cstdio>
#include <map>

#define CHECK(condition) if (!(condition)) { \
  printf("CHECK(%s) failed\n", #condition); \
  exit(0); \
}

using namespace std;

int n;
map<int, int> h;

int main() {
  CHECK(scanf("%d", &n));
  bool first = true;
  for (int i = 0; i < n; ++i) {
    int x;
    CHECK(scanf("%d", &x));
    map<int, int>::iterator it = h.lower_bound(x);
    int parent = -1;
    int height = 0;
    if (it != h.end()) {
      if (it->second + 1 > height) {
        height = it->second + 1;
        parent = it->first;
      }
    }
    if (it != h.begin()) {
      --it;
      if (it->second + 1 > height) {
        height = it->second + 1;
        parent = it->first;
      }
    }
    h[x] = height;
    if (parent != -1) {
      if (!first) printf(" ");
      else first = false;
      printf("%d", parent);
    }
  }
  printf("\n");
  return 0;
}
