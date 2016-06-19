#ifndef LIB_GRAPH_GRAPH_COMMON_H_
#define LIB_GRAPH_GRAPH_COMMON_H_

#include "lib/base/empty.h"

namespace graph {

enum Direction { OUT, IN };

template <class Data = Empty>
class Arc : private Data {
 public:
  Arc(int endpoint) : endpoint_(endpoint) {}
  Arc(int endpoint, const Data& data) : Data(data), endpoint_(endpoint) {}
  int endpoint() const { return endpoint_; }
  const Data& data() const { return *static_cast<const Data*>(this); }
  Data& data() { return *static_cast<Data*>(this); }
 private:
  int endpoint_;
};

template <class Data>
class Arc<Data*> {
 public:
  Arc(int endpoint, Data* data) : endpoint_(endpoint), data_(data) {}
  int endpoint() const { return endpoint_; }
  const Data& data() const { return *data_; }
  Data& data() { return *data_; }
 private:
  int endpoint_;
  Data* data_;
};

}  // namespace graph

#endif  // LIB_GRAPH_GRAPH_COMMON_H_
