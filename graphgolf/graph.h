#ifndef GRAPHGOLF_GRAPH_H_
#define GRAPHGOLF_GRAPH_H_
#include "base/base.h"
#include "graphgolf/slice.h"

typedef int16_t Vertex;
typedef int16_t Degree;
typedef pair<int32_t, int64_t> Score;

class Graph {
 public:
  Graph(Vertex order, Degree degree)
      : order_(order),
        degree_(degree),
        edges_(static_cast<int32_t>(order) * degree, -1),
        degrees_(order, 0) {}
  virtual ~Graph() {}

  // Returns the order of the task.
  inline Vertex order() const { return order_; }

  // Returns the degree of the task.
  inline Degree degree() const { return degree_; }

  // Returns vertices connected from the vertex.
  inline Slice<const Vertex*> Edges(Vertex vertex) const {
    return Slice<const Vertex*>(
        &edges_[GetEdgeIndex(vertex, 0)],
        &edges_[GetEdgeIndex(vertex, 0) + GetVertexDegree(vertex)]);
  }

  // Returns if there is an edge between v1 and v2.
  virtual bool HasEdge(Vertex v1, Vertex v2) const;

  // Adds an edge between v1 and v2.  Dies if this function fails to add the
  // edge and check is true.  Returns true iff succeeded.
  //
  // REQUIRES: v1 != v2, v1 and v2 are valid vertices.
  virtual bool AddEdge(Vertex v1, Vertex v2, bool check);

  // Removes an edge between v1 and v2.  Dies if this function fails to remove
  // the edge and check is true.  REturns true iff succeeded.
  //
  // REQUIRES: v1 != v2, v1 and v2 are valid vertices.
  virtual bool RemoveEdge(Vertex v1, Vertex v2, bool check);

  // Prints the given score with the GraphGolf format.
  void PrintScore(const Score& score) const;

  // Returns the score of the graph.
  virtual Score Evaluate() const;

 protected:
  inline int32_t GetEdgeIndex(Vertex vertex_index, Degree degree_index) const {
    return static_cast<int32_t>(CheckVertex(vertex_index)) * degree() +
           CheckDegree(degree_index);
  }

  inline Vertex CheckVertex(Vertex vertex_index) const {
    DCHECK(0 <= vertex_index && vertex_index < order())
        << "Invalid vertex: vertex_index = " << vertex_index
        << ", order() = " << order();
    return vertex_index;
  }

  inline Degree CheckDegree(Degree degree_index) const {
    DCHECK(0 <= degree_index && degree_index < degree())
        << "Invalid degree: degree_index = " << degree_index
        << ", degree() = " << degree();
    return degree_index;
  }

 private:
  inline Degree GetVertexDegree(Vertex vertex_index) const {
    return degrees_[CheckVertex(vertex_index)];
  }

  bool HasDirectionalEdge(Vertex v1, Vertex v2) const;
  bool AddDirectionalEdge(Vertex v1, Vertex v2, bool check);
  bool RemoveDirectionalEdge(Vertex v1, Vertex v2, bool check);

  const Vertex order_;
  const Degree degree_;
  vector<Vertex> edges_;
  vector<int> degrees_;
};

#endif  // GRAPHGOLF_GRAPH_H_
