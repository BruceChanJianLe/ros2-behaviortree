/**
 * tutorial 1
 * My first behavior tree!
 * https://www.behaviortree.dev/docs/tutorial-basics/tutorial_01_first_tree
 */

// BT
#include "behaviortree_cpp/bt_factory.h"

// STL
#include <string>
#include <iostream>

// ROS2
#include <rclcpp/rclcpp.hpp>

// Example of custom synchronous action
// without ports!
class ApproachObject : public BT::SyncActionNode
{
public:
  ApproachObject(const std::string& name)
    : BT::SyncActionNode(name, {})
  {}

  BT::NodeStatus tick() override
  {
    std::cout << "[Inherit Method] ApproachObject Node: " << this->name() << '\n';
    return BT::NodeStatus::SUCCESS;
  }
};

// Simple funciton
BT::NodeStatus CheckBattery()
{
  std::cout << "[Lambda Method] Battery: OK" << '\n';
  return BT::NodeStatus::SUCCESS;
}

class GripperInterface
{
public:
  GripperInterface() : is_open_{false} {}
  ~GripperInterface() {}

  BT::NodeStatus openGripper()
  {
    is_open_ = true;
    std::cout << "[Class Method] GripperInterface::open" << '\n';
    return BT::NodeStatus::SUCCESS;
  }

  BT::NodeStatus closeGripper()
  {
    is_open_ = false;
    std::cout << "[Class Method] GripperInterface::close" << '\n';
    return BT::NodeStatus::SUCCESS;
  }

private:
  bool is_open_;
};

int main (int argc, char *argv[])
{
  // We use the behaviortree factory to register the custom nodes
  BT::BehaviorTreeFactory factory; 

  // The recommended way to create a Node is through inheritance.
  factory.registerNodeType<ApproachObject>("ApproachObject");

  // Registering a SimpleActionNode using a funciton pointer
  // Use lambda or std::bind
  factory.registerSimpleCondition("CheckBattery", [&](BT::TreeNode&){ return CheckBattery(); });

  // Create SimpleActionNode from methods of a class
  GripperInterface gripper;
  factory.registerSimpleAction("OpenGripper", [&](BT::TreeNode&){ return gripper.openGripper(); });
  factory.registerSimpleAction("CloseGripper", [&](BT::TreeNode&){ return gripper.closeGripper(); });

  // Trees are created at run-time, but only once in the beginning.
  // IMPORTANT: when the object tree goes out of scope, all the TreeNodes are destroyed
  auto tree = factory.createTreeFromFile("./config/tree/tutorial_1.xml");

  // To execute a tree, you will need to tick it
  // Ticking the tree will propagate to children based on the logic of the tree
  // In this case, the entire sequence is executed, because all the children of the Sequence return SUCCESS
  tree.tickWhileRunning();

  return EXIT_SUCCESS;
}
