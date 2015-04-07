#include <stdlib.h>

#include <gtest/gtest.h>

static void* address;

TEST(SampleTests, pass) {
    EXPECT_EQ(0, 0);
}

TEST(SampleTests, memoryLeak) {
    address = malloc(1000);
}

TEST(SampleTests, invalidFree) {
    free(address);
}
