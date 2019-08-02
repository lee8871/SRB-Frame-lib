# CMakeList.txt: SRB_Frame 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.7)

project (proj_test_json)




include_directories(./)
include_directories(../support)



aux_source_directory(. DIR_SRCS)


add_executable (test_json ${DIR_SRCS} "../support/Json.cpp")


