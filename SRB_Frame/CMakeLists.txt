


cmake_minimum_required (VERSION 3.7)

project (proj_SRB_frame)


IF(WIN32)
	include_directories(${LIBUSB_1_0_22_PATH}/include/libusb-1.0)
	if(MSCV_X86)	
		find_library(LIBUSB_LIB libusb-1.0 ${LIBUSB_1_0_22_PATH}/MS64/dll)
	endif(MSCV_X86)
	if(MSCV_X64)	
		find_library(LIBUSB_LIB libusb-1.0 ${LIBUSB_1_0_22_PATH}/MS64/dll)
	endif(MSCV_X64)
	link_libraries(${LIBUSB_LIB})




ELSEIF(UNIX)
include_directories(/usr/include/libusb-1.0)
find_library(LIBUSB_LIB usb-1.0)
link_libraries(${LIBUSB_LIB})
ELSEIF(APPLE)
	MESSAGE(STATUS Sorry, we do not have a MAC, so may you port it by yourself.)
ENDIF(WIN32)


include_directories(
	../readonly/
	./
	./usb_srb/
	./common_cluster/
	../support/
	)
include_directories(../cLogger)
include_directories(../json)


aux_source_directory(.						DIR_SRCS)
aux_source_directory(./usb_srb				DIR_USB)
aux_source_directory(./common_cluster		DIR_COMMON_CLUSTER)
aux_source_directory(./Nodes/MotorX2		DIR_NODES)
aux_source_directory(./Nodes/LiBatX2		DIR_NODES)
aux_source_directory(./Nodes/Ps2Handle		DIR_NODES)
aux_source_directory(../support				DIR_SPPORT)
add_library (srbframe
	${DIR_SRCS}
	${DIR_USB}
	${DIR_COMMON_CLUSTER}
	${DIR_SPPORT} 
	${DIR_NODES} 
	)
	
target_link_libraries (srbframe clogger)
target_link_libraries (srbframe cjson)

