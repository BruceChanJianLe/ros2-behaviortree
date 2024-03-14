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
  ./src/tutorials/tutorial_1.cpp
)
ament_target_dependencies(tutorial_1 ${dependencies})

# Tutorial 2
add_executable(tutorial_2
  ./src/tutorials/tutorial_2.cpp
)
ament_target_dependencies(tutorial_2 ${dependencies})

# Tutorial 2 2
add_executable(tutorial_2_2
  ./src/tutorials/tutorial_2_2.cpp
)
ament_target_dependencies(tutorial_2_2 ${dependencies})

# Tutorial 3
add_executable(tutorial_3
  ./src/tutorials/tutorial_3.cpp
)
ament_target_dependencies(tutorial_3 ${dependencies})

# Tutorial 4
add_executable(tutorial_4
  ./src/tutorials/tutorial_4.cpp
)
ament_target_dependencies(tutorial_4 ${dependencies})

# Tutorial 5
add_executable(tutorial_5
  ./src/tutorials/tutorial_5.cpp
)
ament_target_dependencies(tutorial_5 ${dependencies})

# Tutorial 6
add_executable(tutorial_6
  ./src/tutorials/tutorial_6.cpp
)
ament_target_dependencies(tutorial_6 ${dependencies})

# Tutorial 7
add_executable(tutorial_7
  ./src/tutorials/tutorial_7.cpp
)
ament_target_dependencies(tutorial_7 ${dependencies})

# Tutorial 8
add_executable(tutorial_8
  ./src/tutorials/tutorial_8.cpp
)
ament_target_dependencies(tutorial_8 ${dependencies})

# Tutorial 9
add_executable(tutorial_9
  ./src/tutorials/tutorial_9.cpp
)
ament_target_dependencies(tutorial_9 ${dependencies})

# Tutorial 10
add_executable(tutorial_10
  ./src/tutorials/tutorial_10.cpp
)
ament_target_dependencies(tutorial_10 ${dependencies})

# Tutorial 11
add_executable(tutorial_11
  ./src/tutorials/tutorial_11.cpp
)
ament_target_dependencies(tutorial_11 ${dependencies})

set(TUTORIAL_EXECUTABLES
  tutorial_1
  tutorial_2
  tutorial_2_2
  tutorial_3
  tutorial_4
  tutorial_5
  tutorial_6
  tutorial_7
  tutorial_8
  tutorial_9
  tutorial_10
  tutorial_11
)

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
