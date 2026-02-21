#pragma once

#include <memory>
#include <string>
#include "cv_detector/pipeline/i_postprocessor.hpp"

namespace rm_detector2026 {
namespace factories {

class PostprocessorFactory {
public:
    static std::shared_ptr<pipeline::IPostprocessor> create(const std::string& type);
};

}
}
