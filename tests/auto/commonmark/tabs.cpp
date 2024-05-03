
/*
	SPDX-FileCopyrightText: 2022-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

// doctest include.
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#ifndef MD4QT_QT_SUPPORT
#include <filesystem>
#endif

// 2.2 Tabs

TEST_CASE( "001" )
{
	MESSAGE( "This test is not strict to CommonMark 0.30." );

	const auto doc = load_test( 1 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );
	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );

	auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( c->isInline() == false );
	REQUIRE( c->syntax().isEmpty() );
	REQUIRE( c->text() == u8"foo baz     bim" );
}

TEST_CASE( "002" )
{
	MESSAGE( "This test is not strict to CommonMark 0.30." );

	const auto doc = load_test( 2 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );
	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );

	auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( c->isInline() == false );
	REQUIRE( c->syntax().isEmpty() );
	REQUIRE( c->text() == u8"foo baz     bim" );
}

TEST_CASE( "003" )
{
	MESSAGE( "This test is not strict to CommonMark 0.30." );

	const auto doc = load_test( 3 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );
	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );

	auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( c->isInline() == false );
	REQUIRE( c->syntax().isEmpty() );
	REQUIRE( c->text() == TRAIT::String( (const char *) u8"a   a\nὐ   a" ) );
}

TEST_CASE( "004" )
{
	const auto doc = load_test( 4 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );
	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->items().size() == 1 );

	auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
	REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );
	REQUIRE( li->items().size() == 2 );

	REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
	auto p1 = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
	REQUIRE( p1->items().size() == 1 );
	REQUIRE( p1->items().at( 0 )->type() == MD::ItemType::Text );
	auto t1 = static_cast< MD::Text< TRAIT >* > ( p1->items().at( 0 ).get() );
	REQUIRE( t1->opts() == MD::TextWithoutFormat );
	REQUIRE( t1->text() == u8"foo" );

	REQUIRE( li->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p2 = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 1 ).get() );
	REQUIRE( p2->items().size() == 1 );
	REQUIRE( p2->items().at( 0 )->type() == MD::ItemType::Text );
	auto t2 = static_cast< MD::Text< TRAIT >* > ( p2->items().at( 0 ).get() );
	REQUIRE( t2->opts() == MD::TextWithoutFormat );
	REQUIRE( t2->text() == u8"bar" );
}

TEST_CASE( "005" )
{
	const auto doc = load_test( 5 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );
	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->items().size() == 1 );

	auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
	REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );
	REQUIRE( li->items().size() == 2 );

	REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
	auto p1 = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
	REQUIRE( p1->items().size() == 1 );
	REQUIRE( p1->items().at( 0 )->type() == MD::ItemType::Text );
	auto t1 = static_cast< MD::Text< TRAIT >* > ( p1->items().at( 0 ).get() );
	REQUIRE( t1->opts() == MD::TextWithoutFormat );
	REQUIRE( t1->text() == u8"foo" );

	REQUIRE( li->items().at( 1 )->type() == MD::ItemType::Code );
	auto c2 = static_cast< MD::Code< TRAIT >* > ( li->items().at( 1 ).get() );
	REQUIRE( c2->isInline() == false );
	REQUIRE( c2->syntax().isEmpty() );
	REQUIRE( c2->text() == u8"  bar" );
}

TEST_CASE( "006" )
{
	const auto doc = load_test( 6 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );
	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );

	auto q = static_cast< MD::Blockquote< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( q->items().size() == 1 );

	REQUIRE( q->items().at( 0 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code< TRAIT >* > ( q->items().at( 0 ).get() );
	REQUIRE( c->isInline() == false );
	REQUIRE( c->syntax().isEmpty() );
	REQUIRE( c->text() == u8"  foo" );
}

TEST_CASE( "007" )
{
	const auto doc = load_test( 7 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );
	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->items().size() == 1 );

	auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
	REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );
	REQUIRE( li->items().size() == 1 );

	REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code< TRAIT >* > ( li->items().at( 0 ).get() );
	REQUIRE( c->isInline() == false );
	REQUIRE( c->syntax().isEmpty() );
	REQUIRE( c->text() == u8"  foo" );
}

TEST_CASE( "008" )
{
	const auto doc = load_test( 8 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );
	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );

	auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( c->isInline() == false );
	REQUIRE( c->syntax().isEmpty() );
	REQUIRE( c->text() == u8"foo\nbar" );
}

TEST_CASE( "009" )
{
	const auto doc = load_test( 9 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );
	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	auto l1 = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l1->items().size() == 1 );
	REQUIRE( l1->items().at( 0 )->type() == MD::ItemType::ListItem );
	auto li1 = static_cast< MD::ListItem< TRAIT >* > ( l1->items().at( 0 ).get() );
	REQUIRE( li1->items().size() == 2 );
	REQUIRE( li1->items().at( 0 )->type() == MD::ItemType::Paragraph );
	auto p1 = static_cast< MD::Paragraph< TRAIT >* > ( li1->items().at( 0 ).get() );
	REQUIRE( p1->items().size() == 1 );
	REQUIRE( p1->items().at( 0 )->type() == MD::ItemType::Text );
	auto t1 = static_cast< MD::Text< TRAIT >* > ( p1->items().at( 0 ).get() );
	REQUIRE( t1->opts() == MD::TextWithoutFormat );
	REQUIRE( t1->text() == u8"foo" );

	REQUIRE( li1->items().at( 1 )->type() == MD::ItemType::List );
	auto l2 = static_cast< MD::List< TRAIT >* > ( li1->items().at( 1 ).get() );
	REQUIRE( l2->items().size() == 1 );
	REQUIRE( l2->items().at( 0 )->type() == MD::ItemType::ListItem );
	auto li2 = static_cast< MD::ListItem< TRAIT >* > ( l2->items().at( 0 ).get() );
	REQUIRE( li2->items().size() == 2 );
	REQUIRE( li2->items().at( 0 )->type() == MD::ItemType::Paragraph );
	auto p2 = static_cast< MD::Paragraph< TRAIT >* > ( li2->items().at( 0 ).get() );
	REQUIRE( p2->items().size() == 1 );
	REQUIRE( p2->items().at( 0 )->type() == MD::ItemType::Text );
	auto t2 = static_cast< MD::Text< TRAIT >* > ( p2->items().at( 0 ).get() );
	REQUIRE( t2->opts() == MD::TextWithoutFormat );
	REQUIRE( t2->text() == u8"bar" );

	REQUIRE( li2->items().at( 1 )->type() == MD::ItemType::List );
	auto l3 = static_cast< MD::List< TRAIT >* > ( li2->items().at( 1 ).get() );
	REQUIRE( l3->items().size() == 1 );
	REQUIRE( l3->items().at( 0 )->type() == MD::ItemType::ListItem );
	auto li3 = static_cast< MD::ListItem< TRAIT >* > ( l3->items().at( 0 ).get() );
	REQUIRE( li3->items().size() == 1 );
	REQUIRE( li3->items().at( 0 )->type() == MD::ItemType::Paragraph );
	auto p3 = static_cast< MD::Paragraph< TRAIT >* > ( li3->items().at( 0 ).get() );
	REQUIRE( p3->items().size() == 1 );
	REQUIRE( p3->items().at( 0 )->type() == MD::ItemType::Text );
	auto t3 = static_cast< MD::Text< TRAIT >* > ( p3->items().at( 0 ).get() );
	REQUIRE( t3->opts() == MD::TextWithoutFormat );
	REQUIRE( t3->text() == u8"baz" );
}

TEST_CASE( "010" )
{
	const auto doc = load_test( 10 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );
	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Heading );

	auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( h->isLabeled() );
	typename TRAIT::String fn = u8"/" +
#ifdef MD4QT_QT_SUPPORT
			QDir().absolutePath()
#else
			std::filesystem::canonical( std::filesystem::current_path() ).u8string()
#endif
			+ u8"/tests/commonmark/0.30/010.md";

#ifndef MD4QT_QT_SUPPORT
	std::string tmp;
	fn.toUTF8String( tmp );
	std::replace( tmp.begin(), tmp.end(), '\\', '/' );
	fn = icu::UnicodeString::fromUTF8( tmp );
#endif

	REQUIRE( h->text().get() );
	auto p = h->text().get();
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( h->label() == u8"#" + t->text().toLower() + fn );
	REQUIRE( h->level() == 1 );
	REQUIRE( t->text() == u8"Foo" );
}

TEST_CASE( "011" )
{
	const auto doc = load_test( 11 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::HorizontalLine );
}
