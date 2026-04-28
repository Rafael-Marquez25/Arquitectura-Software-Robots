#include "practica4/states/MovingState.hpp"
#include "practica4/PatrollingRobotNode.hpp"
#include "nav2_msgs/action/navigate_to_pose.hpp"
#include "practica4/states/StoppedState.hpp"
#include "practica4/states/IdleState.hpp"

MovingState::MovingState(PatrollingRobotNode * r)
: robot_(r) {}

void MovingState::on_entry()
{
  RCLCPP_INFO(robot_->get_logger(), "[MOVING] Entrando al estado - Iniciando movimiento");
}

void MovingState::on_do()
{
  last_feedback_ = robot_->navigation_->get_feedback();

  if (last_feedback_ == nullptr) {
    return;
  }

  RCLCPP_INFO(robot_->get_logger(),
    "[MOVING] Current position: (%f, %f), Distant remaining: %f, Estimated time remaining: %d segundos",
                last_feedback_->current_pose.pose.position.x,
                last_feedback_->current_pose.pose.position.y,
                last_feedback_->distance_remaining,
                last_feedback_->estimated_time_remaining.sec);
}

State * MovingState::check_transitions()
{
  if (last_feedback_ == nullptr) {
    return nullptr;
  }

    // if (!robot_->navigation_->is_goal_active()){
    //     RCLCPP_INFO(robot_->get_logger(), "[MOVING] Goal no activo - Parando por seguridad");
    //     return new StoppedState(robot_);
    // }

  if (last_feedback_->number_of_recoveries == 3) {
    RCLCPP_INFO(robot_->get_logger(),
      "[MOVING] El numero de recuperaciones ha llegado a 3 - Omitiendo waypoint");
    robot_->navigation_->cancel_goal();
    return new IdleState(robot_);
  }

  if (robot_->navigation_->is_goal_done() && robot_->navigation_->was_goal_successful()) {
    RCLCPP_INFO(robot_->get_logger(), "[MOVING] Goal terminado con exito");
    return new IdleState(robot_);
  }

  if (robot_->navigation_->is_goal_done()) {
    RCLCPP_INFO(robot_->get_logger(), "[MOVING] Goal terminado sin exito");
    return new IdleState(robot_);
  }

  return nullptr;
}

void MovingState::on_exit()
{
  RCLCPP_INFO(robot_->get_logger(), "[MOVING] Saliendo del estado - Iniciando frenado");
  robot_->publish_velocity(0.0, 0.0);
}
