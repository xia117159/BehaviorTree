#pragma once

#include "BehaviorTree/include/BasicTypes.h"
#include "BehaviorTree/include/BlackBoard.h"
#include "BehaviorTree/include/NodeSetting.h"

namespace BT {

struct OutputMappingItem {
    std::string mapping_variable;
    int         mode = 1;  // 自动输出映射模式，默认仅成功时，自动映射输出。2（仅失败时，自动映射输出），3（无论成功失败，自动映射输出），其它值（仅成功时，自动映射输出）
};

/// @brief 节点配置
struct NodeConfiguration {
    BlackBoard::Ptr                                    blackboard;       // 共享黑板对象
    NodeSettings                                       settings;         // 来自于XML的设置信息
    std::unordered_map<std::string, std::string>       input_mappings;   // 输入映射
    std::unordered_map<std::string, OutputMappingItem> output_mappings;  // 输出映射
};

/// @brief 所有行为树节点的父类
class Node {
public:
    using Ptr = std::shared_ptr<Node>;

    Node(NodeType t)
        : m_type(t) {
    }

    /// @brief 构造函数
    /// @param t 节点类型
    /// @param config 节点配置
    Node(NodeType t, NodeConfiguration& config)
        : m_type(t),
          m_blackboard(config.blackboard),
          m_input_mappings(std::move(config.input_mappings)),
          m_output_mappings(std::move(config.output_mappings)) {
    }

    /// @brief 析构函数
    virtual ~Node() {}

    /// @brief 获取当前节点的状态
    /// @return 当前状态
    NodeStatus status() const {
        return m_status;
    }

    /// @brief 获取当前节点的类型
    /// @return 节点类型
    NodeType type() const {
        return m_type;
    }

    /// @brief 更新函数，在tick的基础上增加了前置条件的判断等
    /// @param delta 时差，单位：s
    /// @return 节点状态
    virtual NodeStatus update(float delta);

    /// @brief 中断正在运行的节点（状态为RUNNING），只有异步节点并且具有RUNNING状态，使用它才有效果
    virtual void interrupt() = 0;

    /// @brief 获取是否已被中断，即：当前节点是否处于空闲状态
    /// @return 是否已被中断
    bool isInterrupted() const {
        return m_status == NodeStatus::IDLE;
    }

protected:
    friend class ControlNode;
    friend class BehaviorTree;
    friend class DecoratorNode;
    /// @brief 重置状态为空闲状态
    void resetStatus() {
        m_status = NodeStatus::IDLE;
    }

    /// @brief 设置节点的运行状态
    /// @param status 状态
    void setStatus(NodeStatus status) {
        m_status = status;
    }

    /// @brief tick函数
    /// @param delta 时差
    /// @return 运行状态
    virtual NodeStatus tick(float delta) = 0;

    /// @brief 获取输入映射信息
    /// @param key 变量键名
    /// @param out 映射信息
    /// @return 是否获取成功
    bool getInputMapping(const std::string& key, const std::string** out) {
        *out      = nullptr;
        auto iter = m_input_mappings.find(key);
        if (iter != m_input_mappings.end()) {
            *out = &(iter->second);
            return true;
        }
        return false;
    }

    /// @brief 获取输出映射信息
    /// @param key 变量键名
    /// @param out 映射信息
    /// @return 是否获取成功
    bool getOutputMapping(const std::string& key, const OutputMappingItem** out) {
        *out      = nullptr;
        auto iter = m_output_mappings.find(key);
        if (iter != m_output_mappings.end()) {
            *out = &(iter->second);
            return true;
        }
        return false;
    }

    NodeStatus                                         m_status = NodeStatus::IDLE;     // 节点状态
    NodeType                                           m_type   = NodeType::UNDEFINED;  // 节点类型
    BlackBoard::Ptr                                    m_blackboard;                    // 共享黑板对象
    std::unordered_map<std::string, std::string>       m_input_mappings;                // 输入映射
    std::unordered_map<std::string, OutputMappingItem> m_output_mappings;               // 输出映射
};
};  // namespace BT
