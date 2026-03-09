#pragma once
#include "cv_detector/pipeline/i_inference_engine.hpp"

// OpenVINO includes
#include <openvino/openvino.hpp>

namespace rm_detector2026 {
namespace pipeline {

class OpenVINOEngine : public IInferenceEngine {
public:
    explicit OpenVINOEngine(const std::string& model_path, const std::string& device_name = "AUTO");
    void infer(DetectionContext& ctx) override;

private:
    ov::Core core;
    ov::CompiledModel compiled_model;
    ov::InferRequest infer_request;
};

}
}
