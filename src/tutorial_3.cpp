/**
 * tutorial 3
 * Generic ports
 * https://www.behaviortree.dev/docs/tutorial-basics/tutorial_03_generic_ports/
 */

// BT
#include <behaviortree_cpp/bt_factory.h>

// STL
#include <string>

// Custom type
struct Position2D
{
  float x;
  float y;
};

// To allow xml loader to instantiate a `Position2D` from a string, we need to provide
// a template specialization of `BT::convertFromString<Position2D>(StringView)`
namespace BT
{
  template <> inline Position2D convertFromString(StringView str)
  {
    // We expect real numbers separeted by semicolons
    auto parts = splitString(str, ';');
    if (2 != parts.size())
    {
      throw RuntimeError("invalid input");
    }
    else
    {
      return {convertFromString<float>(parts[0]), convertFromString<float>(parts[1])};
    }
  }
} // end namespace BT

class CalculateGoalNode : public BT::SyncActionNode
{
public:
  CalculateGoalNode(const std::string& name, const BT::NodeConfig& config)
    : BT::SyncActionNode(name, config)
  {}

  static BT::PortsList providedPorts()
  {
    return { BT::OutputPort<Position2D>("goal") };
  }

  BT::NodeStatus tick() override
  {
    Position2D mygoal {1.1f, 2.3f};
    setOutput<Position2D>("goal", mygoal);
    return BT::NodeStatus::SUCCESS;
  }
};

class PrintTargetNode : public BT::SyncActionNode
{
public:
  PrintTargetNode(const std::string& name, const BT::NodeConfig& config)
    : BT::SyncActionNode(name, config)
  {}

  static BT::PortsList providedPorts()
  {
    const char* description = "Simply print the goal on console...";
    return { BT::InputPort("target", description) };
  }

  BT::NodeStatus tick() override
  {
    auto res = getInput<Position2D>("target");
    if (!res)
    {
      throw BT::RuntimeError("Error reading port [target]: ", res.error());
    }
    Position2D target = res.value();
    std::cout << "Target position: (" << target.x << ", " << target.y << ")\n";
    return BT::NodeStatus::SUCCESS;
  }
};

static const char* xml_text = R"(
<root BTCPP_format="4">
  <BehaviorTree ID="MainTree">
    <Sequence name="root">
      <CalculateGoal goal="{GoalPosition}"/>
      <PrintTarget   target="{GoalPosition}"/>
      <Script        code=" OtherGoal:='-1;3' "/>
      <PrintTarget   target="{OtherGoal}"/>
    </Sequence>
  </BehaviorTree>
</root>
)";

int main (int argc, char *argv[])
{
  BT::BehaviorTreeFactory factory;
  factory.registerNodeType<CalculateGoalNode>("CalculateGoal");
  factory.registerNodeType<PrintTargetNode>("PrintTarget");

  auto tree = factory.createTreeFromText(xml_text);
  tree.tickWhileRunning();

  return EXIT_SUCCESS;
}
