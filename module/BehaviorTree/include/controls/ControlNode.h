#pragma once

#include <vector>

#include "BehaviorTree/include/Node.h"
#include "BehaviorTree/include/Exceptions.h"

namespace BT {
/**
 * @brief 控制节点基类
 * — 注意：控制节点在运行完成后，在父类的update中判断最终运行结果，如果不是正在运行，则中断并且重置所有子节点的状态
 */
class ControlNode : public Node {
public:
    /// @brief 控制节点的构造函数
    ControlNode()
        : Node(NodeType::CONTROL) {}

    /// @brief  析构函数
    virtual ~ControlNode() override = default;

    /// @brief 向控制节点中增加一个子节点
    /// @param node 子节点指针
    void addChild(Node* node) {
        m_children_nodes.push_back(node);
    }

    /// @brief 获取所有子节点
    /// @return 所有子节点
    const std::vector<Node*>& children() const {
        return m_children_nodes;
    }

    /// @brief 获取子节点数量
    /// @return 子节点数量
    size_t childrenCount() const {
        return m_children_nodes.size();
    }

    /// @brief 根据索引获取子节点指针，索引无效将抛出异常
    /// @param index 索引
    /// @return 子节点指针
    const Node* child(size_t index) const {
        return m_children_nodes.at(index);
    }

    /// @brief 中断子节点的执行，同：interruptChildren() 和 interruptChildren(0)
    virtual void interrupt() override {
        interruptChildren();
    }

    /// @brief 从开始索引依次中断子节点的执行
    /// @param start 开始索引
    void interruptChildren(size_t start) {
        for (size_t i = start; i < m_children_nodes.size(); ++i) {
            interruptChild(i);
        }
    }

    /// @brief 中断所有子节点的执行
    void interruptChildren() {
        interruptChildren(0);
    }

    /// @brief 中断指针索引的子节点的执行
    /// @param index 子节点索引
    void interruptChild(size_t index) {
        if (index >= m_children_nodes.size())
            return;
        auto child = m_children_nodes[index];
        if (child->m_status == NodeStatus::RUNNING) {
            child->interrupt();
        }
        child->resetStatus();
    }

    /// @brief 更新函数：当子类重写tick实现控制功能时，其返回的状态不是正在运行，则中断并且重置所有子节点为空闲状态。
    /// @param delta
    /// @return
    virtual NodeStatus update(float delta) override {
        auto status = Node::update(delta);
        if (NodeStatus::RUNNING != status) {
            interrupt();
        }
        return status;
    }

    bool empty() const {
        return m_children_nodes.empty();
    }

protected:
    std::vector<Node*> m_children_nodes;  // 控制接点的子节点：所有节点有Tree负责创建和释放
};
};  // namespace BT
