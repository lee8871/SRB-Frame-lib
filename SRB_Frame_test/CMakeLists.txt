﻿# CMakeList.txt: SRB_Frame 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.7)

project (proj_SRB_frame_test)




include_directories(./)
include_directories(../readonly/)
include_directories(../SRB_Frame)
include_directories(../SRB_Frame/usb_srb)
include_directories(../SRB_Frame/common_cluster)
include_directories(../support)
include_directories(../cLogger)
include_directories(../json)



aux_source_directory(. DIR_SRCS)


add_executable (SRB_Frame_tester ${DIR_SRCS})
target_link_libraries (SRB_Frame_tester srbframe)
target_link_libraries (SRB_Frame_tester clogger)

