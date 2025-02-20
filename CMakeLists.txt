# SPDX-FileCopyrightText: 2022-2025 Igor Mironchik <igor.mironchik@gmail.com>
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.19)

project(md4qt VERSION "4.0.1")

set(MD4QT_VERSION ${PROJECT_VERSION})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(BUILD_MD4QT_BENCHMARK "Build benchmark? Default OFF." OFF)
option(BUILD_MD4QT_QT_TESTS "Build tests with Qt support? Default ON." ON)
option(BUILD_MD4QT_STL_TESTS "Build tests with STL support? Default OFF." OFF)
option(BUILD_MD2HTML_APP "Build md2html utility? Default OFF." OFF)
option(BUILD_TEST_BENCH "Build test benchmark? Default ON." ON)

set(QT_MIN_VERSION "6.0")
set(KF_MIN_VERSION "6.0")

find_package(ECM ${KF_MIN_VERSION} REQUIRED NO_MODULE)

list(APPEND CMAKE_MODULE_PATH ${ECM_MODULE_PATH})
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_BINARY_DIR})
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_BINARY_DIR})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMSetupVersion)

set(CMAKE_WIN32_EXECUTABLE FALSE)
set(CMAKE_MACOSX_BUNDLE FALSE)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release"
         CACHE STRING "Choose the type of build."
         FORCE)
endif(NOT CMAKE_BUILD_TYPE)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})

    set(CMAKE_FIND_PACKAGE_PREFER_CONFIG TRUE)

    if(BUILD_MD4QT_QT_TESTS OR BUILD_MD4QT_STL_TESTS OR BUILD_TEST_BENCH)
        enable_testing()

        add_subdirectory(tests)
    endif()

    if(BUILD_MD4QT_BENCHMARK)
        add_subdirectory(tests/md_benchmark)
    endif(BUILD_MD4QT_BENCHMARK)

    file(GLOB_RECURSE SRC md4qt/*)

    add_library(md4qt INTERFACE ${SRC})
    add_library(md4qt::md4qt ALIAS md4qt)

    target_include_directories(md4qt INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    )

    ecm_setup_version(${PROJECT_VERSION}
        VARIABLE_PREFIX MD4QT
        VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/config-md4qt.h"
    )

    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/config-md4qt.h"
            DESTINATION "${KDE_INSTALL_INCLUDEDIR}/md4qt")

    install(DIRECTORY md4qt
            DESTINATION ${KDE_INSTALL_INCLUDEDIR}
    )

    install(TARGETS md4qt
            EXPORT md4qt-targets
            RUNTIME DESTINATION ${KDE_INSTALL_BINDIR}
            ARCHIVE DESTINATION ${KDE_INSTALL_LIBDIR}
            LIBRARY DESTINATION ${KDE_INSTALL_LIBDIR}
            INCLUDES DESTINATION ${KDE_INSTALL_INCLUDEDIR}
    )

    set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/md4qt")

    install(EXPORT md4qt-targets
            DESTINATION ${CMAKECONFIG_INSTALL_DIR}
            NAMESPACE md4qt::
    )

    include(CMakePackageConfigHelpers)

    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/md4qt-config-version.cmake"
        VERSION ${MD4QT_VERSION}
        COMPATIBILITY AnyNewerVersion)

    configure_package_config_file(md4qt-config.cmake.in
        "${CMAKE_CURRENT_BINARY_DIR}/md4qt-config.cmake"
        INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR})

    install(FILES ${PROJECT_BINARY_DIR}/md4qt-config.cmake
        ${PROJECT_BINARY_DIR}/md4qt-config-version.cmake
        DESTINATION ${CMAKECONFIG_INSTALL_DIR})

    if(BUILD_MD2HTML_APP)
        include_directories(${CMAKE_CURRENT_SOURCE_DIR})

        find_package(Qt6Core REQUIRED)

        add_executable(md2html md2html/main.cpp)

        target_link_libraries(md2html Qt6::Core)
    endif()

else()

    set(md4qt_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE)
    set(md4qt_VERSION_STRING ${MD4QT_VERSION} PARENT_SCOPE)

endif(${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})
