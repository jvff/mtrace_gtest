#ifndef FAKE_MEMORY_TRACER_LISTENER_HPP
#define FAKE_MEMORY_TRACER_LISTENER_HPP

#include <fakeit.hpp>

#include "MemoryTracerListener.hpp"
#include "MockInterceptor.hpp"
#include "MockTraceFileParser.hpp"

#define MOCK_INTERCEPT(x) Interceptor<x, Mock ## x>

class FakeMemoryTracerListener : public MemoryTracerListener {
private:
    bool stopTraceWasCalledFirst;
    int timesCheckTraceResultsWasCalled;
    int timesStopTraceWasCalled;

    MockInterceptor<FailureReporter> reporter;
    MOCK_INTERCEPT(TraceFileParser) parser;

public:
    FakeMemoryTracerListener() : stopTraceWasCalledFirst(false),
            timesCheckTraceResultsWasCalled(0),
            timesStopTraceWasCalled(0),
            reporter(failureReporter),
            parser(traceFileParser, new MockTraceFileParser(mtraceFileName)) {
    }

    ~FakeMemoryTracerListener() noexcept {
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

    fakeit::Mock<FailureReporter>& getFailureReporterMock() {
        return reporter.getMock();
    }

    void checkTraceResults() {
        ++timesCheckTraceResultsWasCalled;
        MemoryTracerListener::checkTraceResults();
    }

    int getTimesCheckTraceResultsWasCalled() {
        return timesCheckTraceResultsWasCalled;
    }

    void stopTrace() {
        ++timesStopTraceWasCalled;
        stopTraceWasCalledFirst = stopTraceWasCalledFirst
                || timesCheckTraceResultsWasCalled == 0;

        MemoryTracerListener::stopTrace();
    }

    int getTimesStopTraceWasCalled() {
        return timesStopTraceWasCalled;
    }

    bool wasStopTraceCalledBeforeCheckTraceResults() {
        return stopTraceWasCalledFirst;
    }
};

#endif
