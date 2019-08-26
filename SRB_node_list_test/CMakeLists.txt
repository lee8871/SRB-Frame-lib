cmake_minimum_required (VERSION 3.7)

project (proj_SRB_Node_list_read)




include_directories(../readonly/)
include_directories(./)
include_directories(../SRB_Frame)
include_directories(../SRB_Frame/usb_srb)
include_directories(../SRB_Frame/common_cluster)
include_directories(../support)
include_directories(../cLogger)

aux_source_directory(. DIR_SRCS)

add_executable (SRB_node_list_test ${DIR_SRCS})
target_link_libraries (SRB_node_list_test SRB_Frame_lib)
target_link_libraries (SRB_node_list_test libclogger)

