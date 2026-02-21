#pragma once

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <cv_bridge/cv_bridge.h>

#include "cv_detector/pipeline/pipeline.hpp"

namespace rm_detector2026 {
namespace node {

class DetectorNode : public rclcpp::Node {
public:
    DetectorNode(const rclcpp::NodeOptions& options);

private:
    void imageKeypointCallback(const sensor_msgs::msg::Image::ConstSharedPtr& msg);

    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr img_sub_;
    std::unique_ptr<pipeline::Pipeline> pipeline_;
};

} // namespace node
} // namespace rm_detector2026
