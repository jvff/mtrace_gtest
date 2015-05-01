#ifndef FAILURE_REPORTER_TEMPLATE_H
#define FAILURE_REPORTER_TEMPLATE_H

#include <string>

#include "gtest/gtest.h"

#define DECLARE_FAILURE_REPORTER_CLASS(className) \
class className { \
public: \
    virtual void fail(std::string errorMessage) { \
        FAIL() << errorMessage; \
    } \
};

#endif
