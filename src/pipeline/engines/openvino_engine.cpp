#include "openvino_engine.hpp"
#include <iostream>

namespace rm_detector2026 {
namespace pipeline {

OpenVINOEngine::OpenVINOEngine(const std::string& model_path, const std::string& device_name) {
    try {
        // 1. Initialize OpenVINO Core
        core = ov::Core();
        
        // 2. Read model
        if (model_path.empty()) {
            std::cerr << "[OpenVINO] Model path is empty!" << std::endl;
            // In a real application, you might want to throw or return early.
            // For now, we'll just log and proceed (which will likely crash later if not handled).
            return;
        }

        auto model = core.read_model(model_path);

        // 3. Configure preprocessing 
        // Here we declare the input layout is HWC and color format is BGR, which is common for OpenCV. 
        auto ppp = ov::preprocess::PrePostProcessor(model);
        ppp.input().tensor()
            .set_element_type(ov::element::u8)
            .set_layout("HWC") // OpenCV Mat is usually HWC (packed)
            .set_color_format(ov::preprocess::ColorFormat::BGR);
            
        // Declare model expects FP32 and NCHW layout.Adjust as needed for specific model. 
        ppp.input().model().set_layout("NCHW");
        ppp.output().tensor().set_element_type(ov::element::f32);
        
        model = ppp.build();

        // 4. Compile model for the specified device (AUTO by default)
        // Using AUTO allows OpenVINO to select the best available device (CPU, GPU, etc.)
        ov::AnyMap config;
        config.emplace(ov::hint::performance_mode(ov::hint::PerformanceMode::LATENCY));
        // You can also add other properties like ov::enable_profiling(true) if needed.
        
        compiled_model = core.compile_model(model, device_name, config);
            
        // 5. Create inference request
        infer_request = compiled_model.create_infer_request();
        
        std::cout << "[OpenVINO] Model loaded successfully on device: " << device_name << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "[OpenVINO] Exception during initialization: " << e.what() << std::endl;
    }
}

void OpenVINOEngine::infer(DetectionContext& ctx) {
    if (!compiled_model) {
        std::cerr << "[OpenVINO] Model not initialized!" << std::endl;
        return;
    }

    try {
        cv::Mat input = ctx.network_input;
        if (input.empty()) return;

        if (!input.isContinuous()) {
            input = input.clone();
        }

        // 1. Set input tensor
        // Get input tensor by index (0)
        auto input_tensor = infer_request.get_input_tensor(0);
        
        // Check input size
        if (input.total() * input.elemSize() != input_tensor.get_byte_size()) {
             std::cerr << "[OpenVINO] Input size mismatch!" << std::endl;
             return;
        }

        // Copy data from OpenCV Mat to OpenVINO Tensor
        std::memcpy(input_tensor.data(), input.data, input.total() * input.elemSize());

        // 2. Start inference
        infer_request.infer();

        // 3. convert outputs to cv::Mat and store in ctx.inference_outputs.
        auto output_tensor = infer_request.get_output_tensor(0);
        ov::Shape output_shape = output_tensor.get_shape();

        std::vector<int> sizes;
        for (const auto& dim : output_shape) {
            sizes.push_back(static_cast<int>(dim));
        }

        cv::Mat output_buffer(sizes.size(), sizes.data(), CV_32F, output_tensor.data());
        ctx.inference_outputs = output_buffer.clone();
        
    } catch (const std::exception& e) {
        std::cerr << "[OpenVINO] Inference error: " << e.what() << std::endl;
    }
}

}
}