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

cc_test(
    name = "chrometracing_test",
    srcs = ["chrometracing_test.cc"],
    deps = [":chrometracing"],
    linkstatic = True
)
