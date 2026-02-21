#include "cv_detector/node/detector_node.hpp"
#include "cv_detector/factories/preprocessor_factory.hpp"
#include "cv_detector/factories/engine_factory.hpp"
#include "cv_detector/factories/postprocessor_factory.hpp"

namespace rm_detector2026 {
namespace node {

DetectorNode::DetectorNode(const rclcpp::NodeOptions& options) 
    : Node("rm_detector_node", options) {
    
    // Declare and get parameters (simplified)
    this->declare_parameter("pipeline.preprocessor.type", "letterbox");
    this->declare_parameter("pipeline.engine.type", "openvino");
    this->declare_parameter("pipeline.engine.model_path", "model.onnx");
    this->declare_parameter("pipeline.postprocessor.type", "nms");
    
    std::string pre_type = this->get_parameter("pipeline.preprocessor.type").as_string();
    std::string engine_type = this->get_parameter("pipeline.engine.type").as_string();
    std::string model_path = this->get_parameter("pipeline.engine.model_path").as_string();
    std::string post_type = this->get_parameter("pipeline.postprocessor.type").as_string();

    RCLCPP_INFO(this->get_logger(), "Initializing Pipeline...");
    
    auto preprocessor = factories::PreprocessorFactory::create(pre_type);
    auto engine = factories::EngineFactory::create(engine_type, model_path);
    auto postprocessor = factories::PostprocessorFactory::create(post_type);
    
    pipeline_ = std::make_unique<pipeline::Pipeline>(preprocessor, engine, postprocessor);
    
    img_sub_ = this->create_subscription<sensor_msgs::msg::Image>(
        "/image_raw", 
        10, 
        std::bind(&DetectorNode::imageKeypointCallback, this, std::placeholders::_1));
        
    RCLCPP_INFO(this->get_logger(), "Detector Node Initialized.");
}

void DetectorNode::imageKeypointCallback(const sensor_msgs::msg::Image::ConstSharedPtr& msg) {
    try {
        cv_bridge::CvImagePtr cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
        
        // Run pipeline
        auto detections = pipeline_->run(cv_ptr->image);
        
        RCLCPP_INFO(this->get_logger(), "Detected %lu objects", detections.size());
        
        // Visualization or publishing logic would go here
        
    } catch (cv_bridge::Exception& e) {
        RCLCPP_ERROR(this->get_logger(), "cv_bridge exception: %s", e.what());
    }
}

} // namespace node
} // namespace rm_detector2026

#include "rclcpp_components/register_node_macro.hpp"
RCLCPP_COMPONENTS_REGISTER_NODE(rm_detector2026::node::DetectorNode)
