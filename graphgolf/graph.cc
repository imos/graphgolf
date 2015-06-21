#include "graphgolf/graph.h"

using std::swap;

bool Graph::HasEdge(Vertex v1, Vertex v2) const {
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

bool Graph::HasDirectionalEdge(Vertex v1, Vertex v2) const {
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

void Graph::PrintScore(const Score& score) const {
  fprintf(stderr,
          "Diameter: %d, ASPL: %.4lf\n",
          score.first,
          static_cast<double>(score.second) / order_ / (order_ - 1));
}

Score Graph::Evaluate() const {
  int64_t distance_sum = 0;
  int distance_max = 0;

  for (int start_vertex = 0; start_vertex < order(); start_vertex++) {
    vector<int> vertices(order());
    vector<int> distances(order(), -1);

    // Set the start_vertex.
    vertices[0] = start_vertex;
    distances[start_vertex] = 0;

    int read_index = 0;
    int write_index = 1;
    for (; read_index < vertices.size(); read_index++) {
      if (read_index >= write_index) {
        return Score(order(), 0);
      }

      // The current vertex.
      const int vertex = vertices[read_index];
      // The distance from the start vertex for the vertices connected from
      // the current vertex.
      const int next_distance = distances[vertex] + 1;

      for (Vertex next_vertex : Edges(vertex)) {
        if (distances[next_vertex] != -1) continue;
        distances[next_vertex] = next_distance;
        distance_sum += next_distance;
        vertices[write_index++] = next_vertex;
      }
    }

    distance_max = max(distance_max, distances[write_index - 1]);
  }
  return Score(distance_max, distance_sum);
}
