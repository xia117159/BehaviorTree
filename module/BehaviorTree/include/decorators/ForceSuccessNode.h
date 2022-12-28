#pragma once

#include "BehaviorTree/include/decorators/DecoratorNode.h"

namespace BT {
/// @brief 无视子节点运行结果，强制返回运行成功。没有子节点时，直接返回成功
class ForceSuccessNode : public DecoratorNode {
public:
    ForceSuccessNode() {}
    virtual ~ForceSuccessNode() override = default;

private:
    virtual NodeStatus tick(float delta) override {
        if (!m_child_node)
            return NodeStatus::SUCCESS;
        const auto child_status = m_child_node->update(delta);
        switch (child_status) {
        case NodeStatus::RUNNING: return NodeStatus::RUNNING;
        case NodeStatus::FAILURE:
        case NodeStatus::SUCCESS:
            interruptChild();
            return NodeStatus::SUCCESS;
        default:
            throw LogicError("A child node must never return IDLE");
        }
    }
};

}  // namespace BT
