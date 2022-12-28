#include "Class.h"
#include "Field.h"

Field::Field(const Cursor& cursor, const Namespace& current_namespace, Class* parent)
    : TypeInfo(cursor, current_namespace),
      m_is_const(cursor.getType().isConst()),
      m_parent(parent),
      m_name(cursor.getSpelling()),
      m_display_name(ParserUtils::getNameWithoutFirstM(m_name)),
      m_type(ParserUtils::getTypeNameWithoutNamespace(cursor.getType())) {
    ParserUtils::replaceAll(m_type, " ", "");

    auto ret_string = ParserUtils::getStringWithoutQuot(m_meta_data.getProperty("default"));
    m_default       = ret_string;
}

bool Field::shouldCompile(void) const {
    return isAccessible();
}

bool Field::isAccessible(void) const {
    return m_meta_data.getFlag(NativeProperty::PropertySetter);
}