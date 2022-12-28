#include <filesystem>
#include <list>
#include "BehaviorTree/include/BehaviorTreeFactory.h"
#include "BehaviorTree/include/Parser.h"
#include "BehaviorTree/include/utils/BehaviorTreeUtils.h"
#include "tinyxml2/tinyxml2.h"

namespace BT {

using namespace tinyxml2;

const char* const kWhitespaceString = " \t\r\n";

struct XmlParser::Pimpl {
    // 已打开的xml的doc
    std::list<std::unique_ptr<XMLDocument>> m_opened_documents;
    /// @brief 从XML的doc对象中创建树
    /// @param doc XML的doc对象
    /// @return 行为树
    BehaviorTree createBehaviorTreeFormDoc(XMLDocument* doc, BlackBoard::Ptr blackBoard);
    /// @brief 递归的创建树
    /// @param tree_ID
    /// @param output_tree
    /// @param root_parent
    void recursivelyCreateTree(const XMLElement* xmlEle, BehaviorTree& outputTree);

    Node::Ptr createNodeFromXML(const XMLElement* element, NodeConfiguration& config, Node::Ptr& nodeParent);

    NodeSettings createSettersFromXML(const XMLElement* element);

    void createMappingsFromXML(const XMLElement* element, NodeConfiguration& config);

    bool isMappingVariable(std::string& str);
};

BehaviorTree XmlParser::Pimpl::createBehaviorTreeFormDoc(XMLDocument* doc, BlackBoard::Ptr blackBoard) {
    if (doc->Error()) {
        char buffer[200];
        sprintf(buffer, "Error parsing the XML: %s", doc->GetErrorStr1());
        throw RuntimeError(buffer);
    }

    const XMLElement* xmlRoot = doc->RootElement();

    BehaviorTree tree;
    tree.blackBoard = blackBoard;

    recursivelyCreateTree(xmlRoot->FirstChildElement(), tree);

    return tree;
}

void XmlParser::Pimpl::recursivelyCreateTree(const XMLElement* element, BehaviorTree& outputTree) {

    std::function<void(const XMLElement*, Node::Ptr&)> recursiveStep;
    NodeConfiguration                                  config;
    config.blackboard = outputTree.blackBoard;

    recursiveStep = [&](const XMLElement* element, Node::Ptr& nodeParent) {
        Node::Ptr new_node = createNodeFromXML(element, config, nodeParent);
        outputTree.nodes.push_back(new_node);
        for (const XMLElement* child_element = element->FirstChildElement(); child_element; child_element = child_element->NextSiblingElement()) {
            recursiveStep(child_element, new_node);
        }
    };

    Node::Ptr root_node;
    recursiveStep(element, root_node);
}

Node::Ptr XmlParser::Pimpl::createNodeFromXML(const XMLElement* element, NodeConfiguration& config, Node::Ptr& nodeParent) {
    BehaviorTreeFactory& factory = BehaviorTreeFactory::getInstance();

    // 根据XML节点创建行为树节点
    Node::Ptr new_node;

    config.settings = createSettersFromXML(element);
    createMappingsFromXML(element, config);

    if (isControlNode(element->Name()) || isDecoratorNode(element->Name())) {
        new_node = factory.instantiateTreeNode(element->Name(), config);
    }
    else if (cstChEqual(element->Name(), kActionNodeName)) {
        auto name = element->FindAttribute(kActionNodeNameAttrName);
        if (name == nullptr)
            throw RuntimeError("XmlParser: action node must set [name] attribute");
        new_node = factory.instantiateTreeNode(name->Value(), config);
    }

    // 判断父节点是否存在，若存在则根据类型将新节点添加到父节点
    if (nodeParent) {
        switch (nodeParent->type()) {
        case NodeType::CONTROL:
            if (auto controlParent = dynamic_cast<ControlNode*>(nodeParent.get())) {
                controlParent->addChild(new_node.get());
            }
            break;
        case NodeType::DECORATOR:
            if (auto controlParent = dynamic_cast<DecoratorNode*>(nodeParent.get())) {
                controlParent->setChild(new_node.get());
            }
            break;
        default:
            break;
        }
    }

    return new_node;
}

NodeSettings XmlParser::Pimpl::createSettersFromXML(const XMLElement* element) {
    NodeSettings settings;

    auto parserSettersElement = [&](const XMLElement* setters) {
        for (const XMLElement* child_element = setters->FirstChildElement(); child_element; child_element = child_element->NextSiblingElement()) {
            // 判断当前节点是否是单个设置器节点
            if (cstChEqual(child_element->Name(), kSetterXmlNodeName)) {
                // 获取设置器的类型、键和值
                auto type_attr  = child_element->FindAttribute(kSetterTypeXmlAttrName);
                auto key_attr   = child_element->FindAttribute(kSetterVariableXmlAttrName);
                auto value_attr = child_element->FindAttribute(kSetterValueXmlAttrName);
                if (type_attr == nullptr || key_attr == nullptr || value_attr == nullptr)
                    continue;
                auto type = type_attr->Value();
                // 整形
                if (cstChEqual(type, kSetterIntegerTypeName)) {
                    int64_t value = value_attr->IntValue();
                    settings.set(key_attr->Value(), value);
                }
                // 浮点型
                else if (cstChEqual(type, kSetterDoubleTypeName)) {
                    double value = value_attr->DoubleValue();
                    settings.set(key_attr->Value(), value);
                }
                // 字符串
                else {
                    settings.set(key_attr->Value(), value_attr->Value());
                }
            }
        }
    };

    for (const XMLElement* child_element = element->FirstChildElement(); child_element; child_element = child_element->NextSiblingElement()) {
        if (cstChEqual(child_element->Name(), kSettersXmlNodeName)) {
            parserSettersElement(child_element);
        }
    }
    return settings;
}

bool XmlParser::Pimpl::isMappingVariable(std::string& str) {
    str             = trim(str, kWhitespaceString);
    const auto size = str.size();
    if (size >= 3 && str.back() == '}') {
        if (str[0] == '{') {
            str = str.substr(1, size - 2);
            return true;
        }
        if (str[0] == '$' && str[1] == '{') {
            str = str.substr(2, size - 3);
            return true;
        }
    }
    return false;
}

void XmlParser::Pimpl::createMappingsFromXML(const XMLElement* element, NodeConfiguration& config) {
    auto parserMappingsElement = [&](const XMLElement* mappings) {
        for (const XMLElement* child_element = mappings->FirstChildElement(); child_element; child_element = child_element->NextSiblingElement()) {
            // 判断当前节点是否是映射项节点
            if (cstChEqual(child_element->Name(), kMappingItemXmlNodeName)) {
                // 获取映射的规则和模式
                auto rule_attr = child_element->FindAttribute(kMappingItemRuleAttrName);
                auto mode_attr = child_element->FindAttribute(kMappingItemModeAttrName);
                if (rule_attr == nullptr)
                    continue;

                auto rule = split(rule_attr->Value(), "=");
                if (rule.size() != 2)
                    continue;
                // 左边是映射变量，则该规则是输出映射
                if (isMappingVariable(rule[0])) {
                    OutputMappingItem item;
                    item.mapping_variable = std::move(rule[0]);
                    if (mode_attr && cstChEqual(mode_attr->Value(), kMappingItemModeFailure)) {
                        item.mode = 2;
                    }
                    else if (mode_attr && cstChEqual(mode_attr->Value(), kMappingItemModeBoth)) {
                        item.mode = 3;
                    }
                    config.output_mappings[trim(rule[1], kWhitespaceString)] = std::move(item);
                }
                // 右边是映射变量，则该规则是输入映射
                else if (isMappingVariable(rule[1])) {
                    config.input_mappings[trim(rule[0], kWhitespaceString)] = std::move(rule[1]);
                }
            }
        }
    };

    for (const XMLElement* child_element = element->FirstChildElement(); child_element; child_element = child_element->NextSiblingElement()) {
        if (cstChEqual(child_element->Name(), kMappingsXmlNodeName)) {
            parserMappingsElement(child_element);
        }
    }
}

XmlParser::XmlParser() {
    _pimpl.reset(new Pimpl());
}

XmlParser::~XmlParser() {
}

BehaviorTree XmlParser::createBehaviorTreeFromXml(const std::string& filePath, BlackBoard::Ptr blackBoard) {
    _pimpl->m_opened_documents.emplace_back(new XMLDocument());

    // 根据路径，打开XML文件
    XMLDocument* doc = _pimpl->m_opened_documents.back().get();
    doc->LoadFile(filePath.c_str());

    return _pimpl->createBehaviorTreeFormDoc(doc, blackBoard);
}
}  // namespace BT