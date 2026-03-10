#pragma once

#include <memory>
#include <opencv2/opencv.hpp>
#include "cv_detector/pipeline/types.hpp"
#include "cv_detector/pipeline/i_preprocessor.hpp"
#include "cv_detector/pipeline/i_inference_engine.hpp"
#include "cv_detector/pipeline/i_decoder.hpp"
#include "cv_detector/pipeline/i_postprocessor.hpp"

namespace rm_detector2026 {
namespace pipeline {

class Pipeline {
public:
    Pipeline(std::shared_ptr<IPreprocessor> pre, 
             std::shared_ptr<IInferenceEngine> engine, 
             std::shared_ptr<IDecoder> decoder,
             std::shared_ptr<IPostprocessor> post);

    /**
     * @brief Process an image end-to-end
     * @param img Original camera image
     * @return List of detections
     */
    std::vector<Armor> run(const cv::Mat& img);

    /**
     * @brief Process a context end-to-end (Useful for debugging/testing to inspecting intermediate states)
     * @param ctx Execution context with ctx.original_image populated
     * @return List of detections
     */
    std::vector<Armor> run(DetectionContext& ctx);

    /**
     * @brief Save all things in context for debugging
     * @param ctx The detection context
     * @param path_prefix The path prefix for saving files
     */
    void save_context(const DetectionContext& ctx, const std::string& path_prefix);

private:
    std::shared_ptr<IPreprocessor> preprocessor_;
    std::shared_ptr<IInferenceEngine> engine_;
    std::shared_ptr<IDecoder> decoder_;
    std::shared_ptr<IPostprocessor> postprocessor_;
};

} // namespace pipeline
} // namespace rm_detector2026
