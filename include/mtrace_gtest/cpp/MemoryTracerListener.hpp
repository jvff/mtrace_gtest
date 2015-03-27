#ifndef MEMORY_TRACER_LISTENER_HPP
#define MEMORY_TRACER_LISTENER_HPP

#include "gtest/gtest.h"

#include "FailureReporter.hpp"
#include "TraceFileParser.hpp"

class MemoryTracerListener : public testing::EmptyTestEventListener {
protected:
    char* mtraceDirName;
    char* mtraceFileName;
    FailureReporter* failureReporter;
    TraceFileParser* traceFileParser;

public:
    MemoryTracerListener();
    virtual ~MemoryTracerListener();

    virtual void OnTestStart(const testing::TestInfo& testInfo);
    virtual void OnTestEnd(const testing::TestInfo& testInfo);

protected:
    virtual void checkTraceResults();
};

#endif
