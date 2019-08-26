


cmake_minimum_required (VERSION 3.7)

project (proj_srbscan)

IF(WIN32)
include_directories(D:/GitHubClone/argh-master/)
ELSEIF(UNIX)

ELSEIF(APPLE)

ENDIF(WIN32)



include_directories(./)
include_directories(../readonly/)
include_directories(../SRB_Frame)
include_directories(../SRB_Frame/usb_srb)
include_directories(../SRB_Frame/common_cluster)
include_directories(../support)
include_directories(../cLogger)
include_directories(../json)

aux_source_directory(. DIR_SRCS)

add_executable (srbscan ${DIR_SRCS})
target_link_libraries (srbscan SRB_Frame_lib)
target_link_libraries (srbscan libclogger)

