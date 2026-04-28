#include <memory>

#include "practica5/PersonDetection.hpp"
#include "rclcpp/rclcpp.hpp"

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);

  auto node_detector = std::make_shared<PersonDetection>();

  rclcpp::spin(node_detector);

  rclcpp::shutdown();
  return 0;
}
