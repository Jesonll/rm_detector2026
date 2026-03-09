#pragma once

#include <opencv2/opencv.hpp>
#include <vector>
#include <rm_utils/data.h>
#include "cv_detector/pipeline/types.hpp"

namespace rm_detector2026 {
namespace pipeline {

/**
 * @brief Abstract base class for decoding inference results
 */
class IDecoder {
public:
    virtual ~IDecoder() = default;

    /**
     * @brief Decode raw inference outputs into detection candidates
     * @param ctx The detection context. Reads ctx.inference_outputs, writes ctx.armors
     */
    virtual void decode(DetectionContext& ctx) = 0;
};

} // namespace pipeline
} // namespace rm_detector2026
