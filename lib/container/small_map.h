#ifndef LIB_CONTAINER_SMALL_MAP_H_
#define LIB_CONTAINER_SMALL_MAP_H_

// Incomplete
#include <algorithm>
#include <vector>

template<typename K, typename V>
class SmallMap {
 public:
  V& operator[](const K& key) {
    for (int i = 0; i < data_.size(); ++i) {
      if (data_[i].first == key) return data_[i].second;
    }
    data_.push_back(std::pair<K, V>(key, V()));
    return data_.back().second;
  }
  void clear() {
    data_.clear();
  }
  typename std::vector<std::pair<K, V>>::iterator begin() {
    return data_.begin();
  }
  typename std::vector<std::pair<K, V>>::iterator end() {
    return data_.end();
  }
 private:
  std::vector<std::pair<K, V>> data_;
};

#endif  // LIB_CONTAINER_SMALL_MAP_H_
