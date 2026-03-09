#pragma once
#include "cv_detector/pipeline/i_decoder.hpp"

namespace rm_detector2026 {
namespace pipeline {

class YoloDecoder : public IDecoder {
public:
    explicit YoloDecoder(float conf_threshold = 0.5f);
    void decode(DetectionContext& ctx) override;

private:
    float conf_threshold_;
};

}
}
