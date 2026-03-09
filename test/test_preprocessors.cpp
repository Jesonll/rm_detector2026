#include <gtest/gtest.h>
#include "cv_detector/pipeline/i_preprocessor.hpp"
#include "cv_detector/factories/preprocessor_factory.hpp"

TEST(PreprocessorTest, BasicInitialization) {
    // Basic test to verify that the tests are running
    EXPECT_TRUE(true);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
