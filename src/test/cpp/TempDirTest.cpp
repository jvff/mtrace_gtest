#include <gtest/gtest.h>

#include "TempDir.hpp"

TEST(TempDirTest, getName) {
    const char* tempDir = "/tmp/";
    const int length = strlen(tempDir);
    TempDir dir;

    EXPECT_TRUE(strncmp(tempDir, dir.getPath().c_str(), length) == 0);
}

TEST(TempDirTest, customDirPrefix) {
    const char* dirPrefix = "mtrace_gtest.";
    const int length = strlen(dirPrefix);
    TempDir dir{dirPrefix};
    const std::string path = dir.getPath();
    const std::string dirName = path.substr(path.find_last_of('/') + 1);

    EXPECT_TRUE(strncmp(dirPrefix, dirName.c_str(), length) == 0);
}
