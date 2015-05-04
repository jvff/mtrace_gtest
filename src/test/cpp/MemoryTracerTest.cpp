#include <gtest/gtest.h>

#include "MemoryTracer.hpp"

TEST(MemoryTracerTest, classExists) {
    MemoryTracer* tracer = new MemoryTracer();

    EXPECT_TRUE(tracer != NULL);

    delete tracer;
}
