#ifndef LIB_GRAPH_GRAPH_BASE_H_
#define LIB_GRAPH_GRAPH_BASE_H_

#include <algorithm>
#include <vector>

#include "lib/base/empty.h"

template <typename ArcData = Empty, typename NodeData = Empty>
class GraphBase {
 public:
  class Arc : public ArcData {
   public:
    Arc(int endpoint) : endpoint_(endpoint) {}
    inline int endpoint() const { return endpoint_; }
   private:
    int endpoint_;
  };

  typedef typename std::vector<Arc> arc_container;

  class Node : public NodeData {
   public:
    inline int num_arcs() const { return arcs_.size(); }
    inline const arc_container& arcs() const { return arcs_; } 
    inline arc_container& arcs() { return arcs_; }
   private:
    std::vector<Arc> arcs_;
  };

  typedef typename std::vector<Node> node_container;

  inline int num_nodes() const { return nodes_.size(); }
  inline const node_container& nodes() const { return nodes_; }
  inline node_container& nodes() { return nodes_; }
  inline const Node& node(int u) const { return nodes_[u]; }
  inline Node& node(int u) { return nodes_[u]; }

  Arc* FindArcOrNull(int u, int v) {
    auto it = std::lower_bound(node(u).arcs().begin(),
                               node(u).arcs().end(), v,
                               [](const Arc& arc, int v) { 
                                 return arc.endpoint() < v;
                               });
    if (it == node(u).arcs().end() || it->endpoint() != v) return nullptr;
    return &*it;
  }

 protected:
  GraphBase(std::vector<Node> nodes) 
      : nodes_(std::move(nodes)) {}

  std::vector<Node> nodes_;
};

#endif  // LIB_GRAPH_GRAPH_BASE_H_
