#include "ParserUtils/ParserUtils.h"
#include "Cursor.h"

std::string Cursor::getSpelling() const {
    std::string spelling;
    ParserUtils::toString(clang_getCursorSpelling(m_handle), spelling);
    return spelling;
}

std::string Cursor::getDisplayName() const {
    std::string display_name;
    ParserUtils::toString(clang_getCursorDisplayName(m_handle), display_name);
    return display_name;
}

std::string Cursor::getSourceFile() const {
    auto range = clang_Cursor_getSpellingNameRange(m_handle, 0, 0);
    auto start = clang_getRangeStart(range);

    CXFile   file;
    unsigned line, column, offset;
    clang_getFileLocation(start, &file, &line, &column, &offset);

    std::string filename;
    ParserUtils::toString(clang_getFileName(file), filename);
    return filename;
}

Cursor::List Cursor::getChildren() const {
    List children;
    auto visitor = [](CXCursor cursor, CXCursor parent, CXClientData data) {
        auto container = static_cast<List*>(data);
        container->emplace_back(cursor);
        if (cursor.kind == CXCursor_LastPreprocessing)
            return CXChildVisit_Break;
        return CXChildVisit_Continue;
    };
    clang_visitChildren(m_handle, visitor, &children);
    return children;
}