#include "base/base.h"
#include "base/test.h"
#include "graphgolf/graph.h"

TEST(GraphTest, TestAccessors) {
  Graph g(10, 3);
  EXPECT_EQ(10, g.order());
  EXPECT_EQ(3, g.degree());
}
