
/*
	SPDX-FileCopyrightText: 2022-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
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

/*
| Table |
| --- |
| Data |
    code

*/

TEST_CASE( "193" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/193.md" );

	REQUIRE( doc->items().size() == 3 );
	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Table );
	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Code );
}

/*
Text
| Table |
| --- |
| Data |
    code

*/

TEST_CASE( "194" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/194.md" );

	REQUIRE( doc->items().size() == 4 );
	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Table );
	REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Code );
}

/*
Text
| Table |
| --- |
| Data |
    code
Text

*/
TEST_CASE( "195" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/195.md" );

	REQUIRE( doc->items().size() == 5 );
	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Table );
	REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Code );
	REQUIRE( doc->items().at( 4 )->type() == MD::ItemType::Paragraph );
}

/*
- list
    - list

    --------------------------------

    text

*/
TEST_CASE( "196" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/196.md" );

	REQUIRE( doc->items().size() == 2 );
	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT > * > ( doc->items().at( 1 ).get() );
	REQUIRE( l->items().size() == 1 );
	REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
	auto i = static_cast< MD::ListItem< TRAIT > * > ( l->items().at( 0 ).get() );
	REQUIRE( i->items().size() == 4 );
	REQUIRE( i->items().at( 0 )->type() == MD::ItemType::Paragraph );
	REQUIRE( i->items().at( 1 )->type() == MD::ItemType::List );
	REQUIRE( i->items().at( 2 )->type() == MD::ItemType::HorizontalLine );
	REQUIRE( i->items().at( 3 )->type() == MD::ItemType::Paragraph );
}

/*
- list

    --------------------------------

    text

*/
TEST_CASE( "197" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/197.md" );

	REQUIRE( doc->items().size() == 2 );
	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT > * > ( doc->items().at( 1 ).get() );
	REQUIRE( l->items().size() == 1 );
	REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
	auto i = static_cast< MD::ListItem< TRAIT > * > ( l->items().at( 0 ).get() );
	REQUIRE( i->items().size() == 3 );
	REQUIRE( i->items().at( 0 )->type() == MD::ItemType::Paragraph );
	REQUIRE( i->items().at( 1 )->type() == MD::ItemType::HorizontalLine );
	REQUIRE( i->items().at( 2 )->type() == MD::ItemType::Paragraph );
}

/*
<std::vector<T>> <std::vector<T>>

*/
TEST_CASE( "198" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/198.md" );

	REQUIRE( doc->items().size() == 2 );
	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT > * > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 5 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT > * > ( p->items().at( 0 ).get() );
		REQUIRE( t->text() == u8"<std::vector" );
	}

	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::RawHtml );

	{
		REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT > * > ( p->items().at( 2 ).get() );
		REQUIRE( t->text() == u8"> <std::vector" );
	}

	REQUIRE( p->items().at( 3 )->type() == MD::ItemType::RawHtml );

	{
		REQUIRE( p->items().at( 4 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT > * > ( p->items().at( 4 ).get() );
		REQUIRE( t->text() == u8">" );
	}
}

/*
  1) Text
    ```
    text
    ```

*/
TEST_CASE( "199" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/199.md" );

	REQUIRE( doc->items().size() == 2 );
	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT > * > ( doc->items().at( 1 ).get() );
	REQUIRE( l->items().size() == 1 );
	REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
	auto i = static_cast< MD::ListItem< TRAIT > * > ( l->items().at( 0 ).get() );
	REQUIRE( i->items().size() == 1 );
	REQUIRE( i->items().at( 0 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT > * > ( i->items().at( 0 ).get() );
	REQUIRE( p->items().size() == 2 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code< TRAIT > * > ( p->items().at( 1 ).get() );
	REQUIRE( c->isInlined() );
	REQUIRE( c->text() == u8"    text    " );
}

/*
~text~

*/
TEST_CASE( "200" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/200.md" );

	REQUIRE( doc->items().size() == 2 );
	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT > * > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT > * > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::StrikethroughText );
	REQUIRE( t->text() == u8"text" );
}

/*
~~text~text~~

*/
TEST_CASE( "201" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/201.md" );

	REQUIRE( doc->items().size() == 2 );
	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT > * > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT > * > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::StrikethroughText );
	REQUIRE( t->text() == u8"text~text" );
}

/*
~~~text~~

*/
TEST_CASE( "202" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/202.md" );

	REQUIRE( doc->items().size() == 2 );
	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code< TRAIT > * > ( doc->items().at( 1 ).get() );
	REQUIRE( c->text().isEmpty() );
}

/*
~~text~

*/
TEST_CASE( "203" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/203.md" );

	REQUIRE( doc->items().size() == 2 );
	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT > * > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT > * > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"~~text~" );
}

/*
<!-- -->
[link]: https://www.google.com

Text [link].

*/
TEST_CASE( "204" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/204.md" );

	REQUIRE( doc->items().size() == 3 );
	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );
	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT > * > ( doc->items().at( 2 ).get() );
	REQUIRE( p->items().size() == 3 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Link );
	REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );
}

/*
<!-- -->
<!-- -->
[link]: https://www.google.com

Text [link].

*/
TEST_CASE( "205" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/205.md" );

	REQUIRE( doc->items().size() == 4 );
	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );
	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::RawHtml );
	REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT > * > ( doc->items().at( 3 ).get() );
	REQUIRE( p->items().size() == 3 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Link );
	REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );
}

/*
<!-- --><!-- -->
<!-- -->
[link]: https://www.google.com

Text [link].

*/
TEST_CASE( "206" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/206.md" );

	REQUIRE( doc->items().size() == 4 );
	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );
	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::RawHtml );
	REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT > * > ( doc->items().at( 3 ).get() );
	REQUIRE( p->items().size() == 3 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Link );
	REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );
}

/*
<!-- -->
[link1]: https://www.google.com
<!-- -->
[link2]: https://www.google.com

Text [link1] and [link2].

*/
TEST_CASE( "207" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/207.md" );

	REQUIRE( doc->items().size() == 4 );
	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );
	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::RawHtml );
	REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT > * > ( doc->items().at( 3 ).get() );
	REQUIRE( p->items().size() == 5 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Link );
	REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );
	REQUIRE( p->items().at( 3 )->type() == MD::ItemType::Link );
	REQUIRE( p->items().at( 4 )->type() == MD::ItemType::Text );
}

/*
1. Text
   - Text
    text

*/
TEST_CASE( "208" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/208.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
		auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( l->items().size() == 1 );

		REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
		auto i = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );

		REQUIRE( i->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( i->items().at( 0 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT > * > ( p->items().at( 0 ).get() );
		REQUIRE( t->text() == u8"Text" );

		{
			REQUIRE( i->items().at( 1 )->type() == MD::ItemType::List );
			auto l = static_cast< MD::List< TRAIT >* > ( i->items().at( 1 ).get() );

			REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
			auto i = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
			REQUIRE( i->items().size() == 1 );

			REQUIRE( i->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph< TRAIT >* > ( i->items().at( 0 ).get() );
			REQUIRE( p->items().size() == 2 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t1 = static_cast< MD::Text< TRAIT > * > ( p->items().at( 0 ).get() );
			REQUIRE( t1->text() == u8"Text" );
			REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
			auto t2 = static_cast< MD::Text< TRAIT > * > ( p->items().at( 1 ).get() );
			REQUIRE( t2->text() == u8"text" );
		}
	}
}

/*
Text
0. text

*/
TEST_CASE( "209" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/209.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 2 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t1 = static_cast< MD::Text< TRAIT > * > ( p->items().at( 0 ).get() );
	REQUIRE( t1->text() == u8"Text" );
	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
	auto t2 = static_cast< MD::Text< TRAIT > * > ( p->items().at( 1 ).get() );
	REQUIRE( t2->text() == u8"0. text" );
}

/*
- [#6899]: Text

[#6899]: https://www.google.com

*/
TEST_CASE( "210" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/210.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->items().size() == 1 );

	REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
	auto i = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );

	REQUIRE( i->items().at( 0 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( i->items().at( 0 ).get() );
	REQUIRE( p->items().size() == 2 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
}

/*
<!--
--comment
-->

*/
TEST_CASE( "211" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/211.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );
}

/*
foo <!--
--
-->

*/
TEST_CASE( "212" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/212.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Heading );
	auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( h->text()->items().size() == 1 );
	REQUIRE( h->text()->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( h->text()->items().at( 0 ).get() );
	REQUIRE( t->text() == u8"foo <!--" );

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->text() == u8"-->" );
	}
}

/*
- 	text
1.	text
    - text $text
    - text $text

*/
TEST_CASE( "213" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/213.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
		auto l = static_cast< MD::List< TRAIT > * > ( doc->items().at( 1 ).get() );
		REQUIRE( l->items().size() == 1 );

		{
			REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
			auto li = static_cast< MD::ListItem< TRAIT > * > ( l->items().at( 0 ).get() );
			REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );
			REQUIRE( li->items().size() == 1 );
			REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		}
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::List );
		auto l = static_cast< MD::List< TRAIT > * > ( doc->items().at( 2 ).get() );
		REQUIRE( l->items().size() == 1 );

		{
			REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
			auto li = static_cast< MD::ListItem< TRAIT > * > ( l->items().at( 0 ).get() );
			REQUIRE( li->listType() == MD::ListItem< TRAIT >::Ordered );
			REQUIRE( li->items().size() == 2 );
			REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
			REQUIRE( li->items().at( 1 )->type() == MD::ItemType::List );
			auto ll = static_cast< MD::List< TRAIT > * > ( li->items().at( 1 ).get() );
			REQUIRE( ll->items().size() == 2 );
		}
	}
}

/*
text[^1] text[^2]

[^1]: footnote1
[^2]: footnote2

*/
TEST_CASE( "214" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/214.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT > * > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 4 );

	REQUIRE( doc->footnotesMap().size() == 2 );
}

/*
Text[^1]
[^1]: Footnote1

*/
TEST_CASE( "215" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/215.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT > * > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 2 );

	REQUIRE( doc->footnotesMap().size() == 1 );
}

/*
Text[^1]
[^1]: Footnote1
Text[^1]

*/
TEST_CASE( "216" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/216.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT > * > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 2 );

	REQUIRE( doc->footnotesMap().size() == 1 );
	auto f = static_cast< MD::Footnote< TRAIT > * > ( doc->footnotesMap().cbegin()->second.get() );
	REQUIRE( f->items().size() == 1 );
	auto pp = static_cast< MD::Paragraph< TRAIT > * > ( f->items().at( 0 ).get() );
	REQUIRE( pp->items().size() == 3 );
	REQUIRE( pp->items().at( 2 )->type() == MD::ItemType::FootnoteRef );
}

/*
Text[^1] text[^2].

[^1]: E.g., James H. Breasted, "Editor's Forward," in Ancient
Records of Assyria and Babylonia, vol. 1, Historical Records of
Assyria from the Earliest Times to Sargon, by Daniel D.
Luckenbill (Chicago: The University of Chicago Press, 1926),
viii.

[^2]: James H. Breasted, The Oriental Institute of the University
of Chicago: A Beginning and A Program. Oriental Institute
Communications 1 (Chicago: University of Chicago Press, 1922),
22-23.

*/
TEST_CASE( "217" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/217.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT > * > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 5 );

	REQUIRE( doc->footnotesMap().size() == 2 );
}
