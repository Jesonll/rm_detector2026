#include "cv_detector/factories/engine_factory.hpp"
#include "../pipeline/engines/openvino_engine.hpp"

namespace rm_detector2026 {
namespace factories {

std::shared_ptr<pipeline::IInferenceEngine> EngineFactory::create(const std::string& type, const std::string& model_path) {
    (void)model_path;
    if (type == "openvino") {
        return std::make_shared<pipeline::OpenVINOEngine>();
    }
    // Default
    return std::make_shared<pipeline::OpenVINOEngine>();
}

}
}
