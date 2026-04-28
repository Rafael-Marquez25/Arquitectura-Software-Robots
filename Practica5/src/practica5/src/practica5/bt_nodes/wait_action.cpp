#include "practica5/bt_nodes/wait_action.hpp"
#include <chrono>
#include <rclcpp/rclcpp.hpp>

WaitAction::WaitAction(
  const std::string & name,
  const BT::NodeConfiguration & config)
: BT::StatefulActionNode(name, config)
{
  node_ = config.blackboard->get<rclcpp::Node::SharedPtr>("node");
}

// Definición de puertos
BT::PortsList WaitAction::providedPorts()
{
  return {BT::InputPort<int>("seconds")};
}

BT::NodeStatus WaitAction::onStart()
{
  if (!getInput("seconds", seconds_)) {
    RCLCPP_ERROR(node_->get_logger(), "Missing seconds parameter");
    return BT::NodeStatus::FAILURE;
  }

    // Guardamos el tiempo de inicio
  start_time_ = std::chrono::steady_clock::now();

  return BT::NodeStatus::RUNNING;
}

BT::NodeStatus WaitAction::onRunning()
{
  auto elapsed = std::chrono::steady_clock::now() - start_time_;

  if (elapsed < std::chrono::seconds(seconds_)) {
    return BT::NodeStatus::RUNNING;
  }

  return BT::NodeStatus::SUCCESS;
}

void WaitAction::onHalted()
{

}
