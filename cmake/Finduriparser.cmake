# SPDX-FileCopyrightText: 2022-2024 Igor Mironchik <igor.mironchik@gmail.com>
# SPDX-License-Identifier: MIT

if( NOT URIPARSER_INCLUDE_DIR )
	find_path( URIPARSER_INCLUDE_DIR NAMES Uri.h PATH_SUFFIXES uriparser )
	set( URIPARSER_INCLUDE_DIR ${URIPARSER_INCLUDE_DIR}/uriparser CACHE PATH "uriparser includes" )
endif()

find_library( URIPARSER_LIBRARY NAMES uriparser )

if( URIPARSER_INCLUDE_DIR AND URIPARSER_LIBRARY )
	set( URIPARSER_FOUND TRUE )
	set( UriParser_FOUND TRUE )
	
	if( NOT TARGET uriparser::uriparser )
		add_library( uriparser::uriparser UNKNOWN IMPORTED )

		target_include_directories( uriparser::uriparser INTERFACE ${URIPARSER_INCLUDE_DIR} )
		set_target_properties( uriparser::uriparser PROPERTIES
			IMPORTED_LOCATION ${URIPARSER_LIBRARY} )
	endif()
endif ()
