#include <stdlib.h>

#include "MemoryTracerListener.hpp"

#define ENVIRONMENT_VARIABLE "MALLOC_TRACE"

MemoryTracerListener::MemoryTracerListener() {
    setenv(ENVIRONMENT_VARIABLE, "", 0);
}
