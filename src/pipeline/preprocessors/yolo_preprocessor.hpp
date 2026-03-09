#pragma once
#include "cv_detector/pipeline/i_preprocessor.hpp"

namespace rm_detector2026 {
namespace pipeline {

class YOLOPreprocessor : public IPreprocessor {
public:
    void preprocess(DetectionContext& ctx) override;
};

}
}
