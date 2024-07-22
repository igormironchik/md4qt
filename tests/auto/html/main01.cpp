
/*
	SPDX-FileCopyrightText: 2022-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

// doctest include.
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>


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
