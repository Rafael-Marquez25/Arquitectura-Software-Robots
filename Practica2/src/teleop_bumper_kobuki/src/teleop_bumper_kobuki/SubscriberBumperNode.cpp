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

#include "rclcpp/rclcpp.hpp"
#include "teleop_bumper_kobuki/SubscriberBumperNode.hpp"

using namespace std::chrono_literals;

SubscriberBumperNode::SubscriberBumperNode()
: Node("subcriber_bumper_node")
{
  bumper_subscription_ = create_subscription<kobuki_ros_interfaces::msg::BumperEvent>(
        "/events/bumper", 10,
    std::bind(&SubscriberBumperNode::subscription_callback, this, std::placeholders::_1));

  timer_ = create_wall_timer(
        100ms, std::bind(&SubscriberBumperNode::timer_callback, this));

  publisher_ = create_publisher<geometry_msgs::msg::Twist>(
        "/cmd_vel", 10);

  v = 0.1;
  w = 0.8;

  state_ = STOP;

  last_msg_state_L_ = 0;
  last_msg_state_C_ = 0;
  last_msg_state_R_ = 0;
}

void
SubscriberBumperNode::subscription_callback(
  const kobuki_ros_interfaces::msg::BumperEvent::SharedPtr msg)
{
  RCLCPP_INFO(get_logger(), "Receiving Bumper: %d, State: %d", msg->bumper, msg->state);

  switch (msg->bumper) {
    case kobuki_ros_interfaces::msg::BumperEvent::LEFT:
      last_msg_state_L_ = msg->state;
      break;

    case kobuki_ros_interfaces::msg::BumperEvent::CENTER:
      last_msg_state_C_ = msg->state;
      break;

    case kobuki_ros_interfaces::msg::BumperEvent::RIGHT:
      last_msg_state_R_ = msg->state;
      break;
  }
}

void
SubscriberBumperNode::timer_callback()
{

  if(last_msg_state_L_ == kobuki_ros_interfaces::msg::BumperEvent::PRESSED &&
    last_msg_state_R_ == kobuki_ros_interfaces::msg::BumperEvent::PRESSED)
  {
    state_ = STOP;
  } else if(last_msg_state_L_ == kobuki_ros_interfaces::msg::BumperEvent::PRESSED) {
    state_ = TURNING_L;
  } else if(last_msg_state_R_ == kobuki_ros_interfaces::msg::BumperEvent::PRESSED) {
    state_ = TURNING_R;
  } else if(last_msg_state_C_ == kobuki_ros_interfaces::msg::BumperEvent::PRESSED) {
    state_ = FOWARD;
  } else {
    state_ = STOP;
  }

  switch (state_) {
    case STOP:

      movement(0, 0);
      break;

    case TURNING_L:

      movement(0, -w);
      break;

    case FOWARD:

      movement(v, 0);
      break;

    case TURNING_R:

      movement(0, w);
      break;
  }
}

void
SubscriberBumperNode::movement(double v, double w)
{
  // Movimiento lieneal
  vel_msg_.linear.x = v;

  // Movimiento angular
  vel_msg_.angular.z = w;

  publisher_->publish(vel_msg_);

  RCLCPP_INFO(get_logger(), "Publishing v: %f, w: %f", vel_msg_.angular.x, vel_msg_.angular.z);

}
