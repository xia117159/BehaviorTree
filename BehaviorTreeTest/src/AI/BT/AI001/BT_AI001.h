#ifndef __BT_AI001_H__
#define __BT_AI001_H__

#include <iostream>
#include "BehaviorTree/include/BehaviorTreeFactory.h"

namespace AI_BT_0001 {

class BT_CUSTOM_ACTION_NODE_PROPERTY(SaySomething) {
public:
    BT_CAN_DEFAULT_CONSTRUCTOR(SaySomething) = default;

    virtual BT::NodeStatus onStart(float delta) override {
        times = 0;
        std::cout << "SaySomething onStart" << std::endl;
        return BT::NodeStatus::RUNNING;
    }

    virtual BT::NodeStatus onRunning(float delta) override {
        std::cout << "SaySomething onRunning" << std::endl;
        ++times;
        if (times == 2)
            return BT::NodeStatus::SUCCESS;
        return BT::NodeStatus::RUNNING;
    }

private:
    int times = 0;
};

class BT_CUSTOM_ACTION_NODE_PROPERTY(OpenDoor) {
public:
    BT_CAN_DEFAULT_CONSTRUCTOR(OpenDoor) = default;

    virtual BT::NodeStatus onRunning(float delta) override {
        std::cout << "Try OpenDoor" << std::endl;
        ++times;
        if (times < 3)
            return BT::NodeStatus::SUCCESS;
        else
            return BT::NodeStatus::FAILURE;
    }

private:
    int times = 0;
};

class BT_CUSTOM_ACTION_NODE_PROPERTY(MoveOut) {
public:
    BT_CAN_DEFAULT_CONSTRUCTOR(MoveOut) = default;

    virtual BT::NodeStatus onRunning(float delta) override {
        std::cout << "MoveOut" << std::endl;
        return BT::NodeStatus::SUCCESS;
    }  // namespace AI_BT_0001

private:
};
};  // namespace AI_BT_0001

#endif