
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

// 6.3 Links

TEST_CASE( "526" )
{
	const auto doc = load_test( 526 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
	auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).data() );
	REQUIRE( l->img()->isEmpty() );
	REQUIRE( l->opts() == MD::TextWithoutFormat );
	REQUIRE( l->text() == QStringLiteral( "foo" ) );
	REQUIRE( doc->labeledLinks().contains( l->url() ) );
	REQUIRE( doc->labeledLinks()[ l->url() ]->url() == QStringLiteral( "/url" ) );
}

TEST_CASE( "527" )
{
	const auto doc = load_test( 527 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
	auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).data() );
	REQUIRE( l->img()->isEmpty() );
	REQUIRE( l->opts() == MD::TextWithoutFormat );
	REQUIRE( l->text() == QStringLiteral( "link [foo [bar]]" ) );
	REQUIRE( doc->labeledLinks().contains( l->url() ) );
	REQUIRE( doc->labeledLinks()[ l->url() ]->url() == QStringLiteral( "/uri" ) );
}

TEST_CASE( "528" )
{
	const auto doc = load_test( 528 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
	auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).data() );
	REQUIRE( l->img()->isEmpty() );
	REQUIRE( l->opts() == MD::TextWithoutFormat );
	REQUIRE( l->text() == QStringLiteral( "link [bar" ) );
	REQUIRE( doc->labeledLinks().contains( l->url() ) );
	REQUIRE( doc->labeledLinks()[ l->url() ]->url() == QStringLiteral( "/uri" ) );
}

TEST_CASE( "529" )
{
	const auto doc = load_test( 529 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
	auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).data() );
	REQUIRE( l->img()->isEmpty() );
	REQUIRE( l->opts() == MD::TextWithoutFormat );
	REQUIRE( l->text() == QStringLiteral( "link *foo **bar** `#`*" ) );
	REQUIRE( doc->labeledLinks().contains( l->url() ) );
	REQUIRE( doc->labeledLinks()[ l->url() ]->url() == QStringLiteral( "/uri" ) );
}

TEST_CASE( "530" )
{
	const auto doc = load_test( 530 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
	auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).data() );
	REQUIRE( l->opts() == MD::TextWithoutFormat );
	REQUIRE( l->text() == QStringLiteral( "![moon](moon.jpg)" ) );
	REQUIRE( !l->img()->isEmpty() );
	REQUIRE( l->img()->text() == QStringLiteral( "moon" ) );
	REQUIRE( l->img()->url() == QStringLiteral( "moon.jpg" ) );
	REQUIRE( doc->labeledLinks().contains( l->url() ) );
	REQUIRE( doc->labeledLinks()[ l->url() ]->url() == QStringLiteral( "/uri" ) );
}

TEST_CASE( "531" )
{
	const auto doc = load_test( 531 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 4 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "[foo" ) );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 1 ).data() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == QStringLiteral( "bar" ) );
		REQUIRE( l->url() == QStringLiteral( "/uri" ) );
	}

	{
		REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 2 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "]" ) );
	}

	{
		REQUIRE( p->items().at( 3 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 3 ).data() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == QStringLiteral( "ref" ) );
		REQUIRE( doc->labeledLinks().contains( l->url() ) );
		REQUIRE( doc->labeledLinks()[ l->url() ]->url() == QStringLiteral( "/uri" ) );
	}
}

TEST_CASE( "532" )
{
	const auto doc = load_test( 532 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 5 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "[foo" ) );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).data() );
		REQUIRE( t->opts() == MD::ItalicText );
		REQUIRE( t->text() == QStringLiteral( "bar" ) );
	}

	{
		REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 2 ).data() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::ItalicText );
		REQUIRE( l->text() == QStringLiteral( "baz" ) );
		REQUIRE( doc->labeledLinks().contains( l->url() ) );
		REQUIRE( doc->labeledLinks()[ l->url() ]->url() == QStringLiteral( "/uri" ) );
	}

	{
		REQUIRE( p->items().at( 3 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 3 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "]" ) );
	}

	{
		REQUIRE( p->items().at( 4 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 4 ).data() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == QStringLiteral( "ref" ) );
		REQUIRE( doc->labeledLinks().contains( l->url() ) );
		REQUIRE( doc->labeledLinks()[ l->url() ]->url() == QStringLiteral( "/uri" ) );
	}
}

TEST_CASE( "533" )
{
	const auto doc = load_test( 533 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 2 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "*" ) );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 1 ).data() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == QStringLiteral( "foo*" ) );
		REQUIRE( doc->labeledLinks().contains( l->url() ) );
		REQUIRE( doc->labeledLinks()[ l->url() ]->url() == QStringLiteral( "/uri" ) );
	}
}

TEST_CASE( "534" )
{
	const auto doc = load_test( 534 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 2 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == QStringLiteral( "foo *bar" ) );
		REQUIRE( doc->labeledLinks().contains( l->url() ) );
		REQUIRE( doc->labeledLinks()[ l->url() ]->url() == QStringLiteral( "/uri" ) );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "*" ) );
	}
}

TEST_CASE( "535" )
{
	const auto doc = load_test( 535 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 2 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == QStringLiteral( "[foo" ) );

	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 1 ).data() );
	REQUIRE( h->text() == QStringLiteral( "<bar attr=\"][ref]\">" ) );
}

TEST_CASE( "536" )
{
	const auto doc = load_test( 536 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 2 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "[foo" ) );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code< TRAIT >* > ( p->items().at( 1 ).data() );
		REQUIRE( c->text() == QStringLiteral( "][ref]" ) );
	}
}

TEST_CASE( "537" )
{
	const auto doc = load_test( 537 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 2 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "[foo" ) );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 1 ).data() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text().isEmpty() );
		REQUIRE( l->url() == QStringLiteral( "http://example.com/?search=][ref]" ) );
	}
}

TEST_CASE( "538" )
{
	const auto doc = load_test( 538 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
	auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).data() );
	REQUIRE( l->img()->isEmpty() );
	REQUIRE( l->opts() == MD::TextWithoutFormat );
	REQUIRE( l->text() == QStringLiteral( "foo" ) );
	REQUIRE( doc->labeledLinks().contains( l->url() ) );
	REQUIRE( doc->labeledLinks()[ l->url() ]->url() == QStringLiteral( "/url" ) );
}

TEST_CASE( "539" )
{
	const auto doc = load_test( 539 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
	auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).data() );
	REQUIRE( l->img()->isEmpty() );
	REQUIRE( l->opts() == MD::TextWithoutFormat );
	REQUIRE( l->text() == QStringLiteral( u"ẞ" ) );
	REQUIRE( doc->labeledLinks().contains( l->url() ) );
	REQUIRE( doc->labeledLinks()[ l->url() ]->url() == QStringLiteral( "/url" ) );
}

TEST_CASE( "540" )
{
	const auto doc = load_test( 540 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
	auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).data() );
	REQUIRE( l->img()->isEmpty() );
	REQUIRE( l->opts() == MD::TextWithoutFormat );
	REQUIRE( l->text() == QStringLiteral( "Baz" ) );
	REQUIRE( doc->labeledLinks().contains( l->url() ) );
	REQUIRE( doc->labeledLinks()[ l->url() ]->url() == QStringLiteral( "/url" ) );
}

TEST_CASE( "541" )
{
	const auto doc = load_test( 541 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 2 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "[foo]" ) );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 1 ).data() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == QStringLiteral( "bar" ) );
		REQUIRE( doc->labeledLinks().contains( l->url() ) );
		REQUIRE( doc->labeledLinks()[ l->url() ]->url() == QStringLiteral( "/url" ) );
	}
}

TEST_CASE( "542" )
{
	const auto doc = load_test( 542 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 2 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "[foo]" ) );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 1 ).data() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == QStringLiteral( "bar" ) );
		REQUIRE( doc->labeledLinks().contains( l->url() ) );
		REQUIRE( doc->labeledLinks()[ l->url() ]->url() == QStringLiteral( "/url" ) );
	}
}

TEST_CASE( "543" )
{
	const auto doc = load_test( 543 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == QStringLiteral( "bar" ) );
		REQUIRE( doc->labeledLinks().contains( l->url() ) );
		REQUIRE( doc->labeledLinks()[ l->url() ]->url() == QStringLiteral( "/url1" ) );
	}
}

TEST_CASE( "544" )
{
	const auto doc = load_test( 544 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "[bar][foo!]" ) );
	}
}

TEST_CASE( "545" )
{
	const auto doc = load_test( 545 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).data() );
		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "[foo][ref[]" ) );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).data() );
		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "[ref[]: /uri" ) );
	}
}

TEST_CASE( "546" )
{
	const auto doc = load_test( 546 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).data() );
		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "[foo][ref[bar]]" ) );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).data() );
		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "[ref[bar]]: /uri" ) );
	}
}

TEST_CASE( "547" )
{
	const auto doc = load_test( 547 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).data() );
		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "[[[foo]]]" ) );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).data() );
		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "[[[foo]]]: /url" ) );
	}
}

TEST_CASE( "548" )
{
	const auto doc = load_test( 548 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == QStringLiteral( "foo" ) );
		REQUIRE( doc->labeledLinks().contains( l->url() ) );
		REQUIRE( doc->labeledLinks()[ l->url() ]->url() == QStringLiteral( "/uri" ) );
	}
}

TEST_CASE( "549" )
{
	const auto doc = load_test( 549 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == QStringLiteral( "bar\\" ) );
		REQUIRE( doc->labeledLinks().contains( l->url() ) );
		REQUIRE( doc->labeledLinks()[ l->url() ]->url() == QStringLiteral( "/uri" ) );
	}
}

TEST_CASE( "550" )
{
	const auto doc = load_test( 550 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).data() );
		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "[]" ) );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).data() );
		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "[]: /uri" ) );
	}
}

TEST_CASE( "551" )
{
	const auto doc = load_test( 551 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).data() );
		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "[ ]" ) );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).data() );
		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "[ ]: /uri" ) );
	}
}

TEST_CASE( "552" )
{
	const auto doc = load_test( 552 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == QStringLiteral( "foo" ) );
		REQUIRE( doc->labeledLinks().contains( l->url() ) );
		REQUIRE( doc->labeledLinks()[ l->url() ]->url() == QStringLiteral( "/url" ) );
	}
}

TEST_CASE( "553" )
{
	const auto doc = load_test( 553 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == QStringLiteral( "*foo* bar" ) );
		REQUIRE( doc->labeledLinks().contains( l->url() ) );
		REQUIRE( doc->labeledLinks()[ l->url() ]->url() == QStringLiteral( "/url" ) );
	}
}

TEST_CASE( "554" )
{
	const auto doc = load_test( 554 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == QStringLiteral( "Foo" ) );
		REQUIRE( doc->labeledLinks().contains( l->url() ) );
		REQUIRE( doc->labeledLinks()[ l->url() ]->url() == QStringLiteral( "/url" ) );
	}
}

TEST_CASE( "555" )
{
	const auto doc = load_test( 555 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 2 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == QStringLiteral( "foo" ) );
		REQUIRE( doc->labeledLinks().contains( l->url() ) );
		REQUIRE( doc->labeledLinks()[ l->url() ]->url() == QStringLiteral( "/url" ) );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "[]" ) );
	}
}

TEST_CASE( "556" )
{
	const auto doc = load_test( 556 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == QStringLiteral( "foo" ) );
		REQUIRE( doc->labeledLinks().contains( l->url() ) );
		REQUIRE( doc->labeledLinks()[ l->url() ]->url() == QStringLiteral( "/url" ) );
	}
}

TEST_CASE( "557" )
{
	const auto doc = load_test( 557 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == QStringLiteral( "*foo* bar" ) );
		REQUIRE( doc->labeledLinks().contains( l->url() ) );
		REQUIRE( doc->labeledLinks()[ l->url() ]->url() == QStringLiteral( "/url" ) );
	}
}

TEST_CASE( "558" )
{
	const auto doc = load_test( 558 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 3 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "[" ) );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 1 ).data() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == QStringLiteral( "*foo* bar" ) );
		REQUIRE( doc->labeledLinks().contains( l->url() ) );
		REQUIRE( doc->labeledLinks()[ l->url() ]->url() == QStringLiteral( "/url" ) );
	}

	{
		REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 2 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "]" ) );
	}
}

TEST_CASE( "559" )
{
	const auto doc = load_test( 559 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 2 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "[[bar" ) );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 1 ).data() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == QStringLiteral( "foo" ) );
		REQUIRE( doc->labeledLinks().contains( l->url() ) );
		REQUIRE( doc->labeledLinks()[ l->url() ]->url() == QStringLiteral( "/url" ) );
	}
}

TEST_CASE( "560" )
{
	const auto doc = load_test( 560 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == QStringLiteral( "Foo" ) );
		REQUIRE( doc->labeledLinks().contains( l->url() ) );
		REQUIRE( doc->labeledLinks()[ l->url() ]->url() == QStringLiteral( "/url" ) );
	}
}

TEST_CASE( "561" )
{
	const auto doc = load_test( 561 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 2 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == QStringLiteral( "foo" ) );
		REQUIRE( doc->labeledLinks().contains( l->url() ) );
		REQUIRE( doc->labeledLinks()[ l->url() ]->url() == QStringLiteral( "/url" ) );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "bar" ) );
	}
}

TEST_CASE( "562" )
{
	const auto doc = load_test( 562 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "[foo]" ) );
	}
}

TEST_CASE( "563" )
{
	const auto doc = load_test( 563 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 2 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "*" ) );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 1 ).data() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == QStringLiteral( "foo*" ) );
		REQUIRE( doc->labeledLinks().contains( l->url() ) );
		REQUIRE( doc->labeledLinks()[ l->url() ]->url() == QStringLiteral( "/url" ) );
	}
}

TEST_CASE( "564" )
{
	const auto doc = load_test( 564 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == QStringLiteral( "foo" ) );
		REQUIRE( doc->labeledLinks().contains( l->url() ) );
		REQUIRE( doc->labeledLinks()[ l->url() ]->url() == QStringLiteral( "/url2" ) );
	}
}

TEST_CASE( "565" )
{
	const auto doc = load_test( 565 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == QStringLiteral( "foo" ) );
		REQUIRE( doc->labeledLinks().contains( l->url() ) );
		REQUIRE( doc->labeledLinks()[ l->url() ]->url() == QStringLiteral( "/url1" ) );
	}
}

TEST_CASE( "566" )
{
	const auto doc = load_test( 566 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == QStringLiteral( "foo" ) );
		REQUIRE( l->url().isEmpty() );
	}
}

TEST_CASE( "567" )
{
	const auto doc = load_test( 567 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 2 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == QStringLiteral( "foo" ) );
		REQUIRE( doc->labeledLinks().contains( l->url() ) );
		REQUIRE( doc->labeledLinks()[ l->url() ]->url() == QStringLiteral( "/url1" ) );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "(not a link)" ) );
	}
}

TEST_CASE( "568" )
{
	const auto doc = load_test( 568 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 2 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "[foo]" ) );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 1 ).data() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == QStringLiteral( "bar" ) );
		REQUIRE( doc->labeledLinks().contains( l->url() ) );
		REQUIRE( doc->labeledLinks()[ l->url() ]->url() == QStringLiteral( "/url" ) );
	}
}

TEST_CASE( "569" )
{
	const auto doc = load_test( 569 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 2 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == QStringLiteral( "foo" ) );
		REQUIRE( doc->labeledLinks().contains( l->url() ) );
		REQUIRE( doc->labeledLinks()[ l->url() ]->url() == QStringLiteral( "/url2" ) );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 1 ).data() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == QStringLiteral( "baz" ) );
		REQUIRE( doc->labeledLinks().contains( l->url() ) );
		REQUIRE( doc->labeledLinks()[ l->url() ]->url() == QStringLiteral( "/url1" ) );
	}
}

TEST_CASE( "570" )
{
	const auto doc = load_test( 570 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 2 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "[foo]" ) );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 1 ).data() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == QStringLiteral( "bar" ) );
		REQUIRE( doc->labeledLinks().contains( l->url() ) );
		REQUIRE( doc->labeledLinks()[ l->url() ]->url() == QStringLiteral( "/url1" ) );
	}
}
