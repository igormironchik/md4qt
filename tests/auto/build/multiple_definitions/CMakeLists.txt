# SPDX-FileCopyrightText: 2022-2024 Igor Mironchik <igor.mironchik@gmail.com>
# SPDX-License-Identifier: MIT

project( test.multiple_definitions )

set( SRC 1.hpp 1.cpp 2.hpp 2.cpp main.cpp )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../../../.. )

if( BUILD_MD4QT_QT_TESTS )
	find_package( Qt6Core REQUIRED )

	add_executable( test.multiple_definitions.qt ${SRC} )
	target_compile_definitions( test.multiple_definitions.qt PUBLIC TRAIT=MD::QStringTrait )
	target_compile_definitions( test.multiple_definitions.qt PUBLIC MD4QT_QT_SUPPORT )
	target_precompile_headers( test.multiple_definitions.qt PRIVATE
		<md4qt/parser.hpp> )

	target_link_libraries( test.multiple_definitions.qt Qt6::Core )
endif()

if( BUILD_MD4QT_STL_TESTS )
	find_package( ICU REQUIRED )
	find_package( uriparser REQUIRED )

	add_executable( test.multiple_definitions.icu ${SRC} )
	target_compile_definitions( test.multiple_definitions.icu PUBLIC TRAIT=MD::UnicodeStringTrait )
	target_compile_definitions( test.multiple_definitions.icu PUBLIC MD4QT_ICU_STL_SUPPORT )
	target_precompile_headers( test.multiple_definitions.icu PRIVATE
		<md4qt/parser.hpp> )

	target_link_libraries( test.multiple_definitions.icu icu::icu uriparser::uriparser )
endif()
