#pragma once

#include <memory>
#include <string>
#include "cv_detector/pipeline/i_decoder.hpp"

namespace rm_detector2026 {
namespace factories {

class DecoderFactory {
public:
    static std::shared_ptr<pipeline::IDecoder> create(const std::string& type, float conf_threshold = 0.5f);
};

}
}
