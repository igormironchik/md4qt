/*
	SPDX-FileCopyrightText: 2022-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

// doctest include.
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

// md4qt include.
#include <md4qt/traits.hpp>
#include <md4qt/parser.hpp>
#include <md4qt/poscache.hpp>


MD::PosCache< TRAIT > g_cache;


inline typename TRAIT::String
to_string( int i )
{
#ifdef MD4QT_QT_SUPPORT
	return QString::number( i );
#else
	return std::to_string( i );
#endif
}

void prepareTest( const typename TRAIT::String & fileName )
{
	
	MD::Parser< TRAIT > p;
	auto doc = p.parse( typename TRAIT::String( "tests/parser/data/" ) + fileName );
	
	g_cache.initialize( doc );
}

TEST_CASE( "001" )
{
	prepareTest( typename TRAIT::String( "001.md" ) );
	REQUIRE( g_cache.findFirstInCache( { 0, 0, 0, 0 } ).empty() );
	REQUIRE( g_cache.findFirstInCache( { 1, 1, 1, 1 } ).empty() );
}

/*
This is just a text!

*/
TEST_CASE( "002" )
{
	prepareTest( typename TRAIT::String( "002.md" ) );
	auto items = g_cache.findFirstInCache( { 0, 0, 0, 0 } );
	REQUIRE( items.size() == 2 );
	REQUIRE( items.at( 0 )->type() == MD::ItemType::Paragraph );
	REQUIRE( items.at( 1 )->type() == MD::ItemType::Text );
	REQUIRE( g_cache.findFirstInCache( { 0, 1, 0, 1 } ).empty() );
}

/*
  
Paragraph 1.

Paragraph 2.

*/
TEST_CASE( "003" )
{
	prepareTest( typename TRAIT::String( "003.md" ) );
	REQUIRE( g_cache.findFirstInCache( { 0, 0, 0, 0 } ).empty() );
	
	for( int i = 0; i < 2; ++i )
	{
		auto items = g_cache.findFirstInCache( { 0, 1 + i * 2, 0, 1 + i * 2 } );
		REQUIRE( items.size() == 2 );
		REQUIRE( items.at( 0 )->type() == MD::ItemType::Paragraph );
		REQUIRE( items.at( 1 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( items.at( 1 ) );
		REQUIRE( t->text() == typename TRAIT::String( "Paragraph " ) + to_string( i + 1 ) +
			typename TRAIT::String( "." ) );
	}
}

/*
Code in the `text`.

*/
TEST_CASE( "012" )
{
	prepareTest( typename TRAIT::String( "012.md" ) );
	
	{
		auto items = g_cache.findFirstInCache( { 0, 0, 0, 0 } );
		REQUIRE( items.size() == 2 );
		REQUIRE( items.at( 0 )->type() == MD::ItemType::Paragraph );
		REQUIRE( items.at( 1 )->type() == MD::ItemType::Text );
	}
	
	{
		auto items = g_cache.findFirstInCache( { 12, 0, 12, 0 } );
		REQUIRE( items.size() == 2 );
		REQUIRE( items.at( 0 )->type() == MD::ItemType::Paragraph );
		REQUIRE( items.at( 1 )->type() == MD::ItemType::Code );
	}
	
	{
		auto items = g_cache.findFirstInCache( { 13, 0, 13, 0 } );
		REQUIRE( items.size() == 2 );
		REQUIRE( items.at( 0 )->type() == MD::ItemType::Paragraph );
		REQUIRE( items.at( 1 )->type() == MD::ItemType::Code );
	}
	
	{
		auto items = g_cache.findFirstInCache( { 0, 0, 17, 0 } );
		REQUIRE( items.size() == 2 );
		REQUIRE( items.at( 0 )->type() == MD::ItemType::Paragraph );
		REQUIRE( items.at( 1 )->type() == MD::ItemType::Text );
	}
	
	{
		auto items = g_cache.findFirstInCache( { 18, 0, 18, 0 } );
		REQUIRE( items.size() == 2 );
		REQUIRE( items.at( 0 )->type() == MD::ItemType::Paragraph );
		REQUIRE( items.at( 1 )->type() == MD::ItemType::Text );
	}
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
	prepareTest( typename TRAIT::String( "017.md" ) );
	
	{
		auto items = g_cache.findFirstInCache( { 0, 0, 0, 0 } );
		REQUIRE( items.size() == 1 );
		REQUIRE( items.at( 0 )->type() == MD::ItemType::Blockquote );
	}
	
	{
		auto items = g_cache.findFirstInCache( { 1, 4, 1, 4 } );
		REQUIRE( items.size() == 2 );
		REQUIRE( items.at( 0 )->type() == MD::ItemType::Blockquote );
		REQUIRE( items.at( 1 )->type() == MD::ItemType::Blockquote );
	}
	
	{
		auto items = g_cache.findFirstInCache( { 2, 0, 2, 0 } );
		REQUIRE( items.size() == 3 );
		REQUIRE( items.at( 0 )->type() == MD::ItemType::Blockquote );
		REQUIRE( items.at( 1 )->type() == MD::ItemType::Paragraph );
		REQUIRE( items.at( 2 )->type() == MD::ItemType::Text );
	}
	
	{
		auto items = g_cache.findFirstInCache( { 0, 0, 0, 1 } );
		REQUIRE( items.size() == 3 );
		REQUIRE( items.at( 0 )->type() == MD::ItemType::Blockquote );
		REQUIRE( items.at( 1 )->type() == MD::ItemType::Paragraph );
		REQUIRE( items.at( 2 )->type() == MD::ItemType::Text );
	}
	
	{
		auto items = g_cache.findFirstInCache( { 3, 4, 3, 4 } );
		REQUIRE( items.size() == 4 );
		REQUIRE( items.at( 0 )->type() == MD::ItemType::Blockquote );
		REQUIRE( items.at( 1 )->type() == MD::ItemType::Blockquote );
		REQUIRE( items.at( 2 )->type() == MD::ItemType::Paragraph );
		REQUIRE( items.at( 3 )->type() == MD::ItemType::Text );
	}
}

/*
```cpp
if( a > b )
  do_something();
else
  dont_do_anything();
```

*/
TEST_CASE( "020" )
{
	prepareTest( typename TRAIT::String( "020.md" ) );
	
	{
		auto items = g_cache.findFirstInCache( { 0, 0, 0, 0 } );
		REQUIRE( items.size() == 1 );
		REQUIRE( items.at( 0 )->type() == MD::ItemType::Code );
	}
	
	{
		auto items = g_cache.findFirstInCache( { 0, 1, 0, 1 } );
		REQUIRE( items.size() == 1 );
		REQUIRE( items.at( 0 )->type() == MD::ItemType::Code );
	}
}

/*
	if( a > b )
      do_something();
    else
      dont_do_anything();
*/
TEST_CASE( "021" )
{
	prepareTest( typename TRAIT::String( "021.md" ) );
	
	{
		auto items = g_cache.findFirstInCache( { 4, 0, 4, 0 } );
		REQUIRE( items.size() == 1 );
		REQUIRE( items.at( 0 )->type() == MD::ItemType::Code );
	}
}

/*
* Item 1
* Item 2
* Item 3

*/
TEST_CASE( "023" )
{
	prepareTest( typename TRAIT::String( "023.md" ) );
	
	{
		auto items = g_cache.findFirstInCache( { 4, 0, 4, 0 } );
		REQUIRE( items.size() == 4 );
		REQUIRE( items.at( 0 )->type() == MD::ItemType::List );
		REQUIRE( items.at( 1 )->type() == MD::ItemType::ListItem );
		REQUIRE( items.at( 2 )->type() == MD::ItemType::Paragraph );
		REQUIRE( items.at( 3 )->type() == MD::ItemType::Text );
	}
	
	{
		auto items = g_cache.findFirstInCache( { 0, 0, 0, 0 } );
		REQUIRE( items.size() == 2 );
		REQUIRE( items.at( 0 )->type() == MD::ItemType::List );
		REQUIRE( items.at( 1 )->type() == MD::ItemType::ListItem );
	}
}

/*
* Item 1
  * Item 1
  * Item 2
* Item 2
  * Item 1
  * Item 2
* Item 3
  * Item 1
  * Item 2

*/
TEST_CASE( "024" )
{
	prepareTest( typename TRAIT::String( "024.md" ) );
	
	{
		auto items = g_cache.findFirstInCache( { 4, 1, 4, 1 } );
		REQUIRE( items.size() == 6 );
		REQUIRE( items.at( 0 )->type() == MD::ItemType::List );
		REQUIRE( items.at( 1 )->type() == MD::ItemType::ListItem );
		REQUIRE( items.at( 2 )->type() == MD::ItemType::List );
		REQUIRE( items.at( 3 )->type() == MD::ItemType::ListItem );
		REQUIRE( items.at( 4 )->type() == MD::ItemType::Paragraph );
		REQUIRE( items.at( 5 )->type() == MD::ItemType::Text );
	}
	
	{
		auto items = g_cache.findFirstInCache( { 2, 1, 2, 1 } );
		REQUIRE( items.size() == 4 );
		REQUIRE( items.at( 0 )->type() == MD::ItemType::List );
		REQUIRE( items.at( 1 )->type() == MD::ItemType::ListItem );
		REQUIRE( items.at( 2 )->type() == MD::ItemType::List );
		REQUIRE( items.at( 3 )->type() == MD::ItemType::ListItem );
	}
}

/*
* Item 1

  Paragraph in list

* Item 2

  Paragraph in list

* Item 3

  Paragraph in list

*/
TEST_CASE( "025" )
{
	prepareTest( typename TRAIT::String( "025.md" ) );
	
	{
		auto items = g_cache.findFirstInCache( { 2, 2, 2, 2 } );
		REQUIRE( items.size() == 4 );
		REQUIRE( items.at( 0 )->type() == MD::ItemType::List );
		REQUIRE( items.at( 1 )->type() == MD::ItemType::ListItem );
		REQUIRE( items.at( 2 )->type() == MD::ItemType::Paragraph );
		REQUIRE( items.at( 3 )->type() == MD::ItemType::Text );
	}
}

/*
Text ![Image 1](a.jpg) continue ![ Image 2 ](b.png) and ![ Image 3]( http://www.where.com/c.jpeg "description" )

*/
TEST_CASE( "030" )
{
	prepareTest( typename TRAIT::String( "030.md" ) );
	
	{
		auto items = g_cache.findFirstInCache( { 5, 0, 5, 0 } );
		REQUIRE( items.size() == 2 );
		REQUIRE( items.at( 0 )->type() == MD::ItemType::Paragraph );
		REQUIRE( items.at( 1 )->type() == MD::ItemType::Image );
	}
}

/*
 [^footnote]: Paragraph in footnote

    Paragraph in footnote

	Paragraph in footnote

*/
TEST_CASE( "045" )
{
	prepareTest( typename TRAIT::String( "045.md" ) );
	
	{
		auto items = g_cache.findFirstInCache( { 1, 0, 1, 0 } );
		REQUIRE( items.size() == 1 );
		REQUIRE( items.at( 0 )->type() == MD::ItemType::Footnote );
	}
	
	{
		auto items = g_cache.findFirstInCache( { 4, 2, 4, 2 } );
		REQUIRE( items.size() == 3 );
		REQUIRE( items.at( 0 )->type() == MD::ItemType::Footnote );
		REQUIRE( items.at( 1 )->type() == MD::ItemType::Paragraph );
		REQUIRE( items.at( 2 )->type() == MD::ItemType::Text );
	}
}

/*

Column 1 | Column 2
---------|---------
Cell 1   | Cell 2

| Column 1 | Column 2 |
|:--------:|---------:|
| Cell 1   | Cell 2   |

*/
TEST_CASE( "047" )
{
	prepareTest( typename TRAIT::String( "047.md" ) );
	
	{
		auto items = g_cache.findFirstInCache( { 0, 1, 0, 1 } );
		REQUIRE( items.size() == 2 );
		REQUIRE( items.at( 0 )->type() == MD::ItemType::Table );
		REQUIRE( items.at( 1 )->type() == MD::ItemType::Text );
	}
	
	{
		auto items = g_cache.findFirstInCache( { 0, 2, 0, 2 } );
		REQUIRE( items.size() == 1 );
		REQUIRE( items.at( 0 )->type() == MD::ItemType::Table );
	}
}
