#ifndef PRACTICA5__PERSONDETECTION_HPP_
#define PRACTICA5__PERSONDETECTION_HPP_
#include "rclcpp/rclcpp.hpp"
#include "yolo_msgs/msg/detection_array.hpp"
#include "std_msgs/msg/bool.hpp"

class PersonDetection : public rclcpp::Node
{
public:
  PersonDetection();

private:
  rclcpp::Subscription<yolo_msgs::msg::DetectionArray>::SharedPtr detection_sub_;
  rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr detection_pub_;

  void detection_callback(const yolo_msgs::msg::DetectionArray::ConstSharedPtr & msg);


};

#endif
