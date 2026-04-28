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

#ifndef TELEOP_BUMPER_KOBUKI__SUBSCRIBER_BUMPER_NODE_HPP_
#define TELEOP_BUMPER_KOBUKI__SUBSCRIBER_BUMPER_NODE_HPP_

#include "rclcpp/rclcpp.hpp"
#include "kobuki_ros_interfaces/msg/bumper_event.hpp"
#include "geometry_msgs/msg/twist.hpp"

class SubscriberBumperNode : public rclcpp::Node
{
public:
  SubscriberBumperNode();

  void subscription_callback(const kobuki_ros_interfaces::msg::BumperEvent::SharedPtr msg);

  void timer_callback();

private:
  rclcpp::Subscription<kobuki_ros_interfaces::msg::BumperEvent>::SharedPtr bumper_subscription_;

  rclcpp::TimerBase::SharedPtr timer_;

  kobuki_ros_interfaces::msg::BumperEvent::SharedPtr last_msg_;

  int last_msg_state_L_, last_msg_state_C_, last_msg_state_R_;

  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;

  geometry_msgs::msg::Twist vel_msg_;

  double v, w;

  void movement(double v, double w);

  enum PracState
  {
    STOP,
    TURNING_L,
    FOWARD,
    TURNING_R,
  };

  PracState state_;
};

#endif
