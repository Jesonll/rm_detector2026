#pragma once

#include <memory>
#include <string>
#include "cv_detector/pipeline/i_postprocessor.hpp"

namespace rm_detector2026 {
namespace factories {

class PostprocessorFactory {
public:
    static std::shared_ptr<pipeline::IPostprocessor> create(const std::string& type, float nms_threshold = 0.45f);
    static std::shared_ptr<pipeline::IPostprocessor> create(const std::vector<std::string>& types, float nms_threshold = 0.45f);
};

}
}
