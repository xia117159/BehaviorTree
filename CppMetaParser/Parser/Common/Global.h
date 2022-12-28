#pragma once

#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using Namespace = std::vector<std::string>;

#include "clang-c/Index.h"

#include "Meta/MetaDataConfig.h"

#include "mustache.hpp"

#include "ParserUtils/ParserUtils.h"
