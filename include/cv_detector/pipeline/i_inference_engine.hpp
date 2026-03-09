#pragma once

#include "cv_detector/pipeline/types.hpp"

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
     * @param ctx Detection context. Reads ctx.network_input, writes ctx.inference_outputs
     */
    virtual void infer(DetectionContext& ctx) = 0;
};

} // namespace pipeline
} // namespace rm_detector2026
