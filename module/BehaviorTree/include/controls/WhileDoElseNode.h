#pragma once

#include "BehaviorTree/include/controls/ControlNode.h"

namespace BT {
/**
 * @brief While Do Else节点：第一个节点为执行条件节点，根据第一个节点返回成功失败状态，再执行第二第三个节点，每次tick均会执行条件节点。
 * — 未添加子节点，直接返回运行成功。
 * — 只有一个子节点时，当前控制节点可以视为不存在。
 * — 第一个节点返回【成功】：不存在第二个节点时，返回成功；存在第二个节点时，则执行该节点，并返回该节点的状态；
 * — 第一个节点返回【失败】：不存在第三个节点时，返回失败；存在第三个节点时，则执行该节点，并返回该节点的状态；
 * — 对第三个及之后的节点，不会进行任何处理。
 */
class WhileDoElseNode : public ControlNode {
public:
    WhileDoElseNode() {}
    virtual ~WhileDoElseNode() override = default;

protected:
    virtual NodeStatus tick(float delta) override;
};

}  // namespace BT
