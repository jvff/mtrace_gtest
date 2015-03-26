#ifndef TRACE_FILE_PARSER_TEST_HPP
#define TRACE_FILE_PARSER_TEST_HPP

#include <stdio.h>
#include <unistd.h>

#include "gtest/gtest.h"

#include "TraceFileParser.hpp"

class TraceFileParserTest : public testing::Test {
private:
    const char* currentAllocation;
    char* traceFileName;
    int traceFileDescriptor;

protected:
    FILE* traceFile;
    TraceFileParser* parser;

    virtual void SetUp() {
        traceFileName = strdup("/tmp/fake_mtrace.XXXXXX");
        traceFileDescriptor = mkstemp(traceFileName);
        traceFile = fdopen(traceFileDescriptor, "w+");

        currentAllocation = (const char*)0x10000000;

        parser = new TraceFileParser(traceFileName);
    }

    virtual void TearDown() {
        if (traceFile != NULL)
            closeTraceFile();

        delete parser;

        unlink(traceFileName);
        free(traceFileName);
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

#endif