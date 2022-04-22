## Chrome Tracing Library

Pure c++11 chrome tracing library inspired by `google/chrometracing`.

#### Requirements

1. A modern compiler that supports c++11
2. bazel build tool

#### Build the project

```
cd /path/to/repo
bazel build '...'
# All the outputs reside in ./bazel-bin/src
```

#### Trace with chrometracing

1. Export `-DCHROMETRACING` symbol to enable chrome tracing
2. Include header `#include <chrometracing>`
3. Put the `TRACE_INIT()` clause in the first line of the main body
4. Put the `TRACE()` clause in the first line of the target function
5. `export CHROMETRACING=/path/to/trace.log` before running your program

