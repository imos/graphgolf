package(default_visibility = ["//visibility:public"])

cc_library(
  name = "fast_evaluation_graph",
  srcs = ["fast_evaluation_graph.cc"],
  hdrs = ["fast_evaluation_graph.h"],
  deps = [
    ":graph",
    "//base",
  ],
  linkstatic = 1,
)

cc_test(
  name = "fast_evaluation_graph_test",
  srcs = ["fast_evaluation_graph_test.cc"],
  deps = [
    ":fast_evaluation_graph",
    ":graph",
    "//base:test_main",
  ],
)

cc_binary(
  name = "generate_base",
  srcs = ["generate_base_main.cc"],
  deps = [
    ":graph",
    ":io",
    "//base",
  ],
)

cc_library(
  name = "graph",
  srcs = ["graph.cc"],
  hdrs = ["graph.h"],
  deps = [
    ":slice",
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
  name = "io",
  srcs = ["io.cc"],
  hdrs = ["io.h"],
  deps = [
    ":fast_evaluation_graph",
    ":graph",
    "//base",
  ],
  linkstatic = 1,
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

cc_binary(
  name = "random_graph",
  srcs = ["random_graph_main.cc"],
  deps = [
    ":graph",
    ":io",
    ":rand",
    "//base",
  ],
)

cc_library(
  name = "slice",
  hdrs = ["slice.h"],
  srcs = ["slice.cc"],
)
