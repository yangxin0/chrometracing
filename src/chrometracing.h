#ifndef _CHROMETRACING_H_
#define _CHROMETRACING_H_
#include <string>
#include <cstdio>
#include <chrono>

#define TRACE_INIT() chrometracing::TraceEnv::Init()
#define TRACE(name) chrometracing::Event(name)

namespace chrometracing {

// c++11: at least 64 bits
typedef long long int64_t; 

enum class Phase: char {
    BEGIN = 'B',
    END = 'E',
    METADATA = 'M',
};

class TraceEnv {
public:
    static void Init();
    static void WriteTrace(const std::string &trace);
    static int64_t GetTime();
    static int64_t GetTid();
    static int64_t GetPid();
    static std::string GetProcessName();

private:
    static int64_t pid_;
    static FILE *trace_file_;
    static std::chrono::time_point<std::chrono::steady_clock> started_at_;
};

struct TraceEvent {
    std::string name;
    Phase phase;
    int64_t pid;
    int64_t tid;
    int64_t time;
    std::string process_name;

    std::string Render();
private:
    std::string Replace(
            const std::string &s, 
            const std::string &src, 
            const std::string &dst);
    std::string Escape(const std::string &s);
};

class PendingEvent {
public:
    PendingEvent(std::string name, int64_t tid)
        : name_(std::move(name)), tid_(tid) {}
    ~PendingEvent();

private:
    std::string name_;
    int64_t tid_;
};

PendingEvent Event(const std::string &name, int64_t tid);
PendingEvent Event(const std::string &name);

} // namespace chrometracing

#endif /* _CHROMETRACING_H_ */
