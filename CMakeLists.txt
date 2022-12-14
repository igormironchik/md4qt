
cmake_minimum_required( VERSION 3.19 )

project( md4qt )

set( MD4QT_VERSION "2.0.4" )

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )

option( BUILD_MD4QT_BENCHMARK "Build benchmark? Default OFF." OFF )
option( BUILD_MD4QT_TESTS "Build tests? Default ON." ON )
option( BUILD_MD4QT_QT_TESTS "Build tests with Qt support? Default ON." ON )
option( BUILD_MD4QT_STL_TESTS "Build tests with STL support? Default OFF." OFF )

set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_BINARY_DIR} )
set( CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${CMAKE_CURRENT_BINARY_DIR} )

if( NOT CMAKE_BUILD_TYPE )
	set( CMAKE_BUILD_TYPE "Release"
		CACHE STRING "Choose the type of build."
		FORCE )
endif( NOT CMAKE_BUILD_TYPE )

set( CMAKE_CXX_STANDARD 17 )

set( CMAKE_CXX_STANDARD_REQUIRED ON )

if( ${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME} )

	if( MD4QT_USE_CONAN )
		include( ${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake )
		conan_basic_setup()
	endif()

	if( BUILD_MD4QT_TESTS AND ( BUILD_MD4QT_QT_TESTS OR BUILD_MD4QT_STL_TESTS ) )
		enable_testing()

		add_subdirectory( tests )
	endif()

	file( GLOB_RECURSE SRC md4qt/* )

	add_library( md4qt INTERFACE ${SRC} )
	add_library( md4qt::md4qt ALIAS md4qt )

	target_include_directories( md4qt INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    )

	install( DIRECTORY md4qt
		DESTINATION include
	)

	install( TARGETS md4qt
		EXPORT md4qt-targets
		RUNTIME DESTINATION bin
		ARCHIVE DESTINATION lib
		LIBRARY DESTINATION lib
		INCLUDES DESTINATION include
	)

	install( EXPORT md4qt-targets
		DESTINATION lib/cmake/md4qt
		NAMESPACE md4qt::
	)

	include( CMakePackageConfigHelpers )

	write_basic_package_version_file(
		"${CMAKE_CURRENT_BINARY_DIR}/md4qt-config-version.cmake"
		VERSION ${MD4QT_VERSION}
		COMPATIBILITY AnyNewerVersion )

	configure_package_config_file( md4qt-config.cmake.in
		"${CMAKE_CURRENT_BINARY_DIR}/md4qt-config.cmake"
		INSTALL_DESTINATION lib/cmake/md4qt )

	install( FILES ${PROJECT_BINARY_DIR}/md4qt-config.cmake
			${PROJECT_BINARY_DIR}/md4qt-config-version.cmake
		DESTINATION lib/cmake/md4qt )

else()

	set( md4qt_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE )

endif( ${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME} )
