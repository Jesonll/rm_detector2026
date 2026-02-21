#pragma once
#include "cv_detector/pipeline/i_preprocessor.hpp"

namespace rm_detector2026 {
namespace pipeline {

class LetterboxPreprocessor : public IPreprocessor {
public:
    cv::Mat preprocess(const cv::Mat& img) override;
};

}
}
