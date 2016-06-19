#ifndef LIB_GRAPH_GRAPH_BASE_H_
#define LIB_GRAPH_GRAPH_BASE_H_

#include <algorithm>
#include <vector>

namespace graph {

enum ArcOrderStrategy {
  ARC_ORDER_NONE,
  ARC_ORDER_SORTED,
};

template <typename ArcType>
void ReorderArcs(std::vector<std::vector<ArcType>>& arcs, ArcOrderStrategy strategy) {
  if (strategy == ARC_ORDER_NONE) return;
  for (std::vector<ArcType>& node_arcs : arcs) {
    std::sort(node_arcs.begin(), node_arcs.end(), 
              [](const ArcType& a, const ArcType& b) {
                return a.endpoint() < b.endpoint();
              });
  }
}

template <typename ArcData, typename OutArc, typename InArc>
class GraphBase {
 public:
  typedef std::vector<OutArc> out_arc_container;
  typedef std::vector<InArc> in_arc_container;

  int num_nodes() const { return num_nodes_; }
 protected:
  GraphBase(int num_nodes,
            std::vector<ArcData> data_store,
            std::vector<std::vector<OutArc>> out_arcs,
            std::vector<std::vector<InArc>> in_arcs) 
      : num_nodes_(num_nodes),
        data_store_(std::move(data_store)),
        out_arcs_(std::move(out_arcs)),
        in_arcs_(std::move(in_arcs)) {}
  
  int num_nodes_;
  std::vector<ArcData> data_store_;
  std::vector<std::vector<OutArc>> out_arcs_;
  std::vector<std::vector<InArc>> in_arcs_;
};

}  // namespace graph

#endif  // LIB_GRAPH_GRAPH_BASE_H_
