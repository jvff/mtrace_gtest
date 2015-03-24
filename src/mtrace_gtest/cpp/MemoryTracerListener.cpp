#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "MemoryTracerListener.hpp"

#define ENVIRONMENT_VARIABLE "MALLOC_TRACE"
#define DIRNAME_TEMPLATE "/tmp/mtrace_gtest.XXXXXX"

MemoryTracerListener::MemoryTracerListener() {
    mtraceDirName = strdup(DIRNAME_TEMPLATE);
    mtraceFileName = strdup(DIRNAME_TEMPLATE "/mtrace");

    if (mkdtemp(mtraceDirName) != NULL && mtraceDirName[0] != 0) {
        memcpy(mtraceFileName, mtraceDirName, strlen(mtraceDirName));
        setenv(ENVIRONMENT_VARIABLE, mtraceFileName, 1);
    }
}

MemoryTracerListener::~MemoryTracerListener() {
    unlink(mtraceFileName);
    rmdir(mtraceDirName);
}
