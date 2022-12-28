#pragma once

#include <vector>

#include "BehaviorTree/include/Node.h"
#include "BehaviorTree/include/Exceptions.h"

namespace BT {
/**
 * @brief 装饰器基类：装饰器只能有一个子节点
 * — 注意：update通过子类重写的tick调用，实现装饰器功能，父类的update会获得子类返回的状态，
 *   如果返回的状态是成功或失败，并且拥有子节点，会自动的重置子节点的状态到空闲状态。
 */
class DecoratorNode : public Node {
public:
    /// @brief 控制节点的构造函数
    DecoratorNode()
        : Node(NodeType::DECORATOR) {}
    DecoratorNode(NodeConfiguration& config)
        : Node(NodeType::DECORATOR, config) {}

    /// @brief  析构函数
    virtual ~DecoratorNode() override = default;

    /// @brief 设置装饰器节点的子节点，多次设置，最后一次设置的子节点生效
    /// @param node 子节点指针
    void setChild(Node* node) {
        m_child_node = node;
    }

    /// @brief 获取所有子节点
    /// @return 所有子节点
    const Node* child() const {
        return m_child_node;
    }

    /// @brief 中断子节点的执行，同：interruptChildren() 和 interruptChildren(0)
    virtual void interrupt() override {
        interruptChild();
    }

    /// @brief 中断指针索引的子节点的执行
    /// @param index 子节点索引
    void interruptChild() {
        if (m_child_node) {
            if (NodeStatus::RUNNING == m_child_node->status())
                m_child_node->interrupt();
            m_child_node->resetStatus();
        }
    }

    virtual NodeStatus update(float delta) final {
        // 先执行自身的tick函数，子类装饰器重写tick，调用子节点的update，获取到状态
        auto status = Node::update(delta);
        // 重置子节点的状态为空闲状态
        if (m_child_node) {
            auto child_status = m_child_node->status();
            if (NodeStatus::SUCCESS == child_status || NodeStatus::FAILURE == child_status) {
                m_child_node->resetStatus();
            }
        }
        return status;
    }

protected:
    Node* m_child_node = nullptr;  // 控制接点的子节点：所有节点有Tree负责创建和释放
};
};  // namespace BT
