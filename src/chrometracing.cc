#include <cstdio>
#include <cstdlib>
#include <pthread.h>
#include <string>
#include <vector>
#include <chrono>
// fixme
#include <unistd.h>
#include "chrometracing.h"


namespace chrometracing {

int64_t TraceEnv::pid_;
FILE *TraceEnv::trace_file_;
std::chrono::time_point<std::chrono::steady_clock> TraceEnv::started_at_;

// not thread-safe
void TraceEnv::Init() {
    if (trace_file_) {
        // already initialized
        return;
    }
    // skip tracing when CHROMETRACING is not set
    const char *trace = getenv("CHROMETRACING");
    if (trace) {
        started_at_ = std::chrono::steady_clock::now();
        trace_file_ = fopen(trace, "w");
        if (!trace_file_) {
            fprintf(stderr, "open trace file error, %s\n", trace);
            return;
        }
    }

    WriteTrace("[\n");
    TraceEvent event = TraceEvent{
        .name = "process_name",
        .phase = Phase::METADATA,
        .pid = GetPid(),
        .tid = GetTid(),
        .time = 0,
        .process_name = GetProcessName()
    };
    WriteTrace(event.Render());
}

void TraceEnv::WriteTrace(const std::string &trace) {
    if (!trace_file_) {
        // not initialized
        return;
    }
    fwrite(trace.c_str(), 1, trace.size(), trace_file_);
}

int64_t TraceEnv::GetTime() {
    auto now = std::chrono::steady_clock::now();
    auto diff = std::chrono::duration_cast<std::chrono::microseconds>(now - started_at_);

    return diff.count();
}

// fixme
int64_t TraceEnv::GetPid() {
    if (!pid_) {
        pid_ = (int64_t) getpid();
    }
    return pid_;
}

// fixme
int64_t TraceEnv::GetTid() {
    return (int64_t) pthread_self();
}

// fixme
std::string TraceEnv::GetProcessName() {
    // default name
    return "chrometracing";
}

std::string TraceEvent::Replace(
        const std::string &s, 
        const std::string &src, 
        const std::string &dst) {

    std::string s2 = s;
    for (size_t p = 0; (p = s2.find(src, p)) != std::string::npos; ) {
        s2.replace(p, src.size(), dst);
        p += dst.size();
    }

    return s2;
}

std::string TraceEvent::Escape(const std::string &s) {
    std::string s2 = s;
    std::vector<std::string> esc = {
        "\\", "\\\\",
        "\"", "\\\"",
        "\n", "\\n",
    };

    for (size_t i = 0; i < esc.size(); i += 2) {
        std::string &src = esc[i];
        std::string &dst = esc[i+1];
        s2 = Replace(s2, src, dst);
    }
    
    return s2;
}

std::string TraceEvent::Render() {
    std::string event = "{";
    event += Replace(R"(name:"$",)", "$", Escape(this->name));
    event += Replace(R"("ph":"$",)", "$", std::string(1, static_cast<char>(this->phase)));
    event += Replace(R"("pid":$,)",  "$", std::to_string(this->pid));
    event += Replace(R"("tid":$,)",  "$", std::to_string(this->tid));
    if (this->time) {
        event += Replace(R"("time":$,)", "$", std::to_string(this->time));
    }
    if (!this->process_name.empty()) {
        event += Replace(R"("args":{"name":"$"},)", "$", Escape(this->process_name));
    }
    event += "},\n";

    return event;
}

PendingEvent::~PendingEvent() {
    TraceEvent event = TraceEvent{
        .name = name_,
        .phase = Phase::END,
        .pid = TraceEnv::GetPid(),
        .tid = tid_,
        .time = TraceEnv::GetTime()
    };
    TraceEnv::WriteTrace(event.Render());
}

PendingEvent Event(const std::string &name, int64_t tid) {
    TraceEvent event = TraceEvent{
        .name = name,
        .phase = Phase::BEGIN,
        .pid = TraceEnv::GetPid(),
        .tid = tid,
        .time = TraceEnv::GetTime()
    };
    TraceEnv::WriteTrace(event.Render());

    return PendingEvent(name, tid);
}

PendingEvent Event(const std::string &name) {
    return Event(name, TraceEnv::GetTid());
}

} // namespace chrometracing
