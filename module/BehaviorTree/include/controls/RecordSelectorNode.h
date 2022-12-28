#pragma once

#include "BehaviorTree/include/controls/ControlNode.h"

namespace BT {
/**
 * @brief 记录选择节点：从左向右开始尝试运行子节点，成功的子节点，将作为下一次最后选择的对象。
 * — 可以运行就继续运行该节点，直到运行完成；
 * — 不可以运行就尝试下一个节点；
 * — 所有节点均不可运行，返回失败；
 * — 完成后不重置记录节点，下次运行，将从记录节点的下一个节点开始选择。
 */
class RecordSelectorNode : public ControlNode {
public:
    RecordSelectorNode() {}
    virtual ~RecordSelectorNode() override = default;

    virtual void interrupt() override {
        curIdxLoop();
        m_record_times = 0;
        ControlNode::interrupt();
    }

protected:
    virtual NodeStatus tick(float delta) override;

private:
    void curIdxLoop() {
        m_current_running_child_idx = m_current_running_child_idx + 1 >= childrenCount() ? 0 : m_current_running_child_idx + 1;
    }

    size_t m_current_running_child_idx = 0;  // 当前正在运行的子节点的索引
    size_t m_record_times              = 0;  // 记录已尝试子节点的数量，当数量等于子节点总数时，返回运行失败
};

}  // namespace BT
