#include "graphgolf/fast_evaluation_graph.h"

bool FastEvaluationGraph::AddEdge(Vertex v1, Vertex v2, bool check) {
  if (!Graph::AddEdge(v1, v2, check)) { return false; }
  UpdateRoutesForEdge(Route(v1, v2));
  return true;
}

bool FastEvaluationGraph::RemoveEdge(Vertex v1, Vertex v2, bool check) {
  if (!Graph::RemoveEdge(v1, v2, check)) { return false; }
  RemoveRoutesForEdge(Route(v1, v2));
  return true;
}
