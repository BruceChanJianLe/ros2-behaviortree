/**
 * tutorial 4
 * Reactive and Asynchronous behaviors
 * https://www.behaviortree.dev/docs/tutorial-basics/tutorial_04_sequence
 */

// BT
#include <behaviortree_cpp/bt_factory.h>

// STL
#include <chrono>
#include <string>
#include <iostream>
#include <thread>

// Custom type
struct Pose2D
{
  float x, y, theta;
};

class MoveBaseActionNode : public BT::StatefulActionNode
{
public:
  // Any node with ports must have at least one constructor with this signature
  MoveBaseActionNode(const std::string& name, const BT::NodeConfig& config)
    : BT::StatefulActionNode(name, config)
  {}

  // It is mandatory to define this static method
  static BT::PortsList providedPorts()
  {
    return { BT::InputPort<Pose2D>("goal") };
  }

  // This method is invoked once in the beginning
  BT::NodeStatus onStart() override;

  // If onStart() returned RUNNING, then we will keep calling
  // this method until it return something different than RUNNING
  BT::NodeStatus onRunning() override;

  // Callback to execute if the action was aborted by another node
  void onHalted() override;

private:
  Pose2D goal_;
  std::chrono::system_clock::time_point completion_time_;
};

// IMPLEMENTATION
BT::NodeStatus MoveBaseActionNode::onStart()
{
  if (!getInput<Pose2D>("goal", goal_))
  {
    throw BT::RuntimeError("missing required input [goal]");
  }
  std::cout << "[MoveBase: SEND REQUEST ]. goal: x=" << goal_.x
    << " y=" << goal_.y << " theta=" << goal_.theta << "\n";

  // We use this counter to simulate an action that takes a certain
  // amount of time to be completed (200ms)
  completion_time_ = std::chrono::system_clock::now() + std::chrono::milliseconds(200);

  return BT::NodeStatus::RUNNING;
}

BT::NodeStatus MoveBaseActionNode::onRunning()
{
  // Pretend that we are checking if the reply has been received
  // try no to block inside this function for too much time
  std::this_thread::sleep_for(std::chrono::milliseconds(10));

  // Pretend after a certain amount of time,
  // we have completed the operation
  if (std::chrono::system_clock::now() >= completion_time_)
  {
    std::cout << "[MoveBase: FINISHED]\n";
    return BT::NodeStatus::SUCCESS;
  }
  return BT::NodeStatus::RUNNING;
}

void MoveBaseActionNode::onHalted()
{
  std::cout << "[MoveBase: ABORTED]";
}

// To allow xml loader to instantiate a `Position2D` from a string, we need to provide
// a template specialization of `BT::convertFromString<Position2D>(StringView)`
namespace BT
{
  template <> inline Pose2D convertFromString(StringView str)
  {
    // We expect real numbers separeted by semicolons
    auto parts = splitString(str, ';');
    if (3 != parts.size())
    {
      throw RuntimeError("invalid input");
    }
    else
    {
      return {convertFromString<float>(parts[0]), convertFromString<float>(parts[1]), convertFromString<float>(parts[2])};
    }
  }
} // end namespace BT

// Simple funciton
BT::NodeStatus CheckBattery()
{
  std::cout << "[Lambda Method] Battery: OK" << '\n';
  return BT::NodeStatus::SUCCESS;
}

class SaySomethingNode : public BT::SyncActionNode
{
public:
  // Note that if your node has ports, then it is necessary to use this constructor signature
  SaySomethingNode(const std::string& name, const BT::NodeConfig& config)
    : BT::SyncActionNode(name, config)
  {}

  // It is necessary to define a STATIC method
  static BT::PortsList providedPorts()
  {
    return { BT::InputPort<std::string>("message") };
  }

  // Override the virtual function tick()
  BT::NodeStatus tick() override
  {
    BT::Expected<std::string> msg = getInput<std::string>("message");
    // Check if expected msg is valid. If not, throw corresponding error.
    if (!msg)
    {
      throw BT::RuntimeError("missing required input [message]: ", msg.error());
    }
    // Use the msg value
    std::cout << "Robot says: " << msg.value() << '\n';
    return BT::NodeStatus::SUCCESS;
  }
};

int main (int argc, char *argv[])
{
  BT::BehaviorTreeFactory factory;
  factory.registerSimpleCondition("BatteryOK", [&](BT::TreeNode&){ return CheckBattery(); });
  factory.registerNodeType<MoveBaseActionNode>("MoveBase");
  factory.registerNodeType<SaySomethingNode>("SaySomething");

  auto tree = factory.createTreeFromFile("./config/tree/tutorial_4.xml");

  // Here instead of tree.tickWhileRunning();
  // we prefer our own loop
  std::cout << "--- ticking\n";
  auto status = tree.tickOnce();
  std::cout << "--- status: " << BT::toStr(status) << "\n\n";

  while (BT::NodeStatus::RUNNING == status)
  {
    // Sleep to aviod busy loops.
    // do NOT use other sleep functions!
    // Small sleep time is alright, for demo purposes we use 
    // a larger sleep time to have less messages to the console
    tree.sleep(std::chrono::milliseconds(40));

    std::cout << "--- ticking\n";
    status = tree.tickOnce();
    std::cout << "--- status: " << BT::toStr(status) << "\n\n";
  }

  return EXIT_SUCCESS;
}
