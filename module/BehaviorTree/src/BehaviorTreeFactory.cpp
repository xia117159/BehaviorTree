#include "BehaviorTree/include/BehaviorTreeFactory.h"
#include "BehaviorTree/include/Parser.h"

namespace BT {
BehaviorTreeFactory::BehaviorTreeFactory() {
    registerNodeType<SequenceNode>(kSequenceNodeName);
    registerNodeType<SelectorNode>(kSelectorNodeName);
    registerNodeType<ParallelNode>(kParallelNodeName);
    registerNodeType<RecordSelectorNode>(kRecordSelectorNodeName);
    registerNodeType<IfThenElseNode>(kIfThenElseNodeName);
    registerNodeType<WhileDoElseNode>(kWhileDoElseNodeName);
    registerNodeType<ReactiveSelectorNode>(kReactiveSelectorNodeName);
    registerNodeType<RandomSelectorNode>(kRandomSelectorNodeName);

    registerNodeType<ForceSuccessNode>(kForceSuccessNodeName);
    registerNodeType<ForceFailureNode>(kForceFailureNodeName);
    registerNodeType<DelayNode>(kDelayNodeName);
    registerNodeType<RepeatNode>(kRepeatNodeName);
    registerNodeType<InverterNode>(kInverterNodeName);
    registerNodeType<KeepRunningUntilFailureNode>(kKeepRunningUntilFailureNodeName);
    registerNodeType<RetryNode>(kRetryNodeName);

    addCustomNodes();
}

BehaviorTree BehaviorTreeFactory::createBehaviorTreeFromText(const std::string& text) {
    BehaviorTree      tree;
    NodeConfiguration config;

    Node::Ptr root = instantiateTreeNode("Sequence", config);
    tree.nodes.push_back(root);
    SequenceNode* sequence = dynamic_cast<SequenceNode*>(root.get());

    Node::Ptr node1 = instantiateTreeNode("SaySomething", config);
    tree.nodes.push_back(node1);
    sequence->addChild(node1.get());

    Node::Ptr node2 = instantiateTreeNode("OpenDoor", config);
    tree.nodes.push_back(node2);
    sequence->addChild(node2.get());

    Node::Ptr node3 = instantiateTreeNode("MoveOut", config);
    tree.nodes.push_back(node3);
    sequence->addChild(node3.get());

    return std::move(tree);
}

BehaviorTree BehaviorTreeFactory::createBehaviorTreeFromXml(const std::string& filename, BlackBoard::Ptr blackBoard) {
    XmlParser parser;
    return parser.createBehaviorTreeFromXml(filename, blackBoard);
}

Node::Ptr BehaviorTreeFactory::instantiateTreeNode(const std::string& key, NodeConfiguration& config) {
    auto iter = _builders.find(key);
    if (iter == _builders.end()) {
        throw RuntimeError("BehaviorTreeFactory: node [" + key + "] not registered");
    }
    return iter->second(config);
}
}  // namespace BT
