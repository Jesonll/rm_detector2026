#include "cv_detector/node/detector_node.hpp"
#include "cv_detector/factories/preprocessor_factory.hpp"
#include "cv_detector/factories/engine_factory.hpp"
#include "cv_detector/factories/decoder_factory.hpp"
#include "cv_detector/factories/postprocessor_factory.hpp"
#include <rm_utils/data.h>

namespace rm_detector2026 {
namespace node {

DetectorNode::DetectorNode(const rclcpp::NodeOptions& options) 
    : Node("rm_detector_node", options) {
    
    // Declare and get parameters
    this->declare_parameter("preprocessor.name", "letterbox");
    this->declare_parameter("engine.name", "openvino");
    this->declare_parameter("engine.model_path", "model.onnx");
    this->declare_parameter("decoder.name", "yolov10_decoder");
    this->declare_parameter("decoder.conf_threshold", 0.5);
    this->declare_parameter("postprocessor.IsUsed", true);
    this->declare_parameter("postprocessor.p1.name", "nms");
    this->declare_parameter("postprocessor.p1.iou_threshold", 0.5);
    this->declare_parameter("postprocessor.p2.name", "rule_based_filter");

    std::string pre_type = this->get_parameter("preprocessor.name").as_string();
    std::string engine_type = this->get_parameter("engine.name").as_string();
    std::string model_path = this->get_parameter("engine.model_path").as_string();
    std::string decoder_type = this->get_parameter("decoder.name").as_string();
    float conf_threshold = this->get_parameter("decoder.conf_threshold").as_double();
    
    // Check if postprocessor is used
    bool use_post = this->get_parameter("postprocessor.IsUsed").as_bool();
    
    std::string post_type_1 = "empty";
    std::string post_type_2 = "empty";
    float nms_threshold = 0.5;

    if (use_post) {
        post_type_1 = this->get_parameter("postprocessor.p1.name").as_string();
        nms_threshold = this->get_parameter("postprocessor.p1.iou_threshold").as_double();
        post_type_2 = this->get_parameter("postprocessor.p2.name").as_string();
    } else {
        RCLCPP_WARN(this->get_logger(), "Postprocessor is disabled (IsUsed=false). Using empty postprocessor.");
    }

    RCLCPP_INFO(this->get_logger(), "Initializing Pipeline...");
    
    auto preprocessor = factories::PreprocessorFactory::create(pre_type);
    auto engine = factories::EngineFactory::create(engine_type, model_path);
    auto decoder = factories::DecoderFactory::create(decoder_type, conf_threshold);
    auto postprocessor = factories::PostprocessorFactory::create({post_type_1, post_type_2}, nms_threshold);
    
    pipeline_ = std::make_unique<pipeline::Pipeline>(preprocessor, engine, decoder, postprocessor);
    
    detection_pub_ = this->create_publisher<rm_interfaces::msg::Detection>("/detector/result", 10);
    
    img_sub_ = this->create_subscription<sensor_msgs::msg::Image>(
        "/image_raw", 
        10, 
        std::bind(&DetectorNode::imageKeypointCallback, this, std::placeholders::_1));
        
    RCLCPP_INFO(this->get_logger(), "Detector Node Initialized.");
}

void DetectorNode::imageKeypointCallback(const sensor_msgs::msg::Image::ConstSharedPtr& msg) {
    try {
        cv_bridge::CvImagePtr cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
        cv::Mat img = cv_ptr->image;
        
        // Run pipeline
        auto armors = pipeline_->run(img);
        
        RCLCPP_INFO(this->get_logger(), "Detected %lu objects", armors.size());
        
        // Visualization: Draw armors
        cv::Mat drawn_img = img.clone();
        for (const auto& armor : armors) {
            // Draw rect
            cv::rectangle(drawn_img, armor.rect, cv::Scalar(0, 255, 0), 2);
            
            // Draw corners (pts 0-3) and center (pt 4)
            for (int i = 0; i < 4; ++i) {
                cv::line(drawn_img, armor.pts[i], armor.pts[(i+1)%4], cv::Scalar(0,0,255), 2);
            }
            cv::circle(drawn_img, armor.pts[4], 3, cv::Scalar(255, 0, 0), -1);

            // Draw text
            std::string text = "T:" + std::to_string(armor.type) + " C:" + std::to_string(armor.color);
            cv::putText(drawn_img, text, armor.rect.tl(), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1);
        }

        // Pack message
        rm_interfaces::msg::Detection detection_msg;
        detection_msg.header = msg->header;
        
        // Pack drawn image
        cv_bridge::CvImage drawn_bridge(msg->header, "bgr8", drawn_img);
        detection_msg.image = *drawn_bridge.toImageMsg();
        
        // Pack armors
        for (const auto& armor : armors) {
            detection_msg.detected_armors.push_back(Armor2Msg(armor));
        }

        // Publish
        detection_pub_->publish(detection_msg);
        
    } catch (cv_bridge::Exception& e) {
        RCLCPP_ERROR(this->get_logger(), "cv_bridge exception: %s", e.what());
    }
}

} // namespace node
} // namespace rm_detector2026

#include "rclcpp_components/register_node_macro.hpp"
RCLCPP_COMPONENTS_REGISTER_NODE(rm_detector2026::node::DetectorNode)
