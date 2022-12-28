#include "BehaviorTree/include/decorators/RepeatNode.h"

namespace BT {
NodeStatus RepeatNode::tick(float delta) {
    if (!m_child_node)
        return NodeStatus::SUCCESS;
    while (m_continuous_success_count < m_repeat_count) {
        const auto child_status = m_child_node->update(delta);
        switch (child_status) {
        case NodeStatus::RUNNING:
            return NodeStatus::RUNNING;
        case NodeStatus::FAILURE: {
            m_continuous_success_count = 0;
            interruptChild();
            return NodeStatus::FAILURE;
        }
        case NodeStatus::SUCCESS:
            ++m_continuous_success_count;
            interruptChild();
            return NodeStatus::RUNNING;
        default:
            throw LogicError("A child node must never return IDLE");
        }
    }
    m_continuous_success_count = 0;
    return NodeStatus::SUCCESS;
}
}  // namespace BT