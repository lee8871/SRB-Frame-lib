


cmake_minimum_required (VERSION 3.7)

project (proj_SRB_frame)


IF(WIN32)
	if(MSCV_X86)	
		include_directories(${DEPEND_PATH}/libusb-master/libusb/)
		find_library(LIBUSB_LIB libusb-1.0 ${DEPEND_PATH}/libusb-1.0.22/MS32/dll)
		link_libraries(${LIBUSB_LIB})
	endif(MSCV_X86)
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
aux_source_directory(./Nodes/dumotor		DIR_DUMOTOR)
aux_source_directory(../support				DIR_SPPORT)
add_library (SRB_Frame_lib
	${DIR_SRCS}
	${DIR_USB}
	${DIR_COMMON_CLUSTER}
	${DIR_SPPORT} 
	${DIR_DUMOTOR} 
	)
	
target_link_libraries (SRB_Frame_lib libclogger)
target_link_libraries (SRB_Frame_lib libjson)

