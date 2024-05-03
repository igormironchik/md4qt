
/*
	SPDX-FileCopyrightText: 2022-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

// doctest include.
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

// 2.4 Backslash escapes

TEST_CASE( "012" )
{
	const auto doc = load_test( 12 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );
	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~" );
}

TEST_CASE( "013" )
{
	const auto doc = load_test( 13 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );
	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == TRAIT::String( (const char *) u8"\\ \\A\\a\\ \\3\\φ\\«" ) );
}

TEST_CASE( "014" )
{
	const auto doc = load_test( 14 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );
	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( p->items().size() == 9 );

	std::vector< typename TRAIT::String > data = { u8"*not emphasized*",
		u8"<br/> not a tag", u8"[not a link](/foo)",
		u8"`not code`", u8"1. not a list", u8"* not a list",
		u8"# not a heading", u8"[foo]: /url \"not a reference\"",
		u8"&ouml; not a character entity" };

	for( size_t i = 0; i < p->items().size(); ++i )
	{
		REQUIRE( p->items().at( i )->type() == MD::ItemType::Text );
		auto t1 = static_cast< MD::Text< TRAIT >* > ( p->items().at( i ).get() );
		REQUIRE( t1->opts() == MD::TextWithoutFormat );
		REQUIRE( t1->text() == data.at( i ) );
	}
}

TEST_CASE( "015" )
{
	const auto doc = load_test( 15 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );
	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( p->items().size() == 2 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t1 = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t1->opts() == MD::TextWithoutFormat );
	REQUIRE( t1->text() == u8"\\" );

	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
	auto t2 = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );
	REQUIRE( t2->opts() == MD::ItalicText );
	REQUIRE( t2->text() == u8"emphasis" );
}

TEST_CASE( "016" )
{
	const auto doc = load_test( 16 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );
	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( p->items().size() == 3 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t1 = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t1->text() == u8"foo" );

	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::LineBreak );

	REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );
	auto t2 = static_cast< MD::Text< TRAIT >* > ( p->items().at( 2 ).get() );
	REQUIRE( t2->text() == u8"bar" );
}

TEST_CASE( "017" )
{
	const auto doc = load_test( 17 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( c->isInline() == true );
	REQUIRE( c->syntax().size() == 0 );
	REQUIRE( c->text() == u8"\\[\\`" );
}

TEST_CASE( "018" )
{
	const auto doc = load_test( 18 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );

	auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( c->isInline() == false );
	REQUIRE( c->syntax().size() == 0 );
	REQUIRE( c->text() == u8"\\[\\]" );
}

TEST_CASE( "019" )
{
	const auto doc = load_test( 19 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );

	auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( c->isInline() == false );
	REQUIRE( c->syntax().size() == 0 );
	REQUIRE( c->text() == u8"\\[\\]" );
}

TEST_CASE( "020" )
{
	MESSAGE( "This test is not strict to CommonMark 0.30." );

	const auto doc = load_test( 20 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->text() == u8"<http://example.com?find=*>" );
}

TEST_CASE( "021" )
{
	const auto doc = load_test( 21 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<a href=\"/bar\\/)\">" );
}

TEST_CASE( "022" )
{
	const auto doc = load_test( 22 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
	auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( l->opts() == MD::TextWithoutFormat );
	REQUIRE( l->img()->isEmpty() );
	REQUIRE( l->text() == u8"foo" );
	REQUIRE( l->url() == u8"/bar*" );
}

TEST_CASE( "023" )
{
	const auto doc = load_test( 23 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
	auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( l->opts() == MD::TextWithoutFormat );
	REQUIRE( l->img()->isEmpty() );
	REQUIRE( l->text() == u8"foo" );
	REQUIRE( doc->labeledLinks().size() == 1 );
	const auto lit = doc->labeledLinks().find( l->url() );
	REQUIRE( lit != doc->labeledLinks().cend() );
	REQUIRE( lit->second->url() == u8"/bar*" );
}

TEST_CASE( "024" )
{
	const auto doc = load_test( 24 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );

	auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( c->isInline() == false );
	REQUIRE( c->syntax() == u8"foo+bar" );
	REQUIRE( c->text() == u8"foo" );
}
