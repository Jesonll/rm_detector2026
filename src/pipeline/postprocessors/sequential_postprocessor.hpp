#pragma once
#include <vector>
#include <memory>
#include "cv_detector/pipeline/i_postprocessor.hpp"

namespace rm_detector2026 {
namespace pipeline {

class SequentialPostprocessor : public IPostprocessor {
public:
    SequentialPostprocessor(std::vector<std::shared_ptr<IPostprocessor>> postprocessors)
        : postprocessors_(std::move(postprocessors)) {}

    void process(DetectionContext& ctx) override {
        for (auto& p : postprocessors_) {
            p->process(ctx);
        }
    }

private:
    std::vector<std::shared_ptr<IPostprocessor>> postprocessors_;
};

} // namespace pipeline
} // namespace rm_detector2026

