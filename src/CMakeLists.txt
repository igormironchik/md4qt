
project( md4qt )

set( MD4QT_VERSION "1.0.0" )

find_package( Qt6Core REQUIRED )

if( ENABLE_COVERAGE )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage" )
	set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage" )
endif( ENABLE_COVERAGE )

set( SRC md_doc.hpp
    md_doc.cpp
    md_parser.hpp
    md_parser.cpp
	md_entities_map.hpp )

add_library( md4qt STATIC ${SRC} )

set( INCLUDES md_doc.hpp
	md_parser.hpp
	md_entities_map.hpp )

target_include_directories( md4qt INTERFACE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

install( FILES ${INCLUDES} DESTINATION include/md4qt )

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

configure_package_config_file( ${CMAKE_CURRENT_SOURCE_DIR}/../md4qt-config.cmake.in
	"${CMAKE_CURRENT_BINARY_DIR}/md4qt-config.cmake"
	INSTALL_DESTINATION lib/cmake/md4qt )

install( FILES ${PROJECT_BINARY_DIR}/md4qt-config.cmake
		${PROJECT_BINARY_DIR}/md4qt-config-version.cmake
	DESTINATION lib/cmake/md4qt )

target_link_libraries( md4qt Qt6::Core )
