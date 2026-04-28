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

#ifndef NEAREST_OBSTACLE_DETECTOR__NEARESTOBSTACLEDETECTORNODE_HPP_
#define NEAREST_OBSTACLE_DETECTOR__NEARESTOBSTACLEDETECTORNODE_HPP_


#include "geometry_msgs/msg/twist.hpp"
#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"
#include "tf2_ros/transform_broadcaster.hpp"
#include "std_msgs/msg/bool.hpp"
#include "geometry_msgs/msg/point_stamped.hpp"


#include "tf2_ros/buffer.h"
#include "tf2_ros/transform_listener.h"

// namespace laser
// {

class NearestObstacleDetectorNode : public rclcpp::Node
{
public:
  NearestObstacleDetectorNode();

private:
  void laser_callback(const sensor_msgs::msg::LaserScan::ConstSharedPtr & scan);
  bool is_obstacle(const sensor_msgs::msg::LaserScan & scan, float dist_thrld);
  void print_obstacle_info(const sensor_msgs::msg::LaserScan & scan, float dist_thrld);

  void publish_tf();
  void generate_tf(double x, double y, rclcpp::Time);

  tf2_ros::Buffer tf_buffer_;
  tf2_ros::TransformListener tf_listener_;

  std::shared_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;
  geometry_msgs::msg::TransformStamped transform_;

  rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr laser_sub_;
  rclcpp::Publisher<geometry_msgs::msg::PointStamped>::SharedPtr obstacle_pub_;

  float min_distance_ {0.5f};
};

// }


#endif
