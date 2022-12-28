#pragma once

#include "LanguageInfo/Class.h"

struct SchemaMoudle {
    std::string                         header_path;
    std::vector<std::shared_ptr<Class>> classes;
};