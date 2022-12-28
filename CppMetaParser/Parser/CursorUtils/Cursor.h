#pragma once

#include "CursorType.h"

/// @brief 游标帮助对象
class Cursor {
public:
    using List    = std::vector<Cursor>;
    using Visitor = CXCursorVisitor;

    Cursor(const CXCursor& handle)
        : m_handle(handle) {}

    /// @brief 获取游标关联类型：类类型，字段，枚举，函数等等。
    /// @return 游标类型
    CXCursorKind getKind() const {
        return m_handle.kind;
    }

    /// @brief 获取此游标引用的实体的名称
    /// @return 实体的名称
    std::string getSpelling() const;

    /// @brief 获取此游标引用的实体的显示名称，显示名称包含有助于标识游标的额外信息，例如：函数或模板的参数或类模板特化的参数
    /// @return 此游标引用的实体的显示名称
    std::string getDisplayName() const;

    /// @brief 获取此游标引用的实体所在的文件路径
    /// @return 引用的实体所在的文件路径
    std::string getSourceFile() const;

    /// @brief 确定此游标所指向的声明是否也是该实体的定义
    /// @return
    bool isDefinition() const {
        return clang_isCursorDefinition(m_handle);
    }

    /// @brief 获取CXCursor的类型（如果有的话）
    /// @return CXCursor的类型
    CursorType getType() const {
        return clang_getCursorType(m_handle);
    }

    /// @brief 获取此游标的所有子节点
    /// @return 子节点集合
    List getChildren() const;

    /// @brief 访问特定游标的子游标。
    /// 这个函数访问给定游标的所有直接子游标，用每个被访问的子游标调用给定的 visitor 函数。
    /// 如果访问器返回 CXChildVisit_Recurse，则遍历可能是递归的。
    /// 如果访问者返回 CXChildVisit_Break，遍历也可能提前结束。
    /// @param visitor visitor 函数
    /// @param data 自定义提供的指针数据，每次调用时将传递给访问者
    void visitChildren(Visitor visitor, void* data = nullptr) {
        clang_visitChildren(m_handle, visitor, data);
    }

private:
    CXCursor m_handle;
};