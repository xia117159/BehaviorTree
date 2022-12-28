set(PRECOMPILE_PARSER ${PRECOMPILE_TOOLS_PATH}/CppMateParser)

set(PRECOMPILE_TARGET "PreCompile")

if (CMAKE_HOST_WIN32)
    set(PRECOMPILE_PRE_EXE)
    set(PRECOMPILE_PARSER ${CPP_META_PARSER_BIN}/CppMateParser.exe)
    set(sys_include "*") 
elseif(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux" )
    set(PRECOMPILE_PRE_EXE)
    set(PRECOMPILE_PARSER ${CPP_META_PARSER_BIN}/CppMateParser)
    set(sys_include "/usr/include/c++/9/") 
endif()

add_custom_target(${PRECOMPILE_TARGET} ALL
COMMAND
  ${CMAKE_COMMAND} -E echo "************************************************************* "
COMMAND
  ${CMAKE_COMMAND} -E echo "**** [Precompile] Begin "
COMMAND
  ${CMAKE_COMMAND} -E echo "************************************************************* "

COMMAND
    # 1、进行文件生成所关联的所有的头文件列表文件：ParserHeaders.txt
    # 2、头文件列表转换为的临时头文件存储路径：ParserHeader.h
    # 3、解析器生成的文件输出路径
    # 4、系统头文件路径
    # 5、是否打开错误输出：0，不打开；1，打开
    ${PRECOMPILE_PARSER} "${CUSTOM_NODES_INCLUDE_PATH}"  "${GENERATED_OUT_PUT_PATH}" "${PARSER_WORK_PATHS}" ${sys_include} 1
COMMAND
    ${CMAKE_COMMAND} -E echo "************************************************************* "
COMMAND
    ${CMAKE_COMMAND} -E echo "**** [Precompile] Complete "
COMMAND
    ${CMAKE_COMMAND} -E echo "************************************************************* "
)

