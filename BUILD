cc_library(
    name = "simple_bptree",
    srcs = [
      'simple_bptree.cpp',
    ],
    hdrs = [
      'simple_bptree.h',
    ],
    includes = [],
    deps = [
    ],
    copts = [
        "-std=c++11",
    ],
    linkopts = [
    ],
    visibility = ["//visibility:public"],
)

cc_binary(
  name = 'main',
  srcs = [
    'main.cpp',
  ],
  deps = [
    ':simple_bptree',
  ],
  copts = [
   '--std=c++11',
  ],
  linkopts = [
  ],
)