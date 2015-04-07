#include <gtest/gtest.h>

#include "MemoryTracerListener.hpp"

using namespace testing;

GTEST_API_ int main(int argc, char** argv) {
    InitGoogleTest(&argc, argv);
    UnitTest::GetInstance()->listeners().Append(new MemoryTracerListener());

    return RUN_ALL_TESTS();
}
