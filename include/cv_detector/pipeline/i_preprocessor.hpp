#pragma once

#include <opencv2/opencv.hpp>
#include <string>

namespace rm_detector2026 {
namespace pipeline {

/**
 * @brief Abstract base class for image preprocessing
 */
class IPreprocessor {
public:
    virtual ~IPreprocessor() = default;
    
    /**
     * @brief Preprocess methods
     * @param img Input image (HWC)
     * @return Preprocessed image and necessary scaling info
     */
    virtual cv::Mat preprocess(const cv::Mat& img) = 0;
};

} // namespace pipeline
} // namespace rm_detector2026
