#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "cv_detector/node/detector_node.hpp"

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::NodeOptions options;
    auto node = std::make_shared<rm_detector2026::node::DetectorNode>(options);
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
