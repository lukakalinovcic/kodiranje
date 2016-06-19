#ifndef LIB_GRAPH_ROOTED_FOREST_H_
#define LIB_GRAPH_ROOTED_FOREST_H_

#include <stack>
#include <vector>

#include "lib/base/check.h"
#include "lib/base/empty.h"
#include "lib/graph/directed_graph.h"

namespace graph {

template <typename ArcData = Empty>
class RootedForest : public DirectedGraph<ArcData> {
 public:
  int parent(int i) const { return parent_[i]; }
  const std::vector<ArcData>& children(int i) const { return this->out_arcs(i); }
  std::vector<ArcData>& children(int i) { return this->out_arcs(i); }

  const std::vector<int>& PreOrderTraversal() { return pre_order_; }

  class Builder : public DirectedGraph<ArcData>::Builder {
   public:
    Builder(int num_nodes) : DirectedGraph<ArcData>::Builder(num_nodes) {}
    RootedForest<ArcData> Build() {
      auto graph = DirectedGraph<ArcData>::Builder::Build();

      std::vector<int> parent(graph.num_nodes(), -1);
      for (int i = 0; i < graph.num_nodes(); ++i) {
        for (const auto& arc : graph.out_arcs(i)) {
          DCHECK(parent[arc.endpoint()] == -1);
          parent[arc.endpoint()] = i;
        }
      }

      std::vector<int> pre_order;
      pre_order.reserve(graph.num_nodes());
      std::stack<int> stk;
      for (int i = 0; i < graph.num_nodes(); ++i) {
        if (parent[i] != -1) continue;
        stk.push(i);
        while (!stk.empty()) {
          const int u = stk.top();
          stk.pop();
          pre_order.push_back(u);
          for (const auto& arc : graph.out_arcs(u)) {
            stk.push(arc.endpoint());
          }
        }
      }

      DCHECK(pre_order.size() == graph.num_nodes());
      return RootedForest<ArcData>(
          std::move(graph), std::move(parent), std::move(pre_order));
    }
  };

 protected:
  RootedForest(DirectedGraph<ArcData> graph,
               std::vector<int> parent,
               std::vector<int> pre_order) 
     : DirectedGraph<ArcData>(std::move(graph)),
       parent_(std::move(parent)),
       pre_order_(std::move(pre_order)) {}

 private:
  std::vector<int> parent_;
  std::vector<int> pre_order_;
};

}  // namespace graph

#endif  // LIB_GRAPH_ROOTED_FOREST_H_
