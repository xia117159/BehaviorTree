#pragma once

#include "TypeInfo.h"

class Class;

/// @brief 字段信息
class Field : public TypeInfo {

public:
    /// @brief 字段信息构造函数啊
    /// @param cursor 字段对应的游标
    /// @param current_namespace 所处命令空间
    /// @param parent 所处类信息，为 nullptr 时，表示该字段未定义在类中
    Field(const Cursor& cursor, const Namespace& current_namespace, Class* parent = nullptr);

    virtual ~Field() {}

    /// @brief 获取该字段是否需要用于生成资源
    /// @return 是否需要用于生成资源
    bool shouldCompile() const;

    bool shouldCompile(const std::string& flag) const {
        return m_meta_data.getFlag(flag);
    }

    const std::string& name() const {
        return m_name;
    }

public:
    Class* m_parent;

    std::string m_name;          // 字段真实名称
    std::string m_display_name;  // 字段的显示名称：相对于真实名称，去掉了 m_ 的前缀
    std::string m_type;          // 字段类型
    std::string m_default;       // 默认属性

    bool m_is_const;  // 是否是 const 字段

    /// @brief 判断该字段是否是可访问字段
    /// @return 是否是可访问字段
    bool isAccessible() const;
};