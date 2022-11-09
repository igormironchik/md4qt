
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

if( BUILD_MD4QT_QT_TESTS )
	add_executable( test.md_parser.commonmark.qt ${SRC} )
	target_compile_definitions( test.md_parser.commonmark.qt PUBLIC TRAIT=MD::QStringTrait )
	target_compile_definitions( test.md_parser.commonmark.qt PUBLIC MD4QT_QT_SUPPORT )

	target_link_libraries( test.md_parser.commonmark.qt Qt6::Core )

	add_test( NAME test.md_parser.commonmark.qt
		COMMAND ${CMAKE_CURRENT_BINARY_DIR}/../../../bin/test.md_parser.commonmark.qt
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../../../bin )
endif()

if( BUILD_MD4QT_STL_TESTS )
	add_executable( test.md_parser.commonmark.stl ${SRC} )
	target_compile_definitions( test.md_parser.commonmark.stl PUBLIC TRAIT=MD::StdStringTrait )

	add_test( NAME test.md_parser.commonmark.stl
		COMMAND ${CMAKE_CURRENT_BINARY_DIR}/../../../bin/test.md_parser.commonmark.stl
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../../../bin )
endif()
