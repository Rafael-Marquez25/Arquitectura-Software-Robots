// bt_nodes/IsPersonDetected.hpp
#ifndef BT_EXAMPLES__BT_NODES__IS_PERSON_DETECTED_HPP_
#define BT_EXAMPLES__BT_NODES__IS_PERSON_DETECTED_HPP_

#include <behaviortree_cpp/bt_factory.h>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/bool.hpp>

class IsPersonDetected : public BT::ConditionNode
{
public:
  IsPersonDetected(const std::string & name, const BT::NodeConfig & config);
  static BT::PortsList providedPorts();
  BT::NodeStatus tick() override;

private:
  rclcpp::Node::SharedPtr node_;
  rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr sub_;
  bool detected_ = false;
};

#endif
