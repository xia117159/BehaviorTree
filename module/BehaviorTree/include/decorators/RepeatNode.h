#pragma once

#include "BehaviorTree/include/decorators/DecoratorNode.h"

namespace BT {
/// @brief 连续成功执行指定次数装饰器，连续执行中，任意时间子节点返回失败，当前节点就返回失败。
/// 设置有效值： >= 1，其它值等同于设置为 1
class RepeatNode : public DecoratorNode {
public:
    RepeatNode(NodeConfiguration& config)
        : DecoratorNode(config) {
        BT_VARIABLE_INIT_BY_SETTING(config.settings, m_repeat_count);
        if (m_repeat_count <= 0)
            m_repeat_count = 1;
    }
    virtual ~RepeatNode() override = default;

    virtual void interrupt() {
        m_continuous_success_count = 0;
        DecoratorNode::interrupt();
    }

private:
    virtual NodeStatus tick(float delta) override;

    int32_t m_repeat_count             = 0;  // 重复运行的次数
    int32_t m_continuous_success_count = 0;  // 连续成功运行的次数
};

}  // namespace BT
