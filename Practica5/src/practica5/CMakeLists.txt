cmake_minimum_required(VERSION 3.5)
project(practica5)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()


find_package(ament_cmake REQUIRED)
find_package(ament_index_cpp REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(nav2_msgs REQUIRED)
find_package(yolo_msgs REQUIRED)
find_package(behaviortree_cpp REQUIRED)
find_package(hri_client REQUIRED)
find_package(std_msgs REQUIRED)
find_package(std_srvs REQUIRED)
find_package(rclcpp_action REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(sensor_msgs REQUIRED)

find_package(rclcpp_action REQUIRED)
# find_package(tf2 REQUIRED)
# find_package(tf2_geometry_msgs REQUIRED)


add_library(${PROJECT_NAME} SHARED
  src/practica5/bt_nodes/IsPersonDetected.cpp
  src/practica5/bt_nodes/move_towards_goal_action.cpp
  src/practica5/PersonDetection.cpp
  src/practica5/bt_nodes/say_text_client_action.cpp
  src/practica5/text_utils.cpp
  src/practica5/bt_nodes/wait_action.cpp
  src/practica5/bt_nodes/bt_node_registration.cpp

  src/practica5/bt_nodes/is_obstacle_near_condition.cpp
  src/practica5/bt_nodes/is_battery_low_condition.cpp
  src/practica5/bt_nodes/has_goal_condition.cpp
  src/practica5/bt_nodes/backup_action.cpp
  src/practica5/bt_nodes/spin_action.cpp
  src/practica5/bt_nodes/move_forward_action.cpp
  src/practica5/bt_nodes/return_to_charger_action.cpp
  src/practica5/bt_nodes/abort_mission_action.cpp
  src/practica5/bt_nodes/say_text_action.cpp
  src/practica5/bt_nodes/listen_text_action.cpp
  src/practica5/bt_nodes/extract_info_action.cpp
  src/practica5/bt_nodes/listen_text_client_action.cpp
  src/practica5/bt_nodes/extract_info_client_action.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
)

target_link_libraries(${PROJECT_NAME}
  rclcpp::rclcpp
  rclcpp_action::rclcpp_action
  ${std_msgs_TARGETS}
  ${std_srvs_TARGETS}
  ${yolo_msgs_TARGETS}
  ${geometry_msgs_TARGETS}
  ${nav2_msgs_TARGETS}
  hri_client::hri_client
  behaviortree_cpp::behaviortree_cpp
  ${simple_hri_interfaces_TARGETS}
  ament_index_cpp::ament_index_cpp
  tf2_ros::tf2_ros
  ${sensor_msgs_TARGETS}
)

add_executable(detection_node src/person_detection_main.cpp)
target_link_libraries(detection_node ${PROJECT_NAME})

add_executable(bt_node src/bt_main.cpp)
target_link_libraries(bt_node ${PROJECT_NAME})

install(TARGETS
  ${PROJECT_NAME}
  detection_node
  bt_node
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