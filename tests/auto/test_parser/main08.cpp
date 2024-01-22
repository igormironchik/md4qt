
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2022-2024 Igor Mironchik

	Permission is hereby granted, free of charge, to any person
	obtaining a copy of this software and associated documentation
	files (the "Software"), to deal in the Software without
	restriction, including without limitation the rights to use,
	copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the
	Software is furnished to do so, subject to the following
	conditions:

	The above copyright notice and this permission notice shall be
	included in all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
	WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
	OTHER DEALINGS IN THE SOFTWARE.
*/

// doctest include.
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

/*
This sentence uses $\` and \`$ delimiters to show math inline:  $`\sqrt{3x-1}+(1+x)^2`$

*/
TEST_CASE( "218" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/218.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT > * > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 4 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Math );

	{
		auto m = static_cast< MD::Math< TRAIT > * > ( p->items().at( 1 ).get() );
		REQUIRE( m->expr() == u8"\\` and \\`" );
	}

	REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );
	REQUIRE( p->items().at( 3 )->type() == MD::ItemType::Math );

	{
		auto m = static_cast< MD::Math< TRAIT > * > ( p->items().at( 3 ).get() );
		REQUIRE( m->expr() == u8"\\sqrt{3x-1}+(1+x)^2" );
	}
}

/*
[^]

[^1]

[^1
]

*/
TEST_CASE( "219" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/219.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 4 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::FootnoteRef );
		auto fr = static_cast< MD::FootnoteRef< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( fr->text() == u8"[^1]" );
		REQUIRE( fr->isSpaceBefore() );
		REQUIRE( fr->isSpaceAfter() );
	}

	{
		REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 3 ).get() );
		REQUIRE( p->items().size() == 2 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
	}
}
