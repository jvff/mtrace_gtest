#ifndef TRACE_FILE_PARSER_HPP
#define TRACE_FILE_PARSER_HPP

class TraceFileParser {
public:
    TraceFileParser(const char* traceFileName);

    void parse();

    int getMemoryLeakCount();
    int getMemoryLeakSize();
    int getInvalidDeallocationCount();
};

#endif
