#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "MemoryTracerListener.hpp"

#define ENVIRONMENT_VARIABLE "MALLOC_TRACE"
#define DIRNAME_TEMPLATE "/tmp/mtrace_gtest.XXXXXX"

MemoryTracerListener::MemoryTracerListener() {
    char* dirName = strdup(DIRNAME_TEMPLATE);

    mtraceFileName = strdup(DIRNAME_TEMPLATE "/mtrace");

    if (mkdtemp(dirName) != NULL && dirName[0] != 0) {
        memcpy(mtraceFileName, dirName, strlen(dirName));
        setenv(ENVIRONMENT_VARIABLE, mtraceFileName, 0);
    }

    free(dirName);
}
