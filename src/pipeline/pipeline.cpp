#include "cv_detector/pipeline/pipeline.hpp"

namespace rm_detector2026 {
namespace pipeline {

Pipeline::Pipeline(std::shared_ptr<IPreprocessor> pre, 
                   std::shared_ptr<IInferenceEngine> engine, 
                   std::shared_ptr<IDecoder> decoder,
                   std::shared_ptr<IPostprocessor> post)
    : preprocessor_(pre), engine_(engine), decoder_(decoder), postprocessor_(post) {}

std::vector<Armor> Pipeline::run(const cv::Mat& img) {
    if (img.empty()) {
        return {};
    }
    
    // Create detection context
    DetectionContext ctx;
    ctx.original_image = img;
    
    // 1. Preprocess
    preprocessor_->preprocess(ctx);
    
    // 2. Inference
    engine_->infer(ctx);
    
    // 3. Decode
    decoder_->decode(ctx);

    // 4. Postprocess
    postprocessor_->process(ctx);
    
    return ctx.armors;
}

} // namespace pipeline
} // namespace rm_detector2026
