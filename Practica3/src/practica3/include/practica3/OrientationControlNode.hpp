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

#ifndef PRACTICA3__ORIENTATIONCONTROLNODE_HPP_
#define PRACTICA3__ORIENTATIONCONTROLNODE_HPP_

#include "vision_msgs/msg/detection3_d_array.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "rclcpp/rclcpp.hpp"
#include "practica3/PIDController.hpp"
#include "vision_msgs/msg/detection3_d_array.hpp"
#include "tf2_ros/buffer.hpp"
#include "geometry_msgs/msg/point_stamped.hpp"

#include "tf2_ros/transform_listener.h"


class OrientationControlNode : public rclcpp::Node
{
public:
  OrientationControlNode();

private:
  void orientation_3d_callback(const vision_msgs::msg::Detection3D::ConstSharedPtr msg);
  void control_cycle();
  void detection_laser_callback(const geometry_msgs::msg::PointStamped::ConstSharedPtr msg);

  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr vel_publisher_;

  rclcpp::Subscription<vision_msgs::msg::Detection3D>::ConstSharedPtr detection_sub_;

  rclcpp::Subscription<geometry_msgs::msg::PointStamped>::ConstSharedPtr detection_laser_sub_;

  rclcpp::TimerBase::SharedPtr timer_;

  tf2_ros::Buffer tf_buffer_;
  tf2_ros::TransformListener tf_listener_;

  tf_seeker::PIDController vrot_atractive_pid_, vlin_atractive_pid_;
  tf_seeker::PIDController vrot_repulsive_pid_, vlin_repulsive_pid_;

  double last_vrot_atractive_ = 0;
  double last_vlin_atractive_ = 0;

  double last_vrot_repulsive_ = 0;
  double last_vlin_repulsive_ = 0;

  rclcpp::Time last_obstacle_time_{0, 0, RCL_ROS_TIME}; // inicializado a cero
  rclcpp::Time last_person_time_{0, 0, RCL_ROS_TIME}; // inicializado a cero

  bool isDetecting = false;
  bool obstacle_active_ = false;
};

#endif
