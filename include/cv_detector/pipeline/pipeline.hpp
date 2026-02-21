#pragma once

#include <memory>
#include <opencv2/opencv.hpp>
#include "cv_detector/pipeline/i_preprocessor.hpp"
#include "cv_detector/pipeline/i_inference_engine.hpp"
#include "cv_detector/pipeline/i_postprocessor.hpp"

namespace rm_detector2026 {
namespace pipeline {

class Pipeline {
public:
    Pipeline(std::shared_ptr<IPreprocessor> pre, 
             std::shared_ptr<IInferenceEngine> engine, 
             std::shared_ptr<IPostprocessor> post);

    std::vector<Detection> run(const cv::Mat& img);

private:
    std::shared_ptr<IPreprocessor> preprocessor_;
    std::shared_ptr<IInferenceEngine> engine_;
    std::shared_ptr<IPostprocessor> postprocessor_;
};

} // namespace pipeline
} // namespace rm_detector2026
