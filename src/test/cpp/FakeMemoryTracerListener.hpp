#ifndef FAKE_MEMORY_TRACER_LISTENER_HPP
#define FAKE_MEMORY_TRACER_LISTENER_HPP

#include <fakeit.hpp>

#include "MemoryTracerListener.hpp"
#include "MockInterceptor.hpp"

class FakeMemoryTracerListener : public MemoryTracerListener {
private:
    MockInterceptor<EnvironmentVariable> envVar;
    MockInterceptor<FailureReporter> reporter;
    MockInterceptor<MemoryTracer> tracer;
    MockInterceptor<TraceFileParser> parser;

public:
    FakeMemoryTracerListener() : MemoryTracerListener(false),
            envVar(mallocTraceEnvVar),
            reporter(failureReporter),
            tracer(memoryTracer),
            parser(traceFileParser) {
    }

    ~FakeMemoryTracerListener() noexcept {
    }

    TempDir& getTempDir() {
        return tempDir;
    }

    EnvironmentVariable* getMallocTraceEnvVar() {
        return envVar.getOriginal();
    }

    fakeit::Mock<EnvironmentVariable>& getMallocTraceEnvVarMock() {
        return envVar.getMock();
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
        return tracer.getOriginal();
    }

    fakeit::Mock<MemoryTracer>& getMemoryTracerMock() {
        return tracer.getMock();
    }

    using MemoryTracerListener::checkTraceResults;
    using MemoryTracerListener::setEnvironmentVariable;
};

#endif
