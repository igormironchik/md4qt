# SPDX-FileCopyrightText: 2022-2024 Igor Mironchik <igor.mironchik@gmail.com>
# SPDX-License-Identifier: MIT

project( test.html )

if( ENABLE_COVERAGE )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage" )
	set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage" )
endif( ENABLE_COVERAGE )

if( MSVC )
	add_compile_options( /bigobj )
	add_compile_options( /utf-8 )
endif()

set( SRC main.cpp )

file( GLOB MD_FILES data/*.md )
file( COPY ${MD_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/../../../bin/tests/html/data )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../../..
	${CMAKE_CURRENT_SOURCE_DIR}/../../../3rdparty )

file( GLOB MAIN *.cpp )

if( BUILD_MD4QT_QT_TESTS )
	find_package( Qt6Core REQUIRED )

	set( COUNTER "0" )
	set( PCH_TARGET "" )

	foreach( main ${MAIN} )
		set( NUMSTRING "" )

		math( EXPR COUNTER "${COUNTER}+1" OUTPUT_FORMAT DECIMAL )
		string( LENGTH ${COUNTER} COUNTER_DIGITS_COUNT )

		if( COUNTER_DIGITS_COUNT LESS 2 )
			string( APPEND NUMSTRING "0" )
		endif()

		string( APPEND NUMSTRING ${COUNTER} )

		add_executable( test.html.qt.${NUMSTRING} ${main} )
		target_compile_definitions( test.html.qt.${NUMSTRING} PUBLIC TRAIT=MD::QStringTrait )
		target_compile_definitions( test.html.qt.${NUMSTRING} PUBLIC MD4QT_QT_SUPPORT )

		if( "${PCH_TARGET}" STREQUAL "" )
			set( PCH_TARGET "test.html.qt.${NUMSTRING}" )
			target_precompile_headers( ${PCH_TARGET} PRIVATE
				<md4qt/parser.hpp>
				<md4qt/traits.hpp>
				<md4qt/html.hpp>
				<fstream>
				<cstring>
				<QDir>
				<QTextStream>
				<QFile> )
		else()
			target_precompile_headers( test.html.qt.${NUMSTRING}
				REUSE_FROM ${PCH_TARGET} )
		endif()

		target_link_libraries( test.html.qt.${NUMSTRING} Qt6::Core )

		add_test( NAME test.html.qt.${NUMSTRING}
			COMMAND ${CMAKE_CURRENT_BINARY_DIR}/../../../bin/test.html.qt.${NUMSTRING}
			WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../../../bin )
	endforeach()
endif()

if( BUILD_MD4QT_STL_TESTS )
	find_package( ICU REQUIRED COMPONENTS data dt uc i18n io in tu )
	find_package( uriparser REQUIRED )

	set( COUNTER "0" )
	set( PCH_TARGET "" )

	foreach( main ${MAIN} )
		set( NUMSTRING "" )

		math( EXPR COUNTER "${COUNTER}+1" OUTPUT_FORMAT DECIMAL )
		string( LENGTH ${COUNTER} COUNTER_DIGITS_COUNT )

		if( COUNTER_DIGITS_COUNT LESS 2 )
			string( APPEND NUMSTRING "0" )
		endif()

		string( APPEND NUMSTRING ${COUNTER} )

		add_executable( test.html.icu.${NUMSTRING} ${main} )
		target_compile_definitions( test.html.icu.${NUMSTRING} PUBLIC TRAIT=MD::UnicodeStringTrait )
		target_compile_definitions( test.html.icu.${NUMSTRING} PUBLIC MD4QT_ICU_STL_SUPPORT )

		if( "${PCH_TARGET}" STREQUAL "" )
			set( PCH_TARGET "test.html.icu.${NUMSTRING}" )
			target_precompile_headers( ${PCH_TARGET} PRIVATE
				<md4qt/parser.hpp>
				<md4qt/traits.hpp>
				<md4qt/html.hpp>
				<fstream>
				<cstring>
				<filesystem> )
		else()
			target_precompile_headers( test.html.icu.${NUMSTRING}
				REUSE_FROM ${PCH_TARGET} )
		endif()

		target_link_libraries( test.html.icu.${NUMSTRING}
			ICU::data ICU::dt ICU::uc ICU::i18n ICU::io ICU::in ICU::tu uriparser::uriparser )

		add_test( NAME test.html.icu.${NUMSTRING}
			COMMAND ${CMAKE_CURRENT_BINARY_DIR}/../../../bin/test.html.icu.${NUMSTRING}
			WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../../../bin )
	endforeach()
endif()
