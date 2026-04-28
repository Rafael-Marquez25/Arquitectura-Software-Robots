cmake_minimum_required(VERSION 3.5)
project(practica3)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()


find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(yolo_msgs REQUIRED)
find_package(vision_msgs REQUIRED)

add_library(${PROJECT_NAME} SHARED
  src/practica3/NearestObstacleDetectorNode.cpp
  src/practica3/Detection2DNode.cpp
  src/practica3/Detection3DNode.cpp
  src/practica3/OrientationControlNode.cpp
  src/practica3/PIDController.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
)

target_link_libraries(${PROJECT_NAME}
  rclcpp::rclcpp
  tf2::tf2
  tf2_ros::tf2_ros
  ${geometry_msgs_TARGETS}
  ${sensor_msgs_TARGETS}
  ${tf2_geometry_msgs_TARGETS}
  ${yolo_msgs_TARGETS}
  ${vision_msgs_TARGETS}
)

add_executable(nearest_obstacle_detector_node src/nearest_obstacle_detector_main.cpp)
target_link_libraries(nearest_obstacle_detector_node ${PROJECT_NAME})

add_executable(detection_2d_node src/detection_2d_main.cpp)
target_link_libraries(detection_2d_node ${PROJECT_NAME})

add_executable(detection_3d_node src/detection_3d_main.cpp)
target_link_libraries(detection_3d_node ${PROJECT_NAME})

add_executable(orientation_control_node src/orientation_control_main.cpp)
target_link_libraries(orientation_control_node ${PROJECT_NAME})


install(TARGETS
  ${PROJECT_NAME}
  nearest_obstacle_detector_node
  detection_2d_node
  detection_3d_node
  orientation_control_node
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
  sensor_msgs
  tf2_geometry_msgs
  tf2
  tf2_ros
  yolo_msgs
  vision_msgs
)

ament_package()