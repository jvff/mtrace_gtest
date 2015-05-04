#include <errno.h>
#include <fstream>
#include <mcheck.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <gtest/gtest.h>

#include "MemoryTracer.hpp"

TEST(MemoryTracerTest, startTrace) {
    const std::string envVarName = "MALLOC_TRACE";
    const std::string mtraceFile = "/tmp/MemoryTracerTest_startTrace.mtrace";
    const std::string constEnvironmentString = envVarName + "=" + mtraceFile;
    char* environmentString = strdup(constEnvironmentString.c_str());
    struct stat info;

    MemoryTracer tracer;

    putenv(environmentString);

    tracer.start();
    muntrace();

    EXPECT_EQ(0, stat(mtraceFile.c_str(), &info));
    EXPECT_TRUE(S_ISREG(info.st_mode));

    unlink(mtraceFile.c_str());
    unsetenv(envVarName.c_str());
    free(environmentString);
}

TEST(MemoryTracerTest, stopTrace) {
    const std::string envVarName = "MALLOC_TRACE";
    const std::string mtraceFile = "/tmp/MemoryTracerTest_stopTrace.mtrace";
    const std::string constEnvironmentString = envVarName + "=" + mtraceFile;
    char* environmentString = strdup(constEnvironmentString.c_str());
    struct stat info;

    MemoryTracer tracer;

    putenv(environmentString);

    mtrace();
    tracer.stop();

    EXPECT_EQ(0, stat(mtraceFile.c_str(), &info));
    EXPECT_TRUE(S_ISREG(info.st_mode));

    std::ifstream traceFile(mtraceFile);
    std::string line;
    std::string lastLine;

    while (std::getline(traceFile, line))
        lastLine = line;

    EXPECT_STREQ("= End", lastLine.c_str());

    unlink(mtraceFile.c_str());
    unsetenv(envVarName.c_str());
    free(environmentString);
}
