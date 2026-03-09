#pragma once
#include "cv_detector/pipeline/i_postprocessor.hpp"

namespace rm_detector2026 {
namespace pipeline {

class NMSPostprocessor : public IPostprocessor {
public:
    explicit NMSPostprocessor(float nms_threshold = 0.45f);
    void process(DetectionContext& ctx) override;

private:
    float nms_threshold_;
};

}
}
