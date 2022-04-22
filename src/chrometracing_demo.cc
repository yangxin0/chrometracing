#include <cstdio>
#include "chrometracing.h"

void Func2() {
    TRACE();
    int sum = 0;
    for (int i = 0; i < 1000000000; i++) {
        sum += i;
    }
    printf("Func2: sum=%d\n", sum);
}

void Func3() {
    TRACE();
    int sum = 0;
    for (int i = 0; i < 100000000; i++) {
        sum += i;
    }
    printf("Func3: sum=%d\n", sum);

}

void Func1() {
    TRACE();
    printf("Func1\n");
    for (int i = 0; i < 60; i++) {
        Func2();
        int sum = 0;
        for (int j = 0; j < 100000000; j++) {
            sum += j;
        }
        printf("Func1: sum=%d\n", sum);
        Func3();
    }
}

int main(int argc, char *argv[]) {
    setenv("CHROMETRACING", "./trace.log", 1);
    TRACE_INIT();
    Func1();

    return 0;
}
