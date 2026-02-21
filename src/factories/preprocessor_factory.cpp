#include "cv_detector/factories/preprocessor_factory.hpp"
#include "../pipeline/preprocessors/letterbox_preprocessor.hpp"

namespace rm_detector2026 {
namespace factories {

std::shared_ptr<pipeline::IPreprocessor> PreprocessorFactory::create(const std::string& type) {
    if (type == "letterbox") {
        return std::make_shared<pipeline::LetterboxPreprocessor>();
    }
    // Default or other types
    return std::make_shared<pipeline::LetterboxPreprocessor>(); // Fallback
}

}
}
