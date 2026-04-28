// Copyright 2026 Rafael Márquez
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "vision_msgs/msg/detection2_d_array.hpp"

#include "practica3/Detection2DNode.hpp"

using std::placeholders::_1;

Detection2DNode::Detection2DNode()
: Node("detection_2d")
{
  detection_sub_ = create_subscription<vision_msgs::msg::Detection2DArray>(
    "input_detection", rclcpp::SensorDataQoS().reliable(),
    std::bind(&Detection2DNode::detection_callback, this, _1));

  detection_pub_ = create_publisher<vision_msgs::msg::Detection2DArray>(
    "output_detection_2d", rclcpp::SensorDataQoS().reliable());
}

void
Detection2DNode::detection_callback(const vision_msgs::msg::Detection2DArray::ConstSharedPtr & msg)
{
  if (!msg->detections.empty()) {
    //RCLCPP_INFO(get_logger(), "Detecting person, publising in /detection_2d");
    detection_pub_->publish(*msg);
  }
}
