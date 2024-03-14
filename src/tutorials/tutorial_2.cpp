/**
 * tutorial 2
 * Blackboard and ports
 * https://www.behaviortree.dev/docs/tutorial-basics/tutorial_02_basic_ports
 */

// BT
#include <behaviortree_cpp/bt_factory.h>

// STL
#include <string>

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

class ThinkWhatToSayNode : public BT::SyncActionNode
{
public:
  ThinkWhatToSayNode(const std::string& name, const BT::NodeConfig& config)
    : BT::SyncActionNode(name, config)
  {}

  static BT::PortsList providedPorts()
  {
    return { BT::OutputPort<std::string>("text") };
  }

  // This node writes a value into the port "text"
  BT::NodeStatus tick() override
  {
    // the output may change at each tick(). Here we keep it simple
    setOutput("text", "This is a text message!");
    return BT::NodeStatus::SUCCESS;
  }
};

int main (int argc, char *argv[])
{
  BT::BehaviorTreeFactory factory;
  factory.registerNodeType<SaySomethingNode>("SaySomething");
  factory.registerNodeType<ThinkWhatToSayNode>("ThinkWhatToSay");

  auto tree = factory.createTreeFromFile("./config/behaviortree/tutorial_2.xml");
  tree.tickWhileRunning();

  return EXIT_SUCCESS;
}
