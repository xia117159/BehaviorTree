#pragma once

#include "Common/Global.h"
#include "CursorUtils/Cursor.h"
#include "Meta/MetaInfo.h"

/// @brief 类型信息：作为类类型和字段的父类
class TypeInfo {
public:
    /// @brief 构造函数
    /// @param cursor 类型所属游标
    /// @param current_namespace 类型所在命令空间
    TypeInfo(const Cursor& cursor, const Namespace& current_namespace)
        : m_meta_data(cursor),
          m_enabled(m_meta_data.getFlag(NativeProperty::Enable)),
          m_root_cursor(cursor),
          m_namespace(current_namespace) {}

    virtual ~TypeInfo() {}

    /// @brief 获取自定义注释属性元信息
    /// @return 注释属性元信息对象的 const 引用
    const MetaInfo& getMetaData() const {
        return m_meta_data;
    }

    /// @brief 获取该类型所在文件
    /// @return 文件路径
    std::string getSourceFile() const {
        return m_root_cursor.getSourceFile();
    }

    /// @brief 获取该类型所在的命令空间
    /// @return 命名空间
    Namespace getCurrentNamespace() const {
        return m_namespace;
    }

    /// @brief 获取该类型关联的自实现游标
    /// @return 自实现游标引用
    Cursor& getCurosr() {
        return m_root_cursor;
    }

protected:
    MetaInfo    m_meta_data;
    bool        m_enabled;
    std::string m_alias_cn;
    Namespace   m_namespace;

private:
    Cursor m_root_cursor;
};
