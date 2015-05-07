#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "MemoryTracerListenerTest.hpp"

using fakeit::Spy;

TEST_F(MemoryTracerListenerTest, listenerUsesMallocTraceEnvVar) {
    EnvironmentVariable* envVar = listener->getMallocTraceEnvVar();

    EXPECT_EQ(environmentVariable, envVar->getName());
}

TEST_F(MemoryTracerListenerTest, envVarIsSetToMtraceFileInTempDir) {
    Mock<EnvironmentVariable>& mock = listener->getMallocTraceEnvVarMock();

    When(OverloadedMethod(mock, operator=, void(const std::string&))).Return();

    listener->setEnvironmentVariable();

    Verify(OverloadedMethod(mock, operator=, void(const std::string&))
            .Using(getFilePath()));
}

TEST_F(MemoryTracerListenerTest, listenerInitializesTempDir) {
    std::string expectedPathPrefix = "/tmp/mtrace_gtest.";
    int expectedLength = expectedPathPrefix.length();
    TempDir& tempDir = listener->getTempDir();

    EXPECT_EQ(expectedPathPrefix, tempDir.getPath().substr(0, expectedLength));
}

TEST_F(MemoryTracerListenerTest, traceFileDoesntExistYet) {
    struct stat info;

    EXPECT_NE(stat(getFilePath(), &info), 0);
    EXPECT_EQ(errno, ENOENT);
}

TEST_F(MemoryTracerListenerTest, traceFileNameIsFixed) {
    EXPECT_STREQ("mtrace", getFileName());
}

TEST_F(MemoryTracerListenerTest, implementsTestListener) {
    testing::TestEventListener* superClass = listener;
}

TEST_F(MemoryTracerListenerTest, listenerInitializesTracer) {
    MemoryTracer* memoryTracer = listener->getMemoryTracer();

    EXPECT_TRUE(memoryTracer != NULL);
}

TEST_F(MemoryTracerListenerTest, tracingStartsWhenTestStarts) {
    const testing::UnitTest* unitTest = testing::UnitTest::GetInstance();
    const testing::TestInfo* testInfo = unitTest->current_test_info();

    listener->OnTestStart(*testInfo);

    Verify(Method(listener->getMemoryTracerMock(), start)).Once();
}

TEST_F(MemoryTracerListenerTest, listenerInitializesParser) {
    TraceFileParser* parser = listener->getParser();

    EXPECT_TRUE(parser != NULL);
}

TEST_F(MemoryTracerListenerTest, traceStopsWhenTestEnds) {
    const testing::UnitTest* unitTest = testing::UnitTest::GetInstance();
    const testing::TestInfo* testInfo = unitTest->current_test_info();

    prepareParserMock(0, 0, 0);
    listener->OnTestEnd(*testInfo);

    Verify(Method(listener->getMemoryTracerMock(), stop)).Once();
}

TEST_F(MemoryTracerListenerTest, parserIsCalledAfterTraceStopsWhenTestEnds) {
    const testing::UnitTest* unitTest = testing::UnitTest::GetInstance();
    const testing::TestInfo* testInfo = unitTest->current_test_info();
    auto listenerSpy = Mock<FakeMemoryTracerListener>(*listener);

    Spy(Method(listenerSpy, OnTestEnd));
    Spy(Method(listenerSpy, checkTraceResults));

    prepareParserMock(0, 0, 0);
    listenerSpy.get().OnTestEnd(*testInfo);

    Verify(Method(listener->getMemoryTracerMock(), stop)
            + Method(listenerSpy, checkTraceResults));
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
