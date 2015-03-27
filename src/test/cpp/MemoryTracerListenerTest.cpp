#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "MemoryTracerListenerTest.hpp"

using testing::Return;
using testing::StrEq;

TEST_F(MemoryTracerListenerTest, setEnvironmentTest) {
    EXPECT_FALSE(getEnvironmentVariable() == NULL);
}

TEST_F(MemoryTracerListenerTest, traceFileIsInTempDir) {
    const char* tempDir = "/tmp/";
    const int length = strlen(tempDir);

    EXPECT_TRUE(strncmp(tempDir, getFilePath(), length) == 0);
}

TEST_F(MemoryTracerListenerTest, traceFileDoesntExistYet) {
    struct stat info;

    EXPECT_NE(stat(getFilePath(), &info), 0);
    EXPECT_EQ(errno, ENOENT);
}

TEST_F(MemoryTracerListenerTest, traceFileNameIsFixed) {
    EXPECT_STREQ("mtrace", getFileName());
}

TEST_F(MemoryTracerListenerTest, traceFileIsInNewTempDir) {
    const char* dirName = getDirName();
    char* dirPrefix = strdup(dirName);
    const int dirNameLength = strlen(dirName);
    const int wildcardLength = 6;

    ASSERT_GT(dirNameLength, wildcardLength);

    dirPrefix[strlen(dirName) - wildcardLength] = '\0';

    EXPECT_STREQ("mtrace_gtest.", dirPrefix);

    free(dirPrefix);
}

TEST_F(MemoryTracerListenerTest, tempDirExists) {
    struct stat info;

    EXPECT_EQ(stat(getDirPath(), &info), 0);
    EXPECT_TRUE(S_ISDIR(info.st_mode));
}

TEST_F(MemoryTracerListenerTest, tempDirIsDeletedAfterDestruction) {
    struct stat info;

    destroy();

    EXPECT_NE(stat(getDirPath(), &info), 0);
    EXPECT_EQ(errno, ENOENT);
}

TEST_F(MemoryTracerListenerTest, implementsTestListener) {
    testing::TestEventListener* superClass = listener;
}

TEST_F(MemoryTracerListenerTest, traceFileIsCreatedWhenTestStarts) {
    const testing::UnitTest* unitTest = testing::UnitTest::GetInstance();
    const testing::TestInfo* testInfo = unitTest->current_test_info();
    struct stat info;

    listener->OnTestStart(*testInfo);

    EXPECT_EQ(stat(getFilePath(), &info), 0);
    EXPECT_TRUE(S_ISREG(info.st_mode));
}

TEST_F(MemoryTracerListenerTest, listenerInitializesParser) {
    TraceFileParser* parser = listener->getParser();

    EXPECT_TRUE(parser != NULL);
}

TEST_F(MemoryTracerListenerTest, parserIsCalledWhenTestEnds) {
    const testing::UnitTest* unitTest = testing::UnitTest::GetInstance();
    const testing::TestInfo* testInfo = unitTest->current_test_info();

    listener->OnTestStart(*testInfo);

    EXPECT_CALL(*parser, parse()).Times(1);
    EXPECT_CALL(*parser, getMemoryLeakCount()).Times(1);
    EXPECT_CALL(*parser, getMemoryLeakSize()).Times(1);
    EXPECT_CALL(*parser, getInvalidDeallocationCount()).Times(1);

    listener->OnTestEnd(*testInfo);

    EXPECT_EQ(1, listener->getTimesCheckTraceResultsWasCalled());
}

TEST_F(MemoryTracerListenerTest, reporterIsInitialized) {
    FailureReporter *reporter = listener->getFailureReporter();

    EXPECT_TRUE(reporter != NULL);
}

TEST_F(MemoryTracerListenerTest, oneMemoryLeak) {
    const char expectedError[] = "1 memory leak detected. 10 bytes total.";

    EXPECT_CALL(*parser, parse()).Times(1);
    EXPECT_CALL(*parser, getMemoryLeakCount()).Times(1).WillOnce(Return(1));
    EXPECT_CALL(*parser, getMemoryLeakSize()).Times(1).WillOnce(Return(10));
    EXPECT_CALL(*parser, getInvalidDeallocationCount()).Times(1)
            .WillOnce(Return(0));
    EXPECT_CALL(*reporter, fail(StrEq(expectedError)));

    listener->checkTraceResults();
}
