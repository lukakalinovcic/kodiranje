#include <algorithm>
#include <cstdio>
#include <cstring>
#include <vector>

#define CHECK(condition) if (!(condition)) { \
  printf("CHECK(%s) failed\n", #condition); \
  exit(0); \
}

typedef long long int64;

using namespace std;

#define MAX 400

int n, m;
int g[MAX][MAX];
int deg[MAX];

template<typename T> 
class SmallSet {
 public:
  void insert(const T& value) {
    for (int i = 0; i < data_.size(); ++i) {
      if (data_[i] == value) return;
    }
    data_.push_back(value);
  }
  void clear() {
    data_.clear();
  }
  typename vector<T>::iterator begin() { return data_.begin(); } 
  typename vector<T>::iterator end() { return data_.end(); } 
 private:
  vector<T> data_;
};

template<typename K, typename V> 
class SmallMap {
 public:
  V& operator[](const K& key) {
    for (int i = 0; i < data_.size(); ++i) {
      if (data_[i].first == key) return data_[i].second;
    }
    data_.push_back(pair<K, V>(key, V()));
    return data_.back().second;
  }
  void clear() {
    data_.clear();
  }
  typename vector<pair<K, V>>::iterator begin() { return data_.begin(); } 
  typename vector<pair<K, V>>::iterator end() { return data_.end(); } 
 private:
  vector<pair<K, V>> data_;
};

double prob[MAX][MAX];
SmallSet<int> neighbour_x[MAX];
SmallMap<int, pair<double, double>> pairs[MAX];
int cnt_a[MAX];
double best[MAX];
double s[MAX];
double p[MAX];

double Solve() {
  double ret = 0.0;
  for (int a = 0; a < n; ++a) {
    for (int x = 0; x < n; ++x) cnt_a[x] = 0;
    for (int i = 0; i < n; ++i) {
      int x = g[a][i];
      ++cnt_a[x];
    }

    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
        prob[i][j] = 0.0;
      }
      neighbour_x[i].clear();
    }

    for (int i = 0; i < n; ++i) {
      int x = g[a][i];
      for (int j = 0; j < n; ++j) {
        if (g[i][j] != 1) continue;
        prob[x][j] += (1.0 / cnt_a[x]) * (1.0 / deg[i]);
        neighbour_x[j].insert(x);
      }
    }

    for (int x = 0; x < n; ++x) {
      best[x] = cnt_a[x] == 0 ? 0.0 : 1.0 / n;
    }
    
    for (int b = 0; b < n; ++b) {
      for (int x = 0; x < n; ++x) {
        s[x] = 0.0;
        p[x] = 0.0;
      }
      for (int y = 0; y < n; ++y) pairs[y].clear();

      for (int i = 0; i < n; ++i) {
        int y = g[b][i];
        for (int x : neighbour_x[i]) {
          pair<double, double>& p = pairs[y][x];
          p.first += prob[x][i];
          p.second = max(p.second, prob[x][i]);
        }
      }
      
      for (int y = 0; y < n; ++y) {
        for (const auto& entry : pairs[y]) {
          int x = entry.first;
          s[x] += entry.second.first;
        }
      }
      for (int y = 0; y < n; ++y) {
        for (const auto& entry : pairs[y]) {
          int x = entry.first;
          p[x] += entry.second.second / entry.second.first * 
                  entry.second.first / s[x];
        }
      }
      for (int x = 0; x < n; ++x) {
        if (cnt_a[x] == 0) continue;
        best[x] = max(best[x], p[x] * cnt_a[x] / n);
      }
    }
    double sum_best = 0;
    for (int x = 0; x < n; ++x) sum_best += best[x];
    ret = max(ret, sum_best);
  }
  return ret;
}

int main() {
  CHECK(scanf("%d%d", &n, &m));
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      g[i][j] = MAX;
    }
    g[i][i] = 0;
  }
  for (int i = 0; i < m; ++i) {
    int a, b;
    CHECK(scanf("%d%d", &a, &b));
    ++deg[a - 1];
    ++deg[b - 1];
    g[a - 1][b - 1] = 1;
    g[b - 1][a - 1] = 1;
  }
  for (int k = 0; k < n; ++k) {
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
        g[i][j] = min(g[i][j], g[i][k] + g[k][j]);
      }
    }
  }
  printf("%.10lf\n", Solve());

  return 0;
}
