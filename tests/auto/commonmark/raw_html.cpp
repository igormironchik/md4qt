
/*
	SPDX-FileCopyrightText: 2022-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

// doctest include.
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

// 6.6 Raw HTML

TEST_CASE( "612" )
{
	const auto doc = load_test( 612 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( p->items().size() == 3 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::RawHtml );
		auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( h->text() == u8"<a>" );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::RawHtml );
		auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( h->text() == u8"<bab>" );
	}

	{
		REQUIRE( p->items().at( 2 )->type() == MD::ItemType::RawHtml );
		auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 2 ).get() );
		REQUIRE( h->text() == u8"<c2c>" );
	}
}

TEST_CASE( "613" )
{
	const auto doc = load_test( 613 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( p->items().size() == 2 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::RawHtml );
		auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( h->text() == u8"<a/>" );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::RawHtml );
		auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( h->text() == u8"<b2/>" );
	}
}

TEST_CASE( "614" )
{
	const auto doc = load_test( 614 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( p->items().size() == 2 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::RawHtml );
		auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( h->text() == u8"<a  />" );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::RawHtml );
		auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( h->text() == u8"<b2\ndata=\"foo\" >" );
	}
}

TEST_CASE( "615" )
{
	const auto doc = load_test( 615 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( h->text() == u8"<a foo=\"bar\" bam = 'baz <em>\"</em>'\n"
										  "_boolean zoop:33=zoop:33 />" );
}

TEST_CASE( "616" )
{
	const auto doc = load_test( 616 );

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
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::RawHtml );
		auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( h->text() == u8"<responsive-image src=\"foo.jpg\" />" );
	}
}

TEST_CASE( "617" )
{
	const auto doc = load_test( 617 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"<33> <__>" );
}

TEST_CASE( "618" )
{
	const auto doc = load_test( 618 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"<a h*#ref=\"hi\">" );
}

TEST_CASE( "619" )
{
	const auto doc = load_test( 619 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"<a href=\"hi'> <a href=hi'>" );
}

TEST_CASE( "620" )
{
	const auto doc = load_test( 620 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 4 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"< a><" );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"foo><bar/ >" );
	}

	{
		REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 2 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"<foo bar=baz" );
	}

	{
		REQUIRE( p->items().at( 3 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 3 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"bim!bop />" );
	}
}

TEST_CASE( "621" )
{
	const auto doc = load_test( 621 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"<a href='bar'title=title>" );
}

TEST_CASE( "622" )
{
	const auto doc = load_test( 622 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( p->items().size() == 2 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::RawHtml );
		auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( h->text() == u8"</a>" );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::RawHtml );
		auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( h->text() == u8"</foo >" );
	}
}

TEST_CASE( "623" )
{
	const auto doc = load_test( 623 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"</a href=\"foo\">" );
}

TEST_CASE( "624" )
{
	const auto doc = load_test( 624 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 2 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"foo" );

	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<!-- this is a\ncomment - with hyphen -->" );
}

TEST_CASE( "625" )
{
	MESSAGE( "Outdated. Changed in CommonMark 0.31.2" );
}

TEST_CASE( "626" )
{
	MESSAGE( "Outdated. Changed in CommonMark 0.31.2" );
}

TEST_CASE( "627" )
{
	const auto doc = load_test( 627 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 2 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"foo" );

	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<?php echo $a; ?>" );
}

TEST_CASE( "628" )
{
	const auto doc = load_test( 628 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 2 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"foo" );

	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<!ELEMENT br EMPTY>" );
}

TEST_CASE( "629" )
{
	const auto doc = load_test( 629 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 2 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"foo" );

	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<![CDATA[>&<]]>" );
}

TEST_CASE( "630" )
{
	const auto doc = load_test( 630 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 2 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"foo" );

	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<a href=\"&ouml;\">" );
}

TEST_CASE( "631" )
{
	const auto doc = load_test( 631 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 2 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"foo" );

	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<a href=\"\\*\">" );
}

TEST_CASE( "632" )
{
	const auto doc = load_test( 632 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"<a href=\"\"\">" );
}
