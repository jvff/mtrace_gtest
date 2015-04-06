#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "MemoryTracerListenerTest.hpp"

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
    testTraceResults(1, 10, 0, "1 memory leak detected. 10 bytes total.");
}

TEST_F(MemoryTracerListenerTest, oneByteMemoryLeak) {
    testTraceResults(1, 1, 0, "1 memory leak detected. 1 byte total.");
}

TEST_F(MemoryTracerListenerTest, impossibleOneByteMemoryLeak) {
    testTraceResults(7, 1, 0, "7 memory leaks detected. 1 byte total.");
}

TEST_F(MemoryTracerListenerTest, twoMemoryLeaks) {
    testTraceResults(2, 30, 0, "2 memory leaks detected. 30 bytes total.");
}

TEST_F(MemoryTracerListenerTest, oneInvalidDeallocation) {
    testTraceResults(0, 0, 1, "1 invalid memory deallocation detected.");
}

TEST_F(MemoryTracerListenerTest, twoInvalidDeallocations) {
    testTraceResults(0, 0, 2, "2 invalid memory deallocations detected.");
}

TEST_F(MemoryTracerListenerTest, oneMemoryLeakAndOneInvalidDeallocation) {
    testTraceResults(1, 56, 1, "1 memory leak detected. 56 bytes total."
            " 1 invalid memory deallocation detected.");
}

TEST_F(MemoryTracerListenerTest, threeMemoryLeaksAndOneInvalidDeallocation) {
    testTraceResults(3, 488, 1, "3 memory leaks detected. 488 bytes total."
            " 1 invalid memory deallocation detected.");
}

TEST_F(MemoryTracerListenerTest, oneMemoryLeakAndNineInvalidDeallocations) {
    testTraceResults(1, 30, 9, "1 memory leak detected. 30 bytes total."
            " 9 invalid memory deallocations detected.");
}

TEST_F(MemoryTracerListenerTest, fiveMemoryLeaksAndSixInvalidDeallocations) {
    testTraceResults(5, 1000000, 6, "5 memory leaks detected. 1000000 bytes"
            " total. 6 invalid memory deallocations detected.");
}
