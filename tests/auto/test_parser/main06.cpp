
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2022 Igor Mironchik

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

inline typename TRAIT::String
to_string( int i )
{
#ifdef MD4QT_QT_SUPPORT
	return QString::number( i );
#else
	return std::to_string( i );
#endif
}

/*
  - list

    ```javascript

    // very bad
    function
    *
    foo() {
      // ...
    }

    // very bad
    const wat = function
    *
    () {
      // ...
    };
    ```


*/
TEST_CASE( "153" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/153.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->items().size() == 1 );
	REQUIRE( l->startColumn() == 2 );
	REQUIRE( l->startLine() == 0 );
	REQUIRE( l->endColumn() == 6 );
	REQUIRE( l->endLine() == 17 );

	REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
	auto i = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
	REQUIRE( i->items().size() == 2 );
	REQUIRE( i->startColumn() == 2 );
	REQUIRE( i->startLine() == 0 );
	REQUIRE( i->endColumn() == 6 );
	REQUIRE( i->endLine() == 17 );

	REQUIRE( i->items().at( 0 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( i->items().at( 0 ).get() );
	REQUIRE( p->startColumn() == 4 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 7 );
	REQUIRE( p->endLine() == 0 );

	REQUIRE( i->items().at( 1 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code< TRAIT >* > ( i->items().at( 1 ).get() );
	REQUIRE( c->startColumn() == 0 );
	REQUIRE( c->startLine() == 3 );
	REQUIRE( c->endColumn() == 5 );
	REQUIRE( c->endLine() == 16 );
	REQUIRE( c->text() == u8"\n// very bad\nfunction\n*\nfoo() {\n  // ...\n}\n\n"
		"// very bad\nconst wat = function\n*\n() {\n  // ...\n};" );
}
