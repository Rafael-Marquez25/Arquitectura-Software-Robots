#include "practica4/states/IdleState.hpp"
#include "practica4/PatrollingRobotNode.hpp"
#include "practica4/states/MovingState.hpp"

IdleState::IdleState(PatrollingRobotNode * r)
: robot_(r) {}

void IdleState::on_entry()
{
  RCLCPP_INFO(robot_->get_logger(), "[IDLE] Entrando al estado - Deteniendo motores");
  robot_->publish_velocity(0.0, 0.0);
  robot_->goal_sent_ = false;
}

void IdleState::on_do()
{
    // no hace nada
}

State * IdleState::check_transitions()
{
  if (!robot_->server_ready_) {
    if (robot_->navigation_->wait_for_action_server(std::chrono::seconds(1))) {
      RCLCPP_INFO(robot_->get_logger(), "Servidor desponible, preparado para navegar");
      robot_->server_ready_ = true;
    }
    return nullptr;
  }

  if (!robot_->goal_sent_) {
    auto current_waypoint = robot_->get_current_waypoint();
    RCLCPP_INFO(robot_->get_logger(), "[IDLE] Enviando Objetivo de navegacion...");
    robot_->navigation_->send_goal(current_waypoint);

    RCLCPP_INFO(robot_->get_logger(), "[IDLE] Goal enviado (%f, %f)",
        current_waypoint.pose.position.x,
        current_waypoint.pose.position.y);
    robot_->goal_sent_ = true;

    robot_->publish_waypooint(current_waypoint);

    return new MovingState(robot_);
  }

  return nullptr;
}

void IdleState::on_exit()
{
  RCLCPP_INFO(robot_->get_logger(), "[IDLE] Saliendo del estado - Preparando para mover");
}
