# SPDX-FileCopyrightText: 2025 Igor Mironchik <igor.mironchik@gmail.com>
# SPDX-License-Identifier: MIT

project(test.funcs)

if(ENABLE_COVERAGE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
endif(ENABLE_COVERAGE)

set(SRC main.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../src
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../3rdparty)

find_package(Qt6Core REQUIRED)

add_executable(test.funcs ${SRC})

target_link_libraries(test.funcs md4qt::md4qt Qt6::Core)

add_test(NAME test.funcs
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/../../../bin/test.funcs
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../../../bin)
