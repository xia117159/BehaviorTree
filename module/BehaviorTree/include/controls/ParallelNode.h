#pragma once

#include <set>
#include "BehaviorTree/include/controls/ControlNode.h"

namespace BT {
/// @brief 并行节点：可设置成功或失败阈值，到达任意一个阈值时，节点完成。
/// 到达任意一个阈值时，正在运行的节点将会被中断运行；
/// 将同时运行所有的节点，已完成的节点将不会在这一轮中继续运行；
/// 默认成功和失败的阈值均为：1。
/// 成功节点数量设置范围为：[1, children_count]，<1 时，设为1，>children_count 时，设为children_count
/// 失败节点数量设置范围为: [0, children_count]，<0 时，设为0，>children_count 时，设为children_count
/// 失败判断额外有：失败节点数量 > children_count - success_threshold，满足此条件，说明成功永远不会发生。
class ParallelNode : public ControlNode {
public:
    ParallelNode(NodeConfiguration& config) {
        BT_VARIABLE_INIT_BY_SETTING(config.settings, m_success_threshold);
        BT_VARIABLE_INIT_BY_SETTING(config.settings, m_failure_threshold);
    }
    virtual ~ParallelNode() override = default;

    int success_threshold() const {
        return m_success_threshold;
    }
    void set_success_threshold(int value) {
        if (value < 1)
            m_success_threshold = 1;
        else if (value > childrenCount())
            m_success_threshold = childrenCount();
        else
            m_success_threshold = value;
    }
    int failure_threshold() const {
        return m_failure_threshold;
    }
    void set_failure_threshold(int value) {
        if (value < 0)
            m_failure_threshold = 0;
        else if (value > childrenCount())
            m_failure_threshold = childrenCount();
        else
            m_failure_threshold = value;
    }

    virtual void interrupt() override {
        m_skip_list.clear();
        ControlNode::interrupt();
    }

protected:
    virtual NodeStatus tick(float delta) override;

private:
    int m_success_threshold = 1;  // 标识并行执行成功节点数量的阈值，到达阈值时，节点返回执行成功
    int m_failure_threshold = 1;  // 标识并行执行失败节点数量的阈值，到达阈值时，节点返回执行失败

    std::set<int> m_skip_list;  // 已完成的节点集合
};

}  // namespace BT