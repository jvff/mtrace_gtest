#ifndef FAKE_MEMORY_TRACER_LISTENER_HPP
#define FAKE_MEMORY_TRACER_LISTENER_HPP

#include "MemoryTracerListener.hpp"
#include "MockTraceFileParser.hpp"

class FakeMemoryTracerListener : public MemoryTracerListener {
private:
    MockTraceFileParser* mockParser;
    TraceFileParser* originalParser;

public:
    FakeMemoryTracerListener() {
        mockParser = new MockTraceFileParser(mtraceFileName);

        originalParser = traceFileParser;
        traceFileParser = mockParser;
    }

    ~FakeMemoryTracerListener() {
        traceFileParser = originalParser;

        delete mockParser;
    }

    TraceFileParser* getParser() {
        return traceFileParser;
    }

    MockTraceFileParser* getMockParser() {
        return mockParser;
    }
};

#endif
