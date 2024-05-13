
/*
	SPDX-FileCopyrightText: 2022-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

// doctest include.
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

// md4qt include.
#include <md4qt/algo.hpp>
#include <md4qt/parser.hpp>

int g_count = 0;

void onItem( MD::Item< TRAIT > * i )
{
	++g_count;
}

TEST_CASE( "001" )
{
	MD::Parser< TRAIT > p;
	auto doc = p.parse( "tests/parser/data/001.md" );
	
	MD::forEach< TRAIT >( {}, doc, onItem );
	REQUIRE( g_count == 0 );
	g_count = 0;
	
	MD::forEach< TRAIT >( { MD::ItemType::Anchor }, doc, onItem );
	REQUIRE( g_count == 1 );
	g_count = 0;
	
	MD::forEach< TRAIT >( { MD::ItemType::Paragraph }, doc, onItem );
	REQUIRE( g_count == 0 );
	g_count = 0;
}

/*
This is just a text!

*/
TEST_CASE( "002" )
{
	MD::Parser< TRAIT > p;
	auto doc = p.parse( "tests/parser/data/002.md" );

	MD::forEach< TRAIT >( { MD::ItemType::Paragraph }, doc, onItem );
	REQUIRE( g_count == 1 );
	g_count = 0;
	
	MD::forEach< TRAIT >( { MD::ItemType::Text }, doc, onItem );
	REQUIRE( g_count == 1 );
	g_count = 0;
}

/*

Paragraph 1.

Paragraph 2.

*/
TEST_CASE( "003" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/003.md" );

	MD::forEach< TRAIT >( { MD::ItemType::Paragraph }, doc, onItem );
	REQUIRE( g_count == 2 );
	g_count = 0;
	
	MD::forEach< TRAIT >( { MD::ItemType::Text }, doc, onItem );
	REQUIRE( g_count == 2 );
	g_count = 0;
}

/*
Line 1...<space><space>
Line 2...
Line 3...

*/
TEST_CASE( "005" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/005.md" );
	
	MD::forEach< TRAIT >( { MD::ItemType::LineBreak }, doc, onItem );
	REQUIRE( g_count == 1 );
	g_count = 0;
}

/*
`code`

*/
TEST_CASE( "011" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/011.md" );

	MD::forEach< TRAIT >( { MD::ItemType::Paragraph }, doc, onItem );
	REQUIRE( g_count == 1 );
	g_count = 0;
	
	MD::forEach< TRAIT >( { MD::ItemType::Code }, doc, onItem );
	REQUIRE( g_count == 1 );
	g_count = 0;
}

/*
Code in the `text`.

*/
TEST_CASE( "012" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/012.md" );

	MD::forEach< TRAIT >( { MD::ItemType::Paragraph }, doc, onItem );
	REQUIRE( g_count == 1 );
	g_count = 0;
	
	MD::forEach< TRAIT >( { MD::ItemType::Text }, doc, onItem );
	REQUIRE( g_count == 2 );
	g_count = 0;
	
	MD::forEach< TRAIT >( { MD::ItemType::Code }, doc, onItem );
	REQUIRE( g_count == 1 );
	g_count = 0;
}

/*
> Quote paragraph 1.
>
> Quote paragraph 2.
>
>> Nested quote

*/
TEST_CASE( "017" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/017.md" );

	MD::forEach< TRAIT >( { MD::ItemType::Blockquote }, doc, onItem );
	REQUIRE( g_count == 2 );
	g_count = 0;
	
	MD::forEach< TRAIT >( { MD::ItemType::Paragraph }, doc, onItem );
	REQUIRE( g_count == 3 );
	g_count = 0;
	
	MD::forEach< TRAIT >( { MD::ItemType::Paragraph }, doc, onItem, 2 );
	REQUIRE( g_count == 2 );
	g_count = 0;
	
	MD::forEach< TRAIT >( { MD::ItemType::Blockquote, MD::ItemType::Paragraph }, doc, onItem );
	REQUIRE( g_count == 5 );
	g_count = 0;
}

/*
	if( a > b )
      do_something();
    else
      dont_do_anything();
*/
TEST_CASE( "021" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/021.md" );
	
	MD::forEach< TRAIT >( { MD::ItemType::Code }, doc, onItem );
	REQUIRE( g_count == 1 );
	g_count = 0;
}

/*
* Item 1

    Paragraph in list

    * Nested

        Paragraph in list

* Item 2

    Paragraph in list

    * Nested

        Paragraph in list

* Item 3

    Paragraph in list

    * Nested

        Paragraph in list

Standalone paragraph

*/
TEST_CASE( "029" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/029.md" );

	MD::forEach< TRAIT >( { MD::ItemType::List }, doc, onItem );
	REQUIRE( g_count == 4 );
	g_count = 0;
	
	MD::forEach< TRAIT >( { MD::ItemType::List }, doc, onItem, 1 );
	REQUIRE( g_count == 1 );
	g_count = 0;
	
	MD::forEach< TRAIT >( { MD::ItemType::Paragraph }, doc, onItem );
	REQUIRE( g_count == 13 );
	g_count = 0;
	
	MD::forEach< TRAIT >( { MD::ItemType::Paragraph }, doc, onItem , 3 );
	REQUIRE( g_count == 7 );
	g_count = 0;
}

/*
Text ![Image 1](a.jpg) continue ![ Image 2 ](b.png) and ![ Image 3]( http://www.where.com/c.jpeg "description" )

*/
TEST_CASE( "030" )
{
	MD::Parser< TRAIT > parser;	
	auto doc = parser.parse( "tests/parser/data/030.md" );
	
	MD::forEach< TRAIT >( { MD::ItemType::Image }, doc, onItem );
	REQUIRE( g_count == 3 );
	g_count = 0;
}

/*
[link 0][wrong-label] [link 1](a.md) [![image 1](a.png)](b.md) [link 3][label] [^ref]

[label]: http://www.where.com/a.md (caption)

[^ref] text

[1]: a.md 'title'

[link 4](#label)

*/
TEST_CASE( "031" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/031.md" );
	
	MD::forEach< TRAIT >( { MD::ItemType::Link }, doc, onItem );
	REQUIRE( g_count == 4 );
	g_count = 0;
	
	MD::forEach< TRAIT >( { MD::ItemType::FootnoteRef }, doc, onItem );
	REQUIRE( g_count == 2 );
	g_count = 0;
	
	MD::forEach< TRAIT >( { MD::ItemType::Image }, doc, onItem );
	REQUIRE( g_count == 1 );
	g_count = 0;
}

/*
Heading 1
=========
Paragraph 1

Heading 2
---------
Paragraph 2

  # Heading 1

Paragraph 1

## Heading 2

Paragraph 2

Heading 1
=========

Paragraph 1

Heading 2
---------

Paragraph 2

### Heading 3 {#heading-3}

*/
TEST_CASE( "046" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/046.md" );
	
	MD::forEach< TRAIT >( { MD::ItemType::Heading }, doc, onItem );
	REQUIRE( g_count == 7 );
	g_count = 0;
}

/*
When $a \ne 0$, there are two solutions to $(ax^2 + bx + c = 0)$ and they are
$$ x = {-b \pm \sqrt{b^2-4ac} \over 2a} $$

*/
TEST_CASE( "065" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/065.md" );
	
	MD::forEach< TRAIT >( { MD::ItemType::Math }, doc, onItem );
	REQUIRE( g_count == 3 );
	g_count = 0;
}

/*
| table |
| ----- |
| <img src="img/img.png"> |

*/
TEST_CASE( "168" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/168.md" );
	
	MD::forEach< TRAIT >( { MD::ItemType::Table }, doc, onItem );
	REQUIRE( g_count == 1 );
	g_count = 0;
	
	MD::forEach< TRAIT >( { MD::ItemType::RawHtml }, doc, onItem );
	REQUIRE( g_count == 1 );
	g_count = 0;
}

/*
Text
***
| table |
| --- |
| data |

*/
TEST_CASE( "174" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/174.md" );
	
	MD::forEach< TRAIT >( { MD::ItemType::HorizontalLine }, doc, onItem );
	REQUIRE( g_count == 1 );
	g_count = 0;
	
	MD::forEach< TRAIT >( { MD::ItemType::Table }, doc, onItem );
	REQUIRE( g_count == 1 );
	g_count = 0;
	
	MD::forEach< TRAIT >( { MD::ItemType::Paragraph }, doc, onItem );
	REQUIRE( g_count == 1 );
	g_count = 0;
	
	MD::forEach< TRAIT >( { MD::ItemType::Text }, doc, onItem );
	REQUIRE( g_count == 3 );
	g_count = 0;
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
	
	MD::forEach< TRAIT >( { MD::ItemType::Footnote }, doc, onItem );
	REQUIRE( g_count == 1 );
	g_count = 0;
	
	MD::forEach< TRAIT >( { MD::ItemType::FootnoteRef }, doc, onItem );
	REQUIRE( g_count == 1 );
	g_count = 0;
	
	MD::forEach< TRAIT >( { MD::ItemType::Code }, doc, onItem );
	REQUIRE( g_count == 1 );
	g_count = 0;
	
	MD::forEach< TRAIT >( { MD::ItemType::Paragraph }, doc, onItem );
	REQUIRE( g_count == 3 );
	g_count = 0;
}
