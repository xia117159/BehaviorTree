#pragma once

#include "Common/Global.h"
#include "MetaParser/SchemaMoudle.h"

namespace Generator {
/// @brief 生成器接口
class GeneratorInterface {
public:
    GeneratorInterface(const std::string& out_path,
                       const std::string& mustache_path)
        : m_out_path(out_path),
          m_mustache_path(mustache_path) {}

    /// @brief 对指定头文件内，满足条件的类类型添加到生成数据中，多个文件会调用多次
    /// @param header_path 头文件路径
    /// @param schema 满足条件的类类型集合
    /// @return 是否添加生成成功
    virtual int generate(const std::string& header_path, const SchemaMoudle& schema) = 0;

    /// @brief 进行生成数据转储文件
    virtual void finish(){};

    virtual ~GeneratorInterface(){};

protected:
    std::string m_out_path{ "gen_src" };  // 生成文件输出路径
    std::string m_mustache_path;          // 绑定的mustache文件路径
};
}  // namespace Generator