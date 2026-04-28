#include "practica4/PatrollingRobotNode.hpp"
#include "practica4/states/IdleState.hpp"

PatrollingRobotNode::PatrollingRobotNode()
: Node("patrolling_robot")
{
  declare_parameter("puntos_x", std::vector<double>{});
  declare_parameter("puntos_y", std::vector<double>{});
  declare_parameter("frame_id", std::string("odom"));

  auto xs = get_parameter("puntos_x").as_double_array();
  auto ys = get_parameter("puntos_y").as_double_array();


  for (size_t i = 0; i < xs.size(); i++) {
    geometry_msgs::msg::PoseStamped ps;
    ps.header.frame_id = get_parameter("frame_id").as_string();
    ps.pose.position.x = xs[i];
    ps.pose.position.y = ys[i];
    points.push_back(ps);

    RCLCPP_INFO(this->get_logger(), "Waypoint %ld: x: %f, y: %f", i, points[i].pose.position.x,
      points[i].pose.position.y);
  }

  if (points.empty()) {
    RCLCPP_ERROR(get_logger(), "No hay una lista de waypoints");
    return;
  }


  navigation_ = std::make_shared<NavigationClient>();

    // publicadores
  vel_pub_ = this->create_publisher<geometry_msgs::msg::Twist>("/cmd_vel", 10);
  state_pub_ = this->create_publisher<std_msgs::msg::String>("/current_state", 10);
  waypoint_pub_ = this->create_publisher<geometry_msgs::msg::PoseStamped>("/current_waypoint", 10);

    // timer
  timer_ = this->create_wall_timer(
        std::chrono::milliseconds(100),
        std::bind(&PatrollingRobotNode::control_cycle, this));

  fsm_ = new StateMachine(new IdleState(this), this->get_logger());
}

PatrollingRobotNode::~PatrollingRobotNode()
{
  delete fsm_;
}

void PatrollingRobotNode::publish_velocity(double linear, double angular)
{
  auto msg = geometry_msgs::msg::Twist();
  msg.linear.x = linear;
  msg.angular.z = angular;
  vel_pub_->publish(msg);
}

void PatrollingRobotNode::control_cycle()
{
  fsm_->step();

  if(fsm_->entering_state_) {
    publish_state();
    fsm_->entering_state_ = false;
  }
}

geometry_msgs::msg::PoseStamped PatrollingRobotNode::get_current_waypoint()
{
  current_point_ = points.front();
  points.pop_front();
  points.push_back(current_point_);

    // RCLCPP_WARN(this->get_logger(), ">>> get_current_waypoint called: returning (%f, %f), queue size: %ld",
    //         current_point_.pose.position.x,
    //         current_point_.pose.position.y,
    //         points.size());

  return current_point_;
}

void PatrollingRobotNode::publish_state()
{
  std_msgs::msg::String msg;
  msg.data = fsm_->current_state_->get_name().c_str();
  state_pub_->publish(msg);
}

void PatrollingRobotNode::publish_waypooint(geometry_msgs::msg::PoseStamped waypoint)
{
  waypoint_pub_->publish(waypoint);
}
