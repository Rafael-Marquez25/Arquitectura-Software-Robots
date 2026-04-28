#include <memory>

#include "practica3/Detection2DNode.hpp"
#include "rclcpp/rclcpp.hpp"

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);

  auto node_detector_2d = std::make_shared<Detection2DNode>();

  rclcpp::spin(node_detector_2d);

  rclcpp::shutdown();
  return 0;
}
