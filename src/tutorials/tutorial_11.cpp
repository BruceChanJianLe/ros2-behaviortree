/**
 * tutorial 11
 * Mock testing in BT.CPP
 * https://www.behaviortree.dev/docs/tutorial-basics/tutorial_11_replace_rules
 */

// BT
#include <behaviortree_cpp/bt_factory.h>

// STL
#include <chrono>
#include <fstream>
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

int main (int argc, char *argv[])
{
  BT::BehaviorTreeFactory factory;
  factory.registerNodeType<SaySomethingNode>("SaySomething");

  // Let's use lambda to register simple action node,
  // to create a dummy node that we can substitute to a given one
  factory.registerSimpleAction("TestAction",
      [](BT::TreeNode& self)
      {
        std::cout << "TestAction substituting: " << self.name() << std::endl;
        return BT::NodeStatus::SUCCESS;
      }
    );

  // Action that is meant to substitute SaySomething,
  // It will try to use the input port "message"
  factory.registerSimpleAction("TestSaySomething",
      [](BT::TreeNode& self)
      {
        auto msg = self.getInput<std::string>("message");
        if (!msg)
        {
          throw BT::RuntimeError("missing required input [message]: ", msg.error());
        }
        std::cout << "TestSaySomething: " << msg.value() << std::endl;
        return BT::NodeStatus::SUCCESS;
      }
    );

  // Pass "no_sub" as first arguement to avoid adding rules
  bool skip_substitution = (argc == 2) && (std::string(argv[1]).compare("no_sub") == 0);

  if (!skip_substitution)
  {
    // We can use a json file to configure the substitution rules
    // or do it manually
    bool const USE_JSON = true;
    // Uncommet to load from code
    // bool const USE_JSON = false;

    if (USE_JSON)
    {
      std::ifstream f("./config/behaviortree/tutorial_11/substitute_rules.json");
      std::stringstream buffer;
      buffer << f.rdbuf();
      factory.loadSubstitutionRuleFromJSON(buffer.str());
    }
    else
    {
      // Substitute nodes which matches this wildcard pattern with TestAction
      factory.addSubstitutionRule("mysub/action_*", "TestAction");
      // Substitute nodes with name [talk] with TestSaySomething
      factory.addSubstitutionRule("talk", "TestSaySomething");

      // This configuration will be passed to a TestNode
      BT::TestNodeConfig test_config;
      // Convert the node in asynchronous and wait 2000ms
      test_config.async_delay = std::chrono::milliseconds(2000);
      // Execute post condition, once completed
      test_config.post_script = " msg ='message SUBSTITUTED from code'";

      // Substitute the node with name [last_action] with a TestNode,
      // configured using test_config
      factory.addSubstitutionRule("last_action", test_config);
    }
  }

  // Create tree from file
  factory.registerBehaviorTreeFromFile("./config/behaviortree/tutorial_11/tutorial_11.xml");

  // During construction phase of tree, the substitution rules will be used to
  // initiate the test nodes, instead of the original ones!
  auto tree = factory.createTree("MainTree");
  tree.tickWhileRunning();

  return EXIT_SUCCESS;
}
