#include "graphgolf/graph.h"

using std::swap;

bool Graph::HasEdge(Vertex v1, Vertex v2) {
  return HasDirectionalEdge(v1, v2);
}

bool Graph::AddEdge(Vertex v1, Vertex v2, bool check) {
  if (!AddDirectionalEdge(v1, v2, check)) { return false; }
  if (AddDirectionalEdge(v2, v1, check)) { return true; }
  RemoveDirectionalEdge(v1, v2, true);
  return false;
}

bool Graph::RemoveEdge(Vertex v1, Vertex v2, bool check) {
  if (!RemoveDirectionalEdge(v1, v2, check)) { return false; }
  if (RemoveDirectionalEdge(v2, v1, check)) { return true; }
  AddDirectionalEdge(v1, v2, true);
  return false;
}

bool Graph::HasDirectionalEdge(Vertex v1, Vertex v2) {
  CheckVertex(v1);
  CheckVertex(v2);
  if (v1 == v2) { return false; }
  for (const Vertex& vertex : Edges(v1)) {
    if (vertex == v2) { return true; }
  }
  return false;
}

bool Graph::AddDirectionalEdge(Vertex v1, Vertex v2, bool check) {
  CheckVertex(v1);
  CheckVertex(v2);
  if (v1 == v2) {
    DCHECK(!check) << "Self-loop edge cannot be added: vertex = " << v1;
    return false;
  }
  if (!check) {
    if (degrees_[v1] >= degree()) { return false; }
    if (HasDirectionalEdge(v1, v2)) { return false; }
  }
  DCHECK_GT(degree(), degrees_[v1]);
  DCHECK(!HasDirectionalEdge(v1, v2))
      << "Duplicated edge: " << v1 << " => " << v2;
  edges_[GetEdgeIndex(v1, degrees_[v1])] = v2;
  degrees_[v1]++;
  return true;
}

bool Graph::RemoveDirectionalEdge(Vertex v1, Vertex v2, bool check) {
  CheckVertex(v1);
  CheckVertex(v2);
  if (v1 == v2) {
    DCHECK(!check) << "Self-loop edge cannot be removed: vertex = " << v1;
    return false;
  }
  auto edges = Edges(v1);
  for (int degree_index = 0; degree_index < edges.size(); degree_index++) {
    if (edges[degree_index] == v2) {
      degrees_[v1]--;
      swap(edges_[GetEdgeIndex(v1, degree_index)],
           edges_[GetEdgeIndex(v1, degrees_[v1])]);
      return true;
    }
  }
  DCHECK(!check) << "No such an edge: " << v1 << " => " << v2;
  return false;
}
