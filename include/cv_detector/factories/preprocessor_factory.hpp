#pragma once

#include <memory>
#include <string>
#include "cv_detector/pipeline/i_preprocessor.hpp"

namespace rm_detector2026 {
namespace factories {

class PreprocessorFactory {
public:
    static std::shared_ptr<pipeline::IPreprocessor> create(const std::string& type);
};

}
}
