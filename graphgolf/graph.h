#include "base/base.h"
#include "graphgolf/slice.h"

typedef int16_t Vertex;
typedef int16_t Degree;

class Graph {
 public:
  Graph(Vertex order, Degree degree)
      : order_(order),
        degree_(degree),
        edges_(static_cast<int32_t>(order) * degree, -1),
        degrees_(order, 0) {}
  virtual ~Graph() {}

  inline Vertex order() { return order_; }
  inline Degree degree() { return degree_; }

  inline Degree GetVertexDegree(Vertex vertex_index) {
    CheckVertex(vertex_index);
    return degrees_[vertex_index];
  }

  // Returns the vertex_index vertex's degree_index-th edge's node.  Returns -1
  // if there is no such an edge.
  inline Vertex GetEdge(Vertex vertex_index, Degree degree_index) {
    return edges_[GetEdgeIndex(vertex_index, degree_index)];
  }

  inline Slice<const Vertex*> Edges(Vertex v1) {
    return Slice<const Vertex*>(
        &edges_[GetEdgeIndex(v1, 0)],
        &edges_[GetEdgeIndex(v1, 0) + GetVertexDegree(v1)]);
  }

  virtual bool HasEdge(Vertex v1, Vertex v2);
  virtual bool AddEdge(Vertex v1, Vertex v2, bool check);
  virtual bool RemoveEdge(Vertex v1, Vertex v2, bool check);

 protected:
  inline int32_t GetEdgeIndex(Vertex vertex_index, Degree degree_index) {
    return static_cast<int32_t>(CheckVertex(vertex_index)) * degree() +
           CheckDegree(degree_index);
  }

  inline Vertex CheckVertex(Vertex vertex_index) {
    DCHECK(0 <= vertex_index && vertex_index < order())
        << "Invalid vertex: vertex_index = " << vertex_index
        << ", order() = " << order();
    return vertex_index;
  }

  inline Degree CheckDegree(Degree degree_index) {
    DCHECK(0 <= degree_index && degree_index < degree())
        << "Invalid degree: degree_index = " << degree_index
        << ", degree() = " << degree();
    return degree_index;
  }

 private:
  bool HasDirectionalEdge(Vertex v1, Vertex v2);
  bool AddDirectionalEdge(Vertex v1, Vertex v2, bool check);
  bool RemoveDirectionalEdge(Vertex v1, Vertex v2, bool check);

  const Vertex order_;
  const Degree degree_;
  vector<Vertex> edges_;
  vector<int> degrees_;
};
