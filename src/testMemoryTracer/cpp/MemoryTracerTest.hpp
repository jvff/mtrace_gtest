#ifndef MEMORY_TRACE_TEST_HPP
#define MEMORY_TRACE_TEST_HPP

#include <errno.h>
#include <fstream>
#include <mcheck.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <gtest/gtest.h>

#include "MemoryTracer.hpp"

class MemoryTracerTest : public testing::Test {
private:
    char* environmentString;
    std::string envVarName;
    std::string mtraceFileName;

protected:
    MemoryTracer memoryTracer;

protected:
    void SetUp() {
        setUpMtraceFileName();
        setUpEnvironmentVariable();
    }

    void TearDown() {
        tearDownEnvironmentVariable();
        deleteMtraceFile();
    }

    void checkThatMtraceFileIsValid() {
        checkThatMtraceFileExists();
        checkThatMtraceFileIsComplete();
    }

    void checkThatMtraceFileExists() {
        struct stat info;

        EXPECT_EQ(0, stat(mtraceFileName.c_str(), &info));
        EXPECT_TRUE(S_ISREG(info.st_mode));
    }

    void checkThatMtraceFileIsComplete() {
        std::ifstream mtraceFile(mtraceFileName);
        std::string line;
        std::string lastLine;

        while (std::getline(mtraceFile, line))
            lastLine = line;

        EXPECT_STREQ("= End", lastLine.c_str());
    }

private:
    void setUpMtraceFileName() {
        const testing::UnitTest* unitTest = testing::UnitTest::GetInstance();
        const testing::TestInfo* testInfo = unitTest->current_test_info();
        const std::string testCaseName = testInfo->name();
        const std::string testName = testInfo->name();
        const std::string fileSuffix = ".mtrace";

        mtraceFileName = testCaseName + "_" + testName + fileSuffix;
    }

    void setUpEnvironmentVariable() {
        envVarName = "MALLOC_TRACE";
        const std::string envStr = envVarName + "=" + mtraceFileName;

        environmentString = strdup(envStr.c_str());

        putenv(environmentString);
    }

    void tearDownEnvironmentVariable() {
        unsetenv(envVarName.c_str());
        free(environmentString);
    }

    void deleteMtraceFile() {
        unlink(mtraceFileName.c_str());
    }
};

#endif
