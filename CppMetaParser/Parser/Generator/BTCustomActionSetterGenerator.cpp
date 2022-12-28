#include "BTCustomActionSetterGenerator.h"

namespace Generator {
int BTCustomActionSetterGenerator::generate(const std::string& header_path, const SchemaMoudle& schema) {
    m_header_files.push_back(kainjow::mustache::data("header_file_path", header_path));

    for (auto& class_item : schema.classes) {
        kainjow::mustache::data action_node_constructor;

        kainjow::mustache::data setters(kainjow::mustache::data::type::list);
        kainjow::mustache::data input_properties(kainjow::mustache::data::type::list);
        kainjow::mustache::data input_with_status_properties(kainjow::mustache::data::type::list);
        kainjow::mustache::data output_properties(kainjow::mustache::data::type::list);
        bool                    auto_mapping = false;

        auto& fields = class_item->getFields();
        for (auto& field : fields) {
            if (field->shouldCompile(NativeProperty::PropertySetter))
                setters.push_back(kainjow::mustache::data("setter_name", field->name()));
            if (field->shouldCompile(NativeProperty::PropertyInput)) {
                auto_mapping = true;
                input_properties.push_back(kainjow::mustache::data("property_name", field->name()));
            }

            if (field->shouldCompile(NativeProperty::PropertyInputWithStatus)) {
                auto_mapping = true;
                input_with_status_properties.push_back(kainjow::mustache::data("property_name", field->name()));
            }

            if (field->shouldCompile(NativeProperty::PropertyOutput)) {
                auto_mapping = true;
                output_properties.push_back(kainjow::mustache::data("property_name", field->name()));
            }
        }

        action_node_constructor.set("class_path", class_item->getQualifiedName());
        action_node_constructor.set("class_name", class_item->getClassName());
        action_node_constructor.set("setters", setters);
        if (auto_mapping) {
            kainjow::mustache::data auto_mapping;
            auto_mapping.set("input_properties", input_properties);
            auto_mapping.set("input_with_status_properties", input_with_status_properties);
            auto_mapping.set("output_properties", output_properties);
            action_node_constructor.set("auto_mapping", auto_mapping);
        }

        m_custom_action_node_constructors.push_back(action_node_constructor);
    }

    return 0;
}
void BTCustomActionSetterGenerator::finish() {
    kainjow::mustache::data mustache_data;
    mustache_data.set("header_files", m_header_files);
    mustache_data.set("custom_action_node_constructors", m_custom_action_node_constructors);

    std::string                 mustache_content = ParserUtils::loadFile(m_mustache_path);
    kainjow::mustache::mustache tmpl(mustache_content);
    std::string                 render_string = tmpl.render(mustache_data);

    std::cout << "render complete : " << tmpl.error_message() << std::endl;
    ParserUtils::saveFile(render_string, m_out_path);
}
}  // namespace Generator