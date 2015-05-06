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

TEST(TempDirTest, tempDirExists) {
    TempDir dir;
    struct stat info;

    EXPECT_EQ(stat(dir.getPath().c_str(), &info), 0);
    EXPECT_TRUE(S_ISDIR(info.st_mode));
}

TEST(TempDirTest, tempDirWithPrefixExists) {
    TempDir dir{"tempdir_prefix_"};
    struct stat info;

    EXPECT_EQ(stat(dir.getPath().c_str(), &info), 0);
    EXPECT_TRUE(S_ISDIR(info.st_mode));
}

TEST(TempDirTest, twoTempDirsDontHaveTheSamePath) {
    TempDir firstDir, secondDir;

    EXPECT_NE(firstDir.getPath(), secondDir.getPath());
}

TEST(TempDirTest, twoTempDirsWithTheSamePrefixDontHaveTheSamePath) {
    TempDir firstDir{"tempdir_prefix_"};
    TempDir secondDir{"tempdir_prefix_"};

    EXPECT_NE(firstDir.getPath(), secondDir.getPath());
}
