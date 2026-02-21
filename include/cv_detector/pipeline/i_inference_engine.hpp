#pragma once

#include <opencv2/opencv.hpp>
#include <vector>

namespace rm_detector2026 {
namespace pipeline {

/**
 * @brief Abstract base class for inference engine
 */
class IInferenceEngine {
public:
    virtual ~IInferenceEngine() = default;

    /**
     * @brief Run inference
     * @param input Preprocessed input tensor (blob)
     * @return Inference output results (could be raw tensor data or semi-processed)
     */
    virtual std::vector<cv::Mat> infer(const cv::Mat& input) = 0;
};

} // namespace pipeline
} // namespace rm_detector2026
