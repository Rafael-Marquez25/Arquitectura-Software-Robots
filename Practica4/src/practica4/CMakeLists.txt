cmake_minimum_required(VERSION 3.5)
project(practica4)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()


find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(nav2_msgs REQUIRED)

find_package(rclcpp_action REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)


add_library(${PROJECT_NAME} SHARED
  src/practica4/states/IdleState.cpp
  src/practica4/states/MovingState.cpp
  src/practica4/states/StateMachine.cpp
  src/practica4/states/StoppedState.cpp

  src/practica4/navigation_client.cpp
  src/practica4/PatrollingRobotNode.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
)

target_link_libraries(${PROJECT_NAME}
  rclcpp::rclcpp
  rclcpp_action::rclcpp_action
  tf2::tf2
  tf2_geometry_msgs::tf2_geometry_msgs
  ${geometry_msgs_TARGETS}
  ${nav2_msgs_TARGETS}
)

add_executable(patrolling_robot_node src/patrolling_robot_node_main.cpp)
target_link_libraries(patrolling_robot_node ${PROJECT_NAME})

install(TARGETS
  ${PROJECT_NAME}
  patrolling_robot_node
  EXPORT export_${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY config launch DESTINATION share/${PROJECT_NAME})


if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_export_include_directories("include/${PROJECT_NAME}")
ament_export_libraries(${PROJECT_NAME})
ament_export_targets(export_${PROJECT_NAME})
ament_export_dependencies(
  rclcpp
  geometry_msgs
  nav2_msgs
)

ament_package()