#include <mcheck.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "MemoryTracerListener.hpp"

#define ENVIRONMENT_VARIABLE "MALLOC_TRACE"
#define DIRNAME_TEMPLATE "/tmp/mtrace_gtest.XXXXXX"

MemoryTracerListener::MemoryTracerListener() : tempDir("mtrace_gtest.") {
    mtraceDirName = strdup(DIRNAME_TEMPLATE);
    mtraceFileName = strdup(DIRNAME_TEMPLATE "/mtrace");

    if (mkdtemp(mtraceDirName) != NULL && mtraceDirName[0] != 0) {
        memcpy(mtraceFileName, mtraceDirName, strlen(mtraceDirName));
        setEnvironmentVariable();
    }

    failureReporter = new FailureReporter();
    memoryTracer = new MemoryTracer();
    traceFileParser = new TraceFileParser(mtraceFileName);
}

MemoryTracerListener::~MemoryTracerListener() {
    unsetenv(ENVIRONMENT_VARIABLE);

    unlink(mtraceFileName);
    rmdir(mtraceDirName);

    free(mtraceDirName);
    free(mtraceFileName);
    free(mtraceEnvironmentVariable);

    delete traceFileParser;
    delete memoryTracer;
    delete failureReporter;
}

void MemoryTracerListener::OnTestStart(const testing::TestInfo& testInfo) {
    mtrace();
}

void MemoryTracerListener::OnTestEnd(const testing::TestInfo& testInfo) {
    stopTrace();
    checkTraceResults();
}

void MemoryTracerListener::stopTrace() {
    muntrace();
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

void MemoryTracerListener::setEnvironmentVariable() {
    int nameLength = strlen(ENVIRONMENT_VARIABLE);
    int valueLength = strlen(mtraceFileName);
    int totalLength = nameLength + 1 + valueLength;

    mtraceEnvironmentVariable = (char*)malloc(totalLength + 1);

    char* name = &mtraceEnvironmentVariable[0];
    char* separator = name + nameLength;
    char* value = separator + 1;
    char* end = value + valueLength;

    memcpy(name, ENVIRONMENT_VARIABLE, nameLength);
    memcpy(value, mtraceFileName, valueLength);
    *separator = '=';
    *end = '\0';

    putenv(mtraceEnvironmentVariable);
}
