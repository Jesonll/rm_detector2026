#include "nms_postprocessor.hpp"

namespace rm_detector2026 {
namespace pipeline {

std::vector<Detection> NMSPostprocessor::process(const std::vector<cv::Mat>& outputs) {
    // Dummy NMS
    (void)outputs;
    std::vector<Detection> dets;
    dets.push_back({1, 0.95f, cv::Rect(10, 10, 50, 50)});
    return dets;
}

}
}
