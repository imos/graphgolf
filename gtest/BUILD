licenses(["notice"])

package(default_visibility = ["//visibility:public"])

cc_library(
  name = "gtest",
  srcs = ["gtest_all.cc"],
  hdrs = [
    "gmock.h",
    "gtest.h",
  ],
  linkstatic = 1,
  alwayslink = 1,
)

cc_library(
  name = "gtest_main",
  srcs = ["gmock_main.cc"],
  deps = [
    ":gtest",
  ],
  linkstatic = 1,
  alwayslink = 1,
)
