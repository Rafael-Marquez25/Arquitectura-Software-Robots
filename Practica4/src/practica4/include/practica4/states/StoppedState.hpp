#ifndef PRACTICA4__STOPPEDSTATED_HPP_
#define PRACTICA4__STOPPEDSTATED_HPP_

#include "practica4/states/State.hpp"
#include "practica4/PatrollingRobotNode.hpp"

class StoppedState : public State
{
  PatrollingRobotNode * robot_;
  rclcpp::Time entry_time_;

public:
  explicit StoppedState(PatrollingRobotNode * r);

  void on_entry() override;
  void on_do() override;
  State * check_transitions() override;
  void on_exit() override;
  std::string get_name() const override {return "STOPPED";}
};

#endif
