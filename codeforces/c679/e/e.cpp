#include <vector>

#include "lib/base/check.h"
#include "lib/base/integral_types.h"
#include "lib/range_statistic/array.h"

using namespace std;

const int64 oo = 1000000000000000LL;

vector<int64> pow42;

void InitPow42() {
  int64 x = 1;
  pow42.push_back(x);
  do {
    x *= 42;
    pow42.push_back(x);
  } while (x <= oo);
}

int64 GetDistToPow42(int64 x) {
  return *lower_bound(pow42.begin(), pow42.end(), x) - x;
}

struct Element {
  int64 value = 0;
  bool disabled = false;
};

struct Stats {
  int min_pow42_dist_index;
  int64 min_pow42_dist_value;
  int64 first_enabled_value;

  static void FromValue(const range_statistic::ArrayRange& range,
                        const Element& element,
                        Stats* stats) {
    stats->min_pow42_dist_index = range.lo;
    stats->min_pow42_dist_value = element.disabled ? oo : GetDistToPow42(element.value);
    stats->first_enabled_value = element.disabled ? 0 : element.value;
  }

  static void Merge(const range_statistic::ArrayRange& range,
                    const Stats& left, const Stats& right, Stats* stats) {
    if (left.min_pow42_dist_value < right.min_pow42_dist_value) {
      stats->min_pow42_dist_index = left.min_pow42_dist_index;
      stats->min_pow42_dist_value = left.min_pow42_dist_value;
    } else {
      stats->min_pow42_dist_index = right.min_pow42_dist_index;
      stats->min_pow42_dist_value = right.min_pow42_dist_value;
    }
    stats->first_enabled_value = 
        left.first_enabled_value > 0 ? left.first_enabled_value
                                     : right.first_enabled_value;
  }
};

struct Propagation {
  int64 delta;
  bool disable;

  static Propagation Add(int64 delta) {
    return Propagation{delta, false};
  }
  static Propagation Disable() {
    return Propagation{0, true};
  }
  void Clear() {
    delta = 0;
    disable = false;
  }
  void Add(const Propagation& other) {
    delta += other.delta;
    disable |= other.disable;
  }
  void PropagateStats(const range_statistic::ArrayRange& range, Stats* stats)
      const {
    if (disable) {
      stats->min_pow42_dist_value = oo;
      stats->first_enabled_value = 0;
    } else {
      stats->min_pow42_dist_value -= delta;
      if (stats->first_enabled_value > 0) {
        stats->first_enabled_value += delta;
      }
    }
  }
  void PropagateValue(const range_statistic::ArrayRange& range, Element* element)
      const {
    if (element->disabled || disable) {
      element->value = oo;
      element->disabled = true;
    } else {
      element->value += delta;
    }
  }
};

int main() {
  InitPow42();
  int n, q;
  CHECK(scanf("%d%d", &n, &q));
  vector<Element> a(n + 1);
  for (int i = 1; i <= n; ++i) {
    CHECK(scanf("%lld", &a[i].value));
    a[i].disabled = false;
  }
  range_statistic::Array<Element, Stats, Propagation> A(a);
  auto Enable = [&A](int index) {
    auto element = A.GetMutable(index);
    if ((*element).disabled) {
      (*element).value = A.GetStats(index, A.size()).first_enabled_value;
      (*element).disabled = false;
    }
  }; 

  for (int i = 0; i < q; ++i) {
    int type;
    CHECK(scanf("%d", &type));
    if (type == 1) {
      int i;
      CHECK(scanf("%d", &i));
      printf("%lld\n", A.GetStats(i, A.size()).first_enabled_value);
    } else if (type == 2) {
      int a, b, x;
      CHECK(scanf("%d%d%d", &a, &b, &x));
      Enable(a - 1);
      Enable(b);
      A.Update(a, b, Propagation::Disable());
      (*A.GetMutable(b)).value = x;
    } else if (type == 3) {
      int a, b, x;
      CHECK(scanf("%d%d%d", &a, &b, &x));
      Enable(a - 1);
      Enable(b);
      A.Update(a, b + 1, Propagation::Add(x));
      for (;;) {
        Stats stats = A.GetStats(a, b + 1);
        if (stats.min_pow42_dist_value > 0) {
          break;
        } if (stats.min_pow42_dist_value == 0) {
          A.Update(a, b + 1, Propagation::Add(x));
        } else {
          A.GetMutable(stats.min_pow42_dist_index);
        }
      }
    }
  }
  return 0;
}
