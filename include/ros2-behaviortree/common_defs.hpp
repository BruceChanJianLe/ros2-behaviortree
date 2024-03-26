#ifndef ROS2_BEHAVIORTREE_COMMON_DEFS_HPP
#define ROS2_BEHAVIORTREE_COMMON_DEFS_HPP

#include <string_view>

namespace common
{

  // Conditions
  static std::string_view CONDITION_NAME_1 = "Ball Found";
  static std::string_view CONDITION_NAME_2 = "Ball Close";
  static std::string_view CONDITION_NAME_3 = "Ball Grasped";
  static std::string_view CONDITION_NAME_4 = "Bin Close";
  static std::string_view CONDITION_NAME_5 = "Ball Placed";

  // Action
  static std::string_view ACTION_NAME_1 = "Find Found";
  static std::string_view ACTION_NAME_2 = "Approach Ball";
  static std::string_view ACTION_NAME_3 = "Grasp Ball";
  static std::string_view ACTION_NAME_4 = "Approach Bin";
  static std::string_view ACTION_NAME_5 = "Place Ball";
} // common

#endif /* ROS2_BEHAVIORTREE_COMMON_DEFS_HPP */
