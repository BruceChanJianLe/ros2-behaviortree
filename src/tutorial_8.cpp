/**
 * tutorial 8
 * Add arguements to your consructor (recommended)
 * https://www.behaviortree.dev/docs/tutorial-basics/tutorial_08_additional_args
 */

// BT
#include <behaviortree_cpp/bt_factory.h>

// STL
#include <memory>
#include <string>
#include <iostream>

class SaySomethingElseNode : public BT::SyncActionNode
{
public:
  // Note that if your node has ports, then it is necessary to use this constructor signature
  SaySomethingElseNode(const std::string& name,
      const BT::NodeConfig& config,
      const std::string message1,
      const std::string message2
    )
    : BT::SyncActionNode(name, config)
    , message1_{message1}
    , message2_{message2}
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
    std::cout << "And it also says: " << message1_ << '\n';
    std::cout << "Along with: " << message2_ << '\n';
    return BT::NodeStatus::SUCCESS;
  }

  // We need to ensure this method is called ONCE and BEFORE the first tick!
  void initialize(const std::string message1, const std::string message2)
  {
    message1_ = message1;
    message2_ = message2;
  }

private:
  std::string message1_;
  std::string message2_;
};

int main (int argc, char *argv[])
{
  BT::BehaviorTreeFactory factory;
  factory.registerNodeType<SaySomethingElseNode>("SaySomethingElse", "message1", "message2");

  // Create tree from file
  auto tree = factory.createTreeFromFile("./config/behaviortree/tutorial_8.xml");

  // Call the original tree once
  std::cout << "--- Original Tree ---\n";
  tree.tickWhileRunning();

  // Create visitor lambda (method 1)
  auto visitor = [](BT::TreeNode* tn)
  {
    if (auto say_something_else_node = dynamic_cast<SaySomethingElseNode*>(tn))
    {
      say_something_else_node->initialize("message3!", "message4!");
    }
  };

  // Apply the visitor to ALL the nodes in tree
  tree.applyVisitor(visitor);

  std::cout << "\n--- First Visitor ---\n";
  tree.tickWhileRunning();

  // Create visitor lambda (method 2)
  tree.applyVisitor([](BT::TreeNode* tn)
    {
      if (auto say_something_else_node = dynamic_cast<SaySomethingElseNode*>(tn))
      {
        say_something_else_node->initialize("message4!", "message5!");
      }
    }
  );
  std::cout << "\n--- Second Visitor ---\n";
  tree.tickWhileRunning();

  return EXIT_SUCCESS;
}
