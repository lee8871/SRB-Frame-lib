cmake_minimum_required (VERSION 3.7)

project (proj_test_json)

include_directories(./)
include_directories(../json)
include_directories(../support)
include_directories(../cLogger)



aux_source_directory(../json				DIR_JSON)
aux_source_directory(../support				DIR_SPPORT)


add_executable (json_test
	./test.cpp
	${DIR_JSON}
	${DIR_SPPORT}
	)


target_link_libraries (json_test cLogger_lib)

add_executable (json_test2 
	./test2.cpp
	${DIR_JSON}
	${DIR_SPPORT}
	)


target_link_libraries (json_test2 cLogger_lib)




