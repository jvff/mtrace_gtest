#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "MemoryTracerListener.hpp"

#define ENVIRONMENT_VARIABLE "MALLOC_TRACE"

MemoryTracerListener::MemoryTracerListener() {
    mtraceFileName = strdup("/tmp/mtrace.XXXXXX");

    if (mktemp(mtraceFileName) != NULL && mtraceFileName[0] != 0)
        setenv(ENVIRONMENT_VARIABLE, mtraceFileName, 0);
}
