#include "yolo_preprocessor.hpp"

namespace rm_detector2026 {
namespace pipeline {

/*
    include: 1. letterbox resize(image in the center)
             2. normalize pixel values to [0,1]
    (Actually, the color format conversion and layout handling are handled in OpenVINO preprocessing step, which is in engine, 
    so we don't need to do them here if we set them correctly in OpenVINO)
*/

void YOLOPreprocessor::preprocess(DetectionContext& ctx) {
    if (ctx.original_image.empty()) return;

    // 1. letterbox resize
    int target_w = 640;
    int target_h = 640;

    int img_w = ctx.original_image.cols;
    int img_h = ctx.original_image.rows;

    // Scale
    float r = std::min((float)target_w / img_w, (float)target_h / img_h);

    // Compute unpadded size
    int unpad_w = std::round(img_w * r);
    int unpad_h = std::round(img_h * r);

    // Compute padding
    int dw = (target_w - unpad_w) / 2;
    int dh = (target_h - unpad_h) / 2;

    // Store in context (Using explicit cast/assignment if types differ, assume int in context)
    ctx.resize_ratio = r;
    ctx.dw = dw;
    ctx.dh = dh;

    // Resize
    if (img_w != unpad_w || img_h != unpad_h) {
        cv::resize(ctx.original_image, ctx.network_input, cv::Size(unpad_w, unpad_h));
        // Note: ctx.network_input is destination.
    } else {
        ctx.network_input = ctx.original_image.clone();
    }

    // Add padding
    int top = dh;
    int bottom = target_h - unpad_h - top;
    int left = dw;
    int right = target_w - unpad_w - left;
    // Default color 114, 114, 114 (grey)
    cv::copyMakeBorder(ctx.network_input, ctx.network_input, top, bottom, left, right, cv::BORDER_CONSTANT, cv::Scalar(114, 114, 114));

    // 3. normalize pixel values to [0,1]
    ctx.network_input.convertTo(ctx.network_input, CV_32FC3, 1.0 / 255.0);
}

}
}
