#include <mcheck.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "MemoryTracerListener.hpp"

#define ENVIRONMENT_VARIABLE "MALLOC_TRACE"
#define DIRNAME_TEMPLATE "/tmp/mtrace_gtest.XXXXXX"

MemoryTracerListener::MemoryTracerListener() {
    mtraceDirName = strdup(DIRNAME_TEMPLATE);
    mtraceFileName = strdup(DIRNAME_TEMPLATE "/mtrace");

    if (mkdtemp(mtraceDirName) != NULL && mtraceDirName[0] != 0) {
        memcpy(mtraceFileName, mtraceDirName, strlen(mtraceDirName));
        setenv(ENVIRONMENT_VARIABLE, mtraceFileName, 1);
    }

    failureReporter = new FailureReporter();
    traceFileParser = new TraceFileParser(mtraceFileName);
}

MemoryTracerListener::~MemoryTracerListener() {
    unlink(mtraceFileName);
    rmdir(mtraceDirName);

    delete traceFileParser;
    delete failureReporter;
}

void MemoryTracerListener::OnTestStart(const testing::TestInfo& testInfo) {
    mtrace();
}

void MemoryTracerListener::OnTestEnd(const testing::TestInfo& testInfo) {
    checkTraceResults();
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

    failureReporter->fail(errorMessage.str().c_str());
}

void MemoryTracerListener::buildMemoryLeakErrorMessage(
        std::ostream& errorMessage, int count, int size) {
    static const char countSingular[] = " memory leak detected. ";
    static const char countPlural[] = " memory leaks detected. ";

    errorMessage << count << (count == 1 ? countSingular :  countPlural);
    errorMessage << size << (size == 1 ? " byte total." : " bytes total.");
}
