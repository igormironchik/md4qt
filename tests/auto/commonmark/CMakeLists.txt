# SPDX-FileCopyrightText: 2025 Igor Mironchik <igor.mironchik@gmail.com>
# SPDX-License-Identifier: MIT

project(test.md_parser.commonmark)

if(ENABLE_COVERAGE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
endif(ENABLE_COVERAGE)

if(MSVC)
    add_compile_options(/bigobj)
    add_compile_options(/utf-8)
endif()

file(GLOB MD_FILES 0.30/*.md)
file(COPY ${MD_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/../../../bin/tests/commonmark/0.30)
file(GLOB MD_FILES_31 0.31.2/*.md)
file(COPY ${MD_FILES_31} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/../../../bin/tests/commonmark/0.31.2)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../src
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../3rdparty)

file(GLOB INL *.cpp)

find_package(Qt6Core REQUIRED)

set(COUNTER "0")
set(PCH_TARGET "")

foreach(inl ${INL})
    set(NUMSTRING "")

    math(EXPR COUNTER "${COUNTER}+1" OUTPUT_FORMAT DECIMAL)
    string(LENGTH ${COUNTER} COUNTER_DIGITS_COUNT)

    if(COUNTER_DIGITS_COUNT LESS 2)
        string(APPEND NUMSTRING "0")
    endif()

    string(APPEND NUMSTRING ${COUNTER})

    add_executable(test.md_parser.commonmark.${NUMSTRING} ${inl})

    target_link_libraries(test.md_parser.commonmark.${NUMSTRING} md4qt::md4qt Qt6::Core)

    add_test(NAME test.md_parser.commonmark.${NUMSTRING}
        COMMAND ${CMAKE_CURRENT_BINARY_DIR}/../../../bin/test.md_parser.commonmark.${NUMSTRING}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../../../bin)
endforeach()
