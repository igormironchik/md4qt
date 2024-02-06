
project( test.md_parser.commonmark )

if( ENABLE_COVERAGE )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage" )
	set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage" )
endif( ENABLE_COVERAGE )

if( MSVC )
	add_compile_options( /bigobj )
	add_compile_options( /utf-8 )
endif()

file( GLOB MD_FILES 0.30/*.md )
file( COPY ${MD_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/../../../bin/tests/commonmark/0.30 )
file( GLOB MD_FILES_31 0.31.2/*.md )
file( COPY ${MD_FILES_31} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/../../../bin/tests/commonmark/0.31.2 )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../../..
	${CMAKE_CURRENT_SOURCE_DIR}/../../../3rdparty )

file( GLOB INL *.cpp )

if( BUILD_MD4QT_QT_TESTS )
	find_package( Qt6Core REQUIRED )

	set( COUNTER "0" )
	set( PCH_TARGET "" )

	foreach( inl ${INL} )
		set( NUMSTRING "" )

		math( EXPR COUNTER "${COUNTER}+1" OUTPUT_FORMAT DECIMAL )
		string( LENGTH ${COUNTER} COUNTER_DIGITS_COUNT )

		if( COUNTER_DIGITS_COUNT LESS 2 )
			string( APPEND NUMSTRING "0" )
		endif()

		string( APPEND NUMSTRING ${COUNTER} )

		add_executable( test.md_parser.commonmark.qt.${NUMSTRING} ${inl} )
		target_compile_definitions( test.md_parser.commonmark.qt.${NUMSTRING} PUBLIC TRAIT=MD::QStringTrait )
		target_compile_definitions( test.md_parser.commonmark.qt.${NUMSTRING} PUBLIC MD4QT_QT_SUPPORT )

		if( "${PCH_TARGET}" STREQUAL "" )
			set( PCH_TARGET "test.md_parser.commonmark.qt.${NUMSTRING}" )
			target_precompile_headers( ${PCH_TARGET} PRIVATE "load.hpp"
				<md4qt/parser.hpp>
				<md4qt/traits.hpp> )
		else()
			target_precompile_headers( test.md_parser.commonmark.qt.${NUMSTRING}
				REUSE_FROM ${PCH_TARGET} )
		endif()

		target_link_libraries( test.md_parser.commonmark.qt.${NUMSTRING} Qt6::Core )

		add_test( NAME test.md_parser.commonmark.qt.${NUMSTRING}
			COMMAND ${CMAKE_CURRENT_BINARY_DIR}/../../../bin/test.md_parser.commonmark.qt.${NUMSTRING}
			WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../../../bin )
	endforeach()
endif()

if( BUILD_MD4QT_STL_TESTS )
	find_package( ICU REQUIRED )
	find_package( uriparser REQUIRED )

	set( COUNTER "0" )
	set( PCH_TARGET "" )

	foreach( inl ${INL} )
		set( NUMSTRING "" )

		math( EXPR COUNTER "${COUNTER}+1" OUTPUT_FORMAT DECIMAL )
		string( LENGTH ${COUNTER} COUNTER_DIGITS_COUNT )

		if( COUNTER_DIGITS_COUNT LESS 2 )
			string( APPEND NUMSTRING "0" )
		endif()

		string( APPEND NUMSTRING ${COUNTER} )

		add_executable( test.md_parser.commonmark.icu.${NUMSTRING} ${inl} )
		target_compile_definitions( test.md_parser.commonmark.icu.${NUMSTRING} PUBLIC TRAIT=MD::UnicodeStringTrait )
		target_compile_definitions( test.md_parser.commonmark.icu.${NUMSTRING} PUBLIC MD4QT_ICU_STL_SUPPORT )

		if( "${PCH_TARGET}" STREQUAL "" )
			set( PCH_TARGET "test.md_parser.commonmark.icu.${NUMSTRING}" )
			target_precompile_headers( ${PCH_TARGET} PRIVATE "load.hpp"
				<md4qt/parser.hpp>
				<md4qt/traits.hpp> )
		else()
			target_precompile_headers( test.md_parser.commonmark.icu.${NUMSTRING}
				REUSE_FROM ${PCH_TARGET} )
		endif()

		target_link_libraries( test.md_parser.commonmark.icu.${NUMSTRING} icu::icu uriparser::uriparser )

		add_test( NAME test.md_parser.commonmark.icu.${NUMSTRING}
			COMMAND ${CMAKE_CURRENT_BINARY_DIR}/../../../bin/test.md_parser.commonmark.icu.${NUMSTRING}
			WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../../../bin )
	endforeach()
endif()
