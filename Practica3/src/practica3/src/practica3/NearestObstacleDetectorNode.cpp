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

#include <algorithm>
#include <cmath>
#include <limits>
#include <memory>

#include "geometry_msgs/msg/point_stamped.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"
#include "std_msgs/msg/bool.hpp"
#include "rclcpp/rclcpp.hpp"

#include "tf2/LinearMath/Transform.h"
#include "tf2/transform_datatypes.h"
#include "tf2_ros/transform_broadcaster.h"
#include "tf2/exceptions.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.hpp"

#include "practica3/NearestObstacleDetectorNode.hpp"

using std::placeholders::_1;

NearestObstacleDetectorNode::NearestObstacleDetectorNode()
: Node("nearest_obstacle_detector"),
  tf_buffer_(this->get_clock()),
  tf_listener_(tf_buffer_)
{
  declare_parameter("min_distance", min_distance_);
  get_parameter("min_distance", min_distance_);

  RCLCPP_INFO(get_logger(), "NearestObstacleDetectorNode set to %f m", min_distance_);

  laser_sub_ = create_subscription<sensor_msgs::msg::LaserScan>(
      "input_scan", rclcpp::SensorDataQoS().reliable(),
      std::bind(&NearestObstacleDetectorNode::laser_callback, this, _1));

  obstacle_pub_ = create_publisher<geometry_msgs::msg::PointStamped>("/nearest_obstacle", 100);

  tf_broadcaster_ = std::make_shared<tf2_ros::TransformBroadcaster>(*this);
}

bool NearestObstacleDetectorNode::is_obstacle(
  const sensor_msgs::msg::LaserScan & scan,
  float dist_thrld)
{
  float distance_min = std::numeric_limits<float>::infinity();

  for (size_t i = 0; i < scan.ranges.size(); ++i) {
    const float range = scan.ranges[i];
    if (!std::isfinite(range)) {continue;}
    if (range < scan.range_min || range > scan.range_max) {continue;}
    distance_min = std::min(distance_min, range);
  }

  if (!std::isfinite(distance_min)) {return false;}

  return distance_min < dist_thrld;
}

void NearestObstacleDetectorNode::print_obstacle_info(
  const sensor_msgs::msg::LaserScan & scan,
  float influence_dist)
{
  if (scan.ranges.empty()) {
    RCLCPP_WARN(get_logger(), "Recibiendo rangos de LaserScan vacíos");
    return;
  }

  size_t min_idx = 0;
  float distance_min = std::numeric_limits<float>::infinity();
  bool found_valid = false;

  for (size_t i = 0; i < scan.ranges.size(); ++i) {
    const float range = scan.ranges[i];
    if (!std::isfinite(range)) {continue;}
    if (range < scan.range_min || range > scan.range_max) {continue;}

    if (range < distance_min) {
      distance_min = range;
      min_idx = i;
      found_valid = true;
    }
  }

  if (!found_valid) {return;}

  // Solo publicar si está dentro del rango de influencia
  if (distance_min > influence_dist) {return;}

  float obstacle_angle = scan.angle_min + static_cast<float>(min_idx) * scan.angle_increment;
  float obstacle_x = distance_min * std::cos(obstacle_angle);
  float obstacle_y = distance_min * std::sin(obstacle_angle);

  geometry_msgs::msg::PointStamped obstacle_point;
  obstacle_point.header = scan.header;
  obstacle_point.point.x = obstacle_x;
  obstacle_point.point.y = obstacle_y;
  obstacle_point.point.z = 0.0;

  std::string error;
  if (!tf_buffer_.canTransform("base_link", scan.header.frame_id, tf2::TimePointZero, &error)) {
    RCLCPP_WARN_STREAM(get_logger(), "TF no disponible: " << error);
    return;
  }

  auto tf = tf_buffer_.lookupTransform("base_link", scan.header.frame_id, tf2::TimePointZero);
  geometry_msgs::msg::PointStamped obstacle_point_base;
  tf2::doTransform(obstacle_point, obstacle_point_base, tf);

  obstacle_point_base.header.frame_id = "base_link";
  obstacle_point_base.header.stamp = scan.header.stamp;

  RCLCPP_INFO(get_logger(), "Obstacle detected at x: %f, y: %f (base_link)",
    obstacle_point_base.point.x, obstacle_point_base.point.x);

  generate_tf(obstacle_point_base.point.x, obstacle_point_base.point.y, scan.header.stamp);
  publish_tf();
  obstacle_pub_->publish(obstacle_point_base);
}

void NearestObstacleDetectorNode::laser_callback(
  const sensor_msgs::msg::LaserScan::ConstSharedPtr & scan)
{
  float influence_distance = 1.5; // Rango donde el obstáculo tiene efecto
  if (is_obstacle(*scan, influence_distance)) {
    print_obstacle_info(*scan, influence_distance);
  }
}

void NearestObstacleDetectorNode::generate_tf(double x, double y, rclcpp::Time stamp)
{
  transform_.header.stamp = stamp;
  transform_.header.frame_id = "base_link";
  transform_.child_frame_id = "nearest_obstacle";

  transform_.transform.translation.x = x;
  transform_.transform.translation.y = y;
  transform_.transform.translation.z = 0.0;

  transform_.transform.rotation.x = 0.0;
  transform_.transform.rotation.y = 0.0;
  transform_.transform.rotation.z = 0.0;
  transform_.transform.rotation.w = 1.0;
}

void NearestObstacleDetectorNode::publish_tf()
{
  // RCLCPP_INFO(get_logger(), "Publishing TF nearest_obstacle at x: %f y: %f",
  //   transform_.transform.translation.x,
  //   transform_.transform.translation.y);
  tf_broadcaster_->sendTransform(transform_);
}
