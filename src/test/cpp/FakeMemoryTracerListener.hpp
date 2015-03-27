#ifndef FAKE_MEMORY_TRACER_LISTENER_HPP
#define FAKE_MEMORY_TRACER_LISTENER_HPP

#include "MemoryTracerListener.hpp"
#include "MockFailureReporter.hpp"
#include "MockTraceFileParser.hpp"

class FakeMemoryTracerListener : public MemoryTracerListener {
private:
    FailureReporter* originalFailureReporter;
    MockFailureReporter* mockFailureReporter;
    MockTraceFileParser* mockParser;
    TraceFileParser* originalParser;

public:
    FakeMemoryTracerListener() {
        mockFailureReporter = new MockFailureReporter();
        mockParser = new MockTraceFileParser(mtraceFileName);

        originalFailureReporter = failureReporter;
        failureReporter = mockFailureReporter;

        originalParser = traceFileParser;
        traceFileParser = mockParser;
    }

    ~FakeMemoryTracerListener() {
        failureReporter = originalFailureReporter;
        traceFileParser = originalParser;

        delete mockFailureReporter;
        delete mockParser;
    }

    TraceFileParser* getParser() {
        return traceFileParser;
    }

    MockTraceFileParser* getMockParser() {
        return mockParser;
    }

    FailureReporter* getFailureReporter() {
        return failureReporter;
    }

    MockFailureReporter* getMockFailureReporter() {
        return mockFailureReporter;
    }
};

#endif
