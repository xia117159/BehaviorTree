#pragma once

#include <cstdint>

namespace BT {

/// @brief 节点类型
enum class NodeType {
    UNDEFINED = 0,  // 未定义的无效节点
    ACTION,         // 动作节点：作为行为树的叶子节点
    CONDITION,      // 条件节点：条件节点不执行update
    CONTROL,        // 控制节点：选择、序列、并行等
    DECORATOR,      // 装饰器节点
    SUBTREE         // 子树
};

/// @brief  节点状态
enum class NodeStatus {
    IDLE = 0,  // 空闲：表示该节点已准备好，可以运行
    RUNNING,   // 正在运行：异步节点在运行未完成时，返回RUNNING
    SUCCESS,   // 运行成功
    FAILURE    // 运行失败
};

constexpr static const char* kSequenceNodeName            = "Sequence";
constexpr static const char* kActionNodeName              = "Action";
constexpr static const char* kActionNodeNameAttrName      = "Name";
constexpr static const char* kSelectorNodeName            = "Selector";
constexpr static const char* kParallelNodeName            = "Parallel";
constexpr static const char* kRecordSelectorNodeName      = "RecordSelector";
constexpr static const char* kProbabilitySelectorNodeName = "ProbabilitySelector";
constexpr static const char* kReactiveSelectorNodeName    = "ReactiveSelector";
constexpr static const char* kRandomSelectorNodeName      = "RandomSelector";
constexpr static const char* kWhileDoElseNodeName         = "WhileDoElse";
constexpr static const char* kIfThenElseNodeName          = "IfThenElse";

constexpr static const char* kForceSuccessNodeName            = "ForceSuccess";
constexpr static const char* kForceFailureNodeName            = "ForceFailure";
constexpr static const char* kDelayNodeName                   = "Delay";
constexpr static const char* kInverterNodeName                = "Inverter";
constexpr static const char* kKeepRunningUntilFailureNodeName = "KeepRunningUntilFailure";
constexpr static const char* kRepeatNodeName                  = "Repeat";
constexpr static const char* kRetryNodeName                   = "Retry";

constexpr static const char* kSettersXmlNodeName        = "Setters";
constexpr static const char* kSetterXmlNodeName         = "Setter";
constexpr static const char* kSetterVariableXmlAttrName = "Variable";
constexpr static const char* kSetterTypeXmlAttrName     = "Type";
constexpr static const char* kSetterValueXmlAttrName    = "Value";

constexpr static const char* kSetterIntegerTypeName = "Integer";
constexpr static const char* kSetterDoubleTypeName  = "Double";
constexpr static const char* kSetterStringTypeName  = "String";

constexpr static const char* kMappingsXmlNodeName     = "Mappings";
constexpr static const char* kMappingItemXmlNodeName  = "Item";
constexpr static const char* kMappingItemRuleAttrName = "Rule";
constexpr static const char* kMappingItemModeAttrName = "Mode";
constexpr static const char* kMappingItemModeSuccess  = "Success";
constexpr static const char* kMappingItemModeFailure  = "Failure";
constexpr static const char* kMappingItemModeBoth     = "Both";

};  // namespace BT
