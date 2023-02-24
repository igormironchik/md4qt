
project( test.internal_string )

if( ENABLE_COVERAGE )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage" )
	set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage" )
endif( ENABLE_COVERAGE )

set( SRC main.cpp )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../../..
	${CMAKE_CURRENT_SOURCE_DIR}/../../../3rdparty )

if( BUILD_MD4QT_QT_TESTS )
	find_package( Qt6Core REQUIRED )

	add_executable( test.internal_string.qt ${SRC} )
	target_compile_definitions( test.internal_string.qt PUBLIC TRAIT=MD::QStringTrait )
	target_compile_definitions( test.internal_string.qt PUBLIC MD4QT_QT_SUPPORT )

	target_link_libraries( test.internal_string.qt Qt6::Core )

	add_test( NAME test.internal_string.qt
		COMMAND ${CMAKE_CURRENT_BINARY_DIR}/../../../bin/test.internal_string.qt
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../../../bin )
endif()

if( BUILD_MD4QT_STL_TESTS )
	find_package( ICU REQUIRED )
	find_package( uriparser REQUIRED )

	add_executable( test.internal_string.stl ${SRC} )
	target_compile_definitions( test.internal_string.stl PUBLIC TRAIT=MD::UnicodeStringTrait )
	target_compile_definitions( test.internal_string.stl PUBLIC MD4QT_ICU_STL_SUPPORT )

	target_link_libraries( test.internal_string.stl icu::icu uriparser::uriparser )

	add_test( NAME test.internal_string.stl
		COMMAND ${CMAKE_CURRENT_BINARY_DIR}/../../../bin/test.internal_string.stl
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../../../bin )
endif()
