#include "base/base.h"
#include "graphgolf/graph.h"
#include "graphgolf/io.h"

DEFINE_int32(order, 16, "Graph's maximal order.");
DEFINE_int32(degree, 3, "Graph's maximal degree.");

void Generate() {
  Graph g(FLAGS_order, FLAGS_degree);

  for (int degree_index = 0; degree_index < g.degree(); degree_index++) {
    for (int vertex_index = 0; vertex_index < g.order(); vertex_index++) {
      g.AddEdge(vertex_index,
                (vertex_index + degree_index + 1) % g.order(),
                false);
    }
  }

  SaveGraph(g);
  g.PrintScore(g.Evaluate());
}

int main(int argc, char** argv) {
  base::Init(argc, argv);
  Generate();
  return 0;
}
