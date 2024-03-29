
/*
	SPDX-FileCopyrightText: 2022-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

// doctest include.
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

// CommonMark 0.31.2

/*
foo <!-- this is a --
comment - with hyphens -->

*/
TEST_CASE( "625" )
{
	const auto doc = load_test( 625, "0.31.2" );

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
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::RawHtml );
		auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( h->text() == u8"<!-- this is a --\ncomment - with hyphens -->" );
	}
}

/*
foo <!--> foo -->

foo <!---> foo -->

*/
TEST_CASE( "626" )
{
	const auto doc = load_test( 626, "0.31.2" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( p->items().size() == 3 );

		{
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"foo" );
		}

		{
			REQUIRE( p->items().at( 1 )->type() == MD::ItemType::RawHtml );
			auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 1 ).get() );
			REQUIRE( h->text() == u8"<!-->" );
		}

		{
			REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 2 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"foo -->" );
		}
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( p->items().size() == 3 );

		{
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"foo" );
		}

		{
			REQUIRE( p->items().at( 1 )->type() == MD::ItemType::RawHtml );
			auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 1 ).get() );
			REQUIRE( h->text() == u8"<!--->" );
		}

		{
			REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 2 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"foo -->" );
		}
	}
}

/*
*$*alpha.

*£*bravo.

*€*charlie.

*/
TEST_CASE( "354" )
{
	const auto doc = load_test( 354, "0.31.2" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 4 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"*$*alpha." );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == TRAIT::String( u8"*£*bravo." ) );
	}

	{
		REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 3 ).get() );
		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == TRAIT::String( u8"*€*charlie." ) );
	}
}
