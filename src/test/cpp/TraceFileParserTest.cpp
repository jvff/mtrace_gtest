#include "TraceFileParserTest.hpp"

TEST_P(TraceFileParserTest, noAllocations) {
    parseAndExpect(0, 0, 0);
}

TEST_P(TraceFileParserTest, oneAllocationAndDeallocation) {
    const void* address = alloc(1);

    dealloc(address);

    parseAndExpect(0, 0, 0);
}

TEST_P(TraceFileParserTest, oneMemoryLeak) {
    alloc(1);

    parseAndExpect(1, 1, 0);
}

TEST_P(TraceFileParserTest, oneLargerMemoryLeak) {
    alloc(100);
}

TEST_P(TraceFileParserTest, twoMemoryLeaks) {
    alloc(10);
    alloc(70);

    parseAndExpect(2, 80, 0);
}

TEST_P(TraceFileParserTest, oneMemoryLeakSurrondedByNormalUsage) {
    const void* address = alloc(1000);
    alloc(10000);
    dealloc(address);

    parseAndExpect(1, 10000, 0);
}

TEST_P(TraceFileParserTest, oneInvalidDeallocation) {
    const void* address = (const void*)0x08000000;

    dealloc(address);

    parseAndExpect(0, 0, 1);
}

TEST_P(TraceFileParserTest, doubleDeallocation) {
    const void* address = alloc(15);

    dealloc(address);
    dealloc(address);

    parseAndExpect(0, 0, 1);
}

TEST_P(TraceFileParserTest, invalidDeallocationAndMemoryLeak) {
    const void* address = alloc(4096);
    const char* byteAddress = (const char*)address;
    const void* invalidAddress = (const void*)(byteAddress - 65536);

    dealloc(invalidAddress);

    parseAndExpect(1, 4096, 1);
}

TEST_P(TraceFileParserTest, activeAllocationsAreClearedOnNextParseExecution) {
    const void* address = alloc(100000000);

    parseAndExpect(1, 100000000, 0);
    resetTraceFile();
    parseAndExpect(0, 0, 0);
}

TEST_P(TraceFileParserTest, invalidDeallocationsAreClearedOnNextParseCall) {
    const void* address = (const void*)0x08000000;

    dealloc(address);

    parseAndExpect(0, 0, 1);
    resetTraceFile();
    parseAndExpect(0, 0, 0);
}

const char* debugInformation[] = {
    "",
    "./a.out:",
    "/usr/lib/x86_64-linux-gnu/libstdc++.so.6:(_Znwm+0x1d)"
};

INSTANTIATE_TEST_CASE_P(TraceFileParserTestInstantiation, TraceFileParserTest,
        testing::ValuesIn(debugInformation));
