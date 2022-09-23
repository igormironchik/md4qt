
cmake_minimum_required( VERSION 3.1 )

project( md4qt )

set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )

link_directories( ${CMAKE_BINARY_DIR}/lib )

option( BUILD_MD4QT_BENCHMARK "Build benchmark? Default OFF." OFF )
option( BUILD_MD4QT_TESTS "Build tests? Default ON." ON )

if( NOT CMAKE_BUILD_TYPE )
	set( CMAKE_BUILD_TYPE "Release"
		CACHE STRING "Choose the type of build."
		FORCE)
endif( NOT CMAKE_BUILD_TYPE )

set( CMAKE_CXX_STANDARD 14 )

set( CMAKE_CXX_STANDARD_REQUIRED ON )

if( ${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME} )

	add_subdirectory( src )

	if( BUILD_MD4QT_TESTS )
		enable_testing()

		add_subdirectory( tests )
	endif()

endif( ${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME} )
