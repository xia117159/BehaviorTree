#pragma once

#include "BehaviorTree/include/controls/ControlNode.h"

namespace BT {
/// @brief 标准选择节点：从左向右开始尝试，可以运行就继续运行该节点，直到运行完成；不可以运行就尝试下一个节点；所有节点均不可运行，返回失败
class SelectorNode : public ControlNode {
public:
    SelectorNode() {}
    virtual ~SelectorNode() override = default;

    virtual void interrupt() override {
        m_current_running_child_idx = 0;
        ControlNode::interrupt();
    }

protected:
    virtual NodeStatus tick(float delta) override;

private:
    size_t m_current_running_child_idx = 0;  // 当前正在运行的子节点的索引
};

}  // namespace BT
