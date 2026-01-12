# SPDX-FileCopyrightText: 2026 Igor Mironchik <igor.mironchik@gmail.com>
# SPDX-License-Identifier: MIT

project(test.md_parser.gfm)

if(ENABLE_COVERAGE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
endif(ENABLE_COVERAGE)

file(GLOB MD_FILES data/*.md)
file(COPY ${MD_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/../../../bin/tests/gfm/data)

set(SRC main.cpp)

find_package(Qt6 REQUIRED COMPONENTS Core)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../src
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../3rdparty)

add_executable(test.md_parser.gfm ${SRC})

target_link_libraries(test.md_parser.gfm md4qt::md4qt Qt6::Core)

add_test(NAME test.md_parser.gfm
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/../../../bin/test.md_parser.gfm
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../../../bin)
