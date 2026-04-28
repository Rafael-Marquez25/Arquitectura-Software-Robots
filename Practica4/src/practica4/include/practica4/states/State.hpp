#ifndef PRACTICA4__STATE_HPP_
#define PRACTICA4__STATE_HPP_

#include <string>
#include <memory>

class State
{
public:
  virtual void on_entry() {}
  virtual void on_do() = 0;
  virtual void on_exit() {}
  virtual State * check_transitions() = 0;
  virtual ~State() = default;

  virtual std::string get_name() const = 0;
};

#endif
