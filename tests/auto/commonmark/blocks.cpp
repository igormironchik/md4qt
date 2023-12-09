
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

// 3 Blocks and inlines

// 3.1Precedence

TEST_CASE( "042" )
{
	const auto doc = load_test( 42 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( l->items().size() == 2 );

	{
		REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
		REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );
		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at(  0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"`one" );
	}

	{
		REQUIRE( l->items().at( 1 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 1 ).get() );
		REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );
		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at(  0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"two`" );
	}
}

// 4 Leaf blocks

// 4.1 Thematic breaks

TEST_CASE( "043" )
{
	const auto doc = load_test( 43 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 4 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::HorizontalLine );
	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::HorizontalLine );
	REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::HorizontalLine );
}

TEST_CASE( "044" )
{
	const auto doc = load_test( 44 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"+++" );
}

TEST_CASE( "045" )
{
	const auto doc = load_test( 45 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"===" );
}

TEST_CASE( "046" )
{
	const auto doc = load_test( 46 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( p->items().size() == 3 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"--" );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"**" );
	}

	{
		REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 2 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"__" );
	}
}

TEST_CASE( "047" )
{
	const auto doc = load_test( 47 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 4 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::HorizontalLine );
	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::HorizontalLine );
	REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::HorizontalLine );
}

TEST_CASE( "048" )
{
	const auto doc = load_test( 48 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );

	auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( c->isInlined() == false );
	REQUIRE( c->syntax().size() == 0 );
	REQUIRE( c->text() == u8"***" );
}

TEST_CASE( "049" )
{
	const auto doc = load_test( 49 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( p->items().size() == 2 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Foo" );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"***" );
	}
}

TEST_CASE( "050" )
{
	const auto doc = load_test( 50 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::HorizontalLine );
}

TEST_CASE( "051" )
{
	const auto doc = load_test( 51 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::HorizontalLine );
}

TEST_CASE( "052" )
{
	const auto doc = load_test( 52 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::HorizontalLine );
}

TEST_CASE( "053" )
{
	const auto doc = load_test( 53 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::HorizontalLine );
}

TEST_CASE( "054" )
{
	const auto doc = load_test( 54 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::HorizontalLine );
}

TEST_CASE( "055" )
{
	const auto doc = load_test( 55 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 4 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"_ _ _ _ a" );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"a------" );
	}

	{
		REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 3 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"---a---" );
	}
}

TEST_CASE( "056" )
{
	const auto doc = load_test( 56 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::ItalicText );
	REQUIRE( t->text() == u8"-" );
}

TEST_CASE( "057" )
{
	const auto doc = load_test( 57 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 4 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
		auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( l->items().size() == 1 );
		REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"foo" );
	}

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::HorizontalLine );

	{
		REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::List );
		auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 3 ).get() );
		REQUIRE( l->items().size() == 1 );
		REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"bar" );
	}
}

TEST_CASE( "058" )
{
	const auto doc = load_test( 58 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 4 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Foo" );
	}

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::HorizontalLine );

	{
		REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 3 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"bar" );
	}
}

TEST_CASE( "059" )
{
	const auto doc = load_test( 59 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( h->level() == 2 );
		REQUIRE( h->text().get() );
		auto p = h->text().get();
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->text() == u8"Foo" );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"bar" );
	}
}

TEST_CASE( "060" )
{
	const auto doc = load_test( 60 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 4 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
		auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( l->items().size() == 1 );
		REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
		REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );
		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Foo" );
	}

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::HorizontalLine );

	{
		REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::List );
		auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 3 ).get() );
		REQUIRE( l->items().size() == 1 );
		REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
		REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );
		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Bar" );
	}
}

TEST_CASE( "061" )
{
	const auto doc = load_test( 61 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->items().size() == 2 );

	{
		REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
		REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );
		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Foo" );
	}

	{
		REQUIRE( l->items().at( 1 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 1 ).get() );
		REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );
		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::HorizontalLine );
	}
}

// 4.2 ATX headings

TEST_CASE( "062" )
{
	const auto doc = load_test( 62 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 7 );

	for( long long int i = 1; i < 7; ++i )
	{
		REQUIRE( doc->items().at( i )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( i ).get() );
		REQUIRE( h->level() == i );
		REQUIRE( h->text().get() );
		auto p = h->text().get();
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->text() == u8"foo" );
	}
}

TEST_CASE( "063" )
{
	const auto doc = load_test( 63 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"####### foo" );
}

TEST_CASE( "064" )
{
	const auto doc = load_test( 64 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"#5 bolt" );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"#hashtag" );
	}
}

TEST_CASE( "065" )
{
	const auto doc = load_test( 65 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"## foo" );
	}
}

TEST_CASE( "066" )
{
	const auto doc = load_test( 66 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( h->level() == 1 );
		REQUIRE( h->text().get() );
		auto p = h->text().get();
		REQUIRE( p->items().size() == 3 );

		{
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"foo" );
		}

		{
			REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );
			REQUIRE( t->opts() == MD::ItalicText );
			REQUIRE( t->text() == u8"bar" );
		}

		{
			REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 2 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"*baz*" );
		}
	}
}

TEST_CASE( "067" )
{
	const auto doc = load_test( 67 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( h->level() == 1 );
		REQUIRE( h->text().get() );
		auto p = h->text().get();
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->text() == u8"foo" );
	}
}

TEST_CASE( "068" )
{
	const auto doc = load_test( 68 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 4 );

	for( long long int i = 1; i < 4; ++i )
	{
		REQUIRE( doc->items().at( i )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( i ).get() );
		REQUIRE( h->level() == 4 - i );
		REQUIRE( h->text().get() );
		auto p = h->text().get();
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->text() == u8"foo" );
	}
}

TEST_CASE( "069" )
{
	const auto doc = load_test( 69 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );

	auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( c->isInlined() == false );
	REQUIRE( c->syntax().size() == 0 );
	REQUIRE( c->text() == u8"# foo" );
}

TEST_CASE( "070" )
{
	const auto doc = load_test( 70 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( p->items().size() == 2 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"foo" );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"# bar" );
	}
}

TEST_CASE( "071" )
{
	const auto doc = load_test( 71 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( h->level() == 2 );
		REQUIRE( h->text().get() );
		auto p = h->text().get();
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->text() == u8"foo" );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( h->level() == 3 );
		REQUIRE( h->text().get() );
		auto p = h->text().get();
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->text() == u8"bar" );
	}
}

TEST_CASE( "072" )
{
	const auto doc = load_test( 72 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( h->level() == 1 );
		REQUIRE( h->text().get() );
		auto p = h->text().get();
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->text() == u8"foo" );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( h->level() == 5 );
		REQUIRE( h->text().get() );
		auto p = h->text().get();
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->text() == u8"foo" );
	}
}

TEST_CASE( "073" )
{
	const auto doc = load_test( 73 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( h->level() == 3 );
		REQUIRE( h->text().get() );
		auto p = h->text().get();
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->text() == u8"foo" );
	}
}

TEST_CASE( "074" )
{
	const auto doc = load_test( 74 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( h->level() == 3 );
		REQUIRE( h->text().get() );
		auto p = h->text().get();
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->text() == u8"foo ### b" );
	}
}

TEST_CASE( "075" )
{
	const auto doc = load_test( 75 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( h->level() == 1 );
		REQUIRE( h->text().get() );
		auto p = h->text().get();
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->text() == u8"foo#" );
	}
}

TEST_CASE( "076" )
{
	const auto doc = load_test( 76 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 4 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( h->level() == 3 );
		REQUIRE( h->text().get() );
		auto p = h->text().get();
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->text() == u8"foo ###" );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( h->level() == 2 );
		REQUIRE( h->text().get() );
		auto p = h->text().get();
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->text() == u8"foo ###" );
	}

	{
		REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 3 ).get() );
		REQUIRE( h->level() == 1 );
		REQUIRE( h->text().get() );
		auto p = h->text().get();
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->text() == u8"foo #" );
	}
}

TEST_CASE( "077" )
{
	const auto doc = load_test( 77 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 4 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::HorizontalLine );

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( h->level() == 2 );
		REQUIRE( h->text().get() );
		auto p = h->text().get();
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->text() == u8"foo" );
	}

	REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::HorizontalLine );
}

TEST_CASE( "078" )
{
	const auto doc = load_test( 78 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 4 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->text() == u8"Foo bar" );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( h->level() == 1 );
		REQUIRE( h->text().get() );
		auto p = h->text().get();
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->text() == u8"baz" );
	}

	{
		REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 3 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->text() == u8"Bar foo" );
	}
}

TEST_CASE( "079" )
{
	const auto doc = load_test( 79 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 4 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( h->level() == 2 );
		REQUIRE( h->text().get() );
		auto p = h->text().get();
		REQUIRE( p->items().size() == 0 );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( h->level() == 1 );
		REQUIRE( h->text().get() );
		auto p = h->text().get();
		REQUIRE( p->items().size() == 0 );
	}

	{
		REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 3 ).get() );
		REQUIRE( h->level() == 3 );
		REQUIRE( h->text().get() );
		auto p = h->text().get();
		REQUIRE( p->items().size() == 0 );
	}
}

// 4.3 Setext headings

TEST_CASE( "080" )
{
	const auto doc = load_test( 80 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	for( auto i = 1; i < 3; ++i )
	{
		REQUIRE( doc->items().at( i )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( i ).get() );
		REQUIRE( h->level() == i );
		REQUIRE( h->text().get() );
		auto p = h->text().get();
		REQUIRE( p->items().size() == 2 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t1 = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t1->opts() == MD::TextWithoutFormat );
		REQUIRE( t1->text() == u8"Foo" );
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
		auto t2 = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( t2->opts() == MD::ItalicText );
		REQUIRE( t2->text() == u8"bar" );
	}
}

TEST_CASE( "081" )
{
	const auto doc = load_test( 81 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( h->level() == 1 );
		REQUIRE( h->text().get() );
		auto p = h->text().get();
		REQUIRE( p->items().size() == 3 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t1 = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t1->opts() == MD::TextWithoutFormat );
		REQUIRE( t1->text() == u8"Foo" );

		{
			REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
			auto t2 = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );
			REQUIRE( t2->opts() == MD::ItalicText );
			REQUIRE( t2->text() == u8"bar" );
		}

		{
			REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );
			auto t2 = static_cast< MD::Text< TRAIT >* > ( p->items().at( 2 ).get() );
			REQUIRE( t2->opts() == MD::ItalicText );
			REQUIRE( t2->text() == u8"baz" );
		}
	}
}

TEST_CASE( "082" )
{
	const auto doc = load_test( 82 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( h->level() == 1 );
		REQUIRE( h->text().get() );
		auto p = h->text().get();
		REQUIRE( p->items().size() == 3 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t1 = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t1->opts() == MD::TextWithoutFormat );
		REQUIRE( t1->text() == u8"Foo" );

		{
			REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
			auto t2 = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );
			REQUIRE( t2->opts() == MD::ItalicText );
			REQUIRE( t2->text() == u8"bar" );
		}

		{
			REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );
			auto t2 = static_cast< MD::Text< TRAIT >* > ( p->items().at( 2 ).get() );
			REQUIRE( t2->opts() == MD::ItalicText );
			REQUIRE( t2->text() == u8"baz" );
		}
	}
}

TEST_CASE( "083" )
{
	const auto doc = load_test( 83 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( h->level() == 2 );
		REQUIRE( h->text().get() );
		auto p = h->text().get();
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Foo" );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( h->level() == 1 );
		REQUIRE( h->text().get() );
		auto p = h->text().get();
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Foo" );
	}
}

TEST_CASE( "084" )
{
	const auto doc = load_test( 84 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 4 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( h->level() == 2 );
		REQUIRE( h->text().get() );
		auto p = h->text().get();
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Foo" );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( h->level() == 2 );
		REQUIRE( h->text().get() );
		auto p = h->text().get();
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Foo" );
	}

	{
		REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 3 ).get() );
		REQUIRE( h->level() == 1 );
		REQUIRE( h->text().get() );
		auto p = h->text().get();
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Foo" );
	}
}

TEST_CASE( "085" )
{
	const auto doc = load_test( 85 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( c->isInlined() == false );
	REQUIRE( c->syntax().size() == 0 );
	REQUIRE( c->text() == u8"Foo\n---\n\nFoo" );

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::HorizontalLine );
}

TEST_CASE( "086" )
{
	const auto doc = load_test( 86 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( h->level() == 2 );
		REQUIRE( h->text().get() );
		auto p = h->text().get();
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Foo" );
	}
}

TEST_CASE( "087" )
{
	const auto doc = load_test( 87 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 2 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Foo" );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"---" );
	}
}

TEST_CASE( "088" )
{
	const auto doc = load_test( 88 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 4 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( p->items().size() == 2 );

		{
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"Foo" );
		}

		{
			REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"= =" );
		}
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( p->items().size() == 1 );

		{
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"Foo" );
		}
	}

	REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::HorizontalLine );
}

TEST_CASE( "089" )
{
	const auto doc = load_test( 89 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( h->level() == 2 );
		REQUIRE( h->text().get() );
		auto p = h->text().get();
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Foo" );
	}
}

TEST_CASE( "090" )
{
	const auto doc = load_test( 90 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( h->level() == 2 );
		REQUIRE( h->text().get() );
		auto p = h->text().get();
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Foo\\" );
	}
}

TEST_CASE( "091" )
{
	const auto doc = load_test( 91 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 5 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( h->level() == 2 );
		REQUIRE( h->text().get() );
		auto p = h->text().get();
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"`Foo" );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"`" );
	}

	{
		REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 3 ).get() );
		REQUIRE( h->level() == 2 );
		REQUIRE( h->text().get() );
		auto p = h->text().get();
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"<a title=\"a lot" );
	}

	{
		REQUIRE( doc->items().at( 4 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 4 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"of dashes\"/>" );
	}
}

TEST_CASE( "092" )
{
	const auto doc = load_test( 92 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );
		auto b = static_cast< MD::Blockquote< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( b->items().size() == 1 );
		REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( b->items().at( 0 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Foo" );
	}

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::HorizontalLine );
}

TEST_CASE( "093" )
{
	const auto doc = load_test( 93 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );
		auto b = static_cast< MD::Blockquote< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( b->items().size() == 1 );
		REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( b->items().at( 0 ).get() );
		REQUIRE( p->items().size() == 3 );

		{
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"foo" );
		}

		{
			REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"bar" );
		}

		{
			REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 2 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"===" );
		}
	}
}

TEST_CASE( "094" )
{
	const auto doc = load_test( 94 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->items().size() == 1 );
	REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
	auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
	REQUIRE( li->items().size() == 1 );
	REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"Foo" );

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::HorizontalLine );
}

TEST_CASE( "095" )
{
	const auto doc = load_test( 95 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( h->level() == 2 );
		REQUIRE( h->text().get() );
		auto p = h->text().get();
		REQUIRE( p->items().size() == 2 );

		{
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"Foo" );
		}

		{
			REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"Bar" );
		}
	}
}

TEST_CASE( "096" )
{
	const auto doc = load_test( 96 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 5 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::HorizontalLine );

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( h->level() == 2 );
		REQUIRE( h->text().get() );
		auto p = h->text().get();
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Foo" );
	}

	{
		REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 3 ).get() );
		REQUIRE( h->level() == 2 );
		REQUIRE( h->text().get() );
		auto p = h->text().get();
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Bar" );
	}

	{
		REQUIRE( doc->items().at( 4 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 4 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Baz" );
	}
}

TEST_CASE( "097" )
{
	const auto doc = load_test( 97 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"====" );
	}
}

TEST_CASE( "098" )
{
	const auto doc = load_test( 98 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::HorizontalLine );
	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::HorizontalLine );
}

TEST_CASE( "099" )
{
	const auto doc = load_test( 99 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->items().size() == 1 );
	REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
	auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
	REQUIRE( li->items().size() == 1 );
	REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"foo" );

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::HorizontalLine );
}

TEST_CASE( "100" )
{
	const auto doc = load_test( 100 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( c->isInlined() == false );
	REQUIRE( c->syntax().size() == 0 );
	REQUIRE( c->text() == u8"foo" );

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::HorizontalLine );
}

TEST_CASE( "101" )
{
	const auto doc = load_test( 101 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );
		auto b = static_cast< MD::Blockquote< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( b->items().size() == 1 );
		REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( b->items().at( 0 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"foo" );
	}

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::HorizontalLine );
}

TEST_CASE( "102" )
{
	const auto doc = load_test( 102 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( h->level() == 2 );
		REQUIRE( h->text().get() );
		auto p = h->text().get();
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"> foo" );
	}
}

TEST_CASE( "103" )
{
	const auto doc = load_test( 103 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 4 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Foo" );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( h->level() == 2 );
		REQUIRE( h->text().get() );
		auto p = h->text().get();
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"bar" );
	}

	{
		REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 3 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"baz" );
	}
}

TEST_CASE( "104" )
{
	const auto doc = load_test( 104 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 4 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( p->items().size() == 2 );

		{
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"Foo" );
		}

		{
			REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"bar" );
		}
	}

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::HorizontalLine );

	{
		REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 3 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"baz" );
	}
}

TEST_CASE( "105" )
{
	const auto doc = load_test( 105 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 4 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( p->items().size() == 2 );

		{
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"Foo" );
		}

		{
			REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"bar" );
		}
	}

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::HorizontalLine );

	{
		REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 3 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"baz" );
	}
}

TEST_CASE( "106" )
{
	const auto doc = load_test( 106 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( p->items().size() == 4 );

		{
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"Foo" );
		}

		{
			REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"bar" );
		}

		{
			REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 2 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"---" );
		}

		{
			REQUIRE( p->items().at( 3 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 3 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"baz" );
		}
	}
}

// 4.4 Indented code blocks

TEST_CASE( "107" )
{
	const auto doc = load_test( 107 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( !c->isInlined() );
	REQUIRE( c->syntax().size() == 0 );
	REQUIRE( c->text() == u8"a simple\n  indented code block" );
}

TEST_CASE( "108" )
{
	const auto doc = load_test( 108 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->items().size() == 1 );

	{
		REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
		REQUIRE( li->items().size() == 2 );
		REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"foo" );

		{
			REQUIRE( li->items().at( 1 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 1 ).get() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"bar" );
		}
	}
}

TEST_CASE( "109" )
{
	const auto doc = load_test( 109 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto ol = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( ol->items().size() == 1 );

	{
		REQUIRE( ol->items().at( 0 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( ol->items().at( 0 ).get() );
		REQUIRE( li->items().size() == 2 );
		REQUIRE( li->listType() == MD::ListItem< TRAIT >::Ordered );
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"foo" );

		REQUIRE( li->items().at( 1 )->type() == MD::ItemType::List );
		auto ul = static_cast< MD::List< TRAIT >* > ( li->items().at( 1 ).get() );
		REQUIRE( ul->items().size() == 1 );

		{
			REQUIRE( ul->items().at( 0 )->type() == MD::ItemType::ListItem );
			auto li = static_cast< MD::ListItem< TRAIT >* > ( ul->items().at( 0 ).get() );
			REQUIRE( li->items().size() == 1 );
			REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );
			REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"bar" );
		}
	}
}

TEST_CASE( "110" )
{
	const auto doc = load_test( 110 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( !c->isInlined() );
	REQUIRE( c->syntax().size() == 0 );
	REQUIRE( c->text() == u8"<a/>\n*hi*\n\n- one" );
}

TEST_CASE( "111" )
{
	const auto doc = load_test( 111 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( !c->isInlined() );
	REQUIRE( c->syntax().size() == 0 );
	REQUIRE( c->text() == u8"chunk1\n\nchunk2\n\n\n\nchunk3" );
}

TEST_CASE( "112" ) // Not strict to CommonMark.
{
	MESSAGE( "This test is not strict to CommonMark 0.30." );

	const auto doc = load_test( 112 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( !c->isInlined() );
	REQUIRE( c->syntax().size() == 0 );
	// I skipped two spaces on the second line. Line is empty, so I guess
	// that this is not so critical for renderer.
	REQUIRE( c->text() == u8"chunk1\n\n  chunk2" );
}

TEST_CASE( "113" )
{
	const auto doc = load_test( 113 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 2 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Foo" );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"bar" );
	}
}

TEST_CASE( "114" )
{
	const auto doc = load_test( 114 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( !c->isInlined() );
	REQUIRE( c->text() == u8"foo" );

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"bar" );
}

TEST_CASE( "115" )
{
	const auto doc = load_test( 115 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 6 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( h->text().get() );
		auto p = h->text().get();
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( h->level() == 1 );
		REQUIRE( t->text() == u8"Heading" );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( !c->isInlined() );
		REQUIRE( c->text() == u8"foo" );
	}

	{
		REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 3 ).get() );
		REQUIRE( h->text().get() );
		auto p = h->text().get();
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( h->level() == 2 );
		REQUIRE( t->text() == u8"Heading" );
	}

	{
		REQUIRE( doc->items().at( 4 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 4 ).get() );
		REQUIRE( !c->isInlined() );
		REQUIRE( c->text() == u8"foo" );
	}

	REQUIRE( doc->items().at( 5 )->type() == MD::ItemType::HorizontalLine );
}

TEST_CASE( "116" )
{
	const auto doc = load_test( 116 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );


	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( !c->isInlined() );
		REQUIRE( c->text() == u8"    foo\nbar" );
	}
}

TEST_CASE( "117" )
{
	const auto doc = load_test( 117 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );


	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( !c->isInlined() );
		REQUIRE( c->text() == u8"foo" );
	}
}

TEST_CASE( "118" )
{
	const auto doc = load_test( 118 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );


	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( !c->isInlined() );
		REQUIRE( c->text() == u8"foo  " );
	}
}

// 4.5 Fenced code blocks

TEST_CASE( "119" )
{
	const auto doc = load_test( 119 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );


	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( !c->isInlined() );
		REQUIRE( c->text() == u8"<\n >" );
	}
}

TEST_CASE( "120" )
{
	const auto doc = load_test( 120 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );


	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( !c->isInlined() );
		REQUIRE( c->text() == u8"<\n >" );
	}
}

TEST_CASE( "121" )
{
	const auto doc = load_test( 121 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( c->isInlined() );
	REQUIRE( c->text() == u8"foo" );
}

TEST_CASE( "122" )
{
	const auto doc = load_test( 122 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( !c->isInlined() );
		REQUIRE( c->text() == u8"aaa\n~~~" );
	}
}

TEST_CASE( "123" )
{
	const auto doc = load_test( 123 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( !c->isInlined() );
		REQUIRE( c->text() == u8"aaa\n```" );
	}
}

TEST_CASE( "124" )
{
	const auto doc = load_test( 124 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( !c->isInlined() );
		REQUIRE( c->text() == u8"aaa\n```" );
	}
}

TEST_CASE( "125" )
{
	const auto doc = load_test( 125 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( !c->isInlined() );
		REQUIRE( c->text() == u8"aaa\n~~~" );
	}
}

TEST_CASE( "126" )
{
	const auto doc = load_test( 126 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( !c->isInlined() );
		REQUIRE( c->text().size() == 0 );
	}
}

TEST_CASE( "127" )
{
	const auto doc = load_test( 127 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( !c->isInlined() );
		REQUIRE( c->text() == u8"\n```\naaa" );
	}
}

TEST_CASE( "128" )
{
	const auto doc = load_test( 128 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );
	auto b = static_cast< MD::Blockquote< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( b->items().size() == 1 );
	REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code< TRAIT >* > ( b->items().at( 0 ).get() );
	REQUIRE( !c->isInlined() );
	REQUIRE( c->text() == u8"aaa" );

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"bbb" );
}

TEST_CASE( "129" )
{
	const auto doc = load_test( 129 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( !c->isInlined() );
		REQUIRE( c->text() == u8"\n  " );
	}
}

TEST_CASE( "130" )
{
	const auto doc = load_test( 130 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( !c->isInlined() );
		REQUIRE( c->text().size() == 0 );
	}
}

TEST_CASE( "131" )
{
	const auto doc = load_test( 131 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( !c->isInlined() );
		REQUIRE( c->text() == u8"aaa\naaa" );
	}
}

TEST_CASE( "132" )
{
	const auto doc = load_test( 132 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( !c->isInlined() );
		REQUIRE( c->text() == u8"aaa\naaa\naaa" );
	}
}

TEST_CASE( "133" )
{
	const auto doc = load_test( 133 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( !c->isInlined() );
		REQUIRE( c->text() == u8"aaa\n aaa\naaa" );
	}
}

TEST_CASE( "134" )
{
	const auto doc = load_test( 134 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( !c->isInlined() );
		REQUIRE( c->text() == u8"```\naaa\n```" );
	}
}

TEST_CASE( "135" )
{
	const auto doc = load_test( 135 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( !c->isInlined() );
		REQUIRE( c->text() == u8"aaa" );
	}
}

TEST_CASE( "136" )
{
	const auto doc = load_test( 136 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( !c->isInlined() );
		REQUIRE( c->text() == u8"aaa" );
	}
}

TEST_CASE( "137" )
{
	const auto doc = load_test( 137 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( !c->isInlined() );
		REQUIRE( c->text() == u8"aaa\n    ```" );
	}
}

TEST_CASE( "138" )
{
	const auto doc = load_test( 138 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 2 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( c->isInlined() );
	REQUIRE( c->text() == u8" " );
	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"aaa" );
}

TEST_CASE( "139" )
{
	const auto doc = load_test( 139 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( !c->isInlined() );
		REQUIRE( c->text() == u8"aaa\n~~~ ~~" );
	}
}

TEST_CASE( "140" )
{
	const auto doc = load_test( 140 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 4 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"foo" );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( !c->isInlined() );
		REQUIRE( c->text() == u8"bar" );
	}

	{
		REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 3 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"baz" );
	}
}

TEST_CASE( "141" )
{
	const auto doc = load_test( 141 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 4 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( h->text().get() );
		auto p = h->text().get();
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( h->level() == 2 );
		REQUIRE( t->text() == u8"foo" );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( !c->isInlined() );
		REQUIRE( c->text() == u8"bar" );
	}

	{
		REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 3 ).get() );
		REQUIRE( h->text().get() );
		auto p = h->text().get();
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( h->level() == 1 );
		REQUIRE( t->text() == u8"baz" );
	}
}

TEST_CASE( "142" )
{
	const auto doc = load_test( 142 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( !c->isInlined() );
	REQUIRE( c->syntax() == u8"ruby" );
	REQUIRE( c->text() == u8"def foo(x)\n  return 3\nend" );
}

TEST_CASE( "143" )
{
	const auto doc = load_test( 143 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( !c->isInlined() );
	REQUIRE( c->syntax() == u8"ruby" );
	REQUIRE( c->text() == u8"def foo(x)\n  return 3\nend" );
}

TEST_CASE( "144" )
{
	const auto doc = load_test( 144 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( !c->isInlined() );
	REQUIRE( c->syntax() == u8";" );
	REQUIRE( c->text().size() == 0 );
}

TEST_CASE( "145" )
{
	const auto doc = load_test( 145 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( p->items().size() == 2 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( c->isInlined() );
		REQUIRE( c->text() == u8"aa" );
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"foo" );
	}
}

TEST_CASE( "146" )
{
	const auto doc = load_test( 146 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( !c->isInlined() );
	REQUIRE( c->syntax() == u8"aa" );
	REQUIRE( c->text() == u8"foo" );
}

TEST_CASE( "147" )
{
	const auto doc = load_test( 147 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( !c->isInlined() );
	REQUIRE( c->syntax().size() == 0 );
	REQUIRE( c->text() == u8"``` aaa" );
}
