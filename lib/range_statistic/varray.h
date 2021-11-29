#ifndef LIB_RANGE_STATISTIC_VARRAY_H_
#define LIB_RANGE_STATISTIC_VARRAY_H_

#include <algorithm>
#include <functional>
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
class VArray {
 public:
  class ElementMutator {
   public:

    V& operator*() {
      return *value_;
    }

    ~ElementMutator() {
      S::FromValue(descent_.back().second, *value_,
                   &array_->tree_[descent_.back().first].statistic);
      for (int i = static_cast<int>(descent_.size()) - 2; i >= 0; --i) {
        int x = descent_[i].first;
        S::Merge(descent_[i].second,
                 array_->tree_[array_->tree_[x].left].statistic,
                 array_->tree_[array_->tree_[x].right].statistic,
                 &array_->tree_[x].statistic);
      }
    }

   private:
    ElementMutator(VArray<V, S, P>* array,
                   V* value,
                   std::vector<std::pair<int, ArrayRange>> descent) :
      array_(array),
      value_(value),
      descent_(descent) {
    }

    VArray<V, S, P>* array_;
    V* value_;
    std::vector<std::pair<int, ArrayRange>> descent_;

    friend class VArray;
  };

  VArray(int n, std::function<V(int)> v) {
    size_ = n;
    root_.push_back(InitStatistics(ArrayRange(0, size_), v));
  }

  VArray(int n, const V& val)
    : VArray(n, [&val](int i) { return val; }) {
  }

  int size() const { return size_; }

  int NewRoot(int version) {
    int new_version = root_.size();
    root_.push_back(CopyNode(root_[version], new_version));
    return new_version;
  }
  
  const V& Get(int version, int index) {
    const auto d = Descend(root_[version], ArrayRange(0, size_), index);
    return tree_[d.back().first].value;
  }

  ElementMutator GetMutable(int version, int index) {
    const auto d = Descend(root_[version], ArrayRange(0, size_), index);
    return ElementMutator(this, &tree_[d.back().first].value,
                          std::move(d));
  }

  S GetStats(int version, int begin, int end) {
    return GetStats(root_[version], ArrayRange(0, size_), begin, end);
  }

  void Update(int version, int begin, int end, const P& propagation) {
    Update(root_[version], ArrayRange(0, size_), begin, end, propagation);
  }

  template<typename WalkOp>
  bool Walk(int version, WalkOp& op) {
    Walk(root_[version], ArrayRange(0, size_), op);
    return op.IsDone();
  }

 private:
  struct Node : P {
    S statistic;
    V value;
    int left, right;
    int version;
    Node() : P() {}
  };

  int NewNode() {
    int i = tree_.size();
    tree_.emplace_back();
    return i;
  }

  int CopyNode(int i, int version) {
    int j = tree_.size();
    tree_.push_back(tree_[i]);
    tree_[j].version = version;
    return j;
  }
  
  int InitStatistics(const ArrayRange& range, std::function<V(int)> v) {
    int i = NewNode();
    if (range.size() == 1) {
      tree_[i].value = v(range.lo);
      S::FromValue(range, tree_[i].value, &tree_[i].statistic);
    } else {
      tree_[i].left = InitStatistics(ArrayRange(range.lo, range.mid), v);
      tree_[i].right = InitStatistics(ArrayRange(range.mid, range.hi), v);
      S::Merge(range,
               tree_[tree_[i].left].statistic,
               tree_[tree_[i].right].statistic,
               &tree_[i].statistic);
    }
    return i;
  }

  std::vector<std::pair<int, ArrayRange>> Descend(int i, ArrayRange range,
                                                  int index) {
    std::vector<std::pair<int, ArrayRange>> descent;
    for (;;) {
      PropagateChildren(i, range);
      descent.push_back(std::make_pair(i, range));
      if (range.size() == 1) break;
      if (index < range.mid) {
        i = tree_[i].left;
        range = ArrayRange(range.lo, range.mid);
      } else {
        i = tree_[i].right;
        range = ArrayRange(range.mid, range.hi);
      }
    }
    return descent;
  }

  S GetStats(int i, const ArrayRange& range, int begin, int end) {
    if (range.lo >= begin && range.hi <= end) return tree_[i].statistic;
    PropagateChildren(i, range);
    if (range.mid >= end) {
      return GetStats(tree_[i].left, ArrayRange(range.lo, range.mid), begin, end);
    }
    if (range.mid <= begin) {
      return GetStats(tree_[i].right, ArrayRange(range.mid, range.hi), begin, end);
    }
    S stats;
    S::Merge(range,
             GetStats(tree_[i].left, ArrayRange(range.lo, range.mid), begin, end),
             GetStats(tree_[i].right, ArrayRange(range.mid, range.hi), begin, end),
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
        Update(tree_[i].left, ArrayRange(range.lo, range.mid), begin, end, propagation);
      }
      if (range.mid < end) {
        Update(tree_[i].right, ArrayRange(range.mid, range.hi), begin, end, propagation);
      }
      S::Merge(range,
               tree_[tree_[i].left].statistic,
               tree_[tree_[i].right].statistic,
               &tree_[i].statistic);
    }
  }

  void PropagateChildren(int i, const ArrayRange& range) {
    if (range.size() == 1) {
      tree_[i].PropagateValue(range, &tree_[i].value);
    } else {
      if (tree_[tree_[i].left].version != tree_[i].version) {
        tree_[i].left = CopyNode(tree_[i].left, tree_[i].version);
      }
      if (tree_[tree_[i].right].version != tree_[i].version) {
        tree_[i].right = CopyNode(tree_[i].right, tree_[i].version);
      }
      tree_[i].PropagateStats(ArrayRange(range.lo, range.mid),
                              &tree_[tree_[i].left].statistic);
      tree_[i].PropagateStats(ArrayRange(range.mid, range.hi),
                              &tree_[tree_[i].right].statistic);
      tree_[tree_[i].left].Add(tree_[i]);
      tree_[tree_[i].right].Add(tree_[i]);
    }
    tree_[i].Clear();
  }

  template<typename WalkOp>
  void Walk(int i, const ArrayRange& range, WalkOp& op) {
    if (op.GoUp(range, tree_[i].statistic)) return;
    PropagateChildren(i, range);
    if (range.size() == 1) {
      op.Leaf(range, tree_[i].value);
    } else {
      if (op.GoLeftFirst(range, tree_[i].statistic)) {
        Walk(tree_[i].left, ArrayRange(range.lo, range.mid), op);
        if (!op.IsDone()) Walk(tree_[i].right, ArrayRange(range.mid, range.hi), op);
      } else {
        Walk(tree_[i].right, ArrayRange(range.mid, range.hi), op);
        if (!op.IsDone()) Walk(tree_[i].left, ArrayRange(range.lo, range.mid), op);
      }
    }
  }

  int size_;
  std::vector<int> root_;
  std::vector<Node> tree_;

  friend class ElementMutator;
};
}  // namespace range_statistic

#endif  // LIB_RANGE_STATISTIC_VARRAY_H_
