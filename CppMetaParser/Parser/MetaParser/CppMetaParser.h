#pragma once

#include <iostream>
#include <list>
#include "ParserUtils/ParserUtils.h"
#include "CursorUtils/Cursor.h"
#include "LanguageInfo/Class.h"
#include "SchemaMoudle.h"
#include "Generator/GeneratorInterface.h"

class CppMetaParser {
public:
    /// @brief 构造函数
    /// @param parser_header_txt_path 用于生成的头文件集合，以分号作为间隔符，在CMake文件中生成
    /// @param out_put_path 生成的文件的输出路径
    /// @param work_paths 用于生成的头文件所关联的文件夹位置
    /// @param sys_include 系统头文件位置
    /// @param is_show_errors 是否打印错误
    CppMetaParser(const std::string& parser_header_txt_path,
                  const std::string& out_put_path,
                  const std::string& work_paths,
                  const std::string& sys_include,
                  bool               is_show_errors);

    ~CppMetaParser();

    /// @brief 将头文件集合转换为标准头文件，用于clang-c解析
    /// @return 转换是否成功
    bool convertPaeserHeader();

    /// @brief 编译解析所有文件，并转为自定的Class类型
    /// @return 是否编译解析成功
    int parse();

    /// @brief 递归的从指定游标和命令空间中解析类型
    /// @param cursor 游标
    /// @param current_namespace 关联命名空间
    void buildClassAST(const Cursor& cursor, Namespace& current_namespace);

    /// @brief 将所有满足条件的类型用于文件生成
    /// @param
    void generateFiles(void);

    /// @brief 通知生成器文件数据传输完毕，可以存储为文件
    void finish();

private:
    std::string              m_parser_header_txt_path;
    std::string              m_parser_header_path;
    std::vector<std::string> m_work_paths;
    std::string              m_sys_include;

    std::list<std::unique_ptr<Generator::GeneratorInterface>> m_generators;

    std::unordered_map<std::string, std::string>  m_type_table;
    std::unordered_map<std::string, SchemaMoudle> m_schema_modules;

    CXIndex           m_index            = nullptr;
    CXTranslationUnit m_translation_unit = nullptr;

    std::vector<const char*> m_arguments = { { "-x",
                                               "c++",
                                               "-std=c++17",
                                               "-D__GENERATOR_ANALYSIS__",
                                               "-DNDEBUG",
                                               "-D__clang__",
                                               "-w",
                                               "-MG",
                                               "-M",
                                               "-ferror-limit=0",
                                               "-o clangLog.txt" } };

    bool m_is_show_errors = true;
};