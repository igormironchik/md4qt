
/*
	SPDX-FileCopyrightText: 2022-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

// doctest include.
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

// 5.2 List items

TEST_CASE( "253" )
{
	const auto doc = load_test( 253 );

	REQUIRE( doc->items().size() == 4 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( p->items().size() == 2 );

		{
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"A paragraph" );
		}

		{
			REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"with two lines." );
		}
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( !c->isInline() );
		REQUIRE( c->text() == u8"indented code" );
	}

	{
		REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Blockquote );
		auto b = static_cast< MD::Blockquote< TRAIT >* > ( doc->items().at( 3 ).get() );
		REQUIRE( b->items().size() == 1 );

		REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( b->items().at( 0 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"A block quote." );
	}
}

TEST_CASE( "254" )
{
	const auto doc = load_test( 254 );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->items().size() == 1 );
	REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
	auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );

	REQUIRE( li->items().size() == 3 );

	{
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
		REQUIRE( p->items().size() == 2 );

		{
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"A paragraph" );
		}

		{
			REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"with two lines." );
		}
	}

	{
		REQUIRE( li->items().at( 1 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code< TRAIT >* > ( li->items().at( 1 ).get() );
		REQUIRE( !c->isInline() );
		REQUIRE( c->text() == u8"indented code" );
	}

	{
		REQUIRE( li->items().at( 2 )->type() == MD::ItemType::Blockquote );
		auto b = static_cast< MD::Blockquote< TRAIT >* > ( li->items().at( 2 ).get() );
		REQUIRE( b->items().size() == 1 );

		REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( b->items().at( 0 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"A block quote." );
	}
}

TEST_CASE( "255" )
{
	const auto doc = load_test( 255 );

	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->items().size() == 1 );
	REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
	auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );

	REQUIRE( li->items().size() == 1 );

	{
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"one" );
	}

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"two" );
}

TEST_CASE( "256" )
{
	const auto doc = load_test( 256 );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->items().size() == 1 );
	REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
	auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );

	REQUIRE( li->items().size() == 2 );

	{
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"one" );
	}

	{
		REQUIRE( li->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 1 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"two" );
	}
}

TEST_CASE( "257" )
{
	const auto doc = load_test( 257 );

	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->items().size() == 1 );
	REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
	auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );

	REQUIRE( li->items().size() == 1 );

	{
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"one" );
	}

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 2 ).get() );
	REQUIRE( !c->isInline() );
	REQUIRE( c->text() == u8" two" );
}

TEST_CASE( "258" )
{
	const auto doc = load_test( 258 );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->items().size() == 1 );
	REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
	auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );

	REQUIRE( li->items().size() == 2 );

	{
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"one" );
	}

	{
		REQUIRE( li->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 1 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"two" );
	}
}

TEST_CASE( "259" )
{
	const auto doc = load_test( 259 );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );
	auto b1 = static_cast< MD::Blockquote< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( b1->items().size() == 1 );
	REQUIRE( b1->items().at( 0 )->type() == MD::ItemType::Blockquote );
	auto b2 = static_cast< MD::Blockquote< TRAIT >* > ( b1->items().at( 0 ).get() );
	REQUIRE( b2->items().size() == 1 );

	REQUIRE( b2->items().at( 0 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( b2->items().at( 0 ).get() );
	REQUIRE( l->items().size() == 1 );
	REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
	auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );

	REQUIRE( li->items().size() == 2 );

	{
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"one" );
	}

	{
		REQUIRE( li->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 1 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"two" );
	}
}

TEST_CASE( "260" )
{
	const auto doc = load_test( 260 );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );
	auto b1 = static_cast< MD::Blockquote< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( b1->items().size() == 1 );
	REQUIRE( b1->items().at( 0 )->type() == MD::ItemType::Blockquote );
	auto b2 = static_cast< MD::Blockquote< TRAIT >* > ( b1->items().at( 0 ).get() );
	REQUIRE( b2->items().size() == 2 );

	REQUIRE( b2->items().at( 0 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( b2->items().at( 0 ).get() );
	REQUIRE( l->items().size() == 1 );
	REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
	auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );

	REQUIRE( li->items().size() == 1 );

	{
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"one" );
	}

	{
		REQUIRE( b2->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( b2->items().at( 1 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"two" );
	}
}

TEST_CASE( "261" )
{
	const auto doc = load_test( 261 );

	REQUIRE( doc->items().size() == 3 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"-one" );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"2.two" );
	}
}

TEST_CASE( "262" )
{
	const auto doc = load_test( 262 );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->items().size() == 1 );
	REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
	auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );

	REQUIRE( li->items().size() == 2 );

	{
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"foo" );
	}

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

TEST_CASE( "263" )
{
	const auto doc = load_test( 263 );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->items().size() == 1 );
	REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
	auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );

	REQUIRE( li->items().size() == 4 );

	{
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"foo" );
	}

	{
		REQUIRE( li->items().at( 1 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code< TRAIT >* > ( li->items().at( 1 ).get() );
		REQUIRE( !c->isInline() );
		REQUIRE( c->text() == u8"bar" );
	}

	{
		REQUIRE( li->items().at( 2 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 2 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"baz" );
	}

	{
		REQUIRE( li->items().at( 3 )->type() == MD::ItemType::Blockquote );
		auto b = static_cast< MD::Blockquote< TRAIT >* > ( li->items().at( 3 ).get() );
		REQUIRE( b->items().size() == 1 );
		REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( b->items().at( 0 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"bam" );
	}
}

TEST_CASE( "264" )
{
	const auto doc = load_test( 264 );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->items().size() == 1 );
	REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
	auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );

	REQUIRE( li->items().size() == 2 );

	{
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Foo" );
	}

	{
		REQUIRE( li->items().at( 1 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code< TRAIT >* > ( li->items().at( 1 ).get() );
		REQUIRE( !c->isInline() );
		REQUIRE( c->text() == u8"bar\n\n\nbaz" );
	}
}

TEST_CASE( "265" )
{
	const auto doc = load_test( 265 );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->items().size() == 1 );
	REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
	auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );

	REQUIRE( li->items().size() == 1 );
	REQUIRE( li->listType() == MD::ListItem< TRAIT >::Ordered );
	REQUIRE( li->startNumber() == 123456789 );

	{
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"ok" );
	}
}

TEST_CASE( "266" )
{
	const auto doc = load_test( 266 );

	REQUIRE( doc->items().size() == 2 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"1234567890. not ok" );
	}
}

TEST_CASE( "267" )
{
	const auto doc = load_test( 267 );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->items().size() == 1 );
	REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
	auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );

	REQUIRE( li->items().size() == 1 );
	REQUIRE( li->listType() == MD::ListItem< TRAIT >::Ordered );
	REQUIRE( li->startNumber() == 0 );

	{
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"ok" );
	}
}

TEST_CASE( "268" )
{
	const auto doc = load_test( 268 );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->items().size() == 1 );
	REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
	auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );

	REQUIRE( li->items().size() == 1 );
	REQUIRE( li->listType() == MD::ListItem< TRAIT >::Ordered );
	REQUIRE( li->startNumber() == 3 );

	{
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"ok" );
	}
}

TEST_CASE( "269" )
{
	const auto doc = load_test( 269 );

	REQUIRE( doc->items().size() == 2 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"-1. not ok" );
	}
}

TEST_CASE( "270" )
{
	const auto doc = load_test( 270 );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->items().size() == 1 );
	REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
	auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );

	REQUIRE( li->items().size() == 2 );
	REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );
	REQUIRE( li->startNumber() == 1 );

	{
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"foo" );
	}

	{
		REQUIRE( li->items().at( 1 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code< TRAIT >* > ( li->items().at( 1 ).get() );
		REQUIRE( !c->isInline() );
		REQUIRE( c->text() == u8"bar" );
	}
}

TEST_CASE( "271" )
{
	const auto doc = load_test( 271 );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->items().size() == 1 );
	REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
	auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );

	REQUIRE( li->items().size() == 2 );
	REQUIRE( li->listType() == MD::ListItem< TRAIT >::Ordered );
	REQUIRE( li->startNumber() == 10 );

	{
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"foo" );
	}

	{
		REQUIRE( li->items().at( 1 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code< TRAIT >* > ( li->items().at( 1 ).get() );
		REQUIRE( !c->isInline() );
		REQUIRE( c->text() == u8"bar" );
	}
}

TEST_CASE( "272" )
{
	const auto doc = load_test( 272 );

	REQUIRE( doc->items().size() == 4 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( c->text() == u8"indented code" );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->text() == u8"paragraph" );
	}

	{
		REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 3 ).get() );
		REQUIRE( c->text() == u8"more code" );
	}
}

TEST_CASE( "273" )
{
	const auto doc = load_test( 273 );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->items().size() == 1 );
	REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
	auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );

	REQUIRE( li->items().size() == 3 );
	REQUIRE( li->listType() == MD::ListItem< TRAIT >::Ordered );
	REQUIRE( li->startNumber() == 1 );

	{
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code< TRAIT >* > ( li->items().at( 0 ).get() );
		REQUIRE( c->text() == u8"indented code" );
	}

	{
		REQUIRE( li->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 1 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->text() == u8"paragraph" );
	}

	{
		REQUIRE( li->items().at( 2 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code< TRAIT >* > ( li->items().at( 2 ).get() );
		REQUIRE( c->text() == u8"more code" );
	}
}

TEST_CASE( "274" )
{
	const auto doc = load_test( 274 );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->items().size() == 1 );
	REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
	auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );

	REQUIRE( li->items().size() == 3 );
	REQUIRE( li->listType() == MD::ListItem< TRAIT >::Ordered );
	REQUIRE( li->startNumber() == 1 );

	{
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code< TRAIT >* > ( li->items().at( 0 ).get() );
		REQUIRE( c->text() == u8" indented code" );
	}

	{
		REQUIRE( li->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 1 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->text() == u8"paragraph" );
	}

	{
		REQUIRE( li->items().at( 2 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code< TRAIT >* > ( li->items().at( 2 ).get() );
		REQUIRE( c->text() == u8"more code" );
	}
}

TEST_CASE( "275" )
{
	const auto doc = load_test( 275 );

	REQUIRE( doc->items().size() == 3 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->text() == u8"foo" );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->text() == u8"bar" );
	}
}

TEST_CASE( "276" )
{
	const auto doc = load_test( 276 );

	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->items().size() == 1 );
	REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
	auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );

	REQUIRE( li->items().size() == 1 );
	REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );

	{
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->text() == u8"foo" );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->text() == u8"bar" );
	}
}

TEST_CASE( "277" )
{
	const auto doc = load_test( 277 );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->items().size() == 1 );
	REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
	auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );

	REQUIRE( li->items().size() == 2 );
	REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );

	{
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->text() == u8"foo" );
	}

	{
		REQUIRE( li->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 1 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->text() == u8"bar" );
	}
}

TEST_CASE( "278" )
{
	const auto doc = load_test( 278 );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->items().size() == 3 );

	{
		REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );

		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );

		{
			REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
			REQUIRE( t->text() == u8"foo" );
		}
	}

	{
		REQUIRE( l->items().at( 1 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 1 ).get() );

		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );

		{
			REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Code );
			auto c = static_cast< MD::Code< TRAIT >* > ( li->items().at( 0 ).get() );
			REQUIRE( c->text() == u8"bar" );
		}
	}

	{
		REQUIRE( l->items().at( 2 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 2 ).get() );

		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );

		{
			REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Code );
			auto c = static_cast< MD::Code< TRAIT >* > ( li->items().at( 0 ).get() );
			REQUIRE( c->text() == u8"baz" );
		}
	}
}

TEST_CASE( "279" )
{
	const auto doc = load_test( 279 );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->items().size() == 1 );

	{
		REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );

		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );

		{
			REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
			REQUIRE( t->text() == u8"foo" );
		}
	}
}

TEST_CASE( "280" )
{
	const auto doc = load_test( 280 );

	REQUIRE( doc->items().size() == 3 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
		auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( l->items().size() == 1 );
		REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
		REQUIRE( li->isEmpty() );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->text() == u8"foo" );
	}
}

TEST_CASE( "281" )
{
	const auto doc = load_test( 281 );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->items().size() == 3 );

	{
		REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );

		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );

		{
			REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
			REQUIRE( t->text() == u8"foo" );
		}
	}

	{
		REQUIRE( l->items().at( 1 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 1 ).get() );
		REQUIRE( li->isEmpty() );
	}

	{
		REQUIRE( l->items().at( 2 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 2 ).get() );

		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );

		{
			REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
			REQUIRE( t->text() == u8"bar" );
		}
	}
}

TEST_CASE( "282" )
{
	const auto doc = load_test( 282 );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->items().size() == 3 );

	{
		REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );

		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );

		{
			REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
			REQUIRE( t->text() == u8"foo" );
		}
	}

	{
		REQUIRE( l->items().at( 1 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 1 ).get() );
		REQUIRE( li->isEmpty() );
	}

	{
		REQUIRE( l->items().at( 2 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 2 ).get() );

		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );

		{
			REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
			REQUIRE( t->text() == u8"bar" );
		}
	}
}

TEST_CASE( "283" )
{
	const auto doc = load_test( 283 );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->items().size() == 3 );

	{
		REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );

		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->listType() == MD::ListItem< TRAIT >::Ordered );
		REQUIRE( li->startNumber() == 1 );

		{
			REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
			REQUIRE( t->text() == u8"foo" );
		}
	}

	{
		REQUIRE( l->items().at( 1 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 1 ).get() );
		REQUIRE( li->isEmpty() );
	}

	{
		REQUIRE( l->items().at( 2 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 2 ).get() );

		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->listType() == MD::ListItem< TRAIT >::Ordered );
		REQUIRE( li->startNumber() == 3 );

		{
			REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
			REQUIRE( t->text() == u8"bar" );
		}
	}
}

TEST_CASE( "284" )
{
	const auto doc = load_test( 284 );

	REQUIRE( doc->items().size() == 2 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
		auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( l->items().size() == 1 );
		REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
		REQUIRE( li->isEmpty() );
	}
}

TEST_CASE( "285" )
{
	const auto doc = load_test( 285 );

	REQUIRE( doc->items().size() == 3 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( p->items().size() == 2 );

		{
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
			REQUIRE( t->text() == u8"foo" );
		}

		{
			REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );
			REQUIRE( t->text() == u8"*" );
		}
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( p->items().size() == 2 );

		{
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
			REQUIRE( t->text() == u8"foo" );
		}

		{
			REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );
			REQUIRE( t->text() == u8"1." );
		}
	}
}

void tests_286_288( int test )
{
	const auto doc = load_test( test );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->items().size() == 1 );

	{
		REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );

		REQUIRE( li->items().size() == 3 );
		REQUIRE( li->listType() == MD::ListItem< TRAIT >::Ordered );
		REQUIRE( li->startNumber() == 1 );

		{
			REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
			REQUIRE( p->items().size() == 2 );

			{
				REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
				auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
				REQUIRE( t->text() == u8"A paragraph" );
			}

			{
				REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
				auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );
				REQUIRE( t->text() == u8"with two lines." );
			}
		}

		{
			REQUIRE( li->items().at( 1 )->type() == MD::ItemType::Code );
			auto c = static_cast< MD::Code< TRAIT >* > ( li->items().at( 1 ).get() );
			REQUIRE( c->text() == u8"indented code" );
		}

		{
			REQUIRE( li->items().at( 2 )->type() == MD::ItemType::Blockquote );
			auto b = static_cast< MD::Blockquote< TRAIT >* > ( li->items().at( 2 ).get() );
			REQUIRE( b->items().size() == 1 );
			REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph< TRAIT >* > ( b->items().at( 0 ).get() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
			REQUIRE( t->text() == u8"A block quote." );
		}
	}
}

TEST_CASE( "286" )
{
	tests_286_288( 286 );
}

TEST_CASE( "287" )
{
	tests_286_288( 287 );
}

TEST_CASE( "288" )
{
	tests_286_288( 288 );
}

TEST_CASE( "289" )
{
	const auto doc = load_test( 289 );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( c->text() == u8"1.  A paragraph\n    with two lines.\n\n"
		"        indented code\n\n    > A block quote." );
}

TEST_CASE( "290" )
{
	tests_286_288( 290 );
}

TEST_CASE( "291" )
{
	const auto doc = load_test( 291 );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->items().size() == 1 );

	{
		REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );

		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->listType() == MD::ListItem< TRAIT >::Ordered );
		REQUIRE( li->startNumber() == 1 );

		{
			REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
			REQUIRE( p->items().size() == 2 );

			{
				REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
				auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
				REQUIRE( t->text() == u8"A paragraph" );
			}

			{
				REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
				auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );
				REQUIRE( t->text() == u8"with two lines." );
			}
		}
	}
}

void test_292_293( int test )
{
	const auto doc = load_test( test );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );
	auto pb = static_cast< MD::Blockquote< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( pb->items().size() == 1 );
	REQUIRE( pb->items().at( 0 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( pb->items().at( 0 ).get() );
	REQUIRE( l->items().size() == 1 );

	{
		REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );

		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->listType() == MD::ListItem< TRAIT >::Ordered );
		REQUIRE( li->startNumber() == 1 );

		{
			REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Blockquote );
			auto b = static_cast< MD::Blockquote< TRAIT >* > ( li->items().at( 0 ).get() );
			REQUIRE( b->items().size() == 1 );
			REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph< TRAIT >* > ( b->items().at( 0 ).get() );
			REQUIRE( p->items().size() == 2 );

			{
				REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
				auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
				REQUIRE( t->text() == u8"Blockquote" );
			}

			{
				REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
				auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );
				REQUIRE( t->text() == u8"continued here." );
			}
		}
	}
}

TEST_CASE( "292" )
{
	test_292_293( 292 );
}

TEST_CASE( "293" )
{
	test_292_293( 293 );
}

TEST_CASE( "294" )
{
	const auto doc = load_test( 294 );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->items().size() == 1 );
	REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
	auto li1 = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
	REQUIRE( li1->items().size() == 2 );
	REQUIRE( li1->items().at( 0 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( li1->items().at( 0 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"foo" );

	{
		REQUIRE( li1->items().at( 1 )->type() == MD::ItemType::List );
		auto l = static_cast< MD::List< TRAIT >* > ( li1->items().at( 1 ).get() );
		REQUIRE( l->items().size() == 1 );
		REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
		auto li2 = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
		REQUIRE( li2->items().size() == 2 );
		REQUIRE( li2->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li2->items().at( 0 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"bar" );

		{
			REQUIRE( li2->items().at( 1 )->type() == MD::ItemType::List );
			auto l = static_cast< MD::List< TRAIT >* > ( li2->items().at( 1 ).get() );
			REQUIRE( l->items().size() == 1 );
			REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
			auto li3 = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
			REQUIRE( li3->items().size() == 2 );
			REQUIRE( li3->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph< TRAIT >* > ( li3->items().at( 0 ).get() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"baz" );

			{
				REQUIRE( li3->items().at( 1 )->type() == MD::ItemType::List );
				auto l = static_cast< MD::List< TRAIT >* > ( li3->items().at( 1 ).get() );
				REQUIRE( l->items().size() == 1 );
				REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
				auto li4 = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
				REQUIRE( li4->items().size() == 1 );
				REQUIRE( li4->items().at( 0 )->type() == MD::ItemType::Paragraph );
				auto p = static_cast< MD::Paragraph< TRAIT >* > ( li4->items().at( 0 ).get() );
				REQUIRE( p->items().size() == 1 );
				REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
				auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
				REQUIRE( t->opts() == MD::TextWithoutFormat );
				REQUIRE( t->text() == u8"boo" );
			}
		}
	}
}

TEST_CASE( "295" )
{
	const auto doc = load_test( 295 );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->items().size() == 4 );

	{
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

	{
		REQUIRE( l->items().at( 1 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 1 ).get() );
		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"bar" );
	}

	{
		REQUIRE( l->items().at( 2 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 2 ).get() );
		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"baz" );
	}

	{
		REQUIRE( l->items().at( 3 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 3 ).get() );
		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"boo" );
	}
}

TEST_CASE( "296" )
{
	const auto doc = load_test( 296 );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->items().size() == 1 );
	REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
	auto li1 = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
	REQUIRE( li1->items().size() == 2 );
	REQUIRE( li1->listType() == MD::ListItem< TRAIT >::Ordered );
	REQUIRE( li1->startNumber() == 10 );
	REQUIRE( li1->items().at( 0 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( li1->items().at( 0 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"foo" );

	{
		REQUIRE( li1->items().at( 1 )->type() == MD::ItemType::List );
		auto l = static_cast< MD::List< TRAIT >* > ( li1->items().at( 1 ).get() );
		REQUIRE( l->items().size() == 1 );
		REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
		auto li2 = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
		REQUIRE( li2->items().size() == 1 );
		REQUIRE( li2->listType() == MD::ListItem< TRAIT >::Unordered );
		REQUIRE( li2->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li2->items().at( 0 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"bar" );
	}
}

TEST_CASE( "297" )
{
	const auto doc = load_test( 297 );

	REQUIRE( doc->items().size() == 3 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
		auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( l->items().size() == 1 );
		REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->listType() == MD::ListItem< TRAIT >::Ordered );
		REQUIRE( li->startNumber() == 10 );
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"foo" );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::List );
		auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( l->items().size() == 1 );
		REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
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

TEST_CASE( "298" )
{
	const auto doc = load_test( 298 );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->items().size() == 1 );

	{
		REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::List );
		auto l = static_cast< MD::List< TRAIT >* > ( li->items().at( 0 ).get() );
		REQUIRE( l->items().size() == 1 );

		{
			REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
			auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
			REQUIRE( li->items().size() == 1 );
			REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );
			REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"foo" );
		}
	}
}

TEST_CASE( "299" )
{
	const auto doc = load_test( 299 );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->items().size() == 1 );

	{
		REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->listType() == MD::ListItem< TRAIT >::Ordered );
		REQUIRE( li->startNumber() == 1 );
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::List );
		auto l = static_cast< MD::List< TRAIT >* > ( li->items().at( 0 ).get() );
		REQUIRE( l->items().size() == 1 );

		{
			REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
			auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
			REQUIRE( li->items().size() == 1 );
			REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );
			REQUIRE( li->items().at( 0 )->type() == MD::ItemType::List );
			auto l = static_cast< MD::List< TRAIT >* > ( li->items().at( 0 ).get() );
			REQUIRE( l->items().size() == 1 );

			{
				REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
				auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
				REQUIRE( li->items().size() == 1 );
				REQUIRE( li->listType() == MD::ListItem< TRAIT >::Ordered );
				REQUIRE( li->startNumber() == 2 );

				REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
				auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
				REQUIRE( p->items().size() == 1 );
				REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
				auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
				REQUIRE( t->opts() == MD::TextWithoutFormat );
				REQUIRE( t->text() == u8"foo" );
			}
		}
	}
}

TEST_CASE( "300" )
{
	const auto doc = load_test( 300 );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->items().size() == 2 );

	{
		REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( li->items().at( 0 ).get() );
		REQUIRE( h->level() == 1 );
		REQUIRE( h->text().get() );
		auto p = h->text().get();
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->text() == u8"Foo" );
	}

	{
		REQUIRE( l->items().at( 1 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 1 ).get() );
		REQUIRE( li->items().size() == 2 );
		REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );

		{
			REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Heading );
			auto h = static_cast< MD::Heading< TRAIT >* > ( li->items().at( 0 ).get() );
			REQUIRE( h->level() == 2 );
			REQUIRE( h->text().get() );
			auto p = h->text().get();
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
			REQUIRE( t->text() == u8"Bar" );
		}

		{
			REQUIRE( li->items().at( 1 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 1 ).get() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
			REQUIRE( t->text() == u8"baz" );
		}
	}
}
