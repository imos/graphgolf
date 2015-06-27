#include "base/base.h"
#include "base/test.h"
#include "graphgolf/fast_evaluation_graph.h"
#include "graphgolf/graph.h"

DEFINE_int32(order, 32, "Graph's maximal order.");
DEFINE_int32(degree, 3, "Graph's maximal degree.");

class FastEvaluationGraphTest : public testing::Test {
 public:
  void SetUp() override {
    rand_x_ = 123456789;
    rand_y_ = 362436069;
    rand_z_ = 0;
    rand_w_ = 0;
  }

 protected:
  uint32_t Rand() {
    uint32_t t = rand_x_ ^ (rand_x_ << 11);
    rand_x_ = rand_y_; rand_y_ = rand_z_; rand_z_ = rand_w_;
    return rand_w_ = (rand_w_ ^ (rand_w_ >> 19)) ^ (t ^ (t >> 8)); 
  }

  uint32_t rand_x_;
  uint32_t rand_y_;
  uint32_t rand_z_;
  uint32_t rand_w_;
};

TEST_F(FastEvaluationGraphTest, Generate) {
  FastEvaluationGraph fe_graph(FLAGS_order, FLAGS_degree);
  Graph graph(FLAGS_order, FLAGS_degree);

  for (int degree_index = 0; degree_index < graph.degree(); degree_index++) {
    for (int vertex_index = 0; vertex_index < graph.order(); vertex_index++) {
      fe_graph.AddEdge(vertex_index,
                      (vertex_index + degree_index + 1) % fe_graph.order(),
                      false);
      graph.AddEdge(vertex_index,
                    (vertex_index + degree_index + 1) % fe_graph.order(),
                    false);

      Score fe_score = fe_graph.Evaluate();
      Score score = graph.Evaluate();

      ASSERT_TRUE(fe_score == score)
          << "fe_score = (" << fe_score.first << ", " << fe_score.second
          << "), score = (" << score.first << ", " << score.second << "), "
          << "degree_index = " << degree_index << ", vertex_index = "
          << vertex_index;
    }
  }

  for (Vertex v1 = 0; v1 < graph.order(); v1++) {
    for (Vertex v2 : graph.Edges(v1)) {
      while (true) {
        Vertex v3 = Rand() % graph.order();
        Vertex v4 = graph.Edges(v3)[Rand() % graph.Edges(v3).size()];
        if (!graph.RemoveEdge(v1, v2, false)) continue;
        if (!graph.RemoveEdge(v3, v4, false)) {
          graph.AddEdge(v1, v2, true);
          continue;
        }
        if (!graph.AddEdge(v1, v4, false)) {
          graph.AddEdge(v3, v4, true);
          graph.AddEdge(v1, v2, true);
          continue;
        }
        if (!graph.AddEdge(v2, v3, false)) {
          graph.RemoveEdge(v1, v4, true);
          graph.AddEdge(v3, v4, true);
          graph.AddEdge(v1, v2, true);
          continue;
        }
        fe_graph.RemoveEdge(v1, v2, true);
        fe_graph.RemoveEdge(v3, v4, true);
        fe_graph.AddEdge(v1, v4, true);
        fe_graph.AddEdge(v2, v3, true);
        break;
      }

      Score fe_score = fe_graph.Evaluate();
      Score score = graph.Evaluate();

      ASSERT_TRUE(fe_score == score)
          << "fe_score = (" << fe_score.first << ", " << fe_score.second
          << "), score = (" << score.first << ", " << score.second << ")";
    }
  }
}
