
/*
	SPDX-FileCopyrightText: 2022-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

// doctest include.
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

typename TRAIT::String
fullPath( int num )
{
	typename TRAIT::String wd =
#ifdef MD4QT_QT_SUPPORT
		QDir().absolutePath()
#else
		std::filesystem::canonical( std::filesystem::current_path() ).u8string()
#endif
		+ u8"/tests/html/data/";

	auto numStr = std::to_string( num );
	numStr = std::string( 3 - numStr.size(), '0' ) + numStr;

	wd += TRAIT::latin1ToString( numStr.c_str() ) + u8".md";

#ifndef MD4QT_QT_SUPPORT
	std::string tmp;
	wd.toUTF8String( tmp );
	std::replace( tmp.begin(), tmp.end(), '\\', '/' );
	wd = icu::UnicodeString::fromUTF8( tmp );
#endif

	return wd;
}


/*
**[google](www.google.com)* text*

*/
TEST_CASE( "001" )
{
	MD::Parser< TRAIT > p;
	auto html = MD::toHtml( p.parse( "tests/html/data/001.md" ), false, {}, false );
	REQUIRE( html == u8"<p><em><em><a href=\"www.google.com\"> google </a>"
		"</em> text </em></p>" );
}

/*
**text* text*

*/
TEST_CASE( "002" )
{
	MD::Parser< TRAIT > p;
	auto html = MD::toHtml( p.parse( "tests/html/data/002.md" ), false, {}, false );
	REQUIRE( html == u8"<p><em><em> text text </em></em></p>" );
}

/*
# heading

*/
TEST_CASE( "003" )
{
	MD::Parser< TRAIT > p;
	auto html = MD::toHtml( p.parse( "tests/html/data/003.md" ), false, {}, false );
	const auto required = u8"\n<h1 id=\"heading/" + fullPath( 3 ) + u8"\"> heading </h1>\n";
	REQUIRE( html == required );
}
