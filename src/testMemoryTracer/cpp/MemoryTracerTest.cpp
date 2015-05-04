#include "MemoryTracerTest.hpp"

TEST_F(MemoryTracerTest, startTrace) {
    memoryTracer.start();
    muntrace();

    checkThatMtraceFileIsValid();
}

TEST_F(MemoryTracerTest, stopTrace) {
    mtrace();
    memoryTracer.stop();

    checkThatMtraceFileIsValid();
}
