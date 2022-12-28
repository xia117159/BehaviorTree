#include "BehaviorTree/include/actions/ActionNode.h"

BT::NodeStatus BT::ActionNode::tick(float delta) {
    auto init_status = status();

    // 在空闲状态进入tick时，调用onStart
    if (BT::NodeStatus::IDLE == init_status) {
        inputAutoMapping();
        auto new_stat = onStart(delta);
        if (BT::NodeStatus::IDLE == new_stat) {
            throw LogicError("ActionNode::onStart must not return IDLE");
        }
        if (NodeStatus::RUNNING != new_stat) {
            outputAutoMapping(new_stat);
        }
        return new_stat;
    }

    // 在运行状态进入tick时，调用onRunning
    if (BT::NodeStatus::RUNNING == init_status) {
        auto new_stat = onRunning(delta);
        if (BT::NodeStatus::IDLE == new_stat) {
            throw LogicError("ActionNode::onRunning must not return IDLE");
        }
        if (NodeStatus::RUNNING != new_stat) {
            outputAutoMapping(new_stat);
        }
        return new_stat;
    }

    return init_status;
}

void BT::ActionNode::interrupt() {
    if (status() == NodeStatus::RUNNING) {
        onInterrupted();
    }
}