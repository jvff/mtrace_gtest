#ifndef MOCK_FAILURE_REPORTER_HPP
#define MOCK_FAILURE_REPORTER_HPP

#include "gmock/gmock.h"

#include "FailureReporter.hpp"

class MockFailureReporter : public FailureReporter {
public:
    MOCK_METHOD1(fail, void(std::string));
};

#endif
