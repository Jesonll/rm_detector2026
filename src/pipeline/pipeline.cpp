#include "cv_detector/pipeline/pipeline.hpp"

namespace rm_detector2026 {
namespace pipeline {

Pipeline::Pipeline(std::shared_ptr<IPreprocessor> pre, 
                   std::shared_ptr<IInferenceEngine> engine, 
                   std::shared_ptr<IPostprocessor> post)
    : preprocessor_(pre), engine_(engine), postprocessor_(post) {}

std::vector<Detection> Pipeline::run(const cv::Mat& img) {
    if (img.empty()) {
        return {};
    }
    // 1. Preprocess
    auto input_blob = preprocessor_->preprocess(img);
    
    // 2. Inference
    auto outputs = engine_->infer(input_blob);
    
    // 3. Postprocess
    auto detections = postprocessor_->process(outputs);
    
    return detections;
}

} // namespace pipeline
} // namespace rm_detector2026
