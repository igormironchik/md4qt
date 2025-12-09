# SPDX-FileCopyrightText: 2025 Igor Mironchik <igor.mironchik@gmail.com>
# SPDX-License-Identifier: MIT

project(test.quick)

set(SRC main.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../..)

find_package(Qt6Core REQUIRED)

add_executable(test.quick ${SRC})
target_link_libraries(test.quick md4qt::md4qt Qt6::Core)
