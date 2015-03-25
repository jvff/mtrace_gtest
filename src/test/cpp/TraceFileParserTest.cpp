#include <stdio.h>
#include <unistd.h>

#include "gtest/gtest.h"

#include "TraceFileParser.hpp"

class TraceFileParserTest : public testing::Test {
private:
    const char* currentAllocation;
    int traceFileDescriptor;

protected:
    FILE* traceFile;
    TraceFileParser* parser;

    virtual void SetUp() {
        char tempFile[] = "/tmp/fake_mtrace.XXXXXX";

        traceFileDescriptor = mkstemp(tempFile);
        traceFile = fdopen(traceFileDescriptor, "w+");

        currentAllocation = (const char*)0x10000000;

        parser = new TraceFileParser(tempFile);
    }

    virtual void TearDown() {
        if (traceFile != NULL)
            closeTraceFile();

        delete parser;
    }

    virtual const void* alloc(int size) {
        const void* address = (const void*)currentAllocation;

        currentAllocation += size;

        fprintf(traceFile, "[0x0] + %p %#x\n", address, size);

        return address;
    }

    virtual void dealloc(const void* address) {
        fprintf(traceFile, "[0x0] - %p\n", address);
    }

    virtual void closeTraceFile() {
        fclose(traceFile);
        close(traceFileDescriptor);
        traceFile = NULL;
    }

    virtual void parse() {
        closeTraceFile();
        parser->parse();
    }

    virtual void parseAndExpect(int memoryLeakCount, int memoryLeakSize,
            int invalidDeallocations) {
        parse();

        ASSERT_EQ(memoryLeakCount, parser->getMemoryLeakCount());
        ASSERT_EQ(memoryLeakSize, parser->getMemoryLeakSize());
        ASSERT_EQ(invalidDeallocations, parser->getInvalidDeallocationCount());
    }
};

TEST_F(TraceFileParserTest, noAllocations) {
    parseAndExpect(0, 0, 0);
}

TEST_F(TraceFileParserTest, oneAllocationAndDeallocation) {
    const void* address = alloc(1);

    dealloc(address);

    parseAndExpect(0, 0, 0);
}
