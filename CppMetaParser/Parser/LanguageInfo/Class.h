#pragma once

#include "TypeInfo.h"
#include "Field.h"

// 类类型的基类
struct BaseClass {
    BaseClass(const Cursor& cursor);
    std::string name;
};

/// @brief 类类型
class Class : public TypeInfo {
    friend class Field;
    friend class CppMetaParser;

public:
    /// @brief 类类型构造函数
    /// @param cursor 关联游标
    /// @param current_namespace 类所处命令空间
    Class(const Cursor& cursor, const Namespace& current_namespace);

    /// @brief 获取该类是否需要被用于生成
    /// @return 是否需要被用于生成
    virtual bool shouldCompile() const;

    /// @brief 获取该类类中字段是否需要被用于生成
    /// @return 类中字段是否需要被用于生成
    bool shouldCompileFilds() const;

    template <typename T>
    using SharedPtrVector = std::vector<std::shared_ptr<T>>;

    /// @brief 获取类名
    /// @return 类名
    std::string getClassName();

    /// @brief 获取包含命令空间的类名
    /// @return 包含命令空间的类名
    std::string getQualifiedName() {
        return m_qualified_name;
    }

    /// @brief 返回所有满足生成条件的字段
    /// @return
    const SharedPtrVector<Field>& getFields() const {
        return m_fields;
    }

    /// @brief 该类的基类集合
    SharedPtrVector<BaseClass> m_base_classes;

public:
    std::string            m_name;            // 类的真实名称
    std::string            m_qualified_name;  // 包含命令空间的类名
    SharedPtrVector<Field> m_fields;          // 字段集合
    std::string            m_display_name;    // 类的显示名称

    bool isAccessible(void) const;
};
