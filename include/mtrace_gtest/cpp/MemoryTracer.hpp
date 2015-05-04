#ifndef MEMORY_TRACER_HPP
#define MEMORY_TRACER_HPP

#include <mcheck.h>

class MemoryTracer {
public:
    void start() {
	mtrace();
    }
};

#endif
