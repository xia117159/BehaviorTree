#pragma once

#include "BehaviorTree/include/decorators/DecoratorNode.h"

namespace BT {
/// @brief 延时节点：无子节点时，延时指定毫秒数后自动返回运行成功
class DelayNode : public DecoratorNode {
public:
    DelayNode(NodeConfiguration& config)
        : DecoratorNode(config) {
        BT_VARIABLE_INIT_BY_SETTING(config.settings, m_delay_msec);
    }
    virtual ~DelayNode() override = default;

    virtual void interrupt() {
        reset();
        DecoratorNode::interrupt();
    }

private:
    virtual NodeStatus tick(float delta) override;

    void reset() {
        m_running_msec = 0;
        m_is_started   = false;
        m_is_completed = false;
    }

    int64_t m_delay_msec   = 0;  // 延时的毫秒数
    int64_t m_running_msec = 0;
    bool    m_is_started   = false;
    bool    m_is_completed = false;
};

}  // namespace BT
