#include <gtest/gtest.h>

#include "EnvironmentVariable.hpp"

TEST(EnvironmentVariableTest, getName) {
    const char* name = "TEST_VAR";
    EnvironmentVariable var{name};

    EXPECT_STREQ(name, var.getName().c_str());
}
