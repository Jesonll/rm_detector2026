#pragma once

#include "cv_detector/pipeline/types.hpp"
#include <rm_utils/data.h>


namespace rm_detector2026 {
namespace pipeline {

/**
 * @brief Abstract base class for postprocessing
 */
class IPostprocessor {
public:
    virtual ~IPostprocessor() = default;

    /**
     * @brief Process detections (e.g., NMS)
     * @param ctx Detection context. Reads and filters ctx.armors
     */
    virtual void process(DetectionContext& ctx) = 0;
};


} // namespace pipeline
} // namespace rm_detector2026
