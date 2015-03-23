#include <stdlib.h>

#include "gtest/gtest.h"

#include "MemoryTracerListener.hpp"

TEST(MemoryTracerListenerTest, setEnvironmentTest) {
    const char environmentVariable[] = "MALLOC_TRACE";

    MemoryTracerListener* listener;

    unsetenv(environmentVariable);
    listener = new MemoryTracerListener();

    EXPECT_FALSE(getenv(environmentVariable) == NULL);

    delete listener;
}
