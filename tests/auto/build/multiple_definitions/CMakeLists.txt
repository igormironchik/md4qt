
project( test.multiple_definitions )

set( SRC 1.hpp 1.cpp 2.hpp 2.cpp main.cpp )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../../../.. )

if( BUILD_MD4QT_QT_TESTS )
	find_package( Qt6Core REQUIRED )

	add_executable( test.multiple_definitions.qt ${SRC} )
	target_compile_definitions( test.multiple_definitions.qt PUBLIC TRAIT=MD::QStringTrait )
	target_compile_definitions( test.multiple_definitions.qt PUBLIC MD4QT_QT_SUPPORT )

	target_link_libraries( test.multiple_definitions.qt Qt6::Core )
endif()

if( BUILD_MD4QT_STL_TESTS )
	find_package( ICU REQUIRED )
	find_package( uriparser REQUIRED )

	add_executable( test.multiple_definitions.stl ${SRC} )
	target_compile_definitions( test.multiple_definitions.stl PUBLIC TRAIT=MD::UnicodeStringTrait )
	target_compile_definitions( test.multiple_definitions.stl PUBLIC MD4QT_ICU_STL_SUPPORT )

	target_link_libraries( test.multiple_definitions.stl icu::icu uriparser::uriparser )
endif()
