#include "base/base.h"
#include "graphgolf/graph.h"
#include "graphgolf/io.h"
#include "graphgolf/rand.h"

int main(int argc, char** argv) {
  base::Init(argc, argv);

  std::unique_ptr<Graph> graph = LoadGraph();

  for (Vertex v1 = 0; v1 < graph->order(); v1++) {
    for (Vertex v2 : graph->Edges(v1)) {
      while (true) {
        Vertex v3 = Rand() % graph->order();
        Vertex v4 = graph->Edges(v3)[Rand() % graph->Edges(v3).size()];
        if (!graph->RemoveEdge(v1, v2, false)) continue;
        if (!graph->RemoveEdge(v3, v4, false)) {
          graph->AddEdge(v1, v2, true);
          continue;
        }
        if (!graph->AddEdge(v1, v4, false)) {
          graph->AddEdge(v3, v4, true);
          graph->AddEdge(v1, v2, true);
          continue;
        }
        if (!graph->AddEdge(v2, v3, false)) {
          graph->RemoveEdge(v1, v4, true);
          graph->AddEdge(v3, v4, true);
          graph->AddEdge(v1, v2, true);
          continue;
        }
        break;
      }
    }
  }

  graph->PrintScore(graph->Evaluate());
  SaveGraph(*graph);

  return 0;
}
