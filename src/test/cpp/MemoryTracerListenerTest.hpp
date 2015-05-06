#ifndef MEMORY_TRACER_LISTENER_TEST_HPP
#define MEMORY_TRACER_LISTENER_TEST_HPP

#include <stdlib.h>
#include <unistd.h>

#include <fakeit.hpp>
#include "gtest/gtest.h"

#include "FakeMemoryTracerListener.hpp"

using fakeit::Mock;
using fakeit::Verify;
using fakeit::VerifyNoOtherInvocations;
using fakeit::When;

class MemoryTracerListenerTest : public testing::Test {
private:
    bool parserShouldHaveBeenUsed = false;
    const char* environmentVariable;
    char* dirPath;

    void verifyMocks() {
        Mock<FailureReporter>& reporter = listener->getFailureReporterMock();
        Mock<MemoryTracer>& tracer = listener->getMemoryTracerMock();
        Mock<TraceFileParser>& parser = listener->getTraceFileParserMock();

        if (parserShouldHaveBeenUsed) {
            Verify(Method(parser, parse)).Once();
            Verify(Method(parser, getMemoryLeakCount)).Once();
            Verify(Method(parser, getMemoryLeakSize)).Once();
            Verify(Method(parser, getInvalidDeallocationCount)).Once();
        }

        VerifyNoOtherInvocations(Method(parser, parse));
        VerifyNoOtherInvocations(Method(parser, getMemoryLeakCount));
        VerifyNoOtherInvocations(Method(parser, getMemoryLeakSize));
        VerifyNoOtherInvocations(Method(parser, getInvalidDeallocationCount));

        VerifyNoOtherInvocations(Method(tracer, start));
        VerifyNoOtherInvocations(Method(tracer, stop));

        VerifyNoOtherInvocations(Method(reporter, fail));
    }

protected:
    FakeMemoryTracerListener* listener;

    virtual void SetUp() {
        dirPath = NULL;

        environmentVariable = "MALLOC_TRACE";
        unsetenv(environmentVariable);

        listener = new FakeMemoryTracerListener();

        When(Method(listener->getMemoryTracerMock(), start)).Return();
        When(Method(listener->getMemoryTracerMock(), stop)).Return();

        When(Method(listener->getFailureReporterMock(), fail)).Return();
    }

    virtual void TearDown() {
        if (listener != NULL)
            destroy();

        if (dirPath != NULL)
            free(dirPath);
    }

    void destroy() {
        verifyMocks();

        delete listener;
        listener = NULL;
    }

    const char* getEnvironmentVariable() {
        return getenv(environmentVariable);
    }

    const char* getFilePath() {
        return getEnvironmentVariable();
    }

    const char* getFileName() {
        const char* filePath = getFilePath();
        const char* fileName = strrchr(filePath, '/');

        return fileName == NULL ? filePath : fileName + 1;
    }

    const char* getDirPath() {
        if (dirPath == NULL) {
            const char* filePath = getFilePath();
            const char* fileName = strrchr(filePath, '/');

            if (fileName == NULL)
                return NULL;

            dirPath = strdup(filePath);
            dirPath[fileName - filePath] = '\0';
        }

        return dirPath;
    }

    const char* getDirName() {
        const char* dirPath = getDirPath();

        if (dirPath == NULL)
            return NULL;

        const char* dirName = strrchr(dirPath, '/');

        return dirName == NULL ? dirPath : dirName + 1;
    }

    void prepareParserMock(int memoryLeakCount, int memoryLeakSize,
            int invalidDeallocationCount) {
        Mock<TraceFileParser>& parser = listener->getTraceFileParserMock();

        When(Method(parser, parse)).Return();
        When(Method(parser, getMemoryLeakCount)).Return(memoryLeakCount);
        When(Method(parser, getMemoryLeakSize)).Return(memoryLeakSize);
        When(Method(parser, getInvalidDeallocationCount))
                .Return(invalidDeallocationCount);

        parserShouldHaveBeenUsed = true;
    }

    void testTraceResults(int memoryLeakCount, int memoryLeakSize,
            int invalidDeallocationCount, std::string expectedError) {
        Mock<FailureReporter>& reporter = listener->getFailureReporterMock();

        prepareParserMock(memoryLeakCount, memoryLeakSize,
                invalidDeallocationCount);

        listener->checkTraceResults();

        Verify(Method(reporter, fail).Using(expectedError));
    }

    void performDummyAllocationAndDeallocation() {
        void* dummyAddress = malloc(123);

        EXPECT_TRUE(dummyAddress != NULL);

        free(dummyAddress);
    }
};

#endif
