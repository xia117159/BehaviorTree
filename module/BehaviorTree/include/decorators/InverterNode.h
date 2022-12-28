#pragma once

#include "BehaviorTree/include/decorators/DecoratorNode.h"

namespace BT {
/// @brief 逆变器装饰器：当子节点运行失败时，返回运行成功；当子节点运行成功时，返回运行失败；
class InverterNode : public DecoratorNode {
public:
    InverterNode() {}
    virtual ~InverterNode() override = default;

private:
    virtual NodeStatus tick(float delta) override {
        if (!m_child_node)
            return NodeStatus::SUCCESS;
        const auto child_status = m_child_node->update(delta);
        switch (child_status) {
        case NodeStatus::RUNNING:
            return NodeStatus::RUNNING;
        case NodeStatus::FAILURE: {
            interruptChild();
            return NodeStatus::SUCCESS;
        }
        case NodeStatus::SUCCESS:
            interruptChild();
            return NodeStatus::FAILURE;
        default:
            throw LogicError("A child node must never return IDLE");
        }
    }
};

}  // namespace BT
