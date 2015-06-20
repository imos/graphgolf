#include "base/base.h"

class Graph {
 public:
  typedef int16_t Node;
  typedef int16_t Degree;

  Graph(Node order, Degree degree)
      : order_(order),
        degree_(degree),
        edges_(order * degree, -1) {}

  inline Node order() { return order_; }
  inline Degree degree() { return degree_; }

  // Returns the order_index node's degree_index-th edge's node.  Returns -1 if
  // there is no such an edge.
  Node GetEdge(Node order_index, Degree degree_index);

 private:
  const Node order_;
  const Degree degree_;
  vector<Node> edges_;
};