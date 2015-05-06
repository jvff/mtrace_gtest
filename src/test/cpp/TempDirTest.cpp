#include <gtest/gtest.h>

#include "TempDir.hpp"

TEST(TempDirTest, getName) {
    const char* tempDir = "/tmp/";
    const int length = strlen(tempDir);
    TempDir dir;

    EXPECT_TRUE(strncmp(tempDir, dir.getPath().c_str(), length) == 0);
}
