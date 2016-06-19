#ifndef LIB_GRAPH_DIRECTED_GRAPH_H_
#define LIB_GRAPH_DIRECTED_GRAPH_H_

#include <algorithm>
#include <vector>

#include "lib/base/empty.h"
#include "lib/graph/graph_base.h"
#include "lib/graph/graph_common.h"

namespace graph {

enum InArcStrategy {
  IN_ARC_NONE,
  IN_ARC_COPY_DATA,
  IN_ARC_SHARE_DATA,
};

template <typename ArcData>
class DirectedGraphBaseBuilder {
 public:
  ArcData& AddArc(int u, int v) {
    out_arcs_[u].emplace_back(v);
    return out_arcs_[u].back().data();
  }

 protected:
  DirectedGraphBaseBuilder(int num_nodes)
      : num_nodes_(num_nodes),
        out_arcs_(num_nodes) {}

  int num_nodes_;
  std::vector<std::vector<Arc<ArcData>>> out_arcs_;
};

template <typename ArcData = Empty,
          InArcStrategy in_arc_strategy = IN_ARC_NONE,
          ArcOrderStrategy arc_order_strategy = ARC_ORDER_NONE>
class DirectedGraph : public GraphBase<ArcData, Arc<ArcData>, Arc<ArcData>> {
 public:
  class Builder : public DirectedGraphBaseBuilder<ArcData> {
   public:
    Builder(int num_nodes) : DirectedGraphBaseBuilder<ArcData>(num_nodes) {}

    DirectedGraph Build() {
      ReorderArcs(this->out_arcs_, arc_order_strategy);
      return DirectedGraph(this->num_nodes_, std::move(this->out_arcs_));
    }
  };

  const std::vector<Arc<ArcData>>& out_arcs(int i) const { return this->out_arcs_[i]; }
  const std::vector<Arc<ArcData>>& in_arcs(int i) const { return empty_in_arcs_; }
  std::vector<Arc<ArcData>>& out_arcs(int i) { return this->out_arcs_[i]; }
  std::vector<Arc<ArcData>>& in_arcs(int i) { return empty_in_arcs_; }

 private:
  DirectedGraph(int num_nodes,
                std::vector<std::vector<Arc<ArcData>>> out_arcs)
      : GraphBase<ArcData, Arc<ArcData>, Arc<ArcData>>(
            num_nodes, {}, std::move(out_arcs), {}) {}
  std::vector<Arc<ArcData>> empty_in_arcs_;
};

template <typename ArcData, ArcOrderStrategy arc_order_strategy>
class DirectedGraph<ArcData, IN_ARC_COPY_DATA, arc_order_strategy>
    : public GraphBase<ArcData, Arc<ArcData>, Arc<ArcData>> {
 public:
  class Builder : public DirectedGraphBaseBuilder<ArcData> {
   public:
    Builder(int num_nodes) : DirectedGraphBaseBuilder<ArcData>(num_nodes) {}

    DirectedGraph Build() {
      ReorderArcs(this->out_arcs_, arc_order_strategy);
      std::vector<std::vector<Arc<ArcData>>> in_arcs(this->num_nodes_);
      for (int i = 0; i < this->num_nodes_; ++i) {
        for (const Arc<ArcData>& arc : this->out_arcs_[i]) {
          in_arcs[arc.endpoint()].emplace_back(i, arc.data());
        }
      }
      return DirectedGraph(this->num_nodes_, std::move(this->out_arcs_), std::move(in_arcs));
    }
  };

  const std::vector<Arc<ArcData>>& out_arcs(int i) const { return this->out_arcs_[i]; }
  const std::vector<Arc<ArcData>>& in_arcs(int i) const { return this->in_arcs_[i]; }
  std::vector<Arc<ArcData>>& out_arcs(int i) { return this->out_arcs_[i]; }
  std::vector<Arc<ArcData>>& in_arcs(int i) { return this->in_arcs_[i]; }

 private:
  DirectedGraph(int num_nodes,
                std::vector<std::vector<Arc<ArcData>>> out_arcs,
                std::vector<std::vector<Arc<ArcData>>> in_arcs)
      : GraphBase<ArcData, Arc<ArcData>, Arc<ArcData>>(
            num_nodes, {}, std::move(out_arcs), std::move(in_arcs)) {}
};

template <typename ArcData, ArcOrderStrategy arc_order_strategy>
class DirectedGraph<ArcData, IN_ARC_SHARE_DATA, arc_order_strategy>
    : public GraphBase<ArcData, Arc<ArcData>, Arc<ArcData*>> {
 public:
  class Builder : public DirectedGraphBaseBuilder<ArcData> {
   public:
    Builder(int num_nodes) : DirectedGraphBaseBuilder<ArcData>(num_nodes) {}

    DirectedGraph Build() {
      ReorderArcs(this->out_arcs_, arc_order_strategy);
      std::vector<std::vector<Arc<ArcData*>>> in_arcs(this->num_nodes_);
      for (int i = 0; i < this->num_nodes_; ++i) {
        for (Arc<ArcData>& arc : this->out_arcs_[i]) {
          in_arcs[arc.endpoint()].emplace_back(i, &arc.data());
        }
      }
      return DirectedGraph(this->num_nodes_, std::move(this->out_arcs_), std::move(in_arcs));
    }
  };

  const std::vector<Arc<ArcData>>& out_arcs(int i) const { return this->out_arcs_[i]; }
  const std::vector<Arc<ArcData*>>& in_arcs(int i) const { return this->in_arcs_[i]; }
  std::vector<Arc<ArcData>>& out_arcs(int i) { return this->out_arcs_[i]; }
  std::vector<Arc<ArcData*>>& in_arcs(int i) { return this->in_arcs_[i]; }

 private:
  DirectedGraph(int num_nodes,
                std::vector<std::vector<Arc<ArcData>>> out_arcs,
                std::vector<std::vector<Arc<ArcData*>>> in_arcs)
      : GraphBase<ArcData, Arc<ArcData>, Arc<ArcData*>>(
            num_nodes, {}, std::move(out_arcs), std::move(in_arcs)) {}
};

template <typename ArcData = Empty, InArcStrategy in_arc_strategy = IN_ARC_NONE>
class SortedDirectedGraph : public DirectedGraph<ArcData, in_arc_strategy, ARC_ORDER_SORTED> {
 public:
  typedef DirectedGraph<ArcData, in_arc_strategy, ARC_ORDER_SORTED> BaseClass;

  class Builder : public BaseClass::Builder {
   public:
    Builder(int num_nodes) : BaseClass::Builder(num_nodes) {}
    SortedDirectedGraph Build() { return SortedDirectedGraph(BaseClass::Builder::Build()); }
  };

  ArcData* OutArcOrNull(int u, int v) { return FindArcOrNull(this->out_arcs(u), v); }
  ArcData* InArcOrNull(int u, int v) { return FindArcOrNull(this->in_arcs(u), v); }
  const ArcData* OutArcOrNull(int u, int v) const { return FindArcOrNull(this->out_arcs(u), v); }
  const ArcData* InArcOrNull(int u, int v) const { return FindArcOrNull(this->in_arcs(u), v); }

 private:
  template <typename ArcType>
  ArcData* FindArcOrNull(const std::vector<ArcType>& arcs, int target) const {
    auto it = std::lower_bound(arcs.begin(), arcs.end(), target,
                               [](const ArcType& arc, int target) { 
                                 return arc.endpoint() < target;
                               });
    if (it == arcs.end() || it->endpoint() != target) return nullptr;
    return const_cast<ArcData*>(&it->data());
  }

  SortedDirectedGraph(BaseClass graph)
      : BaseClass(std::move(graph)) {}
};

}  // namespace graph

#endif  // LIB_GRAPH_DIRECTED_GRAPH_H_
