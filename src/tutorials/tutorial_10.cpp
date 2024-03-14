/**
 * tutorial 10
 * The logger interface
 * https://www.behaviortree.dev/docs/tutorial-basics/tutorial_10_observer
 */

// BT
#include <behaviortree_cpp/bt_factory.h>
#include <behaviortree_cpp/loggers/bt_observer.h>

// STL
#include <string>
#include <map>

// class AlwaysSuccessNode : public BT::SyncActionNode
// {
// public:
//   // Note that if your node has ports, then it is necessary to use this constructor signature
//   AlwaysSuccessNode(const std::string& name)
//     : BT::SyncActionNode(name, {})
//   {}
//
//   // Override the virtual function tick()
//   BT::NodeStatus tick() override
//   {
//     return BT::NodeStatus::SUCCESS;
//   }
// };

// class AlwaysFailureNode : public BT::SyncActionNode
// {
// public:
//   // Note that if your node has ports, then it is necessary to use this constructor signature
//   AlwaysFailureNode(const std::string& name)
//     : BT::SyncActionNode(name, {})
//   {}
//
//   // Override the virtual function tick()
//   BT::NodeStatus tick() override
//   {
//     return BT::NodeStatus::FAILURE;
//   }
// };

int main (int argc, char *argv[])
{
  BT::BehaviorTreeFactory factory;
  // factory.registerNodeType<AlwaysSuccessNode>("AlwaysSuccess2");
  // factory.registerNodeType<AlwaysFailureNode>("AlwaysFailure2");

  // Create tree from file
  factory.registerBehaviorTreeFromFile("./config/behaviortree/tutorial_10.xml");
  auto tree = factory.createTree("MainTree");

  // Helper function to print the tree
  BT::printTreeRecursively(tree.rootNode());

  // The purpose of the observer is to save some statistic about the number of times
  // a certain node returns SUCCESS or FAILURE
  // This is particularly useful to create unit tests and check whether
  // a certain set of transitions has happened as expected
  BT::TreeObserver observer{tree};

  // Print the unique ID the corresponding human readable path
  // Path is also expected to be unique
  std::map<uint16_t, std::string> ordered_UID_to_path;
  for (const auto& [name, uid] : observer.pathToUID())
  {
    ordered_UID_to_path[uid] = name;
  }

  for (const auto& [uid, name] : ordered_UID_to_path)
  {
    std::cout << uid << " -> " << name << std::endl;
  }

  tree.tickWhileRunning();

  // You may access a specific statistic, using its full path or the UID
  const auto& last_action_stats = observer.getStatistics("last_action");
  assert(last_action_stats.transitions_count > 0);

  std::cout << "--------------------" << std::endl;
  // print all stats
  for (const auto& [uid, name] : ordered_UID_to_path)
  {
    const auto& stats = observer.getStatistics(uid);

    std::cout << "[" << name
        << "] \tT/S/F:  " << stats.transitions_count
        << "/" << stats.success_count
        << "/" << stats.failure_count
        << std::endl;
  }

  return EXIT_SUCCESS;
}
