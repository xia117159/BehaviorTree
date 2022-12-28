#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <unistd.h>
#include "clang-c/Index.h"
#include "mustache.hpp"
#include "MetaParser/CppMetaParser.h"

int main(int argc, const char* argv[]) {
    /*const char* test_argv[] = { argv[0],
                                "/mnt/D/mycode/BehaviorTree/build/ParserHeaders.txt",
                                "/mnt/D/mycode/BehaviorTree/BehaviorTreeTest",
                                "/mnt/D/mycode/BehaviorTree/CppMetaParser/;/mnt/D/mycode/BehaviorTree/module/;/mnt/D/mycode/BehaviorTree/BehaviorTreeTest/src/AI/BT",
                                "/usr/include/c++/9/",
                                "1" };
    argv                    = test_argv;*/

    CppMetaParser parser(argv[1], argv[2], argv[3], argv[4], (std::string(argv[4]) == "1"));
    parser.parse();
    parser.generateFiles();

    return 0;
}
