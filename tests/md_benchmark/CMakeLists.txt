
cmake_minimum_required( VERSION 3.1 )

SET( CMAKE_CXX_STANDARD 17 )

SET( CMAKE_CXX_STANDARD_REQUIRED ON )

project( md_benchmark )

add_subdirectory( 3rdparty/md4c )

set( CMARK_TESTS OFF CACHE INTERNAL "" FORCE )
set( CMARK_SHARED OFF CACHE INTERNAL "" FORCE )
add_subdirectory( 3rdparty/cmark-gfm )

include_directories( 3rdparty/md4c/src
	3rdparty/cmark-gfm/src
	${CMAKE_CURRENT_BINARY_DIR}/3rdparty/cmark-gfm/src
	${CMAKE_CURRENT_SOURCE_DIR}/../.. )

find_package( ICU REQUIRED )
find_package( uriparser REQUIRED )
find_package( Qt6Core REQUIRED )

include_directories( ${ICU_INCLUDE_DIRS}
	${uriparser_INCLUDE_DIRS} )

add_executable( md_benchmark main.cpp )

file( COPY ${CMAKE_CURRENT_SOURCE_DIR}/../manual/complex.md
	DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/../../bin/tests/manual )

target_link_libraries( md_benchmark md4c libcmark-gfm_static Qt::Core icu::icu uriparser::uriparser )
