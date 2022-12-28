#include "BehaviorTree/include/controls/WhileDoElseNode.h"

namespace BT {
NodeStatus WhileDoElseNode::tick(float delta) {
    if (empty())
        return NodeStatus::SUCCESS;

    NodeStatus condition_status = m_children_nodes[0]->update(delta);
    switch (condition_status) {
    case NodeStatus::RUNNING:
        return NodeStatus::RUNNING;
    // 条件节点返回成功，则中断第三个节点，运行第二个子节点
    case NodeStatus::SUCCESS:
        if (m_children_nodes.size() < 2) {
            return NodeStatus::SUCCESS;
        }
        interruptChild(0);
        interruptChild(2);
        return m_children_nodes[1]->update(delta);
    // 条件节点返回失败，则中断第二个节点，运行第三个子节点
    case NodeStatus::FAILURE: {
        if (m_children_nodes.size() < 3) {
            return NodeStatus::FAILURE;
        }
        interruptChild(0);
        interruptChild(1);
        return m_children_nodes[2]->update(delta);
    }
    default:
        throw LogicError("A child node must never return IDLE");
    }
    return NodeStatus::SUCCESS;
}
}  // namespace BT