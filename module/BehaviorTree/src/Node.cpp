#include "BehaviorTree/include/Node.h"

BT::NodeStatus BT::Node::update(float delta) {
    NodeStatus new_status = m_status;

    new_status = tick(delta);

    setStatus(new_status);
    return m_status;
}