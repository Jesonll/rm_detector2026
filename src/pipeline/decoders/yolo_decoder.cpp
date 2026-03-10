#include "pipeline/decoders/yolo_decoder.hpp"
#include <rm_utils/data.h>
#include <iostream>

namespace rm_detector2026 {
namespace pipeline {

YoloDecoder::YoloDecoder(float conf_threshold) : conf_threshold_(conf_threshold) {}

void YoloDecoder::decode(DetectionContext& ctx) {
    if (ctx.inference_outputs.empty()) return;

    const cv::Mat& output = ctx.inference_outputs;
    
    if (output.empty()) return;

    // Expected shape: [1, 300, 57] (Batch, Detections, Features)
    int detections = 0;
    int features = 0;

    if (output.dims == 3) {
        detections = output.size[1];
        features = output.size[2];
    } else if (output.dims == 2) {
        detections = output.size[0];
        features = output.size[1];
    } else {
        return;
    }

    // Minimum features: 4(box) + 1(conf) + 1(class) = 6
    if (features < 6) return;

    // Get resize info
    float ratio = ctx.resize_ratio;
    int dw = ctx.dw;
    int dh = ctx.dh;
    if (ratio <= 0) ratio = 1.0f;

    const float* data = (const float*)output.data;
    
    // Clear previous
    ctx.armors.clear();

    for (int i = 0; i < detections; ++i) {
        const float* det = data + i * features;

        // Index 4: Confidence
        float score = det[4];
        if (score < conf_threshold_) continue;

        // Index 5: Class ID
        int class_id = static_cast<int>(det[5]);

        // Index 0-3: Bounding Box [x1, y1, x2, y2]
        float x1_raw = det[0];
        float y1_raw = det[1];
        float x2_raw = det[2];
        float y2_raw = det[3];

        // Restore coordinates
        float x1 = (x1_raw - dw) / ratio;
        float y1 = (y1_raw - dh) / ratio;
        float x2 = (x2_raw - dw) / ratio;
        float y2 = (y2_raw - dh) / ratio;

        Armor armor;
        armor.conf = score;
        armor.rect = cv::Rect(cv::Point(x1, y1), cv::Point(x2, y2));
        
        // Map class_id (0-15)
        // 0-7: Blue, 8-15: Red
        if (class_id < 8) {
            armor.color = 0; // Blue
        } else {
            armor.color = 1; // Red
            class_id -= 8;
        }
        
        // Model IDs mapping to rm_utils types
        switch(class_id) {
            case 0: armor.type = 0; armor.size = 1; break; // Sentry
            case 1: armor.type = 1; armor.size = 1; break; // Hero
            case 2: armor.type = 2; armor.size = 1; break; // Engineer
            case 3: armor.type = 3; armor.size = 0; break; // Std1
            case 4: armor.type = 4; armor.size = 0; break; // Std2
            case 5: armor.type = 6; armor.size = 0; break; // Outpost
            case 6: armor.type = 7; armor.size = 0; break; // Base Small
            case 7: armor.type = 7; armor.size = 1; break; // Base Large
            default: armor.type = 8; armor.size = 0; break; // Unknown
        }

        int kp_stride = (features == 14) ? 2 : 3;
        // Each keypoint has x,y and optionally visibility (v).
        // Keypoints: x,y or x,y,v . Start at index 6.
        for(int k=0; k<4; ++k) {
            int base_idx = 6 + k*kp_stride;
            if (base_idx + 1 < features) {
                float kx_raw = det[base_idx];
                float ky_raw = det[base_idx + 1];
                // float kv_raw = det[base_idx + 2]; // Visibility

                // Restore keypoints
                float kx = (kx_raw - dw) / ratio;
                float ky = (ky_raw - dh) / ratio;
                armor.pts[k] = cv::Point2f(kx, ky);
            }
        }
        // 5th point is center
        armor.pts[4] = cv::Point2f((x1 + x2) / 2.0f, (y1 + y2) / 2.0f);

        ctx.armors.push_back(armor);
    }
}

}
}
