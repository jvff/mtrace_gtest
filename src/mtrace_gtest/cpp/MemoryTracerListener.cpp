#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "MemoryTracerListener.hpp"

#define ENVIRONMENT_VARIABLE "MALLOC_TRACE"

MemoryTracerListener::MemoryTracerListener() : MemoryTracerListener(true) {
}

MemoryTracerListener::MemoryTracerListener(bool shouldSetEnvironmentVariable)
	: tempDir("mtrace_gtest.") {
    mtraceFileName = tempDir.getPath() + "/mtrace";

    mallocTraceEnvVar = new MallocTraceEnvVar();
    failureReporter = new FailureReporter();
    memoryTracer = new MemoryTracer();
    traceFileParser = new TraceFileParser(mtraceFileName);

    if (shouldSetEnvironmentVariable)
	setEnvironmentVariable();
}

MemoryTracerListener::~MemoryTracerListener() {
    unlink(mtraceFileName.c_str());

    delete mallocTraceEnvVar;
    delete traceFileParser;
    delete memoryTracer;
    delete failureReporter;
}

void MemoryTracerListener::OnTestStart(const testing::TestInfo& testInfo) {
    memoryTracer->start();
}

void MemoryTracerListener::OnTestEnd(const testing::TestInfo& testInfo) {
    stopTrace();
    checkTraceResults();
}

void MemoryTracerListener::setEnvironmentVariable() {
    *mallocTraceEnvVar = mtraceFileName;
}

void MemoryTracerListener::stopTrace() {
    memoryTracer->stop();
}

void MemoryTracerListener::checkTraceResults() {
    int memoryLeakCount;
    int memoryLeakSize;
    int invalidDeallocationCount;

    traceFileParser->parse();

    memoryLeakCount = traceFileParser->getMemoryLeakCount();
    memoryLeakSize = traceFileParser->getMemoryLeakSize();
    invalidDeallocationCount = traceFileParser->getInvalidDeallocationCount();

    if (memoryLeakCount != 0 || invalidDeallocationCount != 0)
        fail(memoryLeakCount, memoryLeakSize, invalidDeallocationCount);
}

void MemoryTracerListener::fail(int memoryLeakCount, int memoryLeakSize,
        int invalidDeallocationCount) {
    std::stringstream errorMessage;

    buildMemoryLeakErrorMessage(errorMessage, memoryLeakCount, memoryLeakSize);
    addOptionalSeparator(errorMessage, memoryLeakCount,
            invalidDeallocationCount);
    buildInvalidDeallocationErrorMessage(errorMessage,
            invalidDeallocationCount);

    failureReporter->fail(errorMessage.str());
}

void MemoryTracerListener::buildMemoryLeakErrorMessage(
        std::ostream& errorMessage, int count, int size) {
    static const char countSingular[] = " memory leak detected. ";
    static const char countPlural[] = " memory leaks detected. ";

    if (count > 0) {
        errorMessage << count << (count == 1 ? countSingular :  countPlural);
        errorMessage << size << (size == 1 ? " byte total." : " bytes total.");
    }
}

void MemoryTracerListener::addOptionalSeparator(std::ostream& errorMessage,
        int memoryLeakCount, int invalidDeallocationCount) {
    if (memoryLeakCount > 0 && invalidDeallocationCount > 0)
        errorMessage << " ";
}

void MemoryTracerListener::buildInvalidDeallocationErrorMessage(
        std::ostream& errorMessage, int count) {
    static const char countSingular[] = " invalid memory deallocation"
            " detected.";
    static const char countPlural[] = " invalid memory deallocations detected.";

    if (count > 0)
        errorMessage << count << (count == 1 ? countSingular :  countPlural);
}
