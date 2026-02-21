#include "cv_detector/factories/postprocessor_factory.hpp"
#include "../pipeline/postprocessors/nms_postprocessor.hpp"

namespace rm_detector2026 {
namespace factories {

std::shared_ptr<pipeline::IPostprocessor> PostprocessorFactory::create(const std::string& type) {
    if (type == "nms") {
        return std::make_shared<pipeline::NMSPostprocessor>();
    }
    return std::make_shared<pipeline::NMSPostprocessor>();
}

}
}
