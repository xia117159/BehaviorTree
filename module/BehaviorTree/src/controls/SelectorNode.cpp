#include "BehaviorTree/include/controls/SelectorNode.h"

namespace BT {
NodeStatus SelectorNode::tick(float delta) {
    const size_t children_count = m_children_nodes.size();

    // 设置当前节点正在运行
    setStatus(NodeStatus::RUNNING);

    while (m_current_running_child_idx < children_count) {
        auto       child  = m_children_nodes[m_current_running_child_idx];
        const auto status = child->update(delta);
        switch (status) {
        // 子节点返回正在运行，则直接返回，等到下一个tick继续运行
        case NodeStatus::RUNNING:
            return NodeStatus::RUNNING;
        // 子节点返回失败，则中断当前节点的运行，再尝试下一个节点
        case NodeStatus::FAILURE: {
            interruptChild(m_current_running_child_idx);
            m_current_running_child_idx++;
            break;
        }
        // 子节点返回运行成功，选择器返回成功
        case NodeStatus::SUCCESS: {
            interrupt();
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

    // 运行完成最后一个节点，则中断运行，并重置到索引
    if (m_current_running_child_idx == children_count) {
        interrupt();
    }

    return NodeStatus::FAILURE;
}
}  // namespace BT