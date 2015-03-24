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

TEST(MemoryTracerListenerTest, traceFileHasPrefix) {
    MemoryTracerListener listener;
    const char* fileName = getenv(ENV_VAR);
    const char* expectedPrefix = "/mtrace.";
    int prefixLength = strlen(expectedPrefix);
    int wildcardLength = 6;
    int expectedPrefixPos = strlen(fileName) - wildcardLength - prefixLength;

    ASSERT_GT(expectedPrefixPos, 0);

    const char* prefix = &fileName[expectedPrefixPos];

    EXPECT_TRUE(strncmp(expectedPrefix, prefix, 5) == 0);
}
