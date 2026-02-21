#pragma once
#include "cv_detector/pipeline/i_postprocessor.hpp"

namespace rm_detector2026 {
namespace pipeline {

class NMSPostprocessor : public IPostprocessor {
public:
    std::vector<Detection> process(const std::vector<cv::Mat>& outputs) override;
};

}
}
