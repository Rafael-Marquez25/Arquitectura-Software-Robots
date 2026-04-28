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

#include "tf2/LinearMath/Transform.h"
#include "tf2/transform_datatypes.h"
#include "tf2_ros/transform_broadcaster.h"
#include "geometry_msgs/msg/transform_stamped.hpp"
#include "tf2_geometry_msgs/tf2_geometry_msgs.hpp"

#include "practica3/OrientationControlNode.hpp"

using std::placeholders::_1;
using namespace std::chrono_literals;


OrientationControlNode::OrientationControlNode()
: Node("orientation_control"),
  tf_buffer_(this->get_clock()),
  tf_listener_(tf_buffer_),
  vrot_atractive_pid_(0.0, 1.0, 0.3, 1.0),
  vlin_atractive_pid_(0.0, 1.0, 0.0, 0.7),
  vrot_repulsive_pid_(0.0, 1.0, 0.3, 1.0),
  vlin_repulsive_pid_(0.0, 1.0, 0.0, 0.7)
{
  vrot_atractive_pid_.set_pid(0.15, 0.0, 1.5);

  vel_publisher_ = create_publisher<geometry_msgs::msg::Twist>("/cmd_vel", 10);

  detection_sub_ = create_subscription<vision_msgs::msg::Detection3D>("/detection_3d",
  rclcpp::SensorDataQoS().reliable(),
  std::bind(&OrientationControlNode::orientation_3d_callback, this, _1));

  detection_laser_sub_ = create_subscription<geometry_msgs::msg::PointStamped>("/nearest_obstacle",
  rclcpp::SensorDataQoS().reliable(),
  std::bind(&OrientationControlNode::detection_laser_callback, this, _1));

  timer_ = create_wall_timer(
    50ms, std::bind(&OrientationControlNode::control_cycle, this));
}

void
OrientationControlNode::orientation_3d_callback(vision_msgs::msg::Detection3D::ConstSharedPtr msg)
{
  if(!msg) {
    return;
  }

  // RCLCPP_INFO(get_logger(), "Detection received!");
  std::string error;
  last_person_time_ = now();

  if(tf_buffer_.canTransform("base_footprint", "person_detected", tf2::TimePointZero, &error)) {
    isDetecting = true;

    auto bf2person_msg = tf_buffer_.lookupTransform(
      "base_footprint", "person_detected", tf2::TimePointZero);

    double x_person = bf2person_msg.transform.translation.x;
    double y_person = bf2person_msg.transform.translation.y;

    double angle_person = atan2(y_person, x_person);
    double dist_person = sqrt(x_person * x_person + y_person * y_person);

    if (fabs(angle_person) > 0.1) {
      last_vrot_atractive_ = std::clamp(vrot_atractive_pid_.get_output(angle_person), -2.0, 2.0);
    } else {
      last_vrot_atractive_ = 0.0;
    }
    last_vlin_atractive_ = std::clamp(vlin_atractive_pid_.get_output(dist_person - 1.0), -1.0, 1.0);

  } else {
    RCLCPP_WARN_STREAM(get_logger(), "Error in TF: " << error);
    isDetecting = false;
  }
}

void
OrientationControlNode::detection_laser_callback(
  const geometry_msgs::msg::PointStamped::ConstSharedPtr msg)
{
  // RCLCPP_INFO(get_logger(), "laser recieved");

  if (!msg) {
    return;
  }

  // Obtenemos coordenadas del obstáculo en base_link
  double x_obstacle = msg->point.x;
  double y_obstacle = msg->point.y;

  double angle_obstacle = atan2(y_obstacle, x_obstacle);
  double dist_obstacle = std::sqrt(x_obstacle * x_obstacle + y_obstacle * y_obstacle);

  const double influence_dist = 1.5;  // rango máximo de influencia del obstáculo

  double CONE_HALF_ANGLE = 30.0 * M_PI / 180.0;

  if (!(std::abs(angle_obstacle) < CONE_HALF_ANGLE)) {
    last_vrot_repulsive_ = 0.0;
    last_vlin_repulsive_ = 0.0;
    obstacle_active_ = false;
    return;
  }

  if (dist_obstacle <= influence_dist) {
    obstacle_active_ = true;
    // Calculamos repulsión solo si el obstáculo está cerca
    if (std::fabs(angle_obstacle) > 0.1) {
      last_vrot_repulsive_ = std::clamp(vrot_repulsive_pid_.get_output(-angle_obstacle), -2.0, 2.0);
    } else {
      last_vrot_repulsive_ = 0.0;
    }
    last_vlin_repulsive_ = std::clamp(vlin_repulsive_pid_.get_output(influence_dist -
      dist_obstacle), -1.0, 0.0);

    last_obstacle_time_ = now();  // guardamos el tiempo para resetear después
  } else {
    obstacle_active_ = false;
    // Si no hay obstáculo dentro del rango, repulsión a cero
    last_vrot_repulsive_ = 0.0;
    last_vlin_repulsive_ = 0.0;
  }
}

void
OrientationControlNode::control_cycle()
{
  geometry_msgs::msg::Twist twist;

  if ((now() - last_person_time_).seconds() > 0.5) {
    isDetecting = false;
  }

  if ((now() - last_obstacle_time_).seconds() > 0.5) {
    obstacle_active_ = false;
    last_vrot_repulsive_ = 0.0;
    last_vlin_repulsive_ = 0.0;
  }

  if(obstacle_active_) {
    twist.linear.x = last_vlin_repulsive_;
    twist.angular.z = last_vrot_repulsive_;
    vel_publisher_->publish(twist);
  } else if(isDetecting) {
    twist.linear.x = last_vlin_atractive_;
    twist.angular.z = last_vrot_atractive_;
    vel_publisher_->publish(twist);
  } else {
    // RCLCPP_INFO(get_logger(), "No Detection, searching...");
    last_vrot_repulsive_ = 0.0;
    last_vlin_repulsive_ = 0.0;

    last_vrot_atractive_ = 0.0;
    last_vlin_atractive_ = 0.0;

    twist.angular.z = 0.3;
    vel_publisher_->publish(twist);
  }
}
