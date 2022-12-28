#include "BehaviorTree/include/controls/IfThenElseNode.h"

namespace BT {
NodeStatus IfThenElseNode::tick(float delta) {
    if (empty())
        return NodeStatus::SUCCESS;

    if (0 == m_current_running_child_idx) {
        auto condition_status = m_children_nodes[0]->update(delta);
        switch (condition_status) {
        case NodeStatus::RUNNING:
            return NodeStatus::RUNNING;
        // 条件节点返回成功，则运行第二个子节点
        case NodeStatus::SUCCESS:
            if (m_children_nodes.size() < 2) {
                return NodeStatus::SUCCESS;
            }

            m_current_running_child_idx = 1;
            break;
        // 条件节点返回失败，则运行第三个子节点
        case NodeStatus::FAILURE: {
            if (m_children_nodes.size() < 3) {
                return NodeStatus::FAILURE;
            }
            m_current_running_child_idx = 2;
            break;
        }
        default:
            throw LogicError("A child node must never return IDLE");
        }
    }

    if (m_current_running_child_idx > 0) {
        NodeStatus child_status = m_children_nodes[m_current_running_child_idx]->update(delta);
        if (NodeStatus::RUNNING == child_status) {
            return NodeStatus::RUNNING;
        }
        else {
            interrupt();
            m_current_running_child_idx = 0;
            return child_status;
        }
    }
    return NodeStatus::SUCCESS;
}
}  // namespace BT