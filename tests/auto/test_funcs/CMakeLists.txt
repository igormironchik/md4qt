
project( test.funcs )

find_package( Qt6Core REQUIRED )

if( ENABLE_COVERAGE )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage" )
	set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage" )
endif( ENABLE_COVERAGE )

set( SRC main.cpp )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../../..
	${CMAKE_CURRENT_SOURCE_DIR}/../../../3rdparty )

if( BUILD_MD4QT_QT_TESTS )
	add_executable( test.funcs.qt ${SRC} )
	target_compile_definitions( test.funcs.qt PUBLIC TRAIT=MD::QStringTrait )

	target_link_libraries( test.funcs.qt Qt6::Core )

	add_test( NAME test.funcs.qt
		COMMAND ${CMAKE_CURRENT_BINARY_DIR}/../../../bin/test.funcs.qt
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../../../bin )
endif()

if( BUILD_MD4QT_STL_TESTS )
	add_executable( test.funcs.stl ${SRC} )
	target_compile_definitions( test.funcs.stl PUBLIC TRAIT=MD::StdStringTrait )

	target_link_libraries( test.funcs.stl icu::icu )

	add_test( NAME test.funcs.stl
		COMMAND ${CMAKE_CURRENT_BINARY_DIR}/../../../bin/test.funcs.stl
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../../../bin )
endif()
