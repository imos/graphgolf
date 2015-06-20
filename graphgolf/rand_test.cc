#include "base/base.h"
#include "base/test.h"
#include "graphgolf/rand.h"

TEST(RandTest, Distribution) {
  set<int> values;
  for (int i = 0; i < 1000; i++) {
    values.insert(Rand() % 10);
  }
  EXPECT_EQ(10, values.size());
}
