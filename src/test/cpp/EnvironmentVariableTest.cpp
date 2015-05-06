#include "EnvironmentVariableTest.hpp"

TEST_F(EnvironmentVariableTest, getName) {
    EXPECT_STREQ(name, variable->getName().c_str());
}

TEST_F(EnvironmentVariableTest, set) {
    *variable = value;

    EXPECT_STREQ(value, getenv(name));
}

TEST_F(EnvironmentVariableTest, unsetInDestructor) {
    *variable = value;

    destroy();

    EXPECT_TRUE(getenv(name) == NULL);
}
