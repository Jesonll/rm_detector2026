#pragma once

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <cv_bridge/cv_bridge.h>

#include "cv_detector/pipeline/pipeline.hpp"

#include <rm_interfaces/msg/detection.hpp>

namespace rm_detector2026 {
namespace node {

class DetectorNode : public rclcpp::Node {
public:
    DetectorNode(const rclcpp::NodeOptions& options);

private:
    void imageKeypointCallback(const sensor_msgs::msg::Image::ConstSharedPtr& msg);

    //typical Sub and Pub
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr img_sub_;
    rclcpp::Publisher<rm_interfaces::msg::Detection>::SharedPtr detection_pub_;

    //TO DO: debug pubs

    std::unique_ptr<pipeline::Pipeline> pipeline_;
};

} // namespace node
} // namespace rm_detector2026
