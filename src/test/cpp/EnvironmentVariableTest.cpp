#include <gtest/gtest.h>

#include "EnvironmentVariable.hpp"

TEST(EnvironmentVariableTest, getName) {
    const char* name = "TEST_VAR";
    EnvironmentVariable var{name};

    EXPECT_STREQ(name, var.getName().c_str());
}

TEST(EnvironmentVariableTest, set) {
    const char* name = "TEST_VAR";
    const char* value = "test value";
    EnvironmentVariable var{name};

    unsetenv(name);

    var = value;

    EXPECT_STREQ(value, getenv(name));
}
