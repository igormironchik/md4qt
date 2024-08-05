
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
		REQUIRE( !t->isSpaceBefore() );
		REQUIRE( !t->isSpaceAfter() );
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

/*
Syntax Highlighting QML API   {#qml_api}
==================

*/
TEST_CASE( "250" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/250.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Heading );
	auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( h->label().startsWith( TRAIT::String( "#qml_api" ) ) );
	REQUIRE( h->text()->items().size() == 1 );
	REQUIRE( h->text()->startColumn() == 0 );
	REQUIRE( h->text()->startLine() == 0 );
	REQUIRE( h->text()->endColumn() == 29 );
	REQUIRE( h->text()->endLine() == 0 );
	REQUIRE( h->labelPos() == MD::WithPosition{ 30, 0, 39, 0 } );
	REQUIRE( h->delims().size() == 1 );
	REQUIRE( h->delims().front() == MD::WithPosition{ 0, 1, 17, 1 } );
	auto t = static_cast< MD::Text< TRAIT >* > ( h->text()->items().at( 0 ).get() );
	REQUIRE( t->startColumn() == 0 );
	REQUIRE( t->startLine() == 0 );
	REQUIRE( t->endColumn() == 29 );
	REQUIRE( t->endLine() == 0 );
	REQUIRE( t->text() == u8"Syntax Highlighting QML API" );
}

/*
## headig {#heading} ##

*/
TEST_CASE( "251" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/251.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Heading );
	auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( h->label().startsWith( TRAIT::String( "#heading" ) ) );
	REQUIRE( h->text()->items().size() == 1 );
	REQUIRE( h->text()->startColumn() == 3 );
	REQUIRE( h->text()->startLine() == 0 );
	REQUIRE( h->text()->endColumn() == 8 );
	REQUIRE( h->text()->endLine() == 0 );
	REQUIRE( h->labelPos() == MD::WithPosition{ 10, 0, 19, 0 } );
	REQUIRE( h->delims().size() == 2 );
	REQUIRE( h->delims().front() == MD::WithPosition{ 0, 0, 1, 0 } );
	REQUIRE( h->delims().back() == MD::WithPosition{ 21, 0, 22, 0 } );
	auto t = static_cast< MD::Text< TRAIT >* > ( h->text()->items().at( 0 ).get() );
	REQUIRE( t->startColumn() == 3 );
	REQUIRE( t->startLine() == 0 );
	REQUIRE( t->endColumn() == 8 );
	REQUIRE( t->endLine() == 0 );
	REQUIRE( t->text() == u8"headig" );
}

/*
`code` \
-

*/
TEST_CASE( "252" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/252.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Heading );
	auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( h->text()->items().size() == 2 );
	REQUIRE( h->text()->items().at( 1 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( h->text()->items().at( 1 ).get() );
	REQUIRE( t->text() == u8"\\" );
	REQUIRE( t->startColumn() == 7 );
	REQUIRE( t->endColumn() == 7 );
}

/*
{#id} \
-

*/
TEST_CASE( "253" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/253.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Heading );
	auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( h->text()->items().size() == 1 );
	REQUIRE( h->text()->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( h->text()->items().at( 0 ).get() );
	REQUIRE( t->text() == u8"{#id} \\" );
	REQUIRE( t->startColumn() == 0 );
	REQUIRE( t->endColumn() == 6 );
	REQUIRE( h->labelPos() == MD::WithPosition{ -1, -1, -1, -1 } );
}

/*
{#id}
-

*/
TEST_CASE( "254" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/254.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Heading );
	auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( h->text()->items().empty() );
	REQUIRE( h->labelPos() == MD::WithPosition{ 0, 0, 4, 0 } );
	REQUIRE( h->label().startsWith( typename TRAIT::String( "#id" ) ) );
}

/*
www.google.com

*/
TEST_CASE( "231-1" )
{
	MD::Parser< TRAIT > parser;
	parser.removeTextPlugin( MD::GitHubAutoLinkPluginID );

	auto doc = parser.parse( "tests/parser/data/231.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->startColumn() == 0 );
	REQUIRE( t->startLine() == 0 );
	REQUIRE( t->endColumn() == 13 );
	REQUIRE( t->endLine() == 0 );
	REQUIRE( t->text() == u8"www.google.com" );
}

/*
</

<//

*/
TEST_CASE( "255" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/255.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->text() == u8"</" );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->text() == u8"<//" );
	}
}

/*
**Some* text**

*/
TEST_CASE( "256" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/256.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 2 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->text() == u8"Some text" );
		REQUIRE( t->opts() == MD::ItalicText );
		REQUIRE( t->openStyles().size() == 2 );
		REQUIRE( t->openStyles().at( 0 ) == MD::WithPosition{ 0, 0, 0, 0 } );
		REQUIRE( t->openStyles().at( 1 ) == MD::WithPosition{ 1, 0, 1, 0 } );
		REQUIRE( t->closeStyles().size() == 2 );
		REQUIRE( t->closeStyles().at( 0 ) == MD::WithPosition{ 6, 0, 6, 0 } );
		REQUIRE( t->closeStyles().at( 1 ) == MD::WithPosition{ 12, 0, 12, 0 } );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( t->text() == u8"*" );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->startColumn() == 13 );
		REQUIRE( t->startLine() == 0 );
		REQUIRE( t->endColumn() == 13 );
		REQUIRE( t->endLine() == 0 );
	}
}

/*
**_some_* text*

*/
TEST_CASE( "257" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/257.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->text() == u8"some text" );
	REQUIRE( t->opts() == MD::ItalicText );
	REQUIRE( t->openStyles().size() == 3 );
	REQUIRE( t->openStyles().at( 0 ) == MD::WithPosition{ 0, 0, 0, 0 } );
	REQUIRE( t->openStyles().at( 1 ) == MD::WithPosition{ 1, 0, 1, 0 } );
	REQUIRE( t->openStyles().at( 2 ) == MD::WithPosition{ 2, 0, 2, 0 } );
	REQUIRE( t->closeStyles().size() == 3 );
	REQUIRE( t->closeStyles().at( 0 ) == MD::WithPosition{ 7, 0, 7, 0 } );
	REQUIRE( t->closeStyles().at( 1 ) == MD::WithPosition{ 8, 0, 8, 0 } );
	REQUIRE( t->closeStyles().at( 2 ) == MD::WithPosition{ 14, 0, 14, 0 } );
}

/*
==

==

*/
TEST_CASE( "258" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/258.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	for( int i = 1; i < 3; ++i )
	{
		REQUIRE( doc->items().at( i )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( i ).get() );
		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->text() == u8"==" );
	}
}

/*
*$a \ne 0$*
*`code`*
*[google](https://www.google.com)*
*![](https://www.google.com)*
*[^1]*

[^1]: foot


*/
TEST_CASE( "259" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/259.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );
	REQUIRE( doc->footnotesMap().size() == 1 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 5 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Math );
	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Code );
	REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Link );
	REQUIRE( p->items().at( 3 )->type() == MD::ItemType::Image );
	REQUIRE( p->items().at( 4 )->type() == MD::ItemType::FootnoteRef );

	for( int i = 0; i < 2; ++i )
		REQUIRE( static_cast< MD::ItemWithOpts< TRAIT >* > (
			p->items().at( i ).get() )->startColumn() == 2 );

	for( int i = 2; i < 5; ++i )
		REQUIRE( static_cast< MD::ItemWithOpts< TRAIT >* > (
			p->items().at( i ).get() )->startColumn() == 1 );
}

/*
*[^1]*

*/
TEST_CASE( "260" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/260.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );
	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::FootnoteRef );
	auto f = static_cast< MD::FootnoteRef< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( f->startColumn() == 1 );
	REQUIRE( f->endColumn() == 4 );
	REQUIRE( f->openStyles().size() == 1 );
	REQUIRE( f->openStyles().at( 0 ) == MD::StyleDelim{ MD::ItalicText, 0, 0, 0, 0 } );
	REQUIRE( f->closeStyles().size() == 1 );
	REQUIRE( f->closeStyles().at( 0 ) == MD::StyleDelim{ MD::ItalicText, 5, 0, 5, 0 } );
}
