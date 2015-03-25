#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "gtest/gtest.h"

#include "MemoryTracerListener.hpp"

class MemoryTracerListenerTest : public testing::Test {
private:
    const char* environmentVariable;
    char* dirPath;

protected:
    MemoryTracerListener* listener;

    virtual void SetUp() {
        dirPath = NULL;

        environmentVariable = "MALLOC_TRACE";
        unsetenv(environmentVariable);

        listener = new MemoryTracerListener();
    }

    virtual void TearDown() {
        if (listener != NULL)
            destroy();

        if (dirPath != NULL)
            free(dirPath);
    }

    void destroy() {
        delete listener;
        listener = NULL;
    }

    const char* getEnvironmentVariable() {
        return getenv(environmentVariable);
    }

    const char* getFilePath() {
        return getEnvironmentVariable();
    }

    const char* getFileName() {
        const char* filePath = getFilePath();
        const char* fileName = strrchr(filePath, '/');

        return fileName == NULL ? filePath : fileName + 1;
    }

    const char* getDirPath() {
        if (dirPath == NULL) {
            const char* filePath = getFilePath();
            const char* fileName = strrchr(filePath, '/');

            if (fileName == NULL)
                return NULL;

            dirPath = strdup(filePath);
            dirPath[fileName - filePath] = '\0';
        }

        return dirPath;
    }

    const char* getDirName() {
        const char* dirPath = getDirPath();

        if (dirPath == NULL)
            return NULL;

        const char* dirName = strrchr(dirPath, '/');

        return dirName == NULL ? dirPath : dirName + 1;
    }
};

TEST_F(MemoryTracerListenerTest, setEnvironmentTest) {
    EXPECT_FALSE(getEnvironmentVariable() == NULL);
}

TEST_F(MemoryTracerListenerTest, traceFileIsInTempDir) {
    const char* tempDir = "/tmp/";
    const int length = strlen(tempDir);

    EXPECT_TRUE(strncmp(tempDir, getFilePath(), length) == 0);
}

TEST_F(MemoryTracerListenerTest, traceFileDoesntExistYet) {
    struct stat info;

    EXPECT_NE(stat(getFilePath(), &info), 0);
    EXPECT_EQ(errno, ENOENT);
}

TEST_F(MemoryTracerListenerTest, traceFileNameIsFixed) {
    EXPECT_STREQ("mtrace", getFileName());
}

TEST_F(MemoryTracerListenerTest, traceFileIsInNewTempDir) {
    const char* dirName = getDirName();
    char* dirPrefix = strdup(dirName);
    const int dirNameLength = strlen(dirName);
    const int wildcardLength = 6;

    ASSERT_GT(dirNameLength, wildcardLength);

    dirPrefix[strlen(dirName) - wildcardLength] = '\0';

    EXPECT_STREQ("mtrace_gtest.", dirPrefix);

    free(dirPrefix);
}

TEST_F(MemoryTracerListenerTest, tempDirExists) {
    struct stat info;

    EXPECT_EQ(stat(getDirPath(), &info), 0);
    EXPECT_TRUE(S_ISDIR(info.st_mode));
}

TEST_F(MemoryTracerListenerTest, tempDirIsDeletedAfterDestruction) {
    struct stat info;

    destroy();

    EXPECT_NE(stat(getDirPath(), &info), 0);
    EXPECT_EQ(errno, ENOENT);
}

TEST_F(MemoryTracerListenerTest, implementsTestListener) {
    testing::TestEventListener* superClass = listener;
}

TEST_F(MemoryTracerListenerTest, traceFileIsCreatedWhenTestStarts) {
    const testing::UnitTest* unitTest = testing::UnitTest::GetInstance();
    const testing::TestInfo* testInfo = unitTest->current_test_info();
    struct stat info;

    listener->OnTestStart(*testInfo);

    EXPECT_EQ(stat(getFilePath(), &info), 0);
    EXPECT_TRUE(S_ISREG(info.st_mode));
}
