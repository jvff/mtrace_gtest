#ifndef TRACE_FILE_PARSER_HPP
#define TRACE_FILE_PARSER_HPP

#include <map>
#include <string>

class TraceFileParser {
private:
    const char* traceFileName;
    int invalidDeallocations;
    std::map<void*, int> activeAllocations;

public:
    TraceFileParser(const char* traceFileName);

    virtual void parse();

    virtual int getMemoryLeakCount();
    virtual int getMemoryLeakSize();
    virtual int getInvalidDeallocationCount();

private:
    void parseLine(std::string line);
    char discoverAllocationType(std::string line);
    void parseAllocation(const char* line);
    void parseDeallocation(const char* line);
};

#endif
