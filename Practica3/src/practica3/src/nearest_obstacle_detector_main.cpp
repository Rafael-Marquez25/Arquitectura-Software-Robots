#include <memory>

#include "practica3/NearestObstacleDetectorNode.hpp"
#include "rclcpp/rclcpp.hpp"

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);

  auto node_detector = std::make_shared<NearestObstacleDetectorNode>();

  rclcpp::spin(node_detector);

  rclcpp::shutdown();
  return 0;
}
