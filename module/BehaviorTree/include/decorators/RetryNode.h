#pragma once

#include "BehaviorTree/include/decorators/DecoratorNode.h"

namespace BT {
/// @brief 重复尝试指定次数，直到达到次数或子节点运行成功。未设置子节点时，直接返回成功。有效值：>= 1，无效值等同于 1
class RetryNode : public DecoratorNode {
public:
    RetryNode(NodeConfiguration& config)
        : DecoratorNode(config) {
        BT_VARIABLE_INIT_BY_SETTING(config.settings, m_attempts_count);
        if (m_attempts_count <= 0)
            m_attempts_count = 1;
    }
    virtual ~RetryNode() override = default;

    virtual void interrupt() override {
        m_tried_count = 0;
        DecoratorNode::interrupt();
    }

private:
    virtual NodeStatus tick(float delta) override {
        if (!m_child_node)
            return NodeStatus::RUNNING;
        while (m_tried_count < m_attempts_count) {
            const auto child_status = m_child_node->update(delta);
            switch (child_status) {
            case NodeStatus::RUNNING:
                return NodeStatus::RUNNING;
            case NodeStatus::FAILURE: {
                ++m_tried_count;
                interruptChild();
                return NodeStatus::RUNNING;
            }
            case NodeStatus::SUCCESS:
                m_tried_count = 0;
                interruptChild();
                return NodeStatus::SUCCESS;
            default:
                throw LogicError("A child node must never return IDLE");
            }
        }
        m_tried_count = 0;
        return NodeStatus::FAILURE;
    }

    int32_t m_attempts_count = 1;
    int32_t m_tried_count    = 0;
};

}  // namespace BT
