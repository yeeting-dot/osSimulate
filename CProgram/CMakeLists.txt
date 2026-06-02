cmake_minimum_required(VERSION 3.10)

# 启用 MSVC 热重载（可选，保留原有逻辑）
if(POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# 项目名称和语言（显式指定为 C）
project(CProgram LANGUAGES C)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/target")


# 添加可执行文件，只列出源文件
add_executable(CProgram 
"main.c" "source/round/round.c" "head/tool/tool.h"
"source/tool/tool.c"  "source/tool/maxHeap/MaxHeap.c" "head/tool/hashTable/hashTable.h" 
"source/tool/hashTable/hashTable.c"  "head/tool/maxHeap/maxHeap.h" "head/pageBest/pageBest.h"
"source/pageBest/pageBest.c"   "source/test/test.c" "head/pageBest/pageBestNew.h" "source/pageBest/pageBestNew.c"
"head/groupConnect/groupConnect.h" "source/groupConnect/groupConnect.c")

# 设置头文件搜索路径（现代写法）
target_include_directories(CProgram PRIVATE 
"${PROJECT_SOURCE_DIR}/head/round" "${PROJECT_SOURCE_DIR}/head/tool" 
"${PROJECT_SOURCE_DIR}/head/tool/maxHeap" "${PROJECT_SOURCE_DIR}/head/tool/hashTable"
"${PROJECT_SOURCE_DIR}/head/pageBest" "${PROJECT_SOURCE_DIR}/head/groupConnect"
)
# 设置 C 语言标准（例如 C11 或 C17）
set_target_properties(CProgram PROPERTIES
    C_STANDARD 11
    C_STANDARD_REQUIRED ON
)

# 可选：如果需要特定平台的编译选项，可以在这里添加