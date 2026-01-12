# SPDX-FileCopyrightText: 2026 Igor Mironchik <igor.mironchik@gmail.com>
# SPDX-License-Identifier: MIT

project(test.poscache)

if(ENABLE_COVERAGE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
endif(ENABLE_COVERAGE)

if(MSVC)
    add_compile_options(/bigobj)
    add_compile_options(/utf-8)
endif()

set(SRC main.cpp)

find_package(Qt6 REQUIRED COMPONENTS Core)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../src
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../3rdparty)

add_executable(test.poscache ${SRC})
target_link_libraries(test.poscache md4qt::md4qt Qt6::Core)

add_test(NAME test.poscache
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/../../../bin/test.poscache
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../../../bin)
