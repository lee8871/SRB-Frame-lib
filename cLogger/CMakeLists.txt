cmake_minimum_required (VERSION 3.7)

project (proj_cLogger)
include_directories(
	./
	../support/
	)

aux_source_directory(. DIR_SRCS)
add_library (cLogger_lib
	${DIR_SRCS}
	)


