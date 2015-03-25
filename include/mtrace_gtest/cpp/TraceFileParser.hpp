#ifndef TRACE_FILE_PARSER_HPP
#define TRACE_FILE_PARSER_HPP

#include <string>

class TraceFileParser {
private:
    const char* traceFileName;
    int allocations;

public:
    TraceFileParser(const char* traceFileName);

    void parse();

    int getMemoryLeakCount();
    int getMemoryLeakSize();
    int getInvalidDeallocationCount();

private:
    void parseLine(std::string line);
    char discoverAllocationType(std::string line);
};

#endif
