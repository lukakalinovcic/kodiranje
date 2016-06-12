#include <limits>
#include <map>

template <typename V, typename A>
class Staircase {
 public:
  Staircase(V x0 = 0, V y0 = 0) : area_(0) {
    points_[x0] = std::numeric_limits<V>::max();
    points_[std::numeric_limits<V>::max()] = y0;
  }

  A area() const { return area_; }

  bool Insert(V x, V y) {
    auto curr = points_.lower_bound(x);
    if (curr->second >= y) return false;
    if (curr->first > x) --curr;
    auto next = curr; ++next;
    while (curr->second <= y) {
      auto prev = curr; --prev;
      area_ -= A(curr->first - prev->first) * A(curr->second - next->second);
      points_.erase(curr);
      curr = prev;
    }
    curr = points_.insert(std::pair<V, V>(x, y)).first;
    auto prev = curr; --prev;
    area_ += A(curr->first - prev->first) * A(curr->second - next->second);
    return true;
  }

  void AdvanceX0(V x0) {
    const int y0 = points_.rbegin()->second;
    int prev_x = points_.begin()->first;
    points_.erase(points_.begin());
    while (points_.begin()->first <= x0) {
      area_ -= A(points_.begin()->first - prev_x) *
               A(points_.begin()->second - y0);
      prev_x = points_.begin()->first;
      points_.erase(points_.begin());
    }
    area_ -= A(x0 - prev_x) * A(points_.begin()->second - y0);
    points_[x0] = std::numeric_limits<V>::max();
  }

  void AdvanceY0(V y0) {
    const int x0 = points_.begin()->first;
    int prev_y = points_.rbegin()->second;
    points_.erase(--points_.end());
    while (points_.rbegin()->second <= y0) {
      area_ -= A(points_.rbegin()->first - x0) *
               A(points_.rbegin()->second - prev_y);
      prev_y = points_.rbegin()->second;
      points_.erase(--points_.end());
    }
    area_ -= A(points_.rbegin()->first - x0) * A(y0 - prev_y);
    points_[std::numeric_limits<V>::max()] = y0;
  }

 private:
  A area_;
  std::map<V, V> points_;
};
