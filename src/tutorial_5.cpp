/**
 * tutorial 5
 * Compose behaviors using Subtree
 * https://www.behaviortree.dev/docs/tutorial-basics/tutorial_05_subtrees
 * https://www.gamedeveloper.com/programming/behavior-trees-for-ai-how-they-work#close-modal
 */

// BT
#include <behaviortree_cpp/bt_factory.h>

// STL
#include <string>

class CrossDoor
{
public:
  void registerNodes(BT::BehaviorTreeFactory& factory);

  // SUCCESS if door_open_ != true
  BT::NodeStatus isDoorClose();

  // SUCCESS if door_open_ == true
  BT::NodeStatus passThroughDoor();

  // SUCCESS after 3 attempts, will open a locked door
  BT::NodeStatus pickLock();

  // FAILURE if door locked
  BT::NodeStatus openDoor();

  // Will always open door
  BT::NodeStatus smashDoor();

private:
  bool door_open_  {false};
  bool door_locked_ {true};
  int pick_attempts {0};
};

// Helper method to make registering less painful for the user
void CrossDoor::registerNodes(BT::BehaviorTreeFactory& factory)
{
  factory.registerSimpleCondition("IsDoorClosed", [this](BT::TreeNode&){ return this->isDoorClose(); });
  factory.registerSimpleAction("PassThroughDoor", [this](BT::TreeNode&){ return this->passThroughDoor(); });
  factory.registerSimpleAction("OpenDoor", [this](BT::TreeNode&){ return this->openDoor(); });
  factory.registerSimpleAction("PickLock", [this](BT::TreeNode&){ return this->pickLock(); });
  factory.registerSimpleAction("SmashDoor", [this](BT::TreeNode&){ return this->smashDoor(); });
}

int main (int argc, char *argv[])
{
  BT::BehaviorTreeFactory factory;
  CrossDoor cross_door;
  cross_door.registerNodes(factory);

  // In this example a single xml contains multiple behaviortree
  // to determine which one is the "main one", we should first register
  // the xml and then allocate a specific tree, using its ID
  factory.registerBehaviorTreeFromFile("./config/behavior/tutorial_5.xml");
  auto tree = factory.createTree("MainTree");

  // Helper function to print the tree
  BT::printTreeRecursively(tree.rootNode());

  tree.tickWhileRunning();

  return EXIT_SUCCESS;
}
