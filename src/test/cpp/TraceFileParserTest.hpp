#ifndef TRACE_FILE_PARSER_TEST_HPP
#define TRACE_FILE_PARSER_TEST_HPP

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include "gtest/gtest.h"

#include "TraceFileParser.hpp"

class TraceFileParserTest : public testing::TestWithParam<const char*> {
private:
    const char* currentAllocation;
    char* traceFileName;
    FILE* traceFile;
    int traceFileDescriptor;

protected:
    TraceFileParser* parser;

    virtual void SetUp() {
        traceFileName = strdup("/tmp/fake_mtrace.XXXXXX");
        traceFileDescriptor = mkstemp(traceFileName);
        traceFile = fdopen(traceFileDescriptor, "w+");

        fprintf(traceFile, "= Start\n");

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
        const char* debugInfo = GetParam();
        const void* address = (const void*)currentAllocation;

        currentAllocation += size;

        fprintf(traceFile, "@ %s[0x0] + %p %#x\n", debugInfo, address, size);

        return address;
    }

    virtual void dealloc(const void* address) {
        const char* debugInfo = GetParam();

        fprintf(traceFile, "@ %s[0x0] - %p\n", debugInfo, address);
    }

    virtual void closeTraceFile() {
        fprintf(traceFile, "= End\n");
        fclose(traceFile);
        close(traceFileDescriptor);
        traceFile = NULL;
    }

    virtual void resetTraceFile() {
        if (traceFile != NULL)
            closeTraceFile();

        traceFileDescriptor = open(traceFileName, O_RDWR | O_TRUNC);
        traceFile = fdopen(traceFileDescriptor, "w+");

        fprintf(traceFile, "= Start");
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
