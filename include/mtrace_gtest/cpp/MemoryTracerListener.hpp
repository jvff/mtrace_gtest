#ifndef MEMORY_TRACER_LISTENER_HPP
#define MEMORY_TRACER_LISTENER_HPP

#include <iostream>

#include "gtest/gtest.h"

#include "FailureReporter.hpp"
#include "MallocTraceEnvVar.hpp"
#include "MemoryTracer.hpp"
#include "TempDir.hpp"
#include "TraceFileParser.hpp"

class MemoryTracerListener : public testing::EmptyTestEventListener {
protected:
    std::string mtraceFileName;
    EnvironmentVariable* mallocTraceEnvVar;
    FailureReporter* failureReporter;
    MemoryTracer* memoryTracer;
    TraceFileParser* traceFileParser;
    TempDir tempDir;

public:
    MemoryTracerListener();
    virtual ~MemoryTracerListener();

    virtual void OnTestProgramStart(const testing::UnitTest& unitTest);
    virtual void OnTestStart(const testing::TestInfo& testInfo);
    virtual void OnTestEnd(const testing::TestInfo& testInfo);

protected:
    virtual void checkTraceResults();

private:
    void fail(int memoryLeakCount, int memoryLeakSize,
            int invalidDeallocationCount);
    void buildMemoryLeakErrorMessage(std::ostream& errorMessage, int count,
            int size);
    void addOptionalSeparator(std::ostream& errorMessage, int memoryLeakCount,
            int invalidDeallocationCount);
    void buildInvalidDeallocationErrorMessage(std::ostream& errorMessage,
            int count);
};

#endif
