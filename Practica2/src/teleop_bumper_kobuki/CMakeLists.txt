cmake_minimum_required(VERSION 3.5)

project(teleop_bumper_kobuki)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(kobuki_ros_interfaces REQUIRED)

add_library(${PROJECT_NAME}
    src/teleop_bumper_kobuki/SubscriberBumperNode.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    rclcpp::rclcpp
    ${kobuki_ros_interfaces_TARGETS}
    ${geometry_msgs_TARGETS}
)

add_executable(sub_bumper src/subscriber_bumper_main.cpp)
target_link_libraries(sub_bumper ${PROJECT_NAME})

install(TARGETS
    ${PROJECT_NAME}
    sub_bumper
    EXPORT export_${PROJECT_NAME}
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION lib/${PROJECT_NAME}
)

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
    kobuki_ros_interfaces
)

ament_package()
