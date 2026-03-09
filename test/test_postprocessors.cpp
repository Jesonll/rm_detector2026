#include <gtest/gtest.h>
#include "cv_detector/pipeline/i_postprocessor.hpp"
#include "cv_detector/factories/postprocessor_factory.hpp"

TEST(PostprocessorTest, BasicInitialization) {
    // Basic test to verify that the tests are running
    EXPECT_TRUE(true);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
