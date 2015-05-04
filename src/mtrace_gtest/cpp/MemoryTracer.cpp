#include <mcheck.h>

#include "MemoryTracer.hpp"

void MemoryTracer::start() {
    mtrace();
}

void MemoryTracer::stop() {
    muntrace();
}
