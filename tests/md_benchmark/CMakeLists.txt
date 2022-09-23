
cmake_minimum_required( VERSION 3.1 )

SET( CMAKE_CXX_STANDARD 14 )

SET( CMAKE_CXX_STANDARD_REQUIRED ON )

project( md_benchmark )

add_subdirectory( 3rdparty/md4c )

include_directories( 3rdparty/md4c/src
	${CMAKE_CURRENT_SOURCE_DIR}/../../src )

find_package( Qt6Core REQUIRED )

add_executable( md_benchmark main.cpp )

file( COPY ${CMAKE_CURRENT_SOURCE_DIR}/../manual/complex.md
	DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/../../bin/tests/manual )

target_link_libraries( md_benchmark md4qt md4c Qt::Core )
