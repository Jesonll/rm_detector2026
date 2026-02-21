#include <gtest/gtest.h>
#include <opencv2/opencv.hpp>
#include "cv_detector/factories/preprocessor_factory.hpp"

TEST(PreprocessorTest, LetterboxResize) {
    auto preprocessor = rm_detector2026::factories::PreprocessorFactory::create("letterbox");
    ASSERT_NE(preprocessor, nullptr);

    cv::Mat img = cv::Mat::zeros(100, 200, CV_8UC3);
    auto result = preprocessor->preprocess(img);
    
    // Check if result is not empty and has target size
    EXPECT_FALSE(result.empty());
    EXPECT_EQ(result.cols, 640);
    EXPECT_EQ(result.rows, 640);
}
