cc_library(
  name = "rand",
  srcs = ["rand.cc"],
  hdrs = ["rand.h"],
)

cc_test(
  name = "rand_test",
  srcs = ["rand_test.cc"],
  deps = [
    ":rand",
    "//gtest:gtest_main",
  ],
)
