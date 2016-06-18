#ifndef LIB_GRAPH_TOPOLOGICAL_SORT_H_
#define LIB_GRAPH_TOPOLOGICAL_SORT_H_

#include <algorithm>
#include <stack>
#include <vector>

#include "lib/base/check.h"

template <typename Graph>
std::vector<int> TopologicalSort(const Graph& graph) {
  typedef typename Graph::arc_container::const_iterator const_arc_iterator;
  enum Color { kWhite, kGrey, kBlack };
  std::vector<Color> col(graph.num_nodes(), kWhite);
  std::vector<int> result;
  std::stack<std::pair<int, const_arc_iterator>> stk;
  for (int i = 0; i < graph.num_nodes(); ++i) {
    if (col[i] != kWhite) {
      DCHECK(col[i] == kBlack);
      continue;
    }
    col[i] = kGrey;
    stk.push(std::make_pair(i, graph.node(i).arcs().begin()));
    while (!stk.empty()) {
      int u = stk.top().first;
      if (stk.top().second == graph.node(u).arcs().end()) {
        col[u] = kBlack;
        stk.pop();
        result.push_back(u);
        continue;
      }
      int v = (stk.top().second++)->endpoint();
      if (col[v] == kWhite) {
        col[v] = kGrey;
        stk.push(std::make_pair(v, graph.node(v).arcs().begin()));
      } else if (col[v] == kGrey) {
        result.clear();  // Not a DAG!
        return result;
      }
    }
  }
  std::reverse(result.begin(), result.end());
  return result;
}

#endif  // LIB_GRAPH_TOPOLOGICAL_SORT_H_
