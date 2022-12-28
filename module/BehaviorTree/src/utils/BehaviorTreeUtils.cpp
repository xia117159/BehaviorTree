#include <cstdlib>
#include <time.h>
#include "BehaviorTree/include/utils/BehaviorTreeUtils.h"

namespace BT {
std::vector<std::string> split(const std::string& input, const std::string& pat) {
    std::string              input_op = input;
    std::vector<std::string> ret_list;
    while (true) {
        size_t      index    = input_op.find(pat);
        std::string sub_list = input_op.substr(0, index);
        if (!sub_list.empty()) {

            ret_list.push_back(sub_list);
        }
        input_op.erase(0, index + pat.size());
        if (index == -1) {
            break;
        }
    }
    return ret_list;
}

std::string trim(std::string& source_string, const std::string trim_chars) {
    size_t left_pos = source_string.find_first_not_of(trim_chars);
    if (left_pos == std::string::npos) {
        source_string = std::string();
    }
    else {
        size_t right_pos = source_string.find_last_not_of(trim_chars);
        source_string    = source_string.substr(left_pos, right_pos - left_pos + 1);
    }
    return source_string;
}

int random(int min, int max, bool has_max) {
    std::srand(clock());
    return std::rand() % (max - min + static_cast<int>(has_max)) + min;
}
}  // namespace BT