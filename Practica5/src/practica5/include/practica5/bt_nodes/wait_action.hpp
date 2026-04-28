// bt_nodes/IsPersonDetected.hpp
#ifndef BT_EXAMPLES__BT_NODES__WAIT_ACTION_HPP_
#define BT_EXAMPLES__BT_NODES__WAIT_ACTION_HPP_

#include <behaviortree_cpp/bt_factory.h>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/bool.hpp>

class WaitAction : public BT::StatefulActionNode
{
public:
  WaitAction(
    const std::string & name,
    const BT::NodeConfiguration & config);

  static BT::PortsList providedPorts();

  BT::NodeStatus onStart() override;
  BT::NodeStatus onRunning() override;
  void onHalted() override;

private:
  rclcpp::Node::SharedPtr node_;
  int seconds_;
  std::chrono::steady_clock::time_point start_time_;
};

#endif
