#include <stdlib.h>

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
