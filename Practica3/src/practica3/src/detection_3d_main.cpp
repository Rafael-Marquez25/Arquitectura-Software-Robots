#include <memory>

#include "practica3/Detection3DNode.hpp"
#include "rclcpp/rclcpp.hpp"

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);

  auto node_detector_3d = std::make_shared<Detection3DNode>();

  rclcpp::spin(node_detector_3d);

  rclcpp::shutdown();
  return 0;
}
