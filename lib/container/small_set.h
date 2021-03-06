#ifndef LIB_CONTAINER_SMALL_SET_H_
#define LIB_CONTAINER_SMALL_SET_H_

// Incomplete
#include <vector>

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
  typename std::vector<T>::iterator begin() { return data_.begin(); }
  typename std::vector<T>::iterator end() { return data_.end(); }
 private:
  std::vector<T> data_;
};

#endif  // LIB_CONTAINER_SMALL_SET_H_
