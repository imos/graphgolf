#include <set>

#include "base/base.h"
#include "graphgolf/io.h"

std::unique_ptr<Graph> LoadGraph() {
  map<Vertex, set<Vertex>> edge_groups;

  for (int v1, v2; scanf("%d%d", &v1, &v2) == 2;) {
    CHECK_EQ(0, edge_groups[v1].count(v2))
        << "Duplicated edge: " << v1 << " => " << v2;
    edge_groups[v1].insert(v2);
  }

  size_t degree = 0;
  for (const auto& vertex_and_edge_group : edge_groups) {
    Vertex v1 = vertex_and_edge_group.first;
    const set<Vertex>& edge_group = vertex_and_edge_group.second;
    for (Vertex v2 : edge_group) {
      CHECK_NE(v1, v2) << "Self-loop must not exist: " << v1 << " => " << v2;
      CHECK_EQ(1, edge_groups[v2].count(v1))
          << "Reverse edge must exist: " << v2 << " => " << v1;
    }
    degree = max(degree, edge_group.size());
  }

  size_t order = edge_groups.size();
  for (int i = 0; i < order; i++) {
    CHECK_EQ(1, edge_groups.count(i)) << "No edge from a vertex: " << i;
  }

  std::unique_ptr<Graph> graph;
  LOG(INFO) << "Graph(order = " << order << ", degree = " << degree << ")";
  graph.reset(new Graph(order, degree));

  for (const auto& vertex_and_edge_group : edge_groups) {
    Vertex v1 = vertex_and_edge_group.first;
    const set<Vertex>& edge_group = vertex_and_edge_group.second;
    for (Vertex v2 : edge_group) {
      if (v1 < v2) {
        graph->AddEdge(v1, v2, true);
      }
    }
  }

  return std::move(graph);
}

void SaveGraph(const Graph& graph) {
  for (Vertex v1 = 0; v1 < graph.order(); v1++) {
    for (Vertex v2 : graph.Edges(v1)) {
      printf("%d %d\n", v1, v2);
    }
  }
}
