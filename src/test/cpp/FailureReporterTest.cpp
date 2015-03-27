#include <sstream>

#include "gtest/gtest.h"

static std::stringstream* failStream;

#ifdef FAIL
#undef FAIL
#endif

#define FAIL() *failStream << ""

#include "FailureReporter.hpp"

TEST(FailureReporterTest, failMethod) {
    const char* errorMessage = "Test error message";
    FailureReporter *reporter;
    std::stringstream stream;

    failStream = &stream;

    reporter = new FailureReporter();

    reporter->fail(errorMessage);

    EXPECT_STREQ(errorMessage, stream.str().c_str());

    delete reporter;
}
