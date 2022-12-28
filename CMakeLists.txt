cmake_minimum_required(VERSION 3.14) # grpc require cmake version 3.14 above

project(BehaviorTree LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

IF (CMAKE_SYSTEM_NAME MATCHES "Linux")
    MESSAGE(STATUS "current platform: Linux ")
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")
    MESSAGE(STATUS "current platform: Windows")
ELSE ()
    MESSAGE(STATUS "other platform: ${CMAKE_SYSTEM_NAME}")
ENDIF (CMAKE_SYSTEM_NAME MATCHES "Linux")

set(TARGET_NAME BehaviorTreeTest)

file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/BehaviorTreeTest/*.h")
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/BehaviorTreeTest/*.cpp")
file(GLOB_RECURSE MODULE_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/module/*.h")
file(GLOB_RECURSE MODULE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/module/*.cpp")


add_executable(${TARGET_NAME}

    ${HEADERS}
    ${SOURCES}
    
    ${MODULE_HEADERS}
    ${MODULE_SOURCES}
    )

target_include_directories(${TARGET_NAME}
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/module"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/module/3rdparty"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/BehaviorTreeTest"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/BehaviorTreeTest/src/AI/BT"
)

# 设置 CppMetaParser 可执行文件的输出路径
set(CPP_META_PARSER_BIN "${CMAKE_CURRENT_SOURCE_DIR}/CppMetaParser/bin")
# 设置自定义行为树所需要的所有的头文件列表
file(GLOB_RECURSE AI_BT_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/module/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/BehaviorTreeTest/src/AI/BT/*.h" )
# 设置自定义行为树所需要的所有的头文件列表输出源文件路径
set(CUSTOM_NODES_IN_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CppMetaParser/cmake/ParserHeaders.txt.in")
# 设置自定义行为树所需要的所有的头文件列表输出文件路径
set(CUSTOM_NODES_INCLUDE_PATH "${CPP_META_PARSER_BIN}/ParserHeaders.txt")
# 进行文件生成
configure_file(${CUSTOM_NODES_IN_PATH} ${CUSTOM_NODES_INCLUDE_PATH})

# 设置生成文件输出路径
set(GENERATED_OUT_PUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/BehaviorTreeTest")
# 设置工作位置，第一项为CppMetaParser路径
set(PARSER_WORK_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/CppMetaParser/\;${CMAKE_CURRENT_SOURCE_DIR}/module/\;${CMAKE_CURRENT_SOURCE_DIR}/BehaviorTreeTest/src/AI/BT")

add_subdirectory(CppMetaParser)
add_dependencies(${TARGET_NAME} "PreCompile")
add_dependencies("PreCompile" "CppMateParser")

target_link_libraries(${TARGET_NAME} -Wl,--start-group pthread -Wl,--end-group)


