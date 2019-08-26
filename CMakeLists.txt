## CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.7)

project (SRB_Frame_top)



option(MSCV_X86 "Is build for win32?" OFF)
option(MSCV_X64 "Is build for win64?" OFF)

IF(WIN32)
	if(MSCV_X86)
		SET(DEPEND_PATH D:/GitHubClone)
		MESSAGE("build for Window_x86")
		ADD_DEFINITIONS(-DWINDOW_86)
	endif(MSCV_X86)
ELSEIF(UNIX)
	add_definitions(-std=c++17)
	MESSAGE("build for linux")
	ADD_DEFINITIONS(-DUNIX)
	ADD_DEFINITIONS(-D_DEBUG)
ELSEIF(APPLE)
	MESSAGE(STATUS Sorry, we do not have a MAC, so may you port it by yourself.)
ENDIF(WIN32)

# This define opening Mapping array define in share files,
ADD_DEFINITIONS(-DBUILD_FOR_SRB_MASTER)

# 包含子项目。
add_subdirectory (SRB_Frame)
add_subdirectory (SRB_Frame_test)
add_subdirectory (SRB_node_list_test)
add_subdirectory (scan)
add_subdirectory (json)
add_subdirectory (test_hash)
add_subdirectory (test_json)
add_subdirectory (cLogger)