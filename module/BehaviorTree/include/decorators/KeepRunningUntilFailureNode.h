#pragma once

#include "BehaviorTree/include/decorators/DecoratorNode.h"

namespace BT {
/// @brief 一直运行，直到子节点返回失败，当前节点就返回失败。没有子节点时，直接返回失败
class KeepRunningUntilFailureNode : public DecoratorNode {
public:
    KeepRunningUntilFailureNode() {}
    virtual ~KeepRunningUntilFailureNode() override = default;

private:
    virtual NodeStatus tick(float delta) override {
        if (!m_child_node)
            return NodeStatus::FAILURE;
        const auto child_status = m_child_node->update(delta);
        switch (child_status) {
        case NodeStatus::RUNNING:
            return NodeStatus::RUNNING;
        case NodeStatus::FAILURE: {
            interruptChild();
            return NodeStatus::FAILURE;
        }
        case NodeStatus::SUCCESS:
            interruptChild();
            return NodeStatus::RUNNING;
        default:
            throw LogicError("A child node must never return IDLE");
        }
    }
};

}  // namespace BT
