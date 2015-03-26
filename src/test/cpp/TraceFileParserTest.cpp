#include "TraceFileParserTest.hpp"

TEST_F(TraceFileParserTest, noAllocations) {
    parseAndExpect(0, 0, 0);
}

TEST_F(TraceFileParserTest, oneAllocationAndDeallocation) {
    const void* address = alloc(1);

    dealloc(address);

    parseAndExpect(0, 0, 0);
}

TEST_F(TraceFileParserTest, oneMemoryLeak) {
    alloc(1);

    parseAndExpect(1, 1, 0);
}

TEST_F(TraceFileParserTest, oneLargerMemoryLeak) {
    alloc(100);
}

TEST_F(TraceFileParserTest, twoMemoryLeaks) {
    alloc(10);
    alloc(70);

    parseAndExpect(2, 80, 0);
}

TEST_F(TraceFileParserTest, oneMemoryLeakSurrondedByNormalUsage) {
    const void* address = alloc(1000);
    alloc(10000);
    dealloc(address);

    parseAndExpect(1, 10000, 0);
}

TEST_F(TraceFileParserTest, oneInvalidDeallocation) {
    const void* address = (const void*)0x08000000;

    dealloc(address);

    parseAndExpect(0, 0, 1);
}

TEST_F(TraceFileParserTest, doubleDeallocation) {
    const void* address = alloc(15);

    dealloc(address);
    dealloc(address);

    parseAndExpect(0, 0, 1);
}

TEST_F(TraceFileParserTest, invalidDeallocationAndMemoryLeak) {
    const void* address = alloc(4096);
    const char* byteAddress = (const char*)address;
    const void* invalidAddress = (const void*)(byteAddress - 65536);

    dealloc(invalidAddress);

    parseAndExpect(1, 4096, 1);
}
