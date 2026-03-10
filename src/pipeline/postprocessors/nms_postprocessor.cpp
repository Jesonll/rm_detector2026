#include "nms_postprocessor.hpp"

namespace rm_detector2026 {
namespace pipeline {

NMSPostprocessor::NMSPostprocessor(float nms_threshold) : nms_threshold_(nms_threshold) {}

static float calculateIoU(const cv::Rect& a, const cv::Rect& b) {
    int x1 = std::max(a.x, b.x);
    int y1 = std::max(a.y, b.y);
    int x2 = std::min(a.x + a.width, b.x + b.width);
    int y2 = std::min(a.y + a.height, b.y + b.height);

    int intersection = std::max(0, x2 - x1) * std::max(0, y2 - y1);
    int area_a = a.width * a.height;
    int area_b = b.width * b.height;

    return static_cast<float>(intersection) / (area_a + area_b - intersection);
}

void NMSPostprocessor::process(DetectionContext& ctx) {
    if (ctx.armors.empty()) return;

    std::sort(ctx.armors.begin(), ctx.armors.end(), [](const Armor& a, const Armor& b) {
        return a.conf > b.conf;
    });

    std::vector<Armor> filtered;
    std::vector<bool> is_suppressed(ctx.armors.size(), false);

    for (size_t i = 0; i < ctx.armors.size(); ++i) {
        if (is_suppressed[i]) continue;
        
        filtered.push_back(ctx.armors[i]);
        
        for (size_t j = i + 1; j < ctx.armors.size(); ++j) {
            if (is_suppressed[j]) continue;

            float iou = calculateIoU(ctx.armors[i].rect, ctx.armors[j].rect);
            if (iou > nms_threshold_) {
                is_suppressed[j] = true;
            }
        }
    }
    
    ctx.results = filtered;
}

}
}
