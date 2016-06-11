#include <algorithm>
#include <cstdio>
#include <set>
#include <vector>

#define CHECK(condition) if (!(condition)) { \
  printf("CHECK(%s) failed\n", #condition); \
  exit(0); \
}

typedef long long int64;

using namespace std;

namespace range_statistic {
struct ArrayRange {
  ArrayRange(int lo, int hi) : lo(lo), mid((lo + hi) / 2), hi(hi) {}
  int size() const { return hi - lo; }
  int lo, mid, hi;
};

struct NoPropagation {
  void Clear() {}
  void Add(const NoPropagation& other) {}
  template<typename S>
  void PropagateStats(const ArrayRange& range, S* stats) const {}
  template<typename V>
  void PropagateValue(const ArrayRange& range, V* value) const {}
};
  
template<typename V, typename S, typename P = NoPropagation>
class Array {
 public:
  class ElementMutator {
   public:

    V& operator*() {
      return array_->values_[index_];
    }

    ~ElementMutator() {
      S::FromValue(descent_.back().second, array_->values_[index_],
                   &array_->tree_[descent_.back().first].statistic);
      for (int i = static_cast<int>(descent_.size()) - 2; i >= 0; --i) {
        int x = descent_[i].first;
        S::Merge(descent_[i].second,
                 array_->tree_[2 * x].statistic,
                 array_->tree_[2 * x + 1].statistic,
                 &array_->tree_[x].statistic);
      }
    }

   private:
    ElementMutator(Array<V, S, P>* array, 
                   int index,
                   std::vector<std::pair<int, ArrayRange>> descent) :
      array_(array), 
      index_(index),
      descent_(descent) {}
    
    Array<V, S, P>* array_;
    int index_;
    std::vector<std::pair<int, ArrayRange>> descent_;

    friend class Array;
  };

  Array(int n, const V& val) { 
    values_ = std::vector<V>(n, val);
    tree_height_ = TreeHeight(values_.size());
    tree_ = std::vector<SP>(1 << tree_height_);
    if (!values_.empty()) {
      InitStatistics(1, ArrayRange(0, values_.size()));
    }
  }

  template <typename Iterator>
  Array(Iterator first, Iterator last) {
    values_ = std::vector<V>(first, last);
    tree_height_ = TreeHeight(values_.size());
    tree_ = std::vector<SP>(1 << tree_height_);
    if (!values_.empty()) {
      InitStatistics(1, ArrayRange(0, values_.size()));
    }
  }

  template <typename Container>
  Array(const Container& container)
      : Array(container.begin(), container.end()) {}

  int size() const { return values_.size(); }
  
  const V& Get(int index) {
    Descend(1, ArrayRange(0, values_.size()), index);
    return values_[index];
  }

  ElementMutator GetMutable(int index) {
    return ElementMutator(this, index,
                          Descend(1, ArrayRange(0, values_.size()), index));
  }

  S GetStats(int begin, int end) {
    return GetStats(1, ArrayRange(0, values_.size()), begin, end);
  }

  void Update(int begin, int end, const P& propagation) {
    Update(1, ArrayRange(0, values_.size()), begin, end, propagation);
  }

  template<typename WalkOp>
  bool Walk(WalkOp& op) {
    Walk(1, ArrayRange(0, values_.size()), op);
    return op.IsDone();
  }

 private:
  struct SP : P { S statistic; };

  static int TreeHeight(int n) {
    int height = 0;
    while ((1 << height) < n) ++height;
    return height + 1;
  }

  void InitStatistics(int i, const ArrayRange& range) {
    if (range.size() == 1) {
      S::FromValue(range, values_[range.lo], &tree_[i].statistic);
    } else {
      InitStatistics(2 * i, ArrayRange(range.lo, range.mid));
      InitStatistics(2 * i + 1, ArrayRange(range.mid, range.hi));
      S::Merge(range,
               tree_[2 * i].statistic,
               tree_[2 * i + 1].statistic,
               &tree_[i].statistic);
    }
  }

  std::vector<std::pair<int, ArrayRange>> Descend(int i, ArrayRange range,
                                                  int index) {
    std::vector<std::pair<int, ArrayRange>> descent;
    descent.reserve(tree_height_);
    for (;;) {
      PropagateChildren(i, range);
      descent.push_back(std::make_pair(i, range));
      if (range.size() == 1) break;
      if (index < range.mid) {
        i = 2 * i;
        range = ArrayRange(range.lo, range.mid);
      } else {
        i = 2 * i + 1;
        range = ArrayRange(range.mid, range.hi);
      }
    }
    return descent;
  }

  S GetStats(int i, const ArrayRange& range, int begin, int end) {
    if (range.lo >= begin && range.hi <= end) return tree_[i].statistic;
    PropagateChildren(i, range);
    if (range.mid >= end) {
      return GetStats(2 * i, ArrayRange(range.lo, range.mid), begin, end);
    }
    if (range.mid <= begin) {
      return GetStats(2 * i + 1, ArrayRange(range.mid, range.hi), begin, end);
    }
    S stats;
    S::Merge(range, 
             GetStats(2 * i, ArrayRange(range.lo, range.mid), begin, end),
             GetStats(2 * i + 1, ArrayRange(range.mid, range.hi), begin, end),
             &stats);
    return stats;
  }

  void Update(int i, const ArrayRange& range, int begin, int end,
              const P& propagation) {
    if (range.lo >= begin && range.hi <= end) {
      propagation.PropagateStats(range, &tree_[i].statistic);
      tree_[i].Add(propagation);
    } else {
      PropagateChildren(i, range);
      if (range.mid > begin) {
        Update(2 * i, ArrayRange(range.lo, range.mid), begin, end, propagation);
      }
      if (range.mid < end) {
        Update(2 * i + 1, ArrayRange(range.mid, range.hi), begin, end, propagation);
      }
      S::Merge(range,
               tree_[2 * i].statistic,
               tree_[2 * i + 1].statistic,
               &tree_[i].statistic);
    }
  }

  void PropagateChildren(int i, const ArrayRange& range) {
    if (range.size() == 1) {
      tree_[i].PropagateValue(range, &values_[range.lo]);
    } else {
      tree_[i].PropagateStats(ArrayRange(range.lo, range.mid),
                              &tree_[2 * i].statistic);
      tree_[i].PropagateStats(ArrayRange(range.mid, range.hi),
                              &tree_[2 * i + 1].statistic);
      tree_[2 * i].Add(tree_[i]);
      tree_[2 * i + 1].Add(tree_[i]);
    }
    tree_[i].Clear();
  }

  template<typename WalkOp>
  void Walk(int i, const ArrayRange& range, WalkOp& op) {
    if (op.GoUp(range, tree_[i].statistic)) return;
    PropagateChildren(i, range);
    if (range.size() == 1) {
      op.Leaf(range, values_[range.lo]);
    } else {
      if (op.GoLeftFirst(range, tree_[i].statistic)) {
        Walk(2 * i, ArrayRange(range.lo, range.mid), op);
        if (!op.IsDone()) Walk(2 * i + 1, ArrayRange(range.mid, range.hi), op);
      } else {
        Walk(2 * i + 1, ArrayRange(range.mid, range.hi), op);
        if (!op.IsDone()) Walk(2 * i, ArrayRange(range.lo, range.mid), op);
      }
    }
  }

  int tree_height_;
  std::vector<V> values_;
  std::vector<SP> tree_;

  friend class ElementMutator;
};
}  // namespace range_statistic

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
