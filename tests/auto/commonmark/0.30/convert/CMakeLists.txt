# SPDX-FileCopyrightText: 2022-2024 Igor Mironchik <igor.mironchik@gmail.com>
# SPDX-License-Identifier: MIT

project( convert )

set( CMAKE_CXX_STANDARD 17 )

set( CMAKE_CXX_STANDARD_REQUIRED ON )

set( SRC spec_to_mds.cpp )

add_executable( convert ${SRC} )
