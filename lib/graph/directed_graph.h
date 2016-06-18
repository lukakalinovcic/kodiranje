#ifndef LIB_GRAPH_DIRECTED_GRAPH_H_
#define LIB_GRAPH_DIRECTED_GRAPH_H_

#include <algorithm>
#include <vector>

#include "lib/base/empty.h"
#include "lib/graph/graph_base.h"

template <typename ArcData = Empty, typename NodeData = Empty>
  class DirectedGraph : public GraphBase<ArcData, NodeData> {
 public: 
  typedef typename GraphBase<ArcData, NodeData>::Arc Arc;
  typedef typename GraphBase<ArcData, NodeData>::Node Node;
  typedef typename GraphBase<ArcData, NodeData>::arc_container arc_container;
  typedef typename GraphBase<ArcData, NodeData>::node_container node_container;

  class Builder {
   public:
    Builder(int num_nodes) {
      nodes_.resize(num_nodes);
    }

    NodeData& GetNode(int u) {
      return nodes_[u].data();
    }

    ArcData& AddArc(int u, int v) {
      nodes_[u].arcs().emplace_back(v);
      return nodes_[u].arcs().back().data();
    }

    DirectedGraph<ArcData, NodeData> Build() {
      for (Node& node : nodes_) {
        std::sort(node.arcs().begin(), node.arcs().end(), 
                  [](const Arc& a, const Arc& b) {
                    return a.endpoint() < b.endpoint();
                  });
      }
      return DirectedGraph<ArcData, NodeData>(std::move(nodes_));
    }

   private:
    std::vector<Node> nodes_;
  };

 protected:
  DirectedGraph(std::vector<Node> nodes) 
      : GraphBase<ArcData, NodeData>(nodes) {}
};

#endif  // LIB_GRAPH_DIRECTED_GRAPH_H_
