#include "chrometracing.h"

int main(int argc, char *argv[]) {
    TRACE_INIT();
    TRACE("main");
    return 0;
}
