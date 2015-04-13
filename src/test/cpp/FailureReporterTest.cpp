#include <sstream>

#include "gtest/gtest.h"

#include "FailureReporter.hpp"

TEST(FailureReporterTest, classExists) {
    FailureReporter *reporter;

    reporter = new FailureReporter();

    EXPECT_TRUE(reporter != NULL);

    delete reporter;
}
