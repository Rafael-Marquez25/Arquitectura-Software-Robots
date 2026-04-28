#include <memory>

#include "practica4/PatrollingRobotNode.hpp"
#include "rclcpp/rclcpp.hpp"

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);

  auto patrolling_robot_node = std::make_shared<PatrollingRobotNode>();

  rclcpp::executors::MultiThreadedExecutor executor;
  executor.add_node(patrolling_robot_node);
  executor.add_node(patrolling_robot_node->navigation_);

  executor.spin();

  rclcpp::shutdown();
  return 0;
}
