#ifndef FAKE_MEMORY_TRACER_LISTENER_HPP
#define FAKE_MEMORY_TRACER_LISTENER_HPP

#include <fakeit.hpp>

#include "MemoryTracerListener.hpp"
#include "MockInterceptor.hpp"

class FakeMemoryTracerListener : public MemoryTracerListener {
private:
    bool stopTraceWasCalledFirst;
    int timesCheckTraceResultsWasCalled;
    int timesStopTraceWasCalled;

    MockInterceptor<FailureReporter> reporter;
    MockInterceptor<TraceFileParser> parser;

public:
    FakeMemoryTracerListener() : stopTraceWasCalledFirst(false),
            timesCheckTraceResultsWasCalled(0),
            timesStopTraceWasCalled(0),
            reporter(failureReporter),
            parser(traceFileParser) {
    }

    ~FakeMemoryTracerListener() noexcept {
    }

    TraceFileParser* getParser() {
        return parser.getOriginal();
    }

    fakeit::Mock<TraceFileParser>& getTraceFileParserMock() {
        return parser.getMock();
    }

    FailureReporter* getFailureReporter() {
        return reporter.getOriginal();
    }

    fakeit::Mock<FailureReporter>& getFailureReporterMock() {
        return reporter.getMock();
    }

    MemoryTracer* getMemoryTracer() {
	return memoryTracer;
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
