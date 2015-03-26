#ifndef MOCK_TRACE_FILE_PARSER_HPP
#define MOCK_TRACE_FILE_PARSER_HPP

#include "gmock/gmock.h"

#include "TraceFileParser.hpp"

class MockTraceFileParser : public TraceFileParser {
public:
    MockTraceFileParser(const char* traceFileName)
            : TraceFileParser(traceFileName) {
    }

    MOCK_METHOD0(parse, void());
    MOCK_METHOD0(getMemoryLeakCount, int());
    MOCK_METHOD0(getMemoryLeakSize, int());
    MOCK_METHOD0(getInvalidDeallocationCount, int());
};

#endif
