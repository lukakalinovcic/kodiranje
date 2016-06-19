#ifndef LIB_GRAPH_ROOTED_FOREST_H_
#define LIB_GRAPH_ROOTED_FOREST_H_

#include <algorithm>
#include <stack>
#include <vector>

#include "lib/base/check.h"
#include "lib/base/empty.h"
#include "lib/graph/graph_base.h"

template <typename NodeData>
class WithParent : public NodeData {
 public:
  int parent_index() const { return parent_index_; }
  void set_parent_index(int parent_index) { parent_index_ = parent_index; }
 private:
  int parent_index_ = -1;
};

template <typename ArcData = Empty, typename NodeData = Empty>
  class RootedForest
      : public GraphBase<ArcData, WithParent<NodeData>> {
 public:
  typedef typename GraphBase<ArcData, WithParent<NodeData>>::Arc Arc;
  typedef typename GraphBase<ArcData, WithParent<NodeData>>::Node Node;
  typedef typename GraphBase<ArcData, WithParent<NodeData>>::arc_container arc_container;
  typedef typename GraphBase<ArcData, WithParent<NodeData>>::node_container node_container;

  const std::vector<int>& PreOrderTraversal() { return pre_order_; }

  class Builder {
   public:
    Builder(int num_nodes) {
      nodes_.resize(num_nodes);
    }

    NodeData& GetNode(int u) {
      return nodes_[u];
    }

    ArcData& AddArc(int u, int v) {
      nodes_[u].arcs().emplace_back(v);
      return nodes_[u].arcs().back();
    }

    RootedForest<ArcData, NodeData> Build() {
      for (int i = 0; i < (int)nodes_.size(); ++i) {
        std::sort(nodes_[i].arcs().begin(), nodes_[i].arcs().end(), 
                  [](const Arc& a, const Arc& b) {
                    return a.endpoint() < b.endpoint();
                  });
        for (const Arc& arc : nodes_[i].arcs()) {
          DCHECK(nodes_[arc.endpoint()].parent_index() == -1);
          nodes_[arc.endpoint()].set_parent_index(i);
        }
      }
      std::vector<int> pre_order;
      pre_order.reserve(nodes_.size());
      std::stack<int> stk;
      for (int i = 0; i < (int)nodes_.size(); ++i) {
        if (nodes_[i].parent_index() != -1) continue;
        stk.push(i);
        while (!stk.empty()) {
          const int u = stk.top();
          stk.pop();
          pre_order.push_back(u);
          for (const Arc& arc : nodes_[u].arcs()) {
            stk.push(arc.endpoint());
          }
        }
      }
      return RootedForest<ArcData, NodeData>(std::move(nodes_),
                                             std::move(pre_order));
    }

   private:
    std::vector<Node> nodes_;
  };

 protected:
  RootedForest(std::vector<Node> nodes,
               std::vector<int> pre_order) 
      : GraphBase<ArcData, WithParent<NodeData>>(std::move(nodes)),
        pre_order_(std::move(pre_order)) {}

 private:
  std::vector<int> pre_order_;
};

#endif  // LIB_GRAPH_ROOTED_FOREST_H_
