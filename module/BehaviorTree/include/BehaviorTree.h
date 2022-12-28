#pragma once

#include <vector>
#include "BehaviorTree/include/actions/ActionNode.h"
#include "BehaviorTree/include/controls/SequenceNode.h"
#include "BehaviorTree/include/controls/SelectorNode.h"
#include "BehaviorTree/include/controls/ParallelNode.h"
#include "BehaviorTree/include/controls/IfThenElseNode.h"
#include "BehaviorTree/include/controls/WhileDoElseNode.h"
#include "BehaviorTree/include/controls/ProbabilitySelectorNode.h"
#include "BehaviorTree/include/controls/ReactiveSelectorNode.h"
#include "BehaviorTree/include/controls/RandomSelectorNode.h"
#include "BehaviorTree/include/controls/RecordSelectorNode.h"
#include "BehaviorTree/include/decorators/ForceFailureNode.h"
#include "BehaviorTree/include/decorators/ForceSuccessNode.h"
#include "BehaviorTree/include/decorators/DelayNode.h"
#include "BehaviorTree/include/decorators/InverterNode.h"
#include "BehaviorTree/include/decorators/KeepRunningUntilFailureNode.h"
#include "BehaviorTree/include/decorators/RepeatNode.h"
#include "BehaviorTree/include/decorators/RetryNode.h"

namespace BT {
class BehaviorTree {
public:
    std::vector<Node::Ptr> nodes;
    BlackBoard::Ptr        blackBoard;

    BehaviorTree() {}

    ~BehaviorTree() {
        interrupt();
    }

    BehaviorTree(const BehaviorTree&)            = delete;
    BehaviorTree& operator=(const BehaviorTree&) = delete;

    BehaviorTree(BehaviorTree&& other) {
        (*this) = std::move(other);
    }

    BehaviorTree& operator=(BehaviorTree&& other) {
        nodes      = std::move(other.nodes);
        blackBoard = std::move(other.blackBoard);
        return *this;
    }

    /// @brief 执行行为树
    /// @param delta 时差
    /// @return 结果状态
    NodeStatus update(float delta) {
        if (nullptr == rootNode()) {
            return NodeStatus::FAILURE;
        }

        // 从根节点开始执行
        NodeStatus stat = rootNode()->update(delta);
        // 执行完成，重置为空闲状态
        if (stat == NodeStatus::SUCCESS || stat == NodeStatus::FAILURE) {
            rootNode()->setStatus(BT::NodeStatus::IDLE);
        }
        return stat;
    }

    /// @brief 获取行为树根节点
    /// @return 根节点
    Node* rootNode() {
        return nodes.empty() ? nullptr : nodes.front().get();
    }

    /// @brief 中断树的运行
    void interrupt() {
        if (nullptr == rootNode()) {
            return;
        }
        rootNode()->interrupt();
        rootNode()->setStatus(BT::NodeStatus::IDLE);
    }

private:
};
}  // namespace BT
