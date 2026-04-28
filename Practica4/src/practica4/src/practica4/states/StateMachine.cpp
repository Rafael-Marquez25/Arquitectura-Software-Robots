#include "practica4/states/StateMachine.hpp"


StateMachine::StateMachine(State * initial_state, rclcpp::Logger logger)
: logger_(logger), current_state_(initial_state)
{
  RCLCPP_INFO(logger_, "FMS iniciada en estado: %s", current_state_->get_name().c_str());

  current_state_->on_entry();
  entering_state_ = false;
}

StateMachine::~StateMachine()
{
  delete current_state_;
}

void StateMachine::step()
{
    // 1. Ejecutar lógica del estado actual
  current_state_->on_do();

  // 2. Verificar transiciones
  State * next_state = current_state_->check_transitions();

  // 3. Si hay transición, ejecutar salida, cambiar y ejecutar entrada
  if (next_state != nullptr) {
    RCLCPP_INFO(logger_, "Transición: %s -> %s", current_state_->get_name().c_str(),
      next_state->get_name().c_str());

    current_state_->on_exit();
    delete current_state_;
    current_state_ = next_state;
    current_state_->on_entry();
    entering_state_ = true;
  }
}
