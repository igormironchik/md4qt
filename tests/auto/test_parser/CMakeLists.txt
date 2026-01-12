# SPDX-FileCopyrightText: 2026 Igor Mironchik <igor.mironchik@gmail.com>
# SPDX-License-Identifier: MIT

project(test.md_parser)

if(ENABLE_COVERAGE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
endif(ENABLE_COVERAGE)

if(MSVC)
    add_compile_options(/bigobj)
    add_compile_options(/utf-8)
endif()

set(SRC main.cpp)

file(GLOB MD_FILES data/*.md)
file(COPY ${MD_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/../../../bin/tests/parser/data)

file(COPY data/a.png
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/../../../bin/tests/parser/data)
file(COPY data/a.jpg
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/../../../bin/tests/parser/data)
file(COPY data/b.png
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/../../../bin/tests/parser/data)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../src
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../3rdparty)

file(GLOB MAIN *.cpp)

find_package(Qt6 REQUIRED COMPONENTS Core)

set(COUNTER "0")
set(PCH_TARGET "")

foreach(main ${MAIN})
    set(NUMSTRING "")

    math(EXPR COUNTER "${COUNTER}+1" OUTPUT_FORMAT DECIMAL)
    string(LENGTH ${COUNTER} COUNTER_DIGITS_COUNT)

    if(COUNTER_DIGITS_COUNT LESS 2)
        string(APPEND NUMSTRING "0")
    endif()

    string(APPEND NUMSTRING ${COUNTER})

    add_executable(test.md_parser.${NUMSTRING} ${main})

    target_link_libraries(test.md_parser.${NUMSTRING} md4qt::md4qt Qt6::Core)

    add_test(NAME test.md_parser.${NUMSTRING}
        COMMAND ${CMAKE_CURRENT_BINARY_DIR}/../../../bin/test.md_parser.${NUMSTRING}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../../../bin)
endforeach()
