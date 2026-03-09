#pragma once

#include "cv_detector/pipeline/types.hpp"

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
     * @param ctx Detection context containing the original image
     *            Sets ctx.network_input and ctx.resize_ratio
     */
    virtual void preprocess(DetectionContext& ctx) = 0;
};

} // namespace pipeline
} // namespace rm_detector2026
