#include <gtest/gtest.h>
#include "cv_detector/pipeline/i_inference_engine.hpp"
#include "cv_detector/factories/engine_factory.hpp"

TEST(EngineTest, BasicInitialization) {
    // Basic test to verify that the tests are running
    EXPECT_TRUE(true);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
