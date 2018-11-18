#include <algorithm>
#include <cstdio>
#include <vector>

using namespace std;

int n, q;
char s[200001];
int s_pow2[18][200001];

int suffix_array[200001];
int suffix_index[200001];
int lcp[200000];

struct Str {
  int index;
  int part1, part2;
};
bool operator<(const Str &A, const Str &B) {
  if (A.part1 != B.part1) return A.part1 < B.part1;
  return A.part2 < B.part2;
}

void Process(vector<Str>& a, int* out) {
  sort(a.begin(), a.end());
  int next_id = 0;
  for (int i = 0, j; i <= n; i = j) {
    for (j = i; j <= n && !(a[i] < a[j]); ++j) {
      out[a[j].index] = next_id;
    }
    ++next_id;
  }
}

int GetLcp(int i, int j) {
  int p = 0;
  while ((1 << p) < n) ++p;
  int ret = 0;
  for (--p; p >= 0; --p) {
    if (s_pow2[p][i + ret] == s_pow2[p][j + ret]) {
      ret += (1 << p);
    }
  }
  return ret;
}

void BuildSuffixArray() {
  vector<Str> a;
  a.reserve(n + 1);  
  for (int i = 0; i <= n; ++i) {
    a.push_back(Str{i, s[i], -1});
  }
  Process(a, s_pow2[0]);
  int p = 0;
  for (; (1 << p) <= n; ++p) {
    a.clear();
    for (int i = 0; i <= n; ++i) {
      const int j = i + (1 << p);
      const int part1 = s_pow2[p][i];
      const int part2 = j <= n ? s_pow2[p][j] : -1;
      a.push_back(Str{i, part1, part2});
    }
    Process(a, s_pow2[p + 1]);
  }
  for (int i = 0; i <= n; ++i) {
    suffix_index[i] = s_pow2[p][i];
    suffix_array[suffix_index[i]] = i;
  }

  for (int i = 0; i < n; ++i) {
    lcp[i] = GetLcp(suffix_array[i], suffix_array[i + 1]);
  }
}

int main() {
  scanf("%d%d", &n, &q);
  scanf("%s", s);
  BuildSuffixArray();
  for (int i = 0; i < n; ++i) {
    printf("%s\n", s + suffix_array[i]);
    if (i + 1 < n) printf(": %d\n", lcp[i]);
  }
  printf("\n");
  for (int i = 0; i < n; ++i) {
    printf("%d ", suffix_index[i]);
  }
  printf("\n");
  
  return 0;
}
