# SPDX-FileCopyrightText: 2026 Igor Mironchik <igor.mironchik@gmail.com>
# SPDX-License-Identifier: MIT

project(tests)

if(ECM_FOUND)
    kde_enable_exceptions()
endif()

if(BUILD_MD4QT_TESTS)
    add_subdirectory(test_funcs)
    add_subdirectory(test_parser)
    add_subdirectory(commonmark)
    add_subdirectory(gfm)
    add_subdirectory(algo)
    add_subdirectory(poscache)
    add_subdirectory(html)
    add_subdirectory(plugins)
    add_subdirectory(bench)
endif()
