# SPDX-FileCopyrightText: 2022-2025 Igor Mironchik <igor.mironchik@gmail.com>
# SPDX-License-Identifier: MIT

project(tests)

kde_enable_exceptions()

if(BUILD_MD4QT_QT_TESTS OR BUILD_MD4QT_STL_TESTS)
    add_subdirectory(test_funcs)
    add_subdirectory(test_parser)
    add_subdirectory(commonmark)
    add_subdirectory(gfm)
    add_subdirectory(test_build)
    add_subdirectory(internal_string)
    add_subdirectory(algo)
    add_subdirectory(poscache)
    add_subdirectory(html)
endif()

if(BUILD_TEST_BENCH)
    add_subdirectory(bench)
endif()
