#include "openvino_engine.hpp"

namespace rm_detector2026 {
namespace pipeline {

std::vector<cv::Mat> OpenVINOEngine::infer(const cv::Mat& input) {
    // Dummy inference
    (void)input; // unused
    return {cv::Mat::zeros(100, 6, CV_32F)}; // Dummy output
}

}
}
