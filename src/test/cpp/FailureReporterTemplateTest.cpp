#include <sstream>

#include "gtest/gtest.h"

static std::stringstream* failStream;

#ifdef FAIL
#undef FAIL
#endif

#define FAIL() *failStream << ""

#include "FailureReporterTemplate.hpp"

DECLARE_FAILURE_REPORTER_CLASS(FakeFailureReporter);

TEST(FailureReporterTemplateTest, failMethod) {
    const char* errorMessage = "Test error message";
    FakeFailureReporter *reporter;
    std::stringstream stream;

    failStream = &stream;

    reporter = new FakeFailureReporter();

    reporter->fail(errorMessage);

    EXPECT_STREQ(errorMessage, stream.str().c_str());

    delete reporter;
}
