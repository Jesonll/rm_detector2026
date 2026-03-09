#pragma once

#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <rm_utils/data.h>

namespace rm_detector2026 {
namespace pipeline {

/**
 * @brief Holds all data relevant to a single frame's processing.
 * This struct is passed through the pipeline stages, allowing them to 
 * read what they need and write their results.
 */
struct DetectionContext {
    // --- Input (Set by DetectorNode) ---
    cv::Mat original_image;

    // --- letterbox preprocessor 
    float resize_ratio = 1.0f; // Scale factor (original / input) 
    int dw = 0; // Padding on width (left)
    int dh = 0;// Padding on height (top)
    
    // --- Preprocessor Output ---
    cv::Mat network_input;     // The processed image fed to the engine


    // --- Inference Engine Output ---
    cv::Mat inference_outputs; // Raw tensors

    // --- Decoder/Postprocessor Output ---
    std::vector<Armor> armors; // Final results (using internal struct)
};

} // namespace pipeline
} // namespace rm_detector2026
