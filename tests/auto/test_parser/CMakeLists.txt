
project( test.md_parser )

find_package( Qt6Core REQUIRED )

if( ENABLE_COVERAGE )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage" )
	set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage" )
endif( ENABLE_COVERAGE )

set( SRC main.cpp )

file( GLOB MD_FILES data/*.md )
file( COPY ${MD_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/../../../bin/tests/parser/data )

file( COPY data/a.png
	DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/../../../bin/tests/parser/data )
file( COPY data/a.jpg
	DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/../../../bin/tests/parser/data )
file( COPY data/b.png
	DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/../../../bin/tests/parser/data )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../../..
	${CMAKE_CURRENT_SOURCE_DIR}/../../../3rdparty )

if( BUILD_MD4QT_QT_TESTS )
	add_executable( test.md_parser.qt ${SRC} )
	target_compile_definitions( test.md_parser.qt PUBLIC TRAIT=MD::QStringTrait )
	target_compile_definitions( test.md_parser.qt PUBLIC MD4QT_QT_SUPPORT )

	target_link_libraries( test.md_parser.qt Qt6::Core )

	add_test( NAME test.md_parser.qt
		COMMAND ${CMAKE_CURRENT_BINARY_DIR}/../../../bin/test.md_parser.qt
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../../../bin )
endif()

if( BUILD_MD4QT_STL_TESTS )
	find_package( ICU )

	add_executable( test.md_parser.stl ${SRC} )
	target_compile_definitions( test.md_parser.stl PUBLIC TRAIT=MD::UnicodeStringTrait )
	target_compile_definitions( test.md_parser.stl PUBLIC MD4QT_ICU_STL_SUPPORT )

	target_link_libraries( test.md_parser.qt icu::icu )

	add_test( NAME test.md_parser.stl
		COMMAND ${CMAKE_CURRENT_BINARY_DIR}/../../../bin/test.md_parser.stl
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../../../bin )
endif()
