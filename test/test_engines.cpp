#include <gtest/gtest.h>
#include <opencv2/opencv.hpp>
#include "cv_detector/factories/engine_factory.hpp"

TEST(EngineTest, OpenVINOEngine) {
    // Dummy model path
    auto engine = rm_detector2026::factories::EngineFactory::create("openvino", "dummy_path");
    ASSERT_NE(engine, nullptr);

    cv::Mat input = cv::Mat::zeros(640, 640, CV_32FC3);
    auto outputs = engine->infer(input);
    
    EXPECT_FALSE(outputs.empty());
}
