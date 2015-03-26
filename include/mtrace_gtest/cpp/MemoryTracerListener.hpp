#ifndef MEMORY_TRACER_LISTENER_HPP
#define MEMORY_TRACER_LISTENER_HPP

#include "gtest/gtest.h"

#include "TraceFileParser.hpp"

class MemoryTracerListener : public testing::EmptyTestEventListener {
private:
    char* mtraceDirName;
    char* mtraceFileName;

protected:
    TraceFileParser* traceFileParser;

public:
    MemoryTracerListener();
    virtual ~MemoryTracerListener();

    virtual void OnTestStart(const testing::TestInfo& testInfo);
};

#endif
