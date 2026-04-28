#ifndef PRACTICA4__STATEMACHINE_HPP_
#define PRACTICA4__STATEMACHINE_HPP_

#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "practica4/states/State.hpp"

class StateMachine
{
  rclcpp::Logger logger_;

public:
  StateMachine(State * initial_state, rclcpp::Logger loger);
  ~StateMachine();

  State * current_state_;
  bool entering_state_;

  void step();
};

#endif
