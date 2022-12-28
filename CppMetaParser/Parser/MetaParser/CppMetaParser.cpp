#include "CppMetaParser.h"
#include "Generator/BTCustomFactoryGenerator.h"
#include "Generator/BTCustomActionSetterGenerator.h"

CppMetaParser::CppMetaParser(const std::string& parser_header_txt_path,
                             const std::string& out_put_path,
                             const std::string& work_paths,
                             const std::string& sys_include,
                             bool               is_show_errors)
    : m_parser_header_txt_path(parser_header_txt_path),
      m_sys_include(sys_include),
      m_is_show_errors(is_show_errors) {

    m_parser_header_path = ParserUtils::getFileName(parser_header_txt_path);
    auto pos             = m_parser_header_path.find_last_of('.');
    if (pos != m_parser_header_path.npos) {
        m_parser_header_path = "./" + m_parser_header_path.substr(0, pos) + ".h";
    }
    m_work_paths = ParserUtils::split(work_paths, ";");

    m_generators.emplace_back(new Generator::BTCustomFactoryGenerator(out_put_path + "/_generated/BehaviorTree/addCustomNodes.gen.cpp", m_work_paths[0] + "/MustacheTemplate/addCustomNodes.ipp.mustache"));
    m_generators.emplace_back(new Generator::BTCustomActionSetterGenerator(out_put_path + "/_generated/BehaviorTree/CustomActionNodeGenResources.gen.cpp", m_work_paths[0] + "/MustacheTemplate/CustomActionNodeGenResources.ipp.mustache"));
}

CppMetaParser::~CppMetaParser() {
    if (m_translation_unit)
        clang_disposeTranslationUnit(m_translation_unit);

    if (m_index)
        clang_disposeIndex(m_index);
}

bool CppMetaParser::convertPaeserHeader() {
    // 1、打开头文件列表txt
    std::fstream include_txt_file(m_parser_header_txt_path, std::ios::in);
    if (include_txt_file.fail()) {
        std::cerr << "Could not load file: " << m_parser_header_txt_path << std::endl;
        return false;
    }
    // 2、读取文件内容
    std::stringstream buffer;
    buffer << include_txt_file.rdbuf();
    std::string context = buffer.str();

    // 3、根据分号将头文件列表分割为字符串数组
    auto inlcude_files = ParserUtils::split(context, ";");

    // 4、创建头文件
    std::fstream include_file;
    include_file.open(m_parser_header_path, std::ios::out);
    if (!include_file.is_open()) {
        std::cerr << "Could not open the Source Include file: " << m_parser_header_path << std::endl;
        return false;
    }

    std::cout << "Generating the Source Include file: " << m_parser_header_path << std::endl;

    // 5、添加编译宏头
    include_file << "#ifndef __BEHAVIOR_TREE_GENERATED_PARSER_HEADER_H__" << std::endl;
    include_file << "#define __BEHAVIOR_TREE_GENERATED_PARSER_HEADER_H__" << std::endl;

    // 6、根据头文件列表添加包含头文件
    for (auto include_item : inlcude_files) {
        std::string temp_string(include_item);
        ParserUtils::replace(temp_string, '\\', '/');
        while (temp_string.back() == '\r' || temp_string.back() == '\n') {
            temp_string.pop_back();
        }
        include_file << "#include  \"" << temp_string << "\"" << std::endl;
    }

    // 7、添加编译宏尾
    include_file << "#endif" << std::endl;

    include_file.close();

    std::cout << "Generating the Source Include file complete: " << m_parser_header_path << std::endl;
    return true;
}

int CppMetaParser::parse() {
    // 1、将头文件列表转换为标准头文件
    if (!convertPaeserHeader()) {
        std::cerr << "Generating the Source Include file failed: " << m_parser_header_path << std::endl;
        return -1;
    }

    // 2、根据系统头文件夹和工作文件夹添加编译入参
    std::string pre_include = "-I";
    std::string sys_include_temp;
    if (!(m_sys_include == "*")) {
        sys_include_temp = pre_include + m_sys_include;
        m_arguments.emplace_back(sys_include_temp.c_str());
    }
    auto paths = m_work_paths;
    for (int index = 1; index < paths.size(); ++index) {
        paths[index] = pre_include + paths[index];

        m_arguments.emplace_back(paths[index].c_str());
    }

    // 3、通过clang-c进行编译转换，不生成文件
    m_index = clang_createIndex(true, m_is_show_errors);
    // 参数二：源文件，这里使用包含所有自实现的头文件的头文件
    // 参数三：clang编译器命令行参数个数
    // 参数三：clang编译器命令行参数字符串指针数组
    m_translation_unit = clang_createTranslationUnitFromSourceFile(
        m_index, m_parser_header_path.c_str(), static_cast<int>(m_arguments.size()), m_arguments.data(), 0, nullptr);
    auto cursor = clang_getTranslationUnitCursor(m_translation_unit);

    // 4、递归的将解析类型
    Namespace temp_namespace;
    buildClassAST(cursor, temp_namespace);

    return 0;
}

void CppMetaParser::buildClassAST(const Cursor& cursor, Namespace& current_namespace) {
    Cursor::List children = cursor.getChildren();
    for (auto& child : children) {
        auto kind = child.getKind();

        // 判断游标类型需要是类类型，即class和struct
        if (child.isDefinition() && (kind == CXCursor_ClassDecl || kind == CXCursor_StructDecl)) {
            auto class_ptr = std::make_shared<Class>(child, current_namespace);

            // 通过自定义的属性判断该类是否需要被用于生成
            if (class_ptr->shouldCompile()) {
                auto file = class_ptr->getSourceFile();
                m_schema_modules[file].classes.emplace_back(class_ptr);
                m_type_table[class_ptr->m_display_name] = file;
            }
        }
        else {
            // 如果游标出在命令空间出，则继续向下递归
            if (kind == CXCursor_Namespace) {
                auto display_name = child.getDisplayName();
                if (!display_name.empty()) {
                    current_namespace.emplace_back(display_name);
                    buildClassAST(child, current_namespace);
                    current_namespace.pop_back();
                }
            }
        }
    }
}

void CppMetaParser::finish() {
    for (auto& item : m_generators) {
        item->finish();
    }
}

void CppMetaParser::generateFiles(void) {
    std::cout << "Start generate runtime schemas(" << m_schema_modules.size() << ")..." << std::endl;
    for (auto& schema : m_schema_modules) {
        for (auto& item : m_generators) {
            item->generate(schema.first, schema.second);
        }
    }

    finish();
}