
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

// 6 Inlines

TEST_CASE( "327" )
{
	const auto doc = load_test( 327 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 2 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code* > ( p->items().at( 0 ).data() );
	REQUIRE( c->isInlined() );
	REQUIRE( c->text() == QStringLiteral( "hi" ) );
	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text* > ( p->items().at( 1 ).data() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == QStringLiteral( "lo`" ) );
}

// 6.1 Code spans

TEST_CASE( "328" )
{
	const auto doc = load_test( 328 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code* > ( p->items().at( 0 ).data() );
	REQUIRE( c->isInlined() );
	REQUIRE( c->text() == QStringLiteral( "foo" ) );
}

TEST_CASE( "329" )
{
	const auto doc = load_test( 329 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code* > ( p->items().at( 0 ).data() );
	REQUIRE( c->isInlined() );
	REQUIRE( c->text() == QStringLiteral( "foo ` bar" ) );
}

TEST_CASE( "330" )
{
	const auto doc = load_test( 330 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code* > ( p->items().at( 0 ).data() );
	REQUIRE( c->isInlined() );
	REQUIRE( c->text() == QStringLiteral( "``" ) );
}

TEST_CASE( "331" )
{
	const auto doc = load_test( 331 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code* > ( p->items().at( 0 ).data() );
	REQUIRE( c->isInlined() );
	REQUIRE( c->text() == QStringLiteral( " `` " ) );
}

TEST_CASE( "332" )
{
	const auto doc = load_test( 332 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code* > ( p->items().at( 0 ).data() );
	REQUIRE( c->isInlined() );
	REQUIRE( c->text() == QStringLiteral( " a" ) );
}

TEST_CASE( "333" )
{
	const auto doc = load_test( 333 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code* > ( p->items().at( 0 ).data() );
	REQUIRE( c->isInlined() );
	const QString s = QString( QChar( (char16_t) 160 ) ) +
		QChar( 'b' ) + QChar( QChar( (char16_t) 160 ) );
	REQUIRE( c->text() == s );
}

TEST_CASE( "334" )
{
	const auto doc = load_test( 334 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 2 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code* > ( p->items().at( 0 ).data() );
		REQUIRE( c->isInlined() );
		REQUIRE( c->text() == QString( QChar( (char16_t) 160 ) ) );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code* > ( p->items().at( 1 ).data() );
		REQUIRE( c->isInlined() );
		REQUIRE( c->text() == QStringLiteral( "  " ) );
	}
}

TEST_CASE( "335" )
{
	const auto doc = load_test( 335 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code* > ( p->items().at( 0 ).data() );
	REQUIRE( c->isInlined() );
	REQUIRE( c->text() == QStringLiteral( "foo bar   baz" ) );
}

TEST_CASE( "336" )
{
	const auto doc = load_test( 336 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code* > ( p->items().at( 0 ).data() );
	REQUIRE( c->isInlined() );
	REQUIRE( c->text() == QStringLiteral( "foo " ) );
}

TEST_CASE( "337" )
{
	const auto doc = load_test( 337 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code* > ( p->items().at( 0 ).data() );
	REQUIRE( c->isInlined() );
	REQUIRE( c->text() == QStringLiteral( "foo   bar  baz" ) );
}

TEST_CASE( "338" )
{
	const auto doc = load_test( 338 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 2 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code* > ( p->items().at( 0 ).data() );
	REQUIRE( c->isInlined() );
	REQUIRE( c->text() == QStringLiteral( "foo\\" ) );
	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text* > ( p->items().at( 1 ).data() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == QStringLiteral( "bar`" ) );
}

TEST_CASE( "339" )
{
	const auto doc = load_test( 339 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code* > ( p->items().at( 0 ).data() );
	REQUIRE( c->isInlined() );
	REQUIRE( c->text() == QStringLiteral( "foo`bar" ) );
}

TEST_CASE( "340" )
{
	const auto doc = load_test( 340 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code* > ( p->items().at( 0 ).data() );
	REQUIRE( c->isInlined() );
	REQUIRE( c->text() == QStringLiteral( "foo `` bar" ) );
}

TEST_CASE( "341" )
{
	const auto doc = load_test( 341 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 2 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == QStringLiteral( "*foo" ) );

	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code* > ( p->items().at( 1 ).data() );
	REQUIRE( c->isInlined() );
	REQUIRE( c->text() == QStringLiteral( "*" ) );
}

TEST_CASE( "342" )
{
	const auto doc = load_test( 342 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 3 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "[not a" ) );
	}

	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code* > ( p->items().at( 1 ).data() );
	REQUIRE( c->isInlined() );
	REQUIRE( c->text() == QStringLiteral( "link](/foo" ) );

	{
		REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text* > ( p->items().at( 2 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( ")" ) );
	}
}

TEST_CASE( "343" )
{
	const auto doc = load_test( 343 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 2 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code* > ( p->items().at( 0 ).data() );
	REQUIRE( c->isInlined() );
	REQUIRE( c->text() == QStringLiteral( "<a href=\"" ) );

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text* > ( p->items().at( 1 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "\">`" ) );
	}
}

TEST_CASE( "344" )
{
	const auto doc = load_test( 344 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 2 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml* > ( p->items().at( 0 ).data() );
	REQUIRE( h->text() == QStringLiteral( "<a href=\"`\">" ) );

	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text* > ( p->items().at( 1 ).data() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == QStringLiteral( "`" ) );
}

TEST_CASE( "345" )
{
	const auto doc = load_test( 345 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 2 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code* > ( p->items().at( 0 ).data() );
	REQUIRE( c->isInlined() );
	REQUIRE( c->text() == QStringLiteral( "<http://foo.bar." ) );

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text* > ( p->items().at( 1 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "baz>`" ) );
	}
}

TEST_CASE( "346" ) // Not strict to CommonMark as URL is not valid.
{
	MESSAGE( "This test is not strict to CommonMark 0.30." );

	MESSAGE( "Skip for now." );
}

TEST_CASE( "347" )
{
	const auto doc = load_test( 347 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == QStringLiteral( "```foo``" ) );
}

TEST_CASE( "348" )
{
	const auto doc = load_test( 348 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == QStringLiteral( "`foo" ) );
}

TEST_CASE( "349" )
{
	const auto doc = load_test( 349 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 2 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == QStringLiteral( "`foo" ) );

	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code* > ( p->items().at( 1 ).data() );
	REQUIRE( c->isInlined() );
	REQUIRE( c->text() == QStringLiteral( "bar" ) );
}
