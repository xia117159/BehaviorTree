
#include "ParserUtils/ParserUtils.h"
#include "Cursor.h"

Cursor CursorType::getDeclaration() const {
    return clang_getTypeDeclaration(m_handle);
}

std::string CursorType::getDisplayName() const {
    std::string display_name;
    ParserUtils::toString(clang_getTypeSpelling(m_handle), display_name);
    return display_name;
}