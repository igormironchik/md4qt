
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2022 Igor Mironchik

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

#include "load.hpp"

#include <doctest/doctest.h>

// 2.5 Entity and numeric character references

TEST_CASE( "25" )
{
	const auto doc = load_test( 25 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == QString( QChar( 0x26 ) ) +
		MD::c_32 + QChar( 0xA9 ) +
		MD::c_32 + QChar( 0xC6 ) +
		MD::c_32 + QChar( 0x10E) +
		MD::c_32 + QChar( 0xBE ) +
		MD::c_32 + QChar( 0x210B ) +
		MD::c_32 + QChar( 0x2146 ) +
		MD::c_32 + QChar( 0x2232 ) +
		MD::c_32 + QChar( 0x2267 ) + QChar( 0x0338 ) );
}

TEST_CASE( "26" )
{
	const auto doc = load_test( 26 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == QString( QChar( 35 ) ) +
		MD::c_32 + QChar( 1234 ) +
		MD::c_32 + QChar( 992 ) +
		MD::c_32 + QChar( 0xFFFD ) );
}

TEST_CASE( "27" )
{
	const auto doc = load_test( 27 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == QString( QChar( 0x22 ) ) +
		MD::c_32 + QChar( 0xD06 ) +
		MD::c_32 + QChar( 0xCAB ) );
}

TEST_CASE( "28" )
{
	const auto doc = load_test( 28 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == QStringLiteral(
		"&nbsp &x; &#; &#x; &#87654321; &#abcdef0; &ThisIsNotDefined; &hi?;" ) );
}

TEST_CASE( "29" )
{
	const auto doc = load_test( 29 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == QStringLiteral( "&copy" ) );
}

TEST_CASE( "30" )
{
	const auto doc = load_test( 30 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == QStringLiteral( "&MadeUpEntity;" ) );
}

TEST_CASE( "31" )
{
	const auto doc = load_test( 31 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml* > ( doc->items().at( 1 ).data() );
	REQUIRE( h->text() == QStringLiteral( "<a href=\"&ouml;&ouml;.html\">" ) );
}

TEST_CASE( "32" )
{
	const auto doc = load_test( 32 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
	auto l = static_cast< MD::Link* > ( p->items().at( 0 ).data() );
	REQUIRE( l->opts() == MD::TextWithoutFormat );
	REQUIRE( l->text() == QStringLiteral( "foo" ) );
	REQUIRE( l->url() == QStringLiteral( "/f" ) + QChar( 0xF6 ) + QChar( 0xF6 ) );
}

TEST_CASE( "33" )
{
	const auto doc = load_test( 33 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
	auto l = static_cast< MD::Link* > ( p->items().at( 0 ).data() );
	REQUIRE( l->opts() == MD::TextWithoutFormat );
	REQUIRE( l->text() == QStringLiteral( "foo" ) );
	REQUIRE( doc->labeledLinks().contains( l->url() ) );
	REQUIRE( doc->labeledLinks() [ l->url() ]->url() ==
		QStringLiteral( "/f" ) + QChar( 0xF6 ) + QChar( 0xF6 ) );
}

TEST_CASE( "34" )
{
	const auto doc = load_test( 34 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code* > ( doc->items().at( 1 ).data() );
	REQUIRE( c->syntax() == QStringLiteral( "f" ) + QChar( 0xF6 ) + QChar( 0xF6 ) );
	REQUIRE( c->text() == QStringLiteral( "foo" ) );
}

TEST_CASE( "35" )
{
	const auto doc = load_test( 35 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code* > ( p->items().at( 0 ).data() );
	REQUIRE( c->text() == QStringLiteral( "f&ouml;&ouml;" ) );
}

TEST_CASE( "36" )
{
	const auto doc = load_test( 36 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code* > ( doc->items().at( 1 ).data() );
	REQUIRE( c->text() == QStringLiteral( "f&ouml;f&ouml;" ) );
}

TEST_CASE( "37" )
{
	const auto doc = load_test( 37 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 2 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "*foo*" ) );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text* > ( p->items().at( 1 ).data() );
		REQUIRE( t->opts() == MD::ItalicText );
		REQUIRE( t->text() == QStringLiteral( "foo" ) );
	}
}

TEST_CASE( "38" )
{
	const auto doc = load_test( 38 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "* foo" ) );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::List );
		auto l = static_cast< MD::List* > ( doc->items().at( 2 ).data() );
		REQUIRE( l->items().size() == 1 );
		auto li = static_cast< MD::ListItem* > ( l->items().first().data() );
		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->items().first()->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph* > ( li->items().at( 0 ).data() );
		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "foo" ) );
	}
}

TEST_CASE( "39" )
{
	const auto doc = load_test( 39 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == QStringLiteral( "foo bar" ) );
}

TEST_CASE( "40" )
{
	const auto doc = load_test( 40 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == QStringLiteral( "foo" ) );
	REQUIRE( t->isSpaceBefore() );
}

TEST_CASE( "41" )
{
	const auto doc = load_test( 41 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == QStringLiteral( "[a](url \"tit\")" ) );
}
