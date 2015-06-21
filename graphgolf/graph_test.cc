#include "base/base.h"
#include "base/test.h"
#include "graphgolf/graph.h"

TEST(GraphTest, TestAccessors) {
  Graph g(10, 3);
  EXPECT_EQ(10, g.order());
  EXPECT_EQ(3, g.degree());
}

TEST(GraphTest, ManipulateEdges) {
  {
    Graph g(10, 3);

    EXPECT_FALSE(g.HasEdge(0, 1));
    EXPECT_TRUE(g.AddEdge(0, 1, false));
    EXPECT_TRUE(g.HasEdge(0, 1));
    EXPECT_TRUE(g.HasEdge(1, 0));
    EXPECT_FALSE(g.AddEdge(0, 1, false));
    EXPECT_TRUE(g.RemoveEdge(0, 1, false));
    EXPECT_FALSE(g.HasEdge(0, 1));
    EXPECT_FALSE(g.HasEdge(1, 0));
    EXPECT_FALSE(g.RemoveEdge(0, 1, false));
  }

  {
    Graph g(10, 3);

    // Self-loop cannot be created.
    EXPECT_FALSE(g.AddEdge(0, 0, false));
    EXPECT_TRUE(g.AddEdge(0, 1, false));
    EXPECT_TRUE(g.AddEdge(0, 2, false));
    EXPECT_TRUE(g.AddEdge(0, 3, false));
    // Exceeds 3 degrees.
    EXPECT_FALSE(g.AddEdge(0, 4, false));
    // Edge 0 => 1 has already been added.
    EXPECT_FALSE(g.AddEdge(1, 0, false));
    // Self-loop cannot be created.
    EXPECT_FALSE(g.AddEdge(1, 1, false));
    EXPECT_TRUE(g.AddEdge(1, 2, false));
    EXPECT_TRUE(g.AddEdge(1, 3, false));
    // Exceeds 3 degrees.
    EXPECT_FALSE(g.AddEdge(1, 4, false));

    // No such an edge.
    EXPECT_FALSE(g.RemoveEdge(0, 4, false));
    EXPECT_TRUE(g.RemoveEdge(0, 3, false));
    EXPECT_FALSE(g.RemoveEdge(0, 3, false));
  }
}
