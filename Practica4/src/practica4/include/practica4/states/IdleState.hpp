#ifndef PRACTICA4__IDLESTATE_HPP_
#define PRACTICA4__IDLESTATE_HPP_

#include <memory>

#include "practica4/states/State.hpp"
#include "practica4/PatrollingRobotNode.hpp"

class IdleState : public State
{
  PatrollingRobotNode * robot_;

public:
  explicit IdleState(PatrollingRobotNode * r);

  void on_entry() override;
  void on_do() override;
  State * check_transitions() override;
  void on_exit() override;
  std::string get_name() const override {return "IDLE";}
};

#endif
