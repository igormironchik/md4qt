
project( test.md_parser.commonmark )

find_package( Qt6Core REQUIRED )

if( ENABLE_COVERAGE )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage" )
	set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage" )
endif( ENABLE_COVERAGE )

if( MSVC )
	add_compile_options( /bigobj )
endif()

file( GLOB MD_FILES 0.30/*.md )
file( COPY ${MD_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/../../../bin/tests/commonmark/0.30 )
file( GLOB HPP *.hpp )
file( GLOB INL *.inl )

set( SRC main.cpp ${HPP} ${INL} )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../../..
	${CMAKE_CURRENT_SOURCE_DIR}/../../../3rdparty )

add_executable( test.md_parser.commonmark ${SRC} )

target_link_libraries( test.md_parser.commonmark Qt6::Core )

add_test( NAME test.md_parser.commonmark
	COMMAND ${CMAKE_CURRENT_BINARY_DIR}/../../../bin/test.md_parser.commonmark
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../../../bin )
