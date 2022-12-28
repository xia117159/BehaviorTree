#pragma once
#include "GeneratorInterface.h"

namespace Generator {
/// @brief 行为树自定义节点添加到工厂的生成器
class BTCustomFactoryGenerator : public GeneratorInterface {
public:
    BTCustomFactoryGenerator(const std::string& out_path,
                             const std::string& mustache_path)
        : GeneratorInterface(out_path, mustache_path),
          m_header_files(kainjow::mustache::data::type::list),
          m_class_defines(kainjow::mustache::data::type::list) {}

    virtual int  generate(const std::string& header_path, const SchemaMoudle& schema) override;
    virtual void finish() override;

    virtual ~BTCustomFactoryGenerator() {}

protected:
    kainjow::mustache::data m_header_files;   // 头文件数据集合
    kainjow::mustache::data m_class_defines;  // 工厂添加数据集合
};
}  // namespace Generator