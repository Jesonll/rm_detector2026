#include <gtest/gtest.h>
#include "cv_detector/factories/postprocessor_factory.hpp"

TEST(PostprocessorTest, NMSKeypoints) {
    auto postprocessor = rm_detector2026::factories::PostprocessorFactory::create("nms");
    ASSERT_NE(postprocessor, nullptr);

    std::vector<cv::Mat> dummy_outputs;
    auto detections = postprocessor->process(dummy_outputs);
    
    // Dummy implementation returns 1 detection
    EXPECT_EQ(detections.size(), 1);
}
