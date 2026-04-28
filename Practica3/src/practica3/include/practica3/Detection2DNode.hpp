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

#ifndef PRACTICA3__DETECTION2DNODE_HPP_
#define PRACTICA3__DETECTION2DNODE_HPP_


#include "yolo_msgs/msg/detection_array.hpp"
#include "vision_msgs/msg/detection2_d_array.hpp"
#include "sensor_msgs/msg/image.hpp"
#include "sensor_msgs/msg/camera_info.hpp"

#include "rclcpp/rclcpp.hpp"


class Detection2DNode : public rclcpp::Node
{
public:
  Detection2DNode();

private:
  void detection_callback(const vision_msgs::msg::Detection2DArray::ConstSharedPtr & msg);

  rclcpp::Subscription<vision_msgs::msg::Detection2DArray>::SharedPtr detection_sub_;
  rclcpp::Publisher<vision_msgs::msg::Detection2DArray>::SharedPtr detection_pub_;
};

#endif
