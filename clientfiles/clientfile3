##
# @file CMakeLists.txt
# @brief
# RamGraph项目的根CMakeLists.txt
#
# @author niexw
# @email niexiaowen@uestc.edu.cn
#
project(RamGraph)
cmake_minimum_required(VERSION 2.8)
message(STATUS "### Begin to configure project RamGraph ###")

# 检测cmake运行操作系统/CPU/编译器
message(STATUS "Generator: ${CMAKE_GENERATOR}")
message(STATUS "Building OS: ${CMAKE_SYSTEM_NAME}")
message(STATUS "Building Processor: ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "Compiler: ${CMAKE_C_COMPILER_ID}")

# 打开vobose选项
set(CMAKE_VERBOSE_MAKEFILE on)

# 编译模式，Release/Debug，-DCMAKE_BUILD_TYPE=Release|Debug
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug) # Debug Release
endif()
string(TOLOWER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE)
if (CMAKE_BUILD_TYPE MATCHES "debug")
    set(CMAKE_BUILD_TYPE Debug)
else()
    set(CMAKE_BUILD_TYPE Release)
endif()
message(STATUS "Building mode: ${CMAKE_BUILD_TYPE}")

# c/c++按照11表展
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED OFF)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 设置编译结果输出路径
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${PROJECT_BINARY_DIR}/bin")

# g++编译器版本
if(CMAKE_C_COMPILER_VERSION VERSION_LESS "6.3")
    message(FATAL_ERROR "The least version of gcc is 6.3")
endif()

set(CMAKE_C_FLAGS                     "-Wall -Werror -Wno-attributes")
set(CMAKE_C_FLAGS_DEBUG               "-g -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE             "-O3")
set(CMAKE_C_FLAGS_MINSIZEREL          "-Os")
set(CMAKE_C_RELWITHDEBINFO_RELEASE    "-O2")
set(CMAKE_CXX_FLAGS                   "-Wall -Werror -Wno-attributes -ftemplate-backtrace-limit=0 -fno-exceptions")
set(CMAKE_CXX_FLAGS_DEBUG             "-g -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE           "-O3 -fno-strict-aliasing")
set(CMAKE_CXX_FLAGS_MINSIZEREL        "-Os")
set(CMAKE_CXX_RELWITHDEBINFO_RELEASE  "-O2")

set(CMAKE_EXE_LINKER_FLAGS_DEBUG      "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Bsymbolic -Bsymbolic-functions -Wl,--no-undefined")
set(CMAKE_MODULE_LINKER_FLAGS_DEBUG   "${CMAKE_MODULE_LINKER_FLAGS_DEBUG} -Bsymbolic -Bsymbolic-functions -Wl,--no-undefined")
set(CMAKE_SHARED_LINKER_FLAGS_DEBUG   "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} -Bsymbolic -Bsymbolic-functions -Wl,--no-undefined")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE    "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -s -Bsymbolic -Bsymbolic-functions -Wl,--no-undefined")
set(CMAKE_MODULE_LINKER_FLAGS_RELEASE "${CMAKE_MODULE_LINKER_FLAGS_RELEASE} -s -Bsymbolic -Bsymbolic-functions -Wl,--no-undefined")
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} -s -Bsymbolic -Bsymbolic-functions -Wl,--no-undefined")

# 生成config.h文件
configure_file(
    ${CMAKE_SOURCE_DIR}/config.h.in
    ${CMAKE_SOURCE_DIR}/include/ndsl/config.h
    NEWLINE_STYLE UNIX
)

# 指定编译子目录
add_subdirectory(src)
add_subdirectory(tests)

message(STATUS "### Done ###")
