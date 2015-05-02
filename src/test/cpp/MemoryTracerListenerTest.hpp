#ifndef MEMORY_TRACER_LISTENER_TEST_HPP
#define MEMORY_TRACER_LISTENER_TEST_HPP

#include <stdlib.h>
#include <unistd.h>

#include <fakeit.hpp>
#include "gtest/gtest.h"

#include "FakeMemoryTracerListener.hpp"
#include "MockTraceFileParser.hpp"

using fakeit::Mock;
using fakeit::Verify;
using fakeit::VerifyNoOtherInvocations;
using fakeit::When;
using testing::Return;

class MemoryTracerListenerTest : public testing::Test {
private:
    const char* environmentVariable;
    char* dirPath;

protected:
    FakeMemoryTracerListener* listener;
    MockTraceFileParser* parser;

    virtual void SetUp() {
        dirPath = NULL;

        environmentVariable = "MALLOC_TRACE";
        unsetenv(environmentVariable);

        listener = new FakeMemoryTracerListener();
        parser = listener->getMockParser();

        EXPECT_CALL(*parser, parse()).Times(0);
        EXPECT_CALL(*parser, getMemoryLeakCount()).Times(0);
        EXPECT_CALL(*parser, getMemoryLeakSize()).Times(0);
        EXPECT_CALL(*parser, getInvalidDeallocationCount()).Times(0);

        When(Method(listener->getFailureReporterMock(), fail)).Return();
    }

    virtual void TearDown() {
        if (listener != NULL)
            destroy();

        if (dirPath != NULL)
            free(dirPath);
    }

    void destroy() {
        Mock<FailureReporter>& reporter = listener->getFailureReporterMock();

        VerifyNoOtherInvocations(Method(reporter, fail));

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

    void prepareParserExpectations(int memoryLeakCount, int memoryLeakSize,
            int invalidDeallocationCount) {
        EXPECT_CALL(*parser, parse()).Times(1);
        EXPECT_CALL(*parser, getMemoryLeakCount()).Times(1)
                .WillRepeatedly(Return(memoryLeakCount));
        EXPECT_CALL(*parser, getMemoryLeakSize()).Times(1)
                .WillRepeatedly(Return(memoryLeakSize));
        EXPECT_CALL(*parser, getInvalidDeallocationCount()).Times(1)
                .WillRepeatedly(Return(invalidDeallocationCount));
    }

    void testTraceResults(int memoryLeakCount, int memoryLeakSize,
            int invalidDeallocationCount, std::string expectedError) {
        Mock<FailureReporter>& reporter = listener->getFailureReporterMock();

        prepareParserExpectations(memoryLeakCount, memoryLeakSize,
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
