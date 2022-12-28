#pragma once

#include "BehaviorTree/include/controls/ControlNode.h"

namespace BT {
/**
 * @brief 响应式选择节点：每一次都从第一个节点开始运行，首个返回运行中的节点，将中断除此节点以外的所有节点；当所有节点均失败时，该节点返回失败。
 * — 因每一次都是从第一个节点开始尝试，因此具备优先级特性；
 * — 子节点返回运行中，则返回运行中，并中断除此节点以外的所有节点；
 * — 子节点返回成功，则返回成功；
 * — 子节点返回失败，则尝试下一个节点。
 */
class ReactiveSelectorNode : public ControlNode {
public:
    ReactiveSelectorNode() {}
    virtual ~ReactiveSelectorNode() override = default;

protected:
    virtual NodeStatus tick(float delta) override;
};

}  // namespace BT
