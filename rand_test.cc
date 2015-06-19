#include "gtest/gtest.h"
#include "rand.h"

TEST(RandTest, Rand) {
  ASSERT_NE(0, Rand());
}
