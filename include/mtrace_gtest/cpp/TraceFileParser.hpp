#ifndef TRACE_FILE_PARSER_HPP
#define TRACE_FILE_PARSER_HPP

#include <map>
#include <string>

class TraceFileParser {
private:
    int invalidDeallocations;
    std::string traceFileName;
    std::map<void*, int> activeAllocations;

public:
    TraceFileParser(const std::string& traceFileName);

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
