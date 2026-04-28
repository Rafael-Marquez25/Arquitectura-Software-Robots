#include <behaviortree_cpp/bt_factory.h>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/bool.hpp>
#include "practica5/bt_nodes/IsPersonDetected.hpp"

IsPersonDetected::IsPersonDetected(
  const std::string & name,
  const BT::NodeConfiguration & config)
: BT::ConditionNode(name, config)
{
  node_ = config.blackboard->get<rclcpp::Node::SharedPtr>("node");

  sub_ = node_->create_subscription<std_msgs::msg::Bool>(
    "/person_detected", 10,
    [this](std_msgs::msg::Bool::SharedPtr msg) {
      detected_ = msg->data;
    });
}

BT::NodeStatus IsPersonDetected::tick()
{
  rclcpp::spin_some(node_);

  if (detected_) {
    return BT::NodeStatus::SUCCESS;
  }

  return BT::NodeStatus::FAILURE;
}

BT::PortsList IsPersonDetected::providedPorts()
{
  return {
  };
}
