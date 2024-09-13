
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

void check257WithSemiOpt( std::shared_ptr< MD::Document< TRAIT > > doc )
{
	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 2 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->text() == u8"some" );
		REQUIRE( t->opts() == MD::ItalicText );
		REQUIRE( t->openStyles().size() == 3 );
		REQUIRE( t->openStyles().at( 0 ) == MD::WithPosition{ 0, 0, 0, 0 } );
		REQUIRE( t->openStyles().at( 1 ) == MD::WithPosition{ 1, 0, 1, 0 } );
		REQUIRE( t->openStyles().at( 2 ) == MD::WithPosition{ 2, 0, 2, 0 } );
		REQUIRE( t->closeStyles().size() == 2 );
		REQUIRE( t->closeStyles().at( 0 ) == MD::WithPosition{ 7, 0, 7, 0 } );
		REQUIRE( t->closeStyles().at( 1 ) == MD::WithPosition{ 8, 0, 8, 0 } );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( t->text() == u8"text" );
		REQUIRE( t->opts() == MD::ItalicText );
		REQUIRE( t->openStyles().empty() );
		REQUIRE( t->closeStyles().size() == 1 );
		REQUIRE( t->closeStyles().at( 0 ) == MD::WithPosition{ 14, 0, 14, 0 } );
	}
}

/*
**_some_* text*

*/
TEST_CASE( "257-1" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/257.md", false, { u8"md", u8"markdown" },
		false );

	check257WithSemiOpt( doc );
}

/*
**_some_* text*

*/
TEST_CASE( "257-2" )
{
	const auto fileName = "tests/parser/data/257.md";

#ifdef MD4QT_QT_SUPPORT
	QFile file( fileName );

	if( file.open( QIODeviceBase::ReadOnly ) )
	{
		QTextStream stream( file.readAll() );
		file.close();

		typename TRAIT::String wd = QDir().absolutePath();
#else
	std::ifstream stream( fileName, std::ios::in | std::ios::binary );

	typename TRAIT::String wd = std::filesystem::canonical(
		std::filesystem::current_path() ).u8string();

	{
		std::string tmp;
		wd.toUTF8String( tmp );
		std::replace( tmp.begin(), tmp.end(), '\\', '/' );
		wd = icu::UnicodeString::fromUTF8( tmp );
	}

	if( stream.good() )
	{
#endif
		MD::Parser< TRAIT > parser;

		auto doc = parser.parse( stream, wd, fileName, false );

#ifndef MD4QT_QT_SUPPORT
		stream.close();
#endif

		check257WithSemiOpt( doc );
	}
	else
		REQUIRE( false );
}

/*
+ Create a list by starting a line with `+`, `-`, or `*`
+ Sub-lists are made by indenting 2 spaces:
  - Marker character change forces new list start:
    * Ac tristique libero volutpat at
    + Facilisis in pretium nisl aliquet
    - Nulla volutpat aliquam velit
+ Very easy!

*/
TEST_CASE( "261" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/261.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->items().size() == 3 );
	REQUIRE( l->startColumn() == 0 );
	REQUIRE( l->startLine() == 0 );
	REQUIRE( l->endColumn() == 11 );
	REQUIRE( l->endLine() == 6 );

	{
		REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
		REQUIRE( li->startColumn() == 0 );
		REQUIRE( li->startLine() == 0 );
		REQUIRE( li->endColumn() == 55 );
		REQUIRE( li->endLine() == 0 );
		REQUIRE( li->items().size() == 1 );
	}

	{
		REQUIRE( l->items().at( 1 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 1 ).get() );
		REQUIRE( li->startColumn() == 0 );
		REQUIRE( li->startLine() == 1 );
		REQUIRE( li->endColumn() == 33 );
		REQUIRE( li->endLine() == 5 );
		REQUIRE( li->items().size() == 2 );

		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		REQUIRE( li->items().at( 1 )->type() == MD::ItemType::List );
		auto l = static_cast< MD::List< TRAIT >* > ( li->items().at( 1 ).get() );
		REQUIRE( l->startColumn() == 2 );
		REQUIRE( l->startLine() == 2 );
		REQUIRE( l->endColumn() == 33 );
		REQUIRE( l->endLine() == 5 );

		{
			REQUIRE( l->items().size() == 1 );
			REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
			auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
			REQUIRE( li->startColumn() == 2 );
			REQUIRE( li->startLine() == 2 );
			REQUIRE( li->endColumn() == 33 );
			REQUIRE( li->endLine() == 5 );
			REQUIRE( li->items().size() == 4 );
			REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );

			{
				REQUIRE( li->items().at( 1 )->type() == MD::ItemType::List );
				auto l = static_cast< MD::List< TRAIT >* > ( li->items().at( 1 ).get() );
				REQUIRE( l->startColumn() == 4 );
				REQUIRE( l->startLine() == 3 );
				REQUIRE( l->endColumn() == 36 );
				REQUIRE( l->endLine() == 3 );
			}

			{
				REQUIRE( li->items().at( 2 )->type() == MD::ItemType::List );
				auto l = static_cast< MD::List< TRAIT >* > ( li->items().at( 2 ).get() );
				REQUIRE( l->startColumn() == 4 );
				REQUIRE( l->startLine() == 4 );
				REQUIRE( l->endColumn() == 38 );
				REQUIRE( l->endLine() == 4 );
			}

			{
				REQUIRE( li->items().at( 3 )->type() == MD::ItemType::List );
				auto l = static_cast< MD::List< TRAIT >* > ( li->items().at( 3 ).get() );
				REQUIRE( l->startColumn() == 4 );
				REQUIRE( l->startLine() == 5 );
				REQUIRE( l->endColumn() == 33 );
				REQUIRE( l->endLine() == 5 );
			}
		}
	}

	{
		REQUIRE( l->items().at( 2 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 2 ).get() );
		REQUIRE( li->startColumn() == 0 );
		REQUIRE( li->startLine() == 6 );
		REQUIRE( li->endColumn() == 11 );
		REQUIRE( li->endLine() == 6 );
		REQUIRE( li->items().size() == 1 );
	}
}

/*
- list item
- list item
<div class="some-class">

Content

</div>
- list item

*/
TEST_CASE( "262" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/262.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 5 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->startLine() == 0 );
	REQUIRE( l->endLine() == 1 );

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::RawHtml );
		auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( h->startLine() == 2 );
		REQUIRE( h->endLine() == 2 );
	}

	REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Paragraph );

	{
		REQUIRE( doc->items().at( 4 )->type() == MD::ItemType::RawHtml );
		auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 4 ).get() );
		REQUIRE( h->startLine() == 6 );
		REQUIRE( h->endLine() == 7 );
	}
}

/*
- list
<div>
  - list

*/
TEST_CASE( "263" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/263.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->startLine() == 0 );
	REQUIRE( l->endLine() == 0 );
	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 2 ).get() );
	REQUIRE( h->startLine() == 1 );
	REQUIRE( h->endLine() == 2 );
}

/*
- list
  <div>
  - list

*/
TEST_CASE( "264" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/264.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->startLine() == 0 );
	REQUIRE( l->endLine() == 2 );
	REQUIRE( l->items().size() == 1 );
	auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
	REQUIRE( li->items().size() == 2 );
	REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endLine() == 0 );
	REQUIRE( li->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( li->items().at( 1 ).get() );
	REQUIRE( h->startLine() == 1 );
	REQUIRE( h->endLine() == 2 );
}

/*
- list
<div>

  - list

*/
TEST_CASE( "265" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/265.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 4 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
		auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( l->startLine() == 0 );
		REQUIRE( l->endLine() == 0 );
		REQUIRE( l->items().size() == 1 );
	}

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 2 ).get() );
	REQUIRE( h->startLine() == 1 );
	REQUIRE( h->endLine() == 1 );

	{
		REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::List );
		auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 3 ).get() );
		REQUIRE( l->startLine() == 3 );
		REQUIRE( l->endLine() == 3 );
		REQUIRE( l->items().size() == 1 );
	}
}

/*
- list
  - list
    - list
  <div>
- list

*/
TEST_CASE( "266" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/266.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->startLine() == 0 );
	REQUIRE( l->endLine() == 4 );
	REQUIRE( l->items().size() == 2 );

	REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
	auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
	REQUIRE( li->startLine() == 0 );
	REQUIRE( li->endLine() == 3 );
	REQUIRE( li->items().size() == 3 );
	REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
	REQUIRE( li->items().at( 1 )->type() == MD::ItemType::List );
	REQUIRE( li->items().at( 2 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( li->items().at( 2 ).get() );
	REQUIRE( h->startLine() == 3 );
	REQUIRE( h->endLine() == 3 );
}

/*
- <!--
-->
- list

*/
TEST_CASE( "267" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/267.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->startLine() == 0 );
	REQUIRE( l->endLine() == 2 );
	REQUIRE( l->items().size() == 2 );

	REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
	auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
	REQUIRE( li->startLine() == 0 );
	REQUIRE( li->endLine() == 1 );
	REQUIRE( li->items().size() == 1 );
	REQUIRE( li->items().at( 0 )->type() == MD::ItemType::RawHtml );
}

/*
- list
  <div>
  ---

*/
TEST_CASE( "268" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/268.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->startLine() == 0 );
	REQUIRE( l->endLine() == 2 );
	REQUIRE( l->items().size() == 1 );

	REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
	auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
	REQUIRE( li->startLine() == 0 );
	REQUIRE( li->endLine() == 2 );
	REQUIRE( li->items().size() == 2 );
	REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
	REQUIRE( li->items().at( 1 )->type() == MD::ItemType::RawHtml );
}

/*
- list
  <div>

  ---

*/
TEST_CASE( "269" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/269.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->startLine() == 0 );
	REQUIRE( l->endLine() == 3 );
	REQUIRE( l->items().size() == 1 );

	REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
	auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
	REQUIRE( li->startLine() == 0 );
	REQUIRE( li->endLine() == 3 );
	REQUIRE( li->items().size() == 3 );
	REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
	REQUIRE( li->items().at( 1 )->type() == MD::ItemType::RawHtml );
	REQUIRE( li->items().at( 2 )->type() == MD::ItemType::HorizontalLine );
}

/*
- list
  <div>
---

*/
TEST_CASE( "270" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/270.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->startLine() == 0 );
	REQUIRE( l->endLine() == 1 );
	REQUIRE( l->items().size() == 1 );

	REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
	auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
	REQUIRE( li->startLine() == 0 );
	REQUIRE( li->endLine() == 1 );
	REQUIRE( li->items().size() == 2 );
	REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
	REQUIRE( li->items().at( 1 )->type() == MD::ItemType::RawHtml );

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::HorizontalLine );
}

/*
- list
  - list

    <div>

    text
  <div>
<div>

*/
TEST_CASE( "271" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/271.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->startLine() == 0 );
	REQUIRE( l->endLine() == 7 );
	REQUIRE( l->items().size() == 1 );

	{
		REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
		REQUIRE( li->startLine() == 0 );
		REQUIRE( li->endLine() == 7 );
		REQUIRE( li->items().size() == 3 );
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		REQUIRE( li->items().at( 1 )->type() == MD::ItemType::List );
		REQUIRE( li->items().at( 2 )->type() == MD::ItemType::RawHtml );

		{
			auto l = static_cast< MD::List< TRAIT >* > ( li->items().at( 1 ).get() );
			REQUIRE( l->startLine() == 1 );
			REQUIRE( l->endLine() == 5 );
			REQUIRE( l->items().size() == 1 );
			REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
			auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
			REQUIRE( li->items().size() == 3 );
			REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
			REQUIRE( li->items().at( 1 )->type() == MD::ItemType::RawHtml );
			REQUIRE( li->items().at( 2 )->type() == MD::ItemType::Paragraph );
		}
	}
}
