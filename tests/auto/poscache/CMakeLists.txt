# SPDX-FileCopyrightText: 2022-2025 Igor Mironchik <igor.mironchik@gmail.com>
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

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../..
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../3rdparty)

if(BUILD_MD4QT_QT_TESTS)
    find_package(Qt6Core REQUIRED)

    add_executable(test.poscache.qt ${SRC})
    target_compile_definitions(test.poscache.qt PUBLIC TRAIT=MD::QStringTrait)
    target_compile_definitions(test.poscache.qt PUBLIC MD4QT_QT_SUPPORT)
    target_link_libraries(test.poscache.qt Qt6::Core)

    add_test(NAME test.poscache.qt
        COMMAND ${CMAKE_CURRENT_BINARY_DIR}/../../../bin/test.poscache.qt
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../../../bin)
endif()

if(BUILD_MD4QT_STL_TESTS)
    find_package(ICU REQUIRED COMPONENTS data dt uc i18n io in tu)
    find_package(uriparser REQUIRED)

    add_executable(test.poscache.icu ${SRC})
    target_compile_definitions(test.poscache.icu PUBLIC TRAIT=MD::UnicodeStringTrait)
    target_compile_definitions(test.poscache.icu PUBLIC MD4QT_ICU_STL_SUPPORT)

    target_link_libraries(test.poscache.icu
        ICU::data ICU::dt ICU::uc ICU::i18n ICU::io ICU::in ICU::tu uriparser::uriparser)

    add_test(NAME test.poscache.icu
        COMMAND ${CMAKE_CURRENT_BINARY_DIR}/../../../bin/test.poscache.icu
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../../../bin)
endif()
