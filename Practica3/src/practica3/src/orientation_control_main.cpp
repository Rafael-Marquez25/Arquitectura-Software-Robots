#include <memory>

#include "practica3/OrientationControlNode.hpp"
#include "rclcpp/rclcpp.hpp"

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);

  auto orientation_control_node = std::make_shared<OrientationControlNode>();

  rclcpp::spin(orientation_control_node);

  rclcpp::shutdown();
  return 0;
}
