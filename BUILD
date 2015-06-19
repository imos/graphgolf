cc_library(
  name = "base",
  srcs = ["base.cc"],
  hdrs = ["base.h"],
)

cc_library(
  name = "rand",
  srcs = ["rand.cc"],
  hdrs = ["rand.h"],
  deps = [
    ":base",
  ],
)

cc_test(
  name = "rand_test",
  srcs = ["rand_test.cc"],
  deps = [
    ":rand",
    "//gtest:gtest_main",
  ],
)
