#include "BTCustomFactoryGenerator.h"

namespace Generator {
int BTCustomFactoryGenerator::generate(const std::string& header_path, const SchemaMoudle& schema) {
    m_header_files.push_back(kainjow::mustache::data("header_file_path", header_path));

    for (auto& item : schema.classes) {
        m_class_defines.push_back(kainjow::mustache::data("class_path", item->getQualifiedName()));
    }

    return 0;
}
void BTCustomFactoryGenerator::finish() {
    kainjow::mustache::data mustache_data;
    mustache_data.set("header_files", m_header_files);
    mustache_data.set("class_defines", m_class_defines);

    std::string                 mustache_content = ParserUtils::loadFile(m_mustache_path);
    kainjow::mustache::mustache tmpl(mustache_content);
    std::string                 render_string = tmpl.render(mustache_data);

    std::cout << "render complete : " << tmpl.error_message() << std::endl;
    ParserUtils::saveFile(render_string, m_out_path);
}
}  // namespace Generator