#ifndef FAKE_MEMORY_TRACER_LISTENER_HPP
#define FAKE_MEMORY_TRACER_LISTENER_HPP

#include "Interceptor.hpp"
#include "MemoryTracerListener.hpp"
#include "MockFailureReporter.hpp"
#include "MockTraceFileParser.hpp"

#define MOCK_INTERCEPT(x) Interceptor<x, Mock ## x>

class FakeMemoryTracerListener : public MemoryTracerListener {
private:
    MOCK_INTERCEPT(FailureReporter) reporter;
    MOCK_INTERCEPT(TraceFileParser) parser;

public:
    FakeMemoryTracerListener() :
            reporter(failureReporter, new MockFailureReporter()),
            parser(traceFileParser, new MockTraceFileParser(mtraceFileName)) {
    }

    TraceFileParser* getParser() {
        return parser.getOriginal();
    }

    MockTraceFileParser* getMockParser() {
        return parser.getReplacement();
    }

    FailureReporter* getFailureReporter() {
        return reporter.getOriginal();
    }

    MockFailureReporter* getMockFailureReporter() {
        return reporter.getReplacement();
    }
};

#endif
