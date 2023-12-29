
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

/*
	- code

*/
TEST_CASE( "185" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/185.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( c->startColumn() == 4 );
	REQUIRE( c->startLine() == 0 );
	REQUIRE( c->endColumn() == 9 );
	REQUIRE( c->endLine() == 0 );
	REQUIRE( c->text() == u8"- code" );
}

/*
```
- code
```

*/
TEST_CASE( "186" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/186.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( c->startColumn() == 0 );
	REQUIRE( c->startLine() == 1 );
	REQUIRE( c->endColumn() == 5 );
	REQUIRE( c->endLine() == 1 );
	REQUIRE( c->text() == u8"- code" );
}

/*
- List

  - List
    - List

    ```
    code
    ```

    - List

    Text

    ```
    code
    ```

    - List

*/
TEST_CASE( "187" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/187.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
		auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( l->items().size() == 1 );
		REQUIRE( l->startColumn() == 0 );
		REQUIRE( l->startLine() == 0 );
		REQUIRE( l->endColumn() == 9 );
		REQUIRE( l->endLine() == 17 );

		REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
		auto i = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
		REQUIRE( i->items().size() == 2 );
		REQUIRE( i->startColumn() == 0 );
		REQUIRE( i->startLine() == 0 );
		REQUIRE( i->endColumn() == 9 );
		REQUIRE( i->endLine() == 17 );

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
			REQUIRE( l->endLine() == 17 );
			REQUIRE( l->items().size() == 1 );

			REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
			auto i = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
			REQUIRE( i->items().size() == 7 );
			REQUIRE( i->startColumn() == 2 );
			REQUIRE( i->startLine() == 2 );
			REQUIRE( i->endColumn() == 9 );
			REQUIRE( i->endLine() == 17 );

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

			{
				REQUIRE( i->items().at( 4 )->type() == MD::ItemType::Paragraph );
				auto p = static_cast< MD::Paragraph< TRAIT >* > ( i->items().at( 4 ).get() );
				REQUIRE( p->startColumn() == 4 );
				REQUIRE( p->startLine() == 11 );
				REQUIRE( p->endColumn() == 7 );
				REQUIRE( p->endLine() == 11 );
			}

			{
				REQUIRE( i->items().at( 5 )->type() == MD::ItemType::Code );
				auto c = static_cast< MD::Code< TRAIT >* > ( i->items().at( 5 ).get() );
				REQUIRE( c->startColumn() == 4 );
				REQUIRE( c->startLine() == 14 );
				REQUIRE( c->endColumn() == 7 );
				REQUIRE( c->endLine() == 14 );
				REQUIRE( c->text() == u8"code" );
			}

			{
				REQUIRE( i->items().at( 6 )->type() == MD::ItemType::List );
				auto l = static_cast< MD::List< TRAIT >* > ( i->items().at( 6 ).get() );
				REQUIRE( l->startColumn() == 4 );
				REQUIRE( l->startLine() == 17 );
				REQUIRE( l->endColumn() == 9 );
				REQUIRE( l->endLine() == 17 );
				REQUIRE( l->items().size() == 1 );

				REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
				auto i = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
				REQUIRE( i->items().size() == 1 );
				REQUIRE( i->startColumn() == 4 );
				REQUIRE( i->startLine() == 17 );
				REQUIRE( i->endColumn() == 9 );
				REQUIRE( i->endLine() == 17 );

				REQUIRE( i->items().at( 0 )->type() == MD::ItemType::Paragraph );
			}
		}
	}
}

/*
- list

    not code

<!-- -->

- list
  - list
    - list

    not code

*/
TEST_CASE( "188" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/188.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 4 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
		auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( l->items().size() == 1 );
		REQUIRE( l->startColumn() == 0 );
		REQUIRE( l->startLine() == 0 );
		REQUIRE( l->endColumn() == 11 );
		REQUIRE( l->endLine() == 2 );

		REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
		auto i = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
		REQUIRE( i->items().size() == 2 );
		REQUIRE( i->startColumn() == 0 );
		REQUIRE( i->startLine() == 0 );
		REQUIRE( i->endColumn() == 11 );
		REQUIRE( i->endLine() == 2 );

		REQUIRE( i->items().at( 0 )->type() == MD::ItemType::Paragraph );
		REQUIRE( i->items().at( 1 )->type() == MD::ItemType::Paragraph );
	}

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::RawHtml );

	{
		REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::List );
		auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 3 ).get() );
		REQUIRE( l->items().size() == 1 );
		REQUIRE( l->startColumn() == 0 );
		REQUIRE( l->startLine() == 6 );
		REQUIRE( l->endColumn() == 11 );
		REQUIRE( l->endLine() == 10 );

		REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
		auto i = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
		REQUIRE( i->items().size() == 2 );
		REQUIRE( i->startColumn() == 0 );
		REQUIRE( i->startLine() == 6 );
		REQUIRE( i->endColumn() == 11 );
		REQUIRE( i->endLine() == 10 );

		REQUIRE( i->items().at( 0 )->type() == MD::ItemType::Paragraph );
		REQUIRE( i->items().at( 1 )->type() == MD::ItemType::List );

		{
			auto l = static_cast< MD::List< TRAIT >* > ( i->items().at( 1 ).get() );
			REQUIRE( l->items().size() == 1 );
			REQUIRE( l->startColumn() == 2 );
			REQUIRE( l->startLine() == 7 );
			REQUIRE( l->endColumn() == 11 );
			REQUIRE( l->endLine() == 10 );

			REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
			auto i = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
			REQUIRE( i->items().size() == 3 );
			REQUIRE( i->items().at( 0 )->type() == MD::ItemType::Paragraph );
			REQUIRE( i->items().at( 1 )->type() == MD::ItemType::List );
			REQUIRE( i->items().at( 2 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph< TRAIT >* > ( i->items().at( 2 ).get() );
			REQUIRE( p->startColumn() == 4 );
			REQUIRE( p->startLine() == 10 );
			REQUIRE( p->endColumn() == 11 );
			REQUIRE( p->endLine() == 10 );
		}
	}
}

/*
1. a

  2. b

    code
*/
TEST_CASE( "189" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/189.md" );

	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->items().size() == 2 );

	for( int i = 0; i < 2; ++i )
	{
		REQUIRE( l->items().at( i )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( i ).get() );
		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->listType() == MD::ListItem< TRAIT >::Ordered );
		REQUIRE( li->startNumber() == i + 1 );
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		const char16_t ch = 97 + i;
		char16_t str[ 2 ] = { ch, 0 };
		REQUIRE( t->text() == TRAIT::utf16ToString( &str[ 0 ] ) );
	}

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 2 ).get() );
	REQUIRE( !c->isInlined() );
	REQUIRE( c->text() == u8"code" );
}

/*
 ```c++
     code
 ```

*/
TEST_CASE( "190" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/190.md" );

	REQUIRE( doc->items().size() == 2 );
	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( c->syntax() == u8"c++" );
	REQUIRE( c->text() == u8"    code" );
}

/*
Text[^1]:

[^1]: Footnote.

```
code
```

Text.

*/
TEST_CASE( "191" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/191.md" );

	REQUIRE( doc->items().size() == 4 );
	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 2 ).get() );
	REQUIRE( c->text() == u8"code" );
	REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Paragraph );

	REQUIRE( doc->footnotesMap().size() == 1 );
}

/*
Text[^1]:

[^1]: Footnote.

- List.

Text.


*/
TEST_CASE( "192" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/192.md" );

	REQUIRE( doc->items().size() == 4 );
	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::List );
	REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Paragraph );

	REQUIRE( doc->footnotesMap().size() == 1 );
}
