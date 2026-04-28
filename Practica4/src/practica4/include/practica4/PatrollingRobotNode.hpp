#ifndef PRACTICA4__ROBOT_HPP_
#define PRACTICA4__ROBOT_HPP_

#include <deque>

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"
#include "std_msgs/msg/string.hpp"

#include "practica4/states/StateMachine.hpp"
#include "practica4/navigation_client.hpp"

class PatrollingRobotNode : public rclcpp::Node
{
public:
  PatrollingRobotNode();
  ~PatrollingRobotNode();

    // Interfaz pública para que los estados accedan a la información del robot

  void publish_velocity(double linear, double angular);
  void publish_waypooint(geometry_msgs::msg::PoseStamped waypoint);

  geometry_msgs::msg::PoseStamped get_current_waypoint();

  bool server_ready_ = false;
  bool goal_sent_ = false;

  std::shared_ptr<NavigationClient> navigation_;

  StateMachine * fsm_;

private:
  void control_cycle();

  void publish_state();

  std::deque<geometry_msgs::msg::PoseStamped> points;

  geometry_msgs::msg::PoseStamped current_point_;

  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr state_pub_;

  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr vel_pub_;
  rclcpp::TimerBase::SharedPtr timer_;

  rclcpp::Publisher<geometry_msgs::msg::PoseStamped>::SharedPtr waypoint_pub_;

};

#endif
