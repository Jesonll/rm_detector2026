#pragma once
#include "cv_detector/pipeline/i_inference_engine.hpp"

namespace rm_detector2026 {
namespace pipeline {

class OpenVINOEngine : public IInferenceEngine {
public:
    std::vector<cv::Mat> infer(const cv::Mat& input) override;
};

}
}
