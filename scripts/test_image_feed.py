#!/usr/bin/env python3

import os
import cv2
import json
import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Image
from cv_bridge import CvBridge
from rm_interfaces.msg import Detection
from ament_index_python.packages import get_package_share_directory

class TestImageFeed(Node):
    def __init__(self):
        super().__init__('test_image_feed')
        self.bridge = CvBridge()
        
        # Paths
        pkg_path = get_package_share_directory('rm_detector2026')
        self.asset_dir = os.path.join(pkg_path, '..', '..', '..', '..', 'src', 'rm_detector2026', 'asset')
        self.output_dir = os.path.join(pkg_path, '..', '..', '..', '..', 'src', 'rm_detector2026', 'tmp', 'test_results')
        
        os.makedirs(self.output_dir, exist_ok=True)
        
        # Topics
        self.publisher_ = self.create_publisher(Image, '/image_raw', 10)
        self.subscription = self.create_subscription(
            Detection,
            '/detector/result',
            self.result_callback,
            10)
            
        self.images = [f for f in os.listdir(self.asset_dir) if f.lower().endswith(('.png', '.jpg', '.jpeg'))]
        self.current_idx = 0
        self.pending_results = 0
        
        self.get_logger().info(f"Found {len(self.images)} images in {self.asset_dir}")
        self.timer = self.create_timer(1.0, self.timer_callback)
        self.current_img_name = ""

    def timer_callback(self):
        if self.pending_results > 0:
            return # Wait for result before sending next

        if self.current_idx >= len(self.images):
            self.get_logger().info("All images processed. Exiting...")
            raise SystemExit

        img_file = self.images[self.current_idx]
        self.current_img_name = img_file
        img_path = os.path.join(self.asset_dir, img_file)
        
        cv_img = cv2.imread(img_path)
        if cv_img is None:
            self.get_logger().error(f"Failed to read image {img_path}")
            self.current_idx += 1
            return
            
        img_msg = self.bridge.cv2_to_imgmsg(cv_img, encoding="bgr8")
        img_msg.header.stamp = self.get_clock().now().to_msg()
        img_msg.header.frame_id = img_file # Hack to pass image name
        
        self.publisher_.publish(img_msg)
        self.get_logger().info(f"Published image: {img_file}")
        
        self.pending_results = 1
        self.current_idx += 1

    def result_callback(self, msg):
        self.get_logger().info(f"Received result for frame_id (image name): {msg.header.frame_id}")
        
        img_name = msg.header.frame_id if msg.header.frame_id else "unknown"
        base_name = os.path.splitext(img_name)[0]
        
        # Save image
        cv_img = self.bridge.imgmsg_to_cv2(msg.image, desired_encoding='bgr8')
        out_img_path = os.path.join(self.output_dir, f"{base_name}_result.png")
        cv2.imwrite(out_img_path, cv_img)
        
        # Save info
        out_txt_path = os.path.join(self.output_dir, f"{base_name}_info.json")
        armors_info = []
        for armor in msg.detected_armors:
            armors_info.append({
                "type": armor.type,
                "color": armor.color,
                "size": armor.size,
                "conf": armor.conf,
                "xywh": list(armor.xywh),
                "pts": [{"x": pt.x, "y": pt.y, "z": pt.z} for pt in armor.pts]
            })
            
        with open(out_txt_path, 'w') as f:
            json.dump({"image": img_name, "armors": armors_info}, f, indent=4)
            
        self.get_logger().info(f"Saved {out_img_path} and {out_txt_path}")
        self.pending_results = 0

def main(args=None):
    rclpy.init(args=args)
    node = TestImageFeed()
    try:
        rclpy.spin(node)
    except SystemExit:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    main()
