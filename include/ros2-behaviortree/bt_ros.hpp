#ifndef ROS2_BEHAVIORTREE_BT_ROS_HPP
#define ROS2_BEHAVIORTREE_BT_ROS_HPP

// ROS2
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/bool.hpp>
#include <std_msgs/msg/string.hpp>

// STL
#include <string>
#include <optional>

#include "ros2-behaviortree/common_defs.hpp"

namespace bt_ros
{
  class NodeHandler : public rclcpp::Node
  {
  public:
    NodeHandler();
    ~NodeHandler();
    void startCondition();
    std::optional<bool> getCondition();
    void publishState(const std::string& state);

  private:
    bool update_, condition_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_;
    rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr sub_;
  };
} // bt_ros
#endif /* ROS2_BEHAVIORTREE_BT_ROS_HPP */
