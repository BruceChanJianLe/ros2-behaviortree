/**
 * tutorial 9
 * Introduction to the scripting language
 * https://www.behaviortree.dev/docs/tutorial-basics/tutorial_09_scripting
 */

// BT
#include <behaviortree_cpp/bt_factory.h>

// STL
#include <string>
#include <iostream>

enum Color
{
  RED = 1,
  BLUE = 2,
  GREEN = 3
};

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
  factory.registerNodeType<SaySomethingNode>("SaySomething");

  // We can add these enums to the scripting language.
  // Check the limits of magic_enum 
  // https://github.com/Neargye/magic_enum/blob/master/doc/limitations.md
  // A notable one is that the default range is [-128, 128], unless changes as described in the link above.
  factory.registerScriptingEnums<Color>();

  // Or we could manually assign a number to the label "THE_ANSWERA".
  // This is not affected by any range limitation
  factory.registerScriptingEnum("THE_ANSWER", 42);


  // Create tree from file
  auto tree = factory.createTreeFromFile("./config/behaviortree/tutorial_9.xml");
  tree.tickWhileRunning();

  return EXIT_SUCCESS;
}
