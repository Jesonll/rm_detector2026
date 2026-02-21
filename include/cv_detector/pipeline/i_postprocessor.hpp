#pragma once

#include <opencv2/opencv.hpp>
#include <vector>

namespace rm_detector2026 {
namespace pipeline {

struct Detection {
    int class_id;
    float confidence;
    cv::Rect box;
};

/**
 * @brief Abstract base class for postprocessing
 */
class IPostprocessor {
public:
    virtual ~IPostprocessor() = default;

    /**
     * @brief Process inference results into detections
     * @param outputs Raw output from inference engine
     * @return Vector of detections
     */
    virtual std::vector<Detection> process(const std::vector<cv::Mat>& outputs) = 0;
};

} // namespace pipeline
} // namespace rm_detector2026
