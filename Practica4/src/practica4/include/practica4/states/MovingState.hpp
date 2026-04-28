#ifndef PRACTICA4__MOVINGSTATE_HPP_
#define PRACTICA4__MOVINGSTATE_HPP_

#include "practica4/states/State.hpp"
#include "practica4/PatrollingRobotNode.hpp"

class MovingState : public State
{
  PatrollingRobotNode * robot_;

public:
  explicit MovingState(PatrollingRobotNode * r);

  void on_entry() override;
  void on_do() override;
  State * check_transitions() override;
  void on_exit() override;
  std::string get_name() const override {return "MOVING";}

  std::shared_ptr<const nav2_msgs::action::NavigateToPose::Feedback> last_feedback_;
};

#endif
