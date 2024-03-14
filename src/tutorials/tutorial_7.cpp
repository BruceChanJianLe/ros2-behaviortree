/**
 * tutorial 7
 * Using multiple xml files
 * https://www.behaviortree.dev/docs/tutorial-basics/tutorial_07_multiple_xml
 */

// BT
#include <behaviortree_cpp/bt_factory.h>

// STL
#include <filesystem>
#include <string>
#include <iostream>

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

  // Find all xml files in a folder and register all of them
  // we will use std::filesystem::directory_iterator
  std::string search_directory {"./config/behaviortree/tutorial_7"};

  for (auto const& entry : std::filesystem::directory_iterator(search_directory))
  {
    if (entry.path().extension() == ".xml")
    {
      std::cout << "Loading: " << entry.path().string() << std::endl;
      factory.registerBehaviorTreeFromFile(entry.path().string());
    }
  }

  // This, in our specific case, would be equivalent to
  // factory.registerBehaviorTreeFromFile("./config/behaviortree/tutorial_7/main_tree.xml");
  // factory.registerBehaviorTreeFromFile("./config/behaviortree/tutorial_7/subtree_A.xml");
  // factory.registerBehaviorTreeFromFile("./config/behaviortree/tutorial_7/subtree_B.xml");

  // You can create the main tree and the subtree will be added automatically
  std::cout << "\n--- MainTree ---" << std::endl;
  auto main_tree = factory.createTree("MainTree");
  main_tree.tickWhileRunning();

  // alternatively, you can create only one of the subtrees
  std::cout << "\n--- SubTreeA ---" << std::endl;
  auto subtree_a = factory.createTree("SubTreeA");
  subtree_a.tickWhileRunning();

  return EXIT_SUCCESS;
}
