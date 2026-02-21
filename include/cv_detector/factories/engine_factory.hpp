#pragma once

#include <memory>
#include <string>
#include "cv_detector/pipeline/i_inference_engine.hpp"

namespace rm_detector2026 {
namespace factories {

class EngineFactory {
public:
    static std::shared_ptr<pipeline::IInferenceEngine> create(const std::string& type, const std::string& model_path);
};

}
}
