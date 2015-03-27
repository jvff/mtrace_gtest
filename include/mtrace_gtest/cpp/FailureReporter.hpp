#ifndef FAILURE_REPORTER_H
#define FAILURE_REPORTER_H

#include "gtest/gtest.h"

class FailureReporter {
public:
    void fail(const char* errorMessage) {
        FAIL() << errorMessage;
    }
};

#endif
