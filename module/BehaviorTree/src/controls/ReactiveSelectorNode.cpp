#include "BehaviorTree/include/controls/ReactiveSelectorNode.h"

namespace BT {
NodeStatus ReactiveSelectorNode::tick(float delta) {
    const size_t children_count = m_children_nodes.size();

    // 设置当前节点正在运行
    setStatus(NodeStatus::RUNNING);

    for (size_t i = 0; i < children_count; ++i) {
        auto       child  = m_children_nodes[i];
        const auto status = child->update(delta);
        switch (status) {
        // 子节点返回正在运行，则中断除此节点以外的所有节点
        case NodeStatus::RUNNING:
            for (size_t j = 0; j < children_count; ++j) {
                if (j != i)
                    interruptChild(j);
            }
            return NodeStatus::RUNNING;
        // 子节点返回失败，则中断当前节点的运行，再尝试下一个节点
        case NodeStatus::FAILURE: {
            break;
        }
        // 子节点返回运行成功，选择器返回成功
        case NodeStatus::SUCCESS: {
            return NodeStatus::SUCCESS;
        }
        // 子节点运行过程中永远不能返回空闲状态
        case NodeStatus::IDLE: {
            throw LogicError("A child node must never return IDLE");
            break;
        }
        default:
            break;
        }
    }

    return NodeStatus::FAILURE;
}
}  // namespace BT