#include <memory>

#include "practica5/PersonDetection.hpp"
#include "rclcpp/rclcpp.hpp"

#include <behaviortree_cpp/bt_factory.h>
#include <behaviortree_cpp/loggers/bt_cout_logger.h>
#include <ament_index_cpp/get_package_share_directory.hpp>

#include "hri_client/hri_client.hpp"
#include "practica5/bt_nodes/bt_node_registration.hpp"

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);

  auto node = std::make_shared<rclcpp::Node>("robot_camarero");

  auto hri_client = std::make_shared<HRIClient>(node.get());

  RCLCPP_INFO(node->get_logger(), "Waiting for HRI services...");
  if (!hri_client->wait_for_services(std::chrono::seconds(10))) {
    RCLCPP_ERROR(node->get_logger(), "HRI services not available. Exiting.");
    rclcpp::shutdown();
    return 1;
  }
  RCLCPP_INFO(node->get_logger(), "HRI services ready!");

  BT::BehaviorTreeFactory factory;

  register_bt_nodes(factory);

  node->declare_parameter("kitchen_x", 0.0);
  node->declare_parameter("kitchen_y", 0.0);

  node->declare_parameter("base_x", 0.0);
  node->declare_parameter("base_y", 0.0);

  double kitchen_x = node->get_parameter("kitchen_x").as_double();
  double kitchen_y = node->get_parameter("kitchen_y").as_double();

  double base_x = node->get_parameter("base_x").as_double();
  double base_y = node->get_parameter("base_y").as_double();


  auto blackboard = BT::Blackboard::create();
  blackboard->set("node", node);
  blackboard->set("hri_client", hri_client);
  blackboard->set("kitchen_x", kitchen_x);
  blackboard->set("kitchen_y", kitchen_y);
  blackboard->set("base_x", base_x);
  blackboard->set("base_y", base_y);
  blackboard->set("order", std::string(""));
  blackboard->set("drink_order", std::string(""));
  blackboard->set("food_order", std::string(""));

  std::string package_share_dir = ament_index_cpp::get_package_share_directory("practica5");
  std::string tree_path = package_share_dir + "/config/practica5_tree.xml";

  auto tree = factory.createTreeFromFile(tree_path, blackboard);

  BT::StdCoutLogger logger(tree);

  RCLCPP_INFO(node->get_logger(), "camarero behavior tree started");

  rclcpp::Rate rate(10);
  BT::NodeStatus status = BT::NodeStatus::IDLE;

  while (rclcpp::ok() && status != BT::NodeStatus::FAILURE) {
    RCLCPP_DEBUG(node->get_logger(), "Ticking the behavior tree...");

    // Procesar callbacks de ROS
    rclcpp::spin_some(node);

    // Evaluar árbol
    status = tree.tickOnce();
    rate.sleep();
  }

  if (status == BT::NodeStatus::SUCCESS) {
    RCLCPP_INFO(node->get_logger(), "Mission completed successfully");
  } else {
    RCLCPP_ERROR(node->get_logger(), "Mission failed");
  }

  rclcpp::shutdown();
  return 0;
}
