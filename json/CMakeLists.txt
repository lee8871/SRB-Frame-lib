cmake_minimum_required (VERSION 3.7)

project (proj_libjson)

include_directories(./)
include_directories(../json)
include_directories(../support)
include_directories(../cLogger)


aux_source_directory(. DIR_SRCS)
add_library (libjson
	${DIR_SRCS}
	../support/LString.cpp
	)

