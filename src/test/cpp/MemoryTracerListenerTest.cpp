#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "gtest/gtest.h"

#include "MemoryTracerListener.hpp"

#define ENV_VAR "MALLOC_TRACE"

TEST(MemoryTracerListenerTest, setEnvironmentTest) {
    MemoryTracerListener* listener;

    unsetenv(ENV_VAR);
    listener = new MemoryTracerListener();

    EXPECT_FALSE(getenv(ENV_VAR) == NULL);

    delete listener;
}

TEST(MemoryTracerListenerTest, traceFileIsInTempDir) {
    MemoryTracerListener listener;

    EXPECT_TRUE(memcmp("/tmp/", getenv(ENV_VAR), 5) == 0);
}

TEST(MemoryTracerListenerTest, traceFileDoesntExistYet) {
    MemoryTracerListener listener;
    struct stat info;

    EXPECT_NE(stat(getenv(ENV_VAR), &info), 0);
    EXPECT_EQ(errno, ENOENT);
}

TEST(MemoryTracerListenerTest, traceFileNameIsFixed) {
    MemoryTracerListener listener;

    const char* filePath = getenv(ENV_VAR);
    const char* expectedFileName = "/mtrace";
    int fileNameLength = strlen(expectedFileName);
    int expectedFileNamePos = strlen(filePath) - fileNameLength;

    ASSERT_GT(expectedFileNamePos, 0);

    const char* fileName = &filePath[expectedFileNamePos];

    EXPECT_TRUE(strncmp(expectedFileName, fileName, fileNameLength) == 0);
}

TEST(MemoryTracerListenerTest, traceFileIsInNewTempDir) {
    MemoryTracerListener listener;

    const char* filePath = getenv(ENV_VAR);
    const char* expectedDirAndFileName = "/mtrace_gtest.XXXXXX/mtrace";
    int dirPrefixLength = strlen("/mtrace_gtest.");
    int fileAndDirNameLength = strlen(expectedDirAndFileName);
    int expectedDirNamePos = strlen(filePath) - fileAndDirNameLength;

    ASSERT_GT(expectedDirNamePos, 0);

    const char* dirName = &filePath[expectedDirNamePos];

    EXPECT_TRUE(strncmp(expectedDirAndFileName, dirName, dirPrefixLength) == 0);
}

TEST(MemoryTracerListenerTest, tempDirExists) {
    MemoryTracerListener listener;
    struct stat info;

    const char* filePath = getenv(ENV_VAR);
    char* dirPath = strdup(filePath);
    char* lastPathSeparator = strrchr(dirPath, '/');

    *lastPathSeparator = 0;

    EXPECT_EQ(stat(dirPath, &info), 0);
    EXPECT_TRUE(S_ISDIR(info.st_mode));

    free(dirPath);
}

TEST(MemoryTracerListenerTest, tempDirIsDeletedAfterDestruction) {
    struct stat info;

    MemoryTracerListener* listener = new MemoryTracerListener();

    const char* filePath = getenv(ENV_VAR);
    char* dirPath = strdup(filePath);
    char* lastPathSeparator = strrchr(dirPath, '/');

    *lastPathSeparator = 0;

    delete listener;

    EXPECT_NE(stat(dirPath, &info), 0);
    EXPECT_EQ(errno, ENOENT);

    free(dirPath);
}

TEST(MemoryTracerListenerTest, implementsTestListener) {
    testing::TestEventListener* listener = new MemoryTracerListener();
    delete listener;
}
