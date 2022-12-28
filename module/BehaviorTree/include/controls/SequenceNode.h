#pragma once

#include "BehaviorTree/include/controls/ControlNode.h"

namespace BT {
/// @brief 序列执行节点
class SequenceNode : public ControlNode {
public:
    /// @brief 序列节点构造函数
    SequenceNode() {}

    /// @brief 序列节点析构函数
    virtual ~SequenceNode() override = default;

    virtual void interrupt() override {
        m_current_running_child_idx = 0;
        ControlNode::interrupt();
    }

protected:
    virtual NodeStatus tick(float delta) override;

private:
    size_t m_current_running_child_idx = 0;  // 当前正在运行的子节点的索引
};

};  // namespace BT
