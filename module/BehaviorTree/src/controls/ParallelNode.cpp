#include "BehaviorTree/include/controls/ParallelNode.h"

namespace BT {
NodeStatus ParallelNode::tick(float delta) {
    const int children_count       = m_children_nodes.size();
    int       success_childred_num = 0;
    int       failure_childred_num = 0;

    for (int i = 0; i < children_count; ++i) {
        Node* child_node = m_children_nodes[i];

        // 判断该索引下的节点，是否在运行完毕列表中
        bool in_skip_list = (m_skip_list.count(i) != 0);

        NodeStatus child_status;
        if (in_skip_list) {
            child_status = child_node->status();
        }
        else {
            child_status = child_node->update(delta);
        }

        switch (child_status) {
        case NodeStatus::SUCCESS: {
            if (!in_skip_list) {
                m_skip_list.insert(i);
            }
            success_childred_num++;

            if (success_childred_num == success_threshold()) {
                m_skip_list.clear();
                interrupt();
                return NodeStatus::SUCCESS;
            }
        } break;
        case NodeStatus::FAILURE: {
            if (!in_skip_list) {
                m_skip_list.insert(i);
            }
            failure_childred_num++;

            // 注意：失败节点数量 > 总节点数 - 成功节点数，将永远不可能再执行成功，因此此时返回失败。
            if ((failure_childred_num > children_count - success_threshold()) || (failure_childred_num == failure_threshold())) {
                m_skip_list.clear();
                interrupt();
                return NodeStatus::FAILURE;
            }
        } break;
        case NodeStatus::RUNNING: {
        } break;
        default: {
            throw LogicError("A child node must never return IDLE");
        }
        }
    }

    return NodeStatus::RUNNING;
}
}  // namespace BT
