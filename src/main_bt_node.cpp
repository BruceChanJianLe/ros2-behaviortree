#include "ros2-behaviortree/bt_ros.hpp"
#include <memory>
#include <thread>

int main (int argc, char *argv[])
{
  rclcpp::init(argc, argv);

  // init node
  auto node = std::make_shared<bt_ros::NodeHandler>();
  // auto t = std::thread([node](){ rclcpp::spin(node); });
  rclcpp::spin(node);

  // init bt tree

  // start tree

  rclcpp::shutdown();

  return EXIT_SUCCESS;
}
