#include <gtest/gtest.h>

#include "MallocTraceEnvVar.hpp"

TEST(MallocTraceEnvVarTest, getName) {
    MallocTraceEnvVar var;

    EXPECT_STREQ("MALLOC_TRACE", var.getName().c_str());
}
