#include "ros2-behaviortree/bt_ros.hpp"
#include "ros2-behaviortree/common_defs.hpp"

namespace bt_ros
{
  NodeHandler::NodeHandler()
    : Node("main_bt_node")
    , update_{false}
    , condition_{false}
  {
    RCLCPP_INFO_STREAM(get_logger(), "MAIN BT ROS NODE STARTED!");

    pub_ = create_publisher<std_msgs::msg::String>("bt/state", 5);
    sub_ = create_subscription<std_msgs::msg::Bool>(
        "bt/feedback",
        5,
        [this](const std_msgs::msg::Bool::SharedPtr msg)
        {
          this->update_ = true;
          this->condition_ = msg->data;
        }
      );
  }

  NodeHandler::~NodeHandler()
  {
  }

  void NodeHandler::startCondition()
  {
    update_ = false;
    condition_ = false;
  }

  std::optional<bool> NodeHandler::getCondition()
  {
    if (update_)
    {
      return {condition_};
    }
    else
    {
      return {};
    }
  }

  void NodeHandler::publishState(const std::string& state)
  {
    std_msgs::msg::String msg;
    msg.data = state;
    pub_->publish(msg);
  }
} // bt_ros
