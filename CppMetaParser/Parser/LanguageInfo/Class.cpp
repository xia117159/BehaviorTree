#include "Class.h"

BaseClass::BaseClass(const Cursor& cursor)
    : name(ParserUtils::getTypeNameWithoutNamespace(cursor.getType())) {}

Class::Class(const Cursor& cursor, const Namespace& current_namespace)
    : TypeInfo(cursor, current_namespace),
      m_name(cursor.getDisplayName()),
      m_qualified_name(ParserUtils::getTypeNameWithoutNamespace(cursor.getType())),
      m_display_name(ParserUtils::getNameWithoutFirstM(m_qualified_name)) {
    ParserUtils::replaceAll(m_name, " ", "");

    // 编译子游标
    for (auto& child : cursor.getChildren()) {
        switch (child.getKind()) {
        // 基类类型
        case CXCursor_CXXBaseSpecifier: {
            auto base_class = new BaseClass(child);
            m_base_classes.emplace_back(base_class);
        } break;
        // 类的成员字段
        case CXCursor_FieldDecl:
            m_fields.emplace_back(new Field(child, current_namespace, this));
            break;
        default:
            break;
        }
    }
}

bool Class::shouldCompile(void) const {
    return shouldCompileFilds();
}

bool Class::shouldCompileFilds(void) const {
    return m_meta_data.getFlag(NativeProperty::All) || m_meta_data.getFlag(NativeProperty::Fields) || m_meta_data.getFlag(NativeProperty::ActionNode);
}

std::string Class::getClassName(void) {
    return m_name;
}

bool Class::isAccessible(void) const {
    return m_enabled;
}