cmake_minimum_required(VERSION 3.8)
project(ros2-behaviortree)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_action REQUIRED)
find_package(rclcpp_lifecycle REQUIRED)
find_package(behaviortree_cpp REQUIRED)
find_package(behaviortree_ros2 REQUIRED) # git clone https://github.com/BehaviorTree/BehaviorTree.ROS2.git --branch humble
find_package(std_msgs REQUIRED)
find_package(std_srvs REQUIRED)

# BUILD
set(dependencies
  rclcpp
  rclcpp_action
  rclcpp_lifecycle
  behaviortree_cpp
  behaviortree_ros2
  std_msgs
  std_srvs
)

include_directories(include)

# Main behaviortree node
add_executable(main_bt_node
  ./src/main_bt_node.cpp
  ./src/bt_ros.cpp
)
ament_target_dependencies(main_bt_node ${dependencies})

# Tutorial 1
add_executable(tutorial_1
  ./src/tutorial_1.cpp
)
ament_target_dependencies(tutorial_1 ${dependencies})

# Tutorial 2
add_executable(tutorial_2
  ./src/tutorial_2.cpp
)
ament_target_dependencies(tutorial_2 ${dependencies})

# Tutorial 2 2
add_executable(tutorial_2_2
  ./src/tutorial_2_2.cpp
)
ament_target_dependencies(tutorial_2_2 ${dependencies})

# Tutorial 3
add_executable(tutorial_3
  ./src/tutorial_3.cpp
)
ament_target_dependencies(tutorial_3 ${dependencies})

# Tutorial 4
add_executable(tutorial_4
  ./src/tutorial_4.cpp
)
ament_target_dependencies(tutorial_4 ${dependencies})

# # Tutorial 5
# add_executable(tutorial_5
#   ./src/tutorial_5.cpp
# )
# ament_target_dependencies(tutorial_5 ${dependencies})

# INSTALL
install(TARGETS
  main_bt_node
  tutorial_1
  tutorial_2
  tutorial_2_2
  tutorial_3
  tutorial_4
  # tutorial_5
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
