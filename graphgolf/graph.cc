#include "graphgolf/graph.h"

Graph::Node Graph::GetEdge(Node order_index, Degree degree_index) {
  CHECK_LE(0, order_index);
  CHECK_GT(order(), order_index);
  CHECK_LE(0, degree_index);
  CHECK_GT(degree(), degree_index);
  return edges_[order_index * degree() + degree_index];
}
