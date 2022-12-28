#pragma once

#include "CursorUtils/Cursor.h"

/// @brief 属性元信息：只解析注释属性
class MetaInfo {
public:
    /// @brief 根据游标进行构造
    /// @param cursor
    MetaInfo(const Cursor& cursor);

    /// @brief 获取指定Key的属性：__attribute__((annotate(Key:Property)))，以此方式定义注释，具有Property；其它：__attribute__((annotate(Key)))
    /// @param key Key
    /// @return 属性值
    std::string getProperty(const std::string& key) const;

    /// @brief 获取指定Key是否有定义关联的属性
    /// @param key Key
    /// @return 是否定义关联的属性
    bool getFlag(const std::string& key) const;

private:
    /// @brief __attribute__((annotate(Key:Property))) 其中 Key对应键，Property对应值，Property可为空
    ///        __attribute__((annotate(Key))) Property为空时的定义
    ///        __attribute__((annotate(Key_1, Key_2, Key_3))) 多个注释属性定义，使用 逗号 分隔
    using Property = std::pair<std::string, std::string>;

    std::unordered_map<std::string, std::string> m_properties;

    /// @brief 提取游标所指结构的属性集合，此时游标指向注释属性
    /// @param cursor
    /// @return 属性集合
    std::vector<Property> extractProperties(const Cursor& cursor) const;
};