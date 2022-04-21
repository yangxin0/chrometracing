#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>
#include "chrometracing.h"

namespace {

using testing::StrEq;

TEST(ChromeTracing, EventRenderCorrently) {
    chrometracing::TraceEvent event = chrometracing::TraceEvent{
        .name = "process_name",
        .phase = chrometracing::Phase::METADATA,
        .pid = 1024,
        .tid = 1024,
        .time = 0,
        .process_name = "some\"awful\n name\\to escape",
    };
    std::string gt = R"json({name:"process_name","ph":"M","pid":1024,"tid":1024,"args":{"name":"some\"awful\n name\\to escape"},},
)json";
    EXPECT_THAT(event.Render(), StrEq(gt));

    event = chrometracing::TraceEvent{
        .name = "process_name",
        .phase = chrometracing::Phase::BEGIN,
        .pid = 1024,
        .tid = 1024,
        .time = 1234,
    };
    gt = R"json({name:"process_name","ph":"B","pid":1024,"tid":1024,"time":1234,},
)json";
    EXPECT_THAT(event.Render(), StrEq(gt));
}

} // namespace

int main(int argc, char *argv[]) {
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
