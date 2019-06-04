# CMakeList.txt: SRB_Frame 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project (proj_SRB_frame)

option(MSCV_X86 "Is build for win32?" OFF)
option(MSCV_X64 "Is build for win64?" OFF)

IF(WIN32)
	link_libraries("libusb-1.0.lib")
	if(MSCV_X86)
		link_directories("./Win32/libusb")
		MESSAGE("build for Window_x86")
		ADD_DEFINITIONS(-DWINDOW_86)
	endif()
ELSEIF(UNIX)
	link_libraries(usb-1.0)
	add_definitions(-std=c++11)
	MESSAGE("build for linux")
	ADD_DEFINITIONS(-DUNIX)
ELSEIF(APPLE)
	MESSAGE(STATUS Sorry, we do not have a MAC, so may you port it by yourself.)
ENDIF(WIN32)

include_directories(../readonly/)
include_directories(./)
include_directories(./usb_srb)
include_directories(./common_cluster)

aux_source_directory(. DIR_SRCS)
aux_source_directory(./usb_srb DIR_USB)
aux_source_directory(./common_cluster DIR_COMMON_CLUSTER)

add_executable (SRB_Frame_tester ${DIR_SRCS} ${DIR_USB} ${DIR_COMMON_CLUSTER} )


# TODO: 如有需要，请添加测试并安装目标。
