# SPDX-FileCopyrightText: 2022-2025 Igor Mironchik <igor.mironchik@gmail.com>
# SPDX-License-Identifier: MIT

project( entity )

set( CMAKE_CXX_STANDARD 17 )

set( CMAKE_CXX_STANDARD_REQUIRED ON )

set( SRC main.cpp )

file( COPY ${CMAKE_CURRENT_SOURCE_DIR}/entities.json
	DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )

add_executable( entity ${SRC} )
