#include "cv_detector/factories/postprocessor_factory.hpp"
#include "../pipeline/postprocessors/nms_postprocessor.hpp"
#include "../pipeline/postprocessors/empty_postprocessor.hpp"
#include "../pipeline/postprocessors/sequential_postprocessor.hpp"

namespace rm_detector2026 {
namespace factories {

std::shared_ptr<pipeline::IPostprocessor> PostprocessorFactory::create(const std::string& type, float nms_threshold) {
    if (type == "nms") {
        return std::make_shared<pipeline::NMSPostprocessor>(nms_threshold);
    } else if (type == "rule_based_filter") {
        // Mock rule based filter, assuming NMSPostprocessor for now or empty if you don't have it
        // Or actually create empty since it doesn't exist
        return std::make_shared<pipeline::EmptyPostprocessor>();
    } else if (type == "empty" || type == "none" || type.empty()) {
        return std::make_shared<pipeline::EmptyPostprocessor>();
    }
    // Default
    return std::make_shared<pipeline::NMSPostprocessor>(nms_threshold);
}

std::shared_ptr<pipeline::IPostprocessor> PostprocessorFactory::create(const std::vector<std::string>& types, float nms_threshold) {
    std::vector<std::shared_ptr<pipeline::IPostprocessor>> postprocessors;
    for (const auto& type : types) {
        if (!type.empty() && type != "none" && type != "empty") {
            postprocessors.push_back(create(type, nms_threshold));
        }
    }
    
    if (postprocessors.empty()) {
        return std::make_shared<pipeline::EmptyPostprocessor>();
    } else if (postprocessors.size() == 1) {
        return postprocessors[0];
    } else {
        return std::make_shared<pipeline::SequentialPostprocessor>(postprocessors);
    }
}

}
}
