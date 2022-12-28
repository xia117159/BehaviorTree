#include "BehaviorTree/include/decorators/DelayNode.h"

namespace BT {
NodeStatus DelayNode::tick(float delta) {
    if (m_is_completed) {
        if (m_child_node) {
            const auto child_status = m_child_node->update(delta);
            if (NodeStatus::RUNNING != child_status) {
                interrupt();
            }
            return child_status;
        }
        reset();
        return NodeStatus::SUCCESS;
    }

    if (!m_is_started) {
        m_is_started   = true;
        m_is_completed = false;
        m_running_msec = 0;
    }

    if (m_is_started) {
        m_running_msec += delta * 1000.0f;
    }

    if (m_running_msec >= m_delay_msec) {
        m_is_completed = true;
    }

    if (m_is_completed) {
        if (m_child_node) {
            const auto child_status = m_child_node->update(delta);
            if (NodeStatus::RUNNING != child_status) {
                interrupt();
            }
            return child_status;
        }
        reset();
        return NodeStatus::SUCCESS;
    }

    return NodeStatus::RUNNING;
}
}  // namespace BT