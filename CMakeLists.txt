cmake_minimum_required(VERSION 3.8)
project(ros2-behaviortree)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# BUILD
set(dependencies
  rclcpp
  rclcpp_action
  rclcpp_lifecycle
  behaviortree_cpp
  # behaviortree_ros2 # git clone https://github.com/BehaviorTree/BehaviorTree.ROS2.git --branch humble
  std_msgs
  std_srvs
  ament_index_cpp
)

# find dependencies
foreach(pkg IN LISTS dependencies)
  find_package(${pkg} REQUIRED)
endforeach()

include_directories(include)

# Main behaviortree node
add_executable(main_bt_node
  ./src/main_bt_node.cpp
  ./src/bt_ros.cpp
)
ament_target_dependencies(main_bt_node ${dependencies})

set(TUTORIAL_EXECUTABLES
  tutorial_1
  tutorial_2
  tutorial_2_2
  tutorial_3
  tutorial_4
  tutorial_4_3
  tutorial_4_4
  tutorial_5
  tutorial_6
  tutorial_7
  tutorial_8
  tutorial_9
  tutorial_10
  tutorial_11
)

foreach(tutorial_executable IN LISTS TUTORIAL_EXECUTABLES)
  add_executable(${tutorial_executable} ./src/tutorials/${tutorial_executable}.cpp)
  ament_target_dependencies(${tutorial_executable} ${dependencies})
endforeach()


# INSTALL
install(TARGETS
  main_bt_node
  ${TUTORIAL_EXECUTABLES}
  RUNTIME DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY
  launch
  config
  DESTINATION share/${PROJECT_NAME}/
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
