#ifndef FAKE_MEMORY_TRACER_LISTENER_HPP
#define FAKE_MEMORY_TRACER_LISTENER_HPP

#include "MemoryTracerListener.hpp"

class FakeMemoryTracerListener : public MemoryTracerListener {
public:
    TraceFileParser* getParser() {
        return traceFileParser;
    }
};

#endif
