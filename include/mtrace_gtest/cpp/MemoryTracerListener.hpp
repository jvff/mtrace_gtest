#ifndef MEMORY_TRACER_LISTENER_HPP
#define MEMORY_TRACER_LISTENER_HPP

#include "gtest/gtest.h"

#include "TraceFileParser.hpp"

class MemoryTracerListener : public testing::EmptyTestEventListener {
protected:
    char* mtraceDirName;
    char* mtraceFileName;
    TraceFileParser* traceFileParser;

public:
    MemoryTracerListener();
    virtual ~MemoryTracerListener();

    virtual void OnTestStart(const testing::TestInfo& testInfo);
    virtual void OnTestEnd(const testing::TestInfo& testInfo);
};

#endif
