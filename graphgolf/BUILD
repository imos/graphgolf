cc_library(
  name = "graph",
  srcs = ["graph.cc"],
  hdrs = ["graph.h"],
  deps = [
    "//base",
  ],
  linkstatic = 1,
)

cc_test(
  name = "graph_test",
  srcs = ["graph_test.cc"],
  deps = [
    ":graph",
    "//base:test_main",
  ],
)

cc_library(
  name = "rand",
  srcs = ["rand.cc"],
  hdrs = ["rand.h"],
  deps = [
    "//base",
  ],
)

cc_test(
  name = "rand_test",
  srcs = ["rand_test.cc"],
  deps = [
    ":rand",
    "//base:test_main",
  ],
)
