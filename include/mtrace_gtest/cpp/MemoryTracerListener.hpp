#ifndef MEMORY_TRACER_LISTENER_HPP
#define MEMORY_TRACER_LISTENER_HPP

#include "gtest/gtest.h"

class MemoryTracerListener: public testing::EmptyTestEventListener {
private:
    char* mtraceDirName;
    char* mtraceFileName;

public:
    MemoryTracerListener();
    ~MemoryTracerListener();
};

#endif
