#include "cv_detector/pipeline/pipeline.hpp"
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>

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
    
    return run(ctx);
}

std::vector<Armor> Pipeline::run(DetectionContext& ctx) {
    if (ctx.original_image.empty()) {
        return {};
    }
    
    // 1. Preprocess
    preprocessor_->preprocess(ctx);
    
    // 2. Inference
    engine_->infer(ctx);
    
    // 3. Decode
    decoder_->decode(ctx);

    // 4. Postprocess
    postprocessor_->process(ctx);

    save_context(ctx, "debug/context");
    
    return ctx.results;
}

void Pipeline::save_context(const DetectionContext& ctx, const std::string& path_prefix) {
    if (!ctx.original_image.empty()) {
        cv::imwrite(path_prefix + "_original.png", ctx.original_image);
    }
    if (!ctx.network_input.empty()) {
        cv::imwrite(path_prefix + "_network_input.png", ctx.network_input);
    }
    
    cv::FileStorage fs(path_prefix + "_data.yaml", cv::FileStorage::WRITE);
    fs << "resize_ratio" << ctx.resize_ratio;
    fs << "dw" << ctx.dw;
    fs << "dh" << ctx.dh;
    
    if (!ctx.inference_outputs.empty()) {
        fs << "inference_outputs" << ctx.inference_outputs;
    }
    
    fs << "armors" << "[";
    for (const auto& armor : ctx.armors) {
        fs << "{";
        fs << "type" << armor.type;
        fs << "color" << armor.color;
        fs << "size" << armor.size;
        fs << "conf" << armor.conf;
        fs << "rect" << "[" << armor.rect.x << armor.rect.y << armor.rect.width << armor.rect.height << "]";
        fs << "pts" << "[";
        for (int i = 0; i < 5; ++i) {
            fs << "[" << armor.pts[i].x << armor.pts[i].y << "]";
        }
        fs << "]";
        fs << "}";
    }
    fs << "]";

    fs << "results" << "[";
    for (const auto& armor : ctx.results) {
        fs << "{";
        fs << "type" << armor.type;
        fs << "color" << armor.color;
        fs << "size" << armor.size;
        fs << "conf" << armor.conf;
        fs << "rect" << "[" << armor.rect.x << armor.rect.y << armor.rect.width << armor.rect.height << "]";
        fs << "pts" << "[";
        for (int i = 0; i < 5; ++i) {
            fs << "[" << armor.pts[i].x << armor.pts[i].y << "]";
        }
        fs << "]";
        fs << "}";
    }
    fs << "]";
    fs.release();
}

} // namespace pipeline
} // namespace rm_detector2026
