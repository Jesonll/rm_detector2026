#include "cv_detector/factories/decoder_factory.hpp"
#include "../pipeline/decoders/yolo_decoder.hpp"

namespace rm_detector2026 {
namespace factories {

std::shared_ptr<pipeline::IDecoder> DecoderFactory::create(const std::string& type, float conf_threshold) {
    if (type == "yolo" || type == "yolov10_decoder") {
        return std::make_shared<pipeline::YoloDecoder>(conf_threshold);
    }
    // Default
    return std::make_shared<pipeline::YoloDecoder>(conf_threshold);
}

}
}
