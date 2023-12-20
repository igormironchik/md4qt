
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2022-2023 Igor Mironchik

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
- Text

  - Text
    - Text

    ```bash
    code
    ```

    - Text


*/
TEST_CASE( "184" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/184.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
		auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( l->items().size() == 1 );
		REQUIRE( l->startColumn() == 0 );
		REQUIRE( l->startLine() == 0 );
		REQUIRE( l->endColumn() == 9 );
		REQUIRE( l->endLine() == 9 );

		REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
		auto i = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
		REQUIRE( i->items().size() == 2 );
		REQUIRE( i->startColumn() == 0 );
		REQUIRE( i->startLine() == 0 );
		REQUIRE( i->endColumn() == 9 );
		REQUIRE( i->endLine() == 9 );

		REQUIRE( i->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( i->items().at( 0 ).get() );
		REQUIRE( p->startColumn() == 2 );
		REQUIRE( p->startLine() == 0 );
		REQUIRE( p->endColumn() == 5 );
		REQUIRE( p->endLine() == 0 );

		{
			REQUIRE( i->items().at( 1 )->type() == MD::ItemType::List );
			auto l = static_cast< MD::List< TRAIT >* > ( i->items().at( 1 ).get() );
			REQUIRE( l->startColumn() == 2 );
			REQUIRE( l->startLine() == 2 );
			REQUIRE( l->endColumn() == 9 );
			REQUIRE( l->endLine() == 9 );
			REQUIRE( l->items().size() == 1 );

			REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
			auto i = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
			REQUIRE( i->items().size() == 4 );
			REQUIRE( i->startColumn() == 2 );
			REQUIRE( i->startLine() == 2 );
			REQUIRE( i->endColumn() == 9 );
			REQUIRE( i->endLine() == 9 );

			REQUIRE( i->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph< TRAIT >* > ( i->items().at( 0 ).get() );
			REQUIRE( p->startColumn() == 4 );
			REQUIRE( p->startLine() == 2 );
			REQUIRE( p->endColumn() == 7 );
			REQUIRE( p->endLine() == 2 );

			{
				REQUIRE( i->items().at( 1 )->type() == MD::ItemType::List );
				auto l = static_cast< MD::List< TRAIT >* > ( i->items().at( 1 ).get() );
				REQUIRE( l->startColumn() == 4 );
				REQUIRE( l->startLine() == 3 );
				REQUIRE( l->endColumn() == 9 );
				REQUIRE( l->endLine() == 3 );
				REQUIRE( l->items().size() == 1 );

				REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
				auto i = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
				REQUIRE( i->items().size() == 1 );
				REQUIRE( i->startColumn() == 4 );
				REQUIRE( i->startLine() == 3 );
				REQUIRE( i->endColumn() == 9 );
				REQUIRE( i->endLine() == 3 );

				REQUIRE( i->items().at( 0 )->type() == MD::ItemType::Paragraph );
			}

			{
				REQUIRE( i->items().at( 2 )->type() == MD::ItemType::Code );
				auto c = static_cast< MD::Code< TRAIT >* > ( i->items().at( 2 ).get() );
				REQUIRE( c->startColumn() == 4 );
				REQUIRE( c->startLine() == 6 );
				REQUIRE( c->endColumn() == 7 );
				REQUIRE( c->endLine() == 6 );
				REQUIRE( c->text() == u8"code" );
			}

			{
				REQUIRE( i->items().at( 3 )->type() == MD::ItemType::List );
				auto l = static_cast< MD::List< TRAIT >* > ( i->items().at( 3 ).get() );
				REQUIRE( l->startColumn() == 4 );
				REQUIRE( l->startLine() == 9 );
				REQUIRE( l->endColumn() == 9 );
				REQUIRE( l->endLine() == 9 );
				REQUIRE( l->items().size() == 1 );

				REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
				auto i = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
				REQUIRE( i->items().size() == 1 );
				REQUIRE( i->startColumn() == 4 );
				REQUIRE( i->startLine() == 9 );
				REQUIRE( i->endColumn() == 9 );
				REQUIRE( i->endLine() == 9 );

				REQUIRE( i->items().at( 0 )->type() == MD::ItemType::Paragraph );
			}
		}
	}
}
