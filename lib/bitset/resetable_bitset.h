#ifndef LIB_BITSET_RESETABLE_BITSET_H_
#define LIB_BITSET_RESETABLE_BITSET_H_

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

#endif  // LIB_BITSET_RESETABLE_BITSET_H_
