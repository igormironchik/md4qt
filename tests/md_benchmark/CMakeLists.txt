# SPDX-FileCopyrightText: 2022-2025 Igor Mironchik <igor.mironchik@gmail.com>
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.1)

SET(CMAKE_CXX_STANDARD 17)

SET(CMAKE_CXX_STANDARD_REQUIRED ON)

project(md_benchmark)

if(MSVC)
    add_compile_options(/bigobj)
    add_compile_options(/utf-8)
endif()

set(CMARK_TESTS OFF CACHE INTERNAL "" FORCE)
set(CMARK_SHARED OFF CACHE INTERNAL "" FORCE)
add_subdirectory(3rdparty/cmark-gfm)

include_directories(3rdparty/cmark-gfm/src
    3rdparty/cmark-gfm/extensions
    ${CMAKE_CURRENT_BINARY_DIR}/3rdparty/cmark-gfm/src
    ${CMAKE_CURRENT_BINARY_DIR}/3rdparty/cmark-gfm/extensions
    ${CMAKE_CURRENT_SOURCE_DIR}/../..)

find_package(ICU REQUIRED COMPONENTS data dt uc i18n io in tu)
find_package(uriparser REQUIRED)
find_package(Qt6Core REQUIRED)
find_package(Qt6Test REQUIRED)

set(CMAKE_AUTOMOC ON)

add_executable(md_benchmark main.cpp)

kde_target_enable_exceptions(md_benchmark PRIVATE)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../manual/complex.md
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/../../bin/tests/manual)

target_link_libraries(md_benchmark libcmark-gfm-extensions_static
    libcmark-gfm_static Qt::Test Qt::Core ICU::data ICU::dt ICU::uc ICU::i18n ICU::io ICU::in ICU::tu
    uriparser::uriparser)
