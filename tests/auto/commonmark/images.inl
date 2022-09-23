
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

// 6.4 Images

TEST_CASE( "571" )
{
	const auto doc = load_test( 571 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Image );
	auto i = static_cast< MD::Image* > ( p->items().at( 0 ).data() );
	REQUIRE( !i->isEmpty() );
	REQUIRE( i->text() == QStringLiteral( "foo" ) );
	REQUIRE( i->url() == QStringLiteral( "/url" ) );

	REQUIRE( !i->p().isNull() );
	REQUIRE( i->p()->items().size() == 1 );
	REQUIRE( i->p()->items().at( 0 )->type() == MD::ItemType::Text );
	auto t  = static_cast< MD::Text* > ( i->p()->items().at( 0 ).data() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == QStringLiteral( "foo" ) );
}

TEST_CASE( "572" )
{
	const auto doc = load_test( 572 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Image );
	auto i = static_cast< MD::Image* > ( p->items().at( 0 ).data() );
	REQUIRE( !i->isEmpty() );
	REQUIRE( i->text() == QStringLiteral( "foo *bar*" ) );
	REQUIRE( i->url() == QStringLiteral( "train.jpg" ) );
}

TEST_CASE( "573" )
{
	const auto doc = load_test( 573 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Image );
	auto i = static_cast< MD::Image* > ( p->items().at( 0 ).data() );
	REQUIRE( !i->isEmpty() );
	REQUIRE( i->text() == QStringLiteral( "foo ![bar](/url)" ) );
	REQUIRE( i->url() == QStringLiteral( "/url2" ) );
}

TEST_CASE( "574" )
{
	const auto doc = load_test( 574 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Image );
	auto i = static_cast< MD::Image* > ( p->items().at( 0 ).data() );
	REQUIRE( !i->isEmpty() );
	REQUIRE( i->text() == QStringLiteral( "foo [bar](/url)" ) );
	REQUIRE( i->url() == QStringLiteral( "/url2" ) );
}

TEST_CASE( "575" )
{
	const auto doc = load_test( 575 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Image );
	auto i = static_cast< MD::Image* > ( p->items().at( 0 ).data() );
	REQUIRE( !i->isEmpty() );
	REQUIRE( i->text() == QStringLiteral( "foo *bar*" ) );
	REQUIRE( i->url() == QStringLiteral( "train.jpg" ) );
}

TEST_CASE( "576" )
{
	const auto doc = load_test( 576 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Image );
	auto i = static_cast< MD::Image* > ( p->items().at( 0 ).data() );
	REQUIRE( !i->isEmpty() );
	REQUIRE( i->text() == QStringLiteral( "foo *bar*" ) );
	REQUIRE( i->url() == QStringLiteral( "train.jpg" ) );
}

TEST_CASE( "577" )
{
	const auto doc = load_test( 577 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Image );
	auto i = static_cast< MD::Image* > ( p->items().at( 0 ).data() );
	REQUIRE( !i->isEmpty() );
	REQUIRE( i->text() == QStringLiteral( "foo" ) );
	REQUIRE( i->url() == QStringLiteral( "train.jpg" ) );
}

TEST_CASE( "578" )
{
	const auto doc = load_test( 578 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 2 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == QStringLiteral( "My" ) );

	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Image );
	auto i = static_cast< MD::Image* > ( p->items().at( 1 ).data() );
	REQUIRE( !i->isEmpty() );
	REQUIRE( i->text() == QStringLiteral( "foo bar" ) );
	REQUIRE( i->url() == QStringLiteral( "/path/to/train.jpg" ) );
}

TEST_CASE( "579" )
{
	const auto doc = load_test( 579 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Image );
	auto i = static_cast< MD::Image* > ( p->items().at( 0 ).data() );
	REQUIRE( !i->isEmpty() );
	REQUIRE( i->text() == QStringLiteral( "foo" ) );
	REQUIRE( i->url() == QStringLiteral( "url" ) );
}

TEST_CASE( "580" )
{
	const auto doc = load_test( 580 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Image );
	auto i = static_cast< MD::Image* > ( p->items().at( 0 ).data() );
	REQUIRE( !i->isEmpty() );
	REQUIRE( i->text().isEmpty() );
	REQUIRE( i->url() == QStringLiteral( "/url" ) );
}

TEST_CASE( "581" )
{
	const auto doc = load_test( 581 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Image );
	auto i = static_cast< MD::Image* > ( p->items().at( 0 ).data() );
	REQUIRE( !i->isEmpty() );
	REQUIRE( i->text() == QStringLiteral( "foo" ) );
	REQUIRE( i->url() == QStringLiteral( "/url" ) );
}

TEST_CASE( "582" )
{
	const auto doc = load_test( 582 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Image );
	auto i = static_cast< MD::Image* > ( p->items().at( 0 ).data() );
	REQUIRE( !i->isEmpty() );
	REQUIRE( i->text() == QStringLiteral( "foo" ) );
	REQUIRE( i->url() == QStringLiteral( "/url" ) );
}

TEST_CASE( "583" )
{
	const auto doc = load_test( 583 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Image );
	auto i = static_cast< MD::Image* > ( p->items().at( 0 ).data() );
	REQUIRE( !i->isEmpty() );
	REQUIRE( i->text() == QStringLiteral( "foo" ) );
	REQUIRE( i->url() == QStringLiteral( "/url" ) );
}

TEST_CASE( "584" )
{
	const auto doc = load_test( 584 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Image );
	auto i = static_cast< MD::Image* > ( p->items().at( 0 ).data() );
	REQUIRE( !i->isEmpty() );
	REQUIRE( i->text() == QStringLiteral( "*foo* bar" ) );
	REQUIRE( i->url() == QStringLiteral( "/url" ) );
}

TEST_CASE( "585" )
{
	const auto doc = load_test( 585 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Image );
	auto i = static_cast< MD::Image* > ( p->items().at( 0 ).data() );
	REQUIRE( !i->isEmpty() );
	REQUIRE( i->text() == QStringLiteral( "Foo" ) );
	REQUIRE( i->url() == QStringLiteral( "/url" ) );
}

TEST_CASE( "586" )
{
	const auto doc = load_test( 586 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 2 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Image );
	auto i = static_cast< MD::Image* > ( p->items().at( 0 ).data() );
	REQUIRE( !i->isEmpty() );
	REQUIRE( i->text() == QStringLiteral( "foo" ) );
	REQUIRE( i->url() == QStringLiteral( "/url" ) );

	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text* > ( p->items().at( 1 ).data() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == QStringLiteral( "[]" ) );
}

TEST_CASE( "587" )
{
	const auto doc = load_test( 587 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Image );
	auto i = static_cast< MD::Image* > ( p->items().at( 0 ).data() );
	REQUIRE( !i->isEmpty() );
	REQUIRE( i->text() == QStringLiteral( "foo" ) );
	REQUIRE( i->url() == QStringLiteral( "/url" ) );
}

TEST_CASE( "588" )
{
	const auto doc = load_test( 588 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Image );
	auto i = static_cast< MD::Image* > ( p->items().at( 0 ).data() );
	REQUIRE( !i->isEmpty() );
	REQUIRE( i->text() == QStringLiteral( "*foo* bar" ) );
	REQUIRE( i->url() == QStringLiteral( "/url" ) );
}

TEST_CASE( "589" )
{
	const auto doc = load_test( 589 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "![[foo]]" ) );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph* > ( doc->items().at( 2 ).data() );
		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "[[foo]]: /url \"title\"" ) );
	}
}

TEST_CASE( "590" )
{
	const auto doc = load_test( 590 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Image );
	auto i = static_cast< MD::Image* > ( p->items().at( 0 ).data() );
	REQUIRE( !i->isEmpty() );
	REQUIRE( i->text() == QStringLiteral( "Foo" ) );
	REQUIRE( i->url() == QStringLiteral( "/url" ) );
}

TEST_CASE( "591" )
{
	const auto doc = load_test( 591 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == QStringLiteral( "![foo]" ) );
}

TEST_CASE( "592" )
{
	const auto doc = load_test( 592 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 2 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == QStringLiteral( "!" ) );

	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Link );
	auto l = static_cast< MD::Link* > ( p->items().at( 1 ).data() );
	REQUIRE( l->opts() == MD::TextWithoutFormat );
	REQUIRE( l->text() == QStringLiteral( "foo" ) );
	REQUIRE( doc->labeledLinks().contains( l->url() ) );
	REQUIRE( doc->labeledLinks()[ l->url() ]->url() == QStringLiteral( "/url" ) );
}
