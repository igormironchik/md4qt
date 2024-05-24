
/*
	SPDX-FileCopyrightText: 2022-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

// doctest include.
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

/*
*<a>text</a>*

*/
TEST_CASE( "249" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/249.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );
	
	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 3 );
	
	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::RawHtml );
		auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( h->opts() == MD::ItalicText );
		REQUIRE( h->text() == u8"<a>" );
		REQUIRE( h->openStyles().size() == 1 );
		REQUIRE( h->openStyles().at( 0 ) == MD::StyleDelim{ MD::ItalicText, 0, 0, 0, 0 } );
		REQUIRE( h->closeStyles().empty() );
	}
	
	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( t->opts() == MD::ItalicText );
		REQUIRE( t->text() == u8"text" );
		REQUIRE( t->openStyles().empty() );
		REQUIRE( t->closeStyles().empty() );
	}
	
	{
		REQUIRE( p->items().at( 2 )->type() == MD::ItemType::RawHtml );
		auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 2 ).get() );
		REQUIRE( h->opts() == MD::ItalicText );
		REQUIRE( h->text() == u8"</a>" );
		REQUIRE( h->openStyles().empty() );
		REQUIRE( h->closeStyles().size() == 1 );
		REQUIRE( h->closeStyles().at( 0 ) == MD::StyleDelim{ MD::ItalicText, 12, 0, 12, 0 } );
	}
}
