#include "practica4/states/StoppedState.hpp"

StoppedState::StoppedState(PatrollingRobotNode * r)
: robot_(r) {}

void StoppedState::on_entry()
{
  RCLCPP_INFO(robot_->get_logger(),
    "[STOPPED] Entrando en el estado - Robot detenido por seguridad");
}

void StoppedState::on_do()
{
  robot_->publish_velocity(0.0, 0.0);
}

State * StoppedState::check_transitions()
{
    // logica de transicion
  return nullptr;
}

void StoppedState::on_exit()
{
  RCLCPP_INFO(robot_->get_logger(), "[STOPPED] Saliendo del estado - Retomando operacion");
}
