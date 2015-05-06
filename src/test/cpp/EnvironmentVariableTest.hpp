#ifndef ENVIRONMENT_VARIABLE_TEST_HPP
#define ENVIRONMENT_VARIABLE_TEST_HPP

#include <gtest/gtest.h>

#include "EnvironmentVariable.hpp"

class EnvironmentVariableTest : public testing::Test {
protected:
    const char* name;
    const char* value;
    EnvironmentVariable* variable;

public:
    void SetUp() {
        name = "TEST_VAR";
        value = "test value";
        variable = new EnvironmentVariable(name);

        unsetenv(name);
    }

    void TearDown() {
        destroy();
    }

    void destroy() {
        if (variable != NULL)
            delete variable;

        variable = NULL;
    }
};

#endif
