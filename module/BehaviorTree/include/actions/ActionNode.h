#ifndef __BEHAVIOR_TREE_ACTIONNODE_H__
#define __BEHAVIOR_TREE_ACTIONNODE_H__

#include "BehaviorTree/include/Node.h"
#include "BehaviorTree/include/Exceptions.h"

namespace BT {
/// @brief 所有动作节点的基类
class ActionNodeBase : public Node {
public:
    ActionNodeBase(NodeConfiguration& config)
        : Node(NodeType::ACTION, config) {}

    virtual ~ActionNodeBase() = default;
};
/// @brief 同步动作节点：同步动作节点不能被打断
class SyncActionNode : public ActionNodeBase {
public:
    SyncActionNode(NodeConfiguration& config)
        : ActionNodeBase(config) {}
    virtual ~SyncActionNode() override = default;

    /// @brief 同步节点执行，注意：同步节点永远不能返回RUNNING状态，若返回RUNNING状态，将抛出异常
    /// @param delta 时差
    /// @return 结果状态
    virtual NodeStatus update(float delta) {
        NodeStatus stat = ActionNodeBase::update(delta);
        if (stat == NodeStatus::RUNNING) {
            throw LogicError("SyncActionNode MUST never return RUNNING");
        }
        return stat;
    }

    /// @brief 同步动作节点不能被打断
    virtual void interrupt() final {}
};

/// @brief 动作节点：这是优选的动作节点。
/// 它支持以下事件调用：
/// 1、节点处于IDLE状态时，将会调用 onStart()；
/// 2、节点处于RUNNING状态时，将会调用 onRunning()；
/// 3、节点处于RUNNING状态，但被中断运行时，将会调用 onInterrupted();
class ActionNode : public ActionNodeBase {
public:
    ActionNode(NodeConfiguration& config)
        : ActionNodeBase(config) {}
    virtual ~ActionNode() override = default;

    /// @brief 初始化节点，在自定义设置完成后调用。此时在配置中的设置数据是无效的。
    /// @param
    virtual void init() {}

    /// @brief Action节点仅在运行时需要被中断
    virtual void interrupt() override;

    /// @brief 由IDLE状态进入Tick时调用，注意：永远不能返回IDLE状态
    /// @param delta 时差
    /// @return 运行后的状态
    virtual NodeStatus onStart(float delta) {
        return onRunning(delta);
    };

    /// @brief 在RUNNING状态进入Tick时调用，注意：永远不能返回IDLE状态
    /// @param delta 时差
    /// @return 运行后的状态
    virtual NodeStatus onRunning(float delta) = 0;

    /// @brief 仅在处于RUNNING状态是，被中断时调用
    virtual void onInterrupted(){};

private:
    /// @brief 自动输入映射
    virtual void inputAutoMapping() {}
    /// @brief 自动输出映射
    virtual void outputAutoMapping(NodeStatus) {}

    virtual NodeStatus tick(float delta) final;
};

};  // namespace BT

#endif