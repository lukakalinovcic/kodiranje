#include <algorithm>
#include <vector>

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
