
project( test.md_parser.gfm )

find_package( Qt6Core REQUIRED )

if( ENABLE_COVERAGE )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage" )
	set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage" )
endif( ENABLE_COVERAGE )

file( GLOB MD_FILES data/*.md )
file( COPY ${MD_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/../../../bin/tests/gfm/data )

set( SRC main.cpp )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../../..
	${CMAKE_CURRENT_SOURCE_DIR}/../../../3rdparty )

if( BUILD_MD4QT_QT_TESTS )
	add_executable( test.md_parser.gfm.qt ${SRC} )
	target_compile_definitions( test.md_parser.gfm.qt PUBLIC TRAIT=MD::QStringTrait )

	target_link_libraries( test.md_parser.gfm.qt Qt6::Core )

	add_test( NAME test.md_parser.gfm.qt
		COMMAND ${CMAKE_CURRENT_BINARY_DIR}/../../../bin/test.md_parser.gfm.qt
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../../../bin )
endif()

if( BUILD_MD4QT_STL_TESTS )
	add_executable( test.md_parser.gfm.stl ${SRC} )
	target_compile_definitions( test.md_parser.gfm.stl PUBLIC TRAIT=MD::StdStringTrait )

	target_link_libraries( test.md_parser.gfm.stl icu::icu )

	add_test( NAME test.md_parser.gfm.stl
		COMMAND ${CMAKE_CURRENT_BINARY_DIR}/../../../bin/test.md_parser.gfm.stl
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../../../bin )
endif()
