#include <algorithm>
#include <cstdio>

#define CHECK(condition) if (!(condition)) { \
  printf("CHECK(%s) failed\n", #condition); \
  exit(0); \
}

typedef long long int64;

using namespace std;

#define MAX 500

int n, k;
char a[MAX][MAX + 1];
int comp[MAX][MAX];

int Comp(int r, int c) {
  if (r < 0 || r >= n) return 0;
  if (c < 0 || c >= n) return 0;
  return comp[r][c];
}

const int dr[4] = { -1, 0, 1, 0 };
const int dc[4] = { 0, 1, 0, -1 };

void dfs(int r, int c, int x) {
  if (r < 0 || r >= n) return;
  if (c < 0 || c >= n) return;
  if (a[r][c] != '.') return;
  if (comp[r][c]) return;
  comp[r][c] = x;
  for (int d = 0; d < 4; ++d) dfs(r + dr[d], c + dc[d], x);
}

class ResetableBitSet {
 public:
  ResetableBitSet(int n) {
    cookie_.resize(n);
  }

  bool Set(int i) {
    bool was_set = (cookie_[i] == active_cookie_);
    cookie_[i] = active_cookie_;
    return !was_set;
  }
  void Reset() {
    if (++active_cookie_ == 0) {
      active_cookie_ = 1;
      fill(cookie_.begin(), cookie_.end(), 0);
    }
  }
 private:
  int active_cookie_ = 0;
  vector<int> cookie_;
};

int main() {
  CHECK(scanf("%d%d", &n, &k));
  for (int i = 0; i < n; ++i) {
    CHECK(scanf("%s", a[i]));
  }

  int num_components = 0;
  for (int r = 0; r < n; ++r) {
    for (int c = 0; c < n; ++c) {
      if (a[r][c] == '.' && comp[r][c] == 0) {
        dfs(r, c, ++num_components);
      }
    }
  }
  vector<int> sum(num_components + 1, 0);
  for (int r = 0; r < n; ++r) {
    for (int c = 0; c < n; ++c) {
      ++sum[comp[r][c]];
    }
  }

  int ret = 0;
  ResetableBitSet neighbours(num_components + 1);
  for (int r = k - 1; r < n; ++r) {
    vector<int> cnt(num_components + 1, 0);
    for (int c = 0; c < n; ++c) {
      for (int rr = 0; rr < k; ++rr) {
        ++cnt[comp[r - rr][c]];
      }
      if (c < k - 1) continue;

      neighbours.Reset();
      int total = k * k;
      for (int rr = 0; rr < k; ++rr) {
        int L = Comp(r - rr, c - k);
        int R = Comp(r - rr, c + 1);
        if (L && neighbours.Set(L)) total += sum[L] - cnt[L];
        if (R && neighbours.Set(R)) total += sum[R] - cnt[R];
      }
      for (int cc = 0; cc < k; ++cc) {
        int U = Comp(r - k, c - cc);
        int D = Comp(r + 1, c - cc);
        if (U && neighbours.Set(U)) total += sum[U] - cnt[U];
        if (D && neighbours.Set(D)) total += sum[D] - cnt[D];
      }
      ret = max(ret, total);

      for (int rr = 0; rr < k; ++rr) {
        --cnt[comp[r - rr][c - k + 1]];
      }      
    }
  }
  printf("%d\n", ret);
  return 0;
}
