#include <fstream>

#include "TraceFileParser.hpp"

TraceFileParser::TraceFileParser(const char* traceFileName) {
    this->traceFileName = traceFileName;

    allocations = 0;
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

    if (type == '+')
        ++allocations;
    else if (type == '-')
        --allocations;
}

char TraceFileParser::discoverAllocationType(std::string line) {
    size_t pos = line.find(']');

    if (pos == std::string::npos || (pos + 2) >= line.length())
        return '?';

    return line[pos + 2];
}

int TraceFileParser::getMemoryLeakCount() {
    return allocations;
}

int TraceFileParser::getMemoryLeakSize() {
    return allocations;
}

int TraceFileParser::getInvalidDeallocationCount() {
    return 0;
}
