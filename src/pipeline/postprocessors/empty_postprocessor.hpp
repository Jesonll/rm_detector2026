#pragma once

#include "cv_detector/pipeline/i_postprocessor.hpp"

namespace rm_detector2026 {
namespace pipeline {

class EmptyPostprocessor : public IPostprocessor {
public:
    void process(DetectionContext& /*ctx*/) override {
        // Do nothing, pass through
    }
};

} // namespace pipeline
} // namespace rm_detector2026
