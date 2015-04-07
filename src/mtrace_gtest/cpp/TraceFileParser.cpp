#include <fstream>

#include "TraceFileParser.hpp"

TraceFileParser::TraceFileParser(const char* traceFileName) {
    this->traceFileName = traceFileName;

    invalidDeallocations = 0;
}

void TraceFileParser::parse() {
    std::ifstream traceFile(traceFileName);
    std::string line;
    void *address;
    char type;

    while (std::getline(traceFile, line))
        parseLine(line);
}

void TraceFileParser::parseLine(std::string line) {
    char type = discoverAllocationType(line);
    const char* lineString = line.c_str();

    if (type == '+')
        parseAllocation(lineString);
    else if (type == '-')
        parseDeallocation(lineString);
}

char TraceFileParser::discoverAllocationType(std::string line) {
    size_t pos = line.find(']');

    if (pos == std::string::npos || (pos + 2) >= line.length())
        return '?';

    return line[pos + 2];
}

void TraceFileParser::parseAllocation(const char* line) {
    void* position;
    void* address;
    int size;

    sscanf(line, "@%*[^[][%p] + %p %x", &position, &address, &size);

    activeAllocations[address] = size;
}

void TraceFileParser::parseDeallocation(const char* line) {
    void* position;
    void* address;

    sscanf(line, "@%*[^[][%p] - %p", &position, &address);

    if (activeAllocations.erase(address) == 0)
        ++invalidDeallocations;
}

int TraceFileParser::getMemoryLeakCount() {
    return activeAllocations.size();
}

int TraceFileParser::getMemoryLeakSize() {
    if (activeAllocations.size() == 0)
        return 0;

    int size = 0;
    std::map<void*, int>::iterator iterator = activeAllocations.begin();
    std::map<void*, int>::iterator end = activeAllocations.end();

    for (; iterator != end; ++iterator)
        size += iterator->second;

    return size;
}

int TraceFileParser::getInvalidDeallocationCount() {
    return invalidDeallocations;
}
