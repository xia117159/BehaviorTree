#pragma once

#include <unordered_map>
#include <functional>
#include "BehaviorTree/include/BehaviorTree.h"

#if defined(__GENERATOR_ANALYSIS__)
#define BT_CUSTOM_META(...) __attribute__((annotate(#__VA_ARGS__)))  // 添加属性：由于格式化的原因，建议使用它
#define BT_CUSTOM_CLASS(class_name, ...) class __attribute__((annotate(#__VA_ARGS__))) class_name
#define BT_CUSTOM_STRUCT(struct_name, ...) struct __attribute__((annotate(#__VA_ARGS__))) struct_name
#else
#define BT_CUSTOM_META(...)
#define BT_CUSTOM_CLASS(class_name, ...) class class_name
#define BT_CUSTOM_STRUCT(struct_name, ...) struct struct_name
#endif  // __GENERATOR_ANALYSIS__

#define BT_CUSTOM_ACTION_NODE(type) BT_CUSTOM_CLASS(type, ActionNode)
#define BT_CUSTOM_ACTION_NODE_PROPERTY(type) BT_CUSTOM_META(ActionNode) type : public BT::ActionNode
#define BT_CAN_DEFAULT_CONSTRUCTOR(type) \
    type(BT::NodeConfiguration&);        \
    virtual ~type()

#define BT_CAN_DEFAULT_CONSTRUCTOR_AND_INOUT(type)  \
    virtual void inputAutoMapping();                \
    virtual void outputAutoMapping(BT::NodeStatus); \
    BT_CAN_DEFAULT_CONSTRUCTOR(type)

#define BT_AUTO_OUTPUT_MAPPING_TEMPLATE(variable, mapping_item, stat)                                  \
    if (getOutputMapping(#variable, &mapping_item)) {                                                  \
        if (BT::NodeStatus::FAILURE == stat && (mapping_item->mode == 2 || mapping_item->mode == 3)) { \
            m_blackboard->set(mapping_item->mapping_variable, variable);                               \
        }                                                                                              \
        else if (mapping_item->mode != 2) {                                                            \
            m_blackboard->set(mapping_item->mapping_variable, variable);                               \
        }                                                                                              \
    }

/*********************************** 自动代码生成属性 ***********************************/
#define BT_PROPERTY_SETTER(type, name) type BT_CUSTOM_META(PropertySetter) name
#define BT_PROPERTY_INPUT(type, name) type BT_CUSTOM_META(PropertyInput) name
#define BT_PROPERTY_INPUT_WITH_STATUS(type, name) \
    bool name##_input_status = false;             \
    type BT_CUSTOM_META(PropertyInputWithStatus) name
#define BT_PROPERTY_OUTPUT(type, name) type BT_CUSTOM_META(PropertyOutput) name

/***************************************************************************
 * class BT_CUSTOM_META(ActionNode) CustomNodeType : public BT:ActionNode {}
 * class BT_CUSTOM_ACTION_NODE_PROPERTY(CustomNodeType) {}
 * 将自动将类 CustomNode 添加到工厂中，无视 namespace 深度。
 ***************************************************************************/

namespace BT {

using NodeBuilder = std::function<Node::Ptr(NodeConfiguration&)>;

template <typename T>
using has_default_constructor = typename std::is_constructible<T>;

template <typename T>
using has_params_constructor = typename std::is_constructible<T, NodeConfiguration&>;

/// @brief 同时具备默认构造函数和入参构造函数
template <typename T>
NodeBuilder creatBuilder(typename std::enable_if<has_default_constructor<T>::value && has_params_constructor<T>::value>::type* = nullptr) {
    return [](NodeConfiguration& config) {
        return std::make_unique<T>(config);
    };
}

/// @brief 只具备默认构造函数
template <typename T>
NodeBuilder creatBuilder(typename std::enable_if<has_default_constructor<T>::value && !has_params_constructor<T>::value>::type* = nullptr) {
    return [](NodeConfiguration& config) {
        return std::make_unique<T>();
    };
}

/// @brief 只具备入参构造函数
template <typename T>
NodeBuilder creatBuilder(typename std::enable_if<!has_default_constructor<T>::value && has_params_constructor<T>::value>::type* = nullptr) {
    return [](NodeConfiguration& config) {
        return std::make_unique<T>(config);
    };
}

class BehaviorTreeFactory {
public:
    static BehaviorTreeFactory& getInstance() {
        static BehaviorTreeFactory factory;
        return factory;
    }

    BehaviorTreeFactory(const BehaviorTreeFactory& other)            = delete;
    BehaviorTreeFactory& operator=(const BehaviorTreeFactory& other) = delete;

    template <typename T>
    void registerNodeType(const std::string& key) {
        // 注册节点需要继承至：ActionNodeBase、ControlNode
        static_assert(std::is_base_of<ActionNodeBase, T>::value || std::is_base_of<ControlNode, T>::value || std::is_base_of<DecoratorNode, T>::value, "[registerNode]: accepts only classed derived from either "
                                                                                                                                                       "ActionNodeBase, "
                                                                                                                                                       "DecoratorNode, ControlNode or ConditionNode");
        // 注册节点不能有纯虚函数
        static_assert(!std::is_abstract<T>::value, "[registerNode]: Some methods are pure virtual. ");

        _builders.insert({ key, creatBuilder<T>() });
    }

    BehaviorTree createBehaviorTreeFromText(const std::string& text);

    Node::Ptr instantiateTreeNode(const std::string& key, NodeConfiguration& config);

    static BehaviorTree createBehaviorTreeFromXml(const std::string& filename, BlackBoard::Ptr blackBoard);

private:
    BehaviorTreeFactory();
    ~BehaviorTreeFactory() {}

    void addCustomNodes();

    std::unordered_map<std::string, NodeBuilder> _builders;
};
};  // namespace BT
