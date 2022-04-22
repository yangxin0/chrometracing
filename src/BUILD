package(
    default_visibility = [
        "//visibility:public"
    ]
)

cc_library(
    name = "chrometracing",
    srcs = ["chrometracing.cc"],
    hdrs = ["chrometracing.h"]
)

cc_binary(
    name = "chrometracing_demo",
    srcs = ["chrometracing_demo.cc"],
    deps = [
        "chrometracing"
    ],
    linkstatic = True,
    copts = ["-DCHROMETRACING"]
)

cc_test(
    name = "chrometracing_test",
    srcs = ["chrometracing_test.cc"],
    deps = [
        "@com_google_googletest//:gtest",
        "@com_google_googletest//:gtest_main",
        ":chrometracing"
    ],
    linkstatic = True
)
