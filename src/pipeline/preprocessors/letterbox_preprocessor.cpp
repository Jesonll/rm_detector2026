#include "letterbox_preprocessor.hpp"

namespace rm_detector2026 {
namespace pipeline {

cv::Mat LetterboxPreprocessor::preprocess(const cv::Mat& img) {
    // Dummy implementation of letterbox resize
    cv::Mat resized;
    cv::resize(img, resized, cv::Size(640, 640)); 
    return resized;
}

}
}
