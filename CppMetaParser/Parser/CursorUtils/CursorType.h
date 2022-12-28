#pragma once

class Cursor;

/// @brief 游标类型
class CursorType {
public:
    CursorType(const CXType& handle)
        : m_handle(handle) {}

    std::string getDisplayName() const;

    int getArgumentCount() const {
        return clang_getNumArgTypes(m_handle);
    }

    CursorType getArgument(unsigned index) const {
        return clang_getArgType(m_handle, index);
    }

    CursorType getCanonicalType() const {
        return clang_getCanonicalType(m_handle);
    }

    Cursor getDeclaration() const;

    CXTypeKind getKind() const {
        return m_handle.kind;
    }

    bool isConst() const {
        return clang_isConstQualifiedType(m_handle) ? true : false;
    }

private:
    CXType m_handle;
};