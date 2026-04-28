#include "practica5/PersonDetection.hpp"

using std::placeholders::_1;

PersonDetection::PersonDetection()
: Node("detection_node")
{
  detection_sub_ = create_subscription<yolo_msgs::msg::DetectionArray>(
    "input_detection", rclcpp::SensorDataQoS().reliable(),
    std::bind(&PersonDetection::detection_callback, this, _1));

  detection_pub_ = create_publisher<std_msgs::msg::Bool>("/person_detected", 10);
}

void PersonDetection::detection_callback(const yolo_msgs::msg::DetectionArray::ConstSharedPtr & msg)
{
  std_msgs::msg::Bool detection_msg;
  if (!msg->detections.empty()) {
    detection_msg.data = true;
  } else {
    detection_msg.data = false;
  }

  detection_pub_->publish(detection_msg);

}
