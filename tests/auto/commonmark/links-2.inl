
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
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
	auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( l->img()->isEmpty() );
	REQUIRE( l->opts() == MD::TextWithoutFormat );
	REQUIRE( l->text() == u8"foo" );
	const auto lit = doc->labeledLinks().find( l->url() );
	REQUIRE( lit != doc->labeledLinks().cend() );
#ifdef MD4QT_QT_SUPPORT
	REQUIRE( (*lit)->url() == u8"/url" );
#else
	REQUIRE( lit->second->url() == u8"/url" );
#endif
}

TEST_CASE( "527" )
{
	const auto doc = load_test( 527 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
	auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( l->img()->isEmpty() );
	REQUIRE( l->opts() == MD::TextWithoutFormat );
	REQUIRE( l->text() == u8"link [foo [bar]]" );
	const auto lit = doc->labeledLinks().find( l->url() );
	REQUIRE( lit != doc->labeledLinks().cend() );
#ifdef MD4QT_QT_SUPPORT
	REQUIRE( (*lit)->url() == u8"/uri" );
#else
	REQUIRE( lit->second->url() == u8"/uri" );
#endif
}

TEST_CASE( "528" )
{
	const auto doc = load_test( 528 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
	auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( l->img()->isEmpty() );
	REQUIRE( l->opts() == MD::TextWithoutFormat );
	REQUIRE( l->text() == u8"link [bar" );
	const auto lit = doc->labeledLinks().find( l->url() );
	REQUIRE( lit != doc->labeledLinks().cend() );
#ifdef MD4QT_QT_SUPPORT
	REQUIRE( (*lit)->url() == u8"/uri" );
#else
	REQUIRE( lit->second->url() == u8"/uri" );
#endif
}

TEST_CASE( "529" )
{
	const auto doc = load_test( 529 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
	auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( l->img()->isEmpty() );
	REQUIRE( l->opts() == MD::TextWithoutFormat );
	REQUIRE( l->text() == u8"link *foo **bar** `#`*" );
	const auto lit = doc->labeledLinks().find( l->url() );
	REQUIRE( lit != doc->labeledLinks().cend() );
#ifdef MD4QT_QT_SUPPORT
	REQUIRE( (*lit)->url() == u8"/uri" );
#else
	REQUIRE( lit->second->url() == u8"/uri" );
#endif
}

TEST_CASE( "530" )
{
	const auto doc = load_test( 530 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
	auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( l->opts() == MD::TextWithoutFormat );
	REQUIRE( l->text() == u8"![moon](moon.jpg)" );
	REQUIRE( !l->img()->isEmpty() );
	REQUIRE( l->img()->text() == u8"moon" );
	REQUIRE( l->img()->url() == u8"moon.jpg" );
	const auto lit = doc->labeledLinks().find( l->url() );
	REQUIRE( lit != doc->labeledLinks().cend() );
#ifdef MD4QT_QT_SUPPORT
	REQUIRE( (*lit)->url() == u8"/uri" );
#else
	REQUIRE( lit->second->url() == u8"/uri" );
#endif
}

TEST_CASE( "531" )
{
	const auto doc = load_test( 531 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 4 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"[foo" );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == u8"bar" );
		REQUIRE( l->url() == u8"/uri" );
	}

	{
		REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 2 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"]" );
	}

	{
		REQUIRE( p->items().at( 3 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 3 ).get() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == u8"ref" );
		const auto lit = doc->labeledLinks().find( l->url() );
		REQUIRE( lit != doc->labeledLinks().cend() );
#ifdef MD4QT_QT_SUPPORT
		REQUIRE( (*lit)->url() == u8"/uri" );
#else
		REQUIRE( lit->second->url() == u8"/uri" );
#endif
	}
}

TEST_CASE( "532" )
{
	const auto doc = load_test( 532 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 5 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"[foo" );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( t->opts() == MD::ItalicText );
		REQUIRE( t->text() == u8"bar" );
	}

	{
		REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 2 ).get() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::ItalicText );
		REQUIRE( l->text() == u8"baz" );
		const auto lit = doc->labeledLinks().find( l->url() );
		REQUIRE( lit != doc->labeledLinks().cend() );
#ifdef MD4QT_QT_SUPPORT
		REQUIRE( (*lit)->url() == u8"/uri" );
#else
		REQUIRE( lit->second->url() == u8"/uri" );
#endif
	}

	{
		REQUIRE( p->items().at( 3 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 3 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"]" );
	}

	{
		REQUIRE( p->items().at( 4 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 4 ).get() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == u8"ref" );
		const auto lit = doc->labeledLinks().find( l->url() );
		REQUIRE( lit != doc->labeledLinks().cend() );
#ifdef MD4QT_QT_SUPPORT
		REQUIRE( (*lit)->url() == u8"/uri" );
#else
		REQUIRE( lit->second->url() == u8"/uri" );
#endif
	}
}

TEST_CASE( "533" )
{
	const auto doc = load_test( 533 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 2 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"*" );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == u8"foo*" );
		const auto lit = doc->labeledLinks().find( l->url() );
		REQUIRE( lit != doc->labeledLinks().cend() );
#ifdef MD4QT_QT_SUPPORT
		REQUIRE( (*lit)->url() == u8"/uri" );
#else
		REQUIRE( lit->second->url() == u8"/uri" );
#endif
	}
}

TEST_CASE( "534" )
{
	const auto doc = load_test( 534 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 2 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == u8"foo *bar" );
		const auto lit = doc->labeledLinks().find( l->url() );
		REQUIRE( lit != doc->labeledLinks().cend() );
#ifdef MD4QT_QT_SUPPORT
		REQUIRE( (*lit)->url() == u8"/uri" );
#else
		REQUIRE( lit->second->url() == u8"/uri" );
#endif
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"*" );
	}
}

TEST_CASE( "535" )
{
	const auto doc = load_test( 535 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 2 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"[foo" );

	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<bar attr=\"][ref]\">" );
}

TEST_CASE( "536" )
{
	const auto doc = load_test( 536 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 2 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"[foo" );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( c->text() == u8"][ref]" );
	}
}

TEST_CASE( "537" )
{
	const auto doc = load_test( 537 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 2 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"[foo" );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text().isEmpty() );
		REQUIRE( l->url() == u8"http://example.com/?search=][ref]" );
	}
}

TEST_CASE( "538" )
{
	const auto doc = load_test( 538 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
	auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( l->img()->isEmpty() );
	REQUIRE( l->opts() == MD::TextWithoutFormat );
	REQUIRE( l->text() == u8"foo" );
	const auto lit = doc->labeledLinks().find( l->url() );
	REQUIRE( lit != doc->labeledLinks().cend() );
#ifdef MD4QT_QT_SUPPORT
	REQUIRE( (*lit)->url() == u8"/url" );
#else
	REQUIRE( lit->second->url() == u8"/url" );
#endif
}

TEST_CASE( "539" )
{
	const auto doc = load_test( 539 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
	auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( l->img()->isEmpty() );
	REQUIRE( l->opts() == MD::TextWithoutFormat );
	REQUIRE( l->text() == u8"ẞ" );
	const auto lit = doc->labeledLinks().find( l->url() );
	REQUIRE( lit != doc->labeledLinks().cend() );
#ifdef MD4QT_QT_SUPPORT
	REQUIRE( (*lit)->url() == u8"/url" );
#else
	REQUIRE( lit->second->url() == u8"/url" );
#endif
}

TEST_CASE( "540" )
{
	const auto doc = load_test( 540 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
	auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( l->img()->isEmpty() );
	REQUIRE( l->opts() == MD::TextWithoutFormat );
	REQUIRE( l->text() == u8"Baz" );
	const auto lit = doc->labeledLinks().find( l->url() );
	REQUIRE( lit != doc->labeledLinks().cend() );
#ifdef MD4QT_QT_SUPPORT
	REQUIRE( (*lit)->url() == u8"/url" );
#else
	REQUIRE( lit->second->url() == u8"/url" );
#endif
}

TEST_CASE( "541" )
{
	const auto doc = load_test( 541 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 2 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"[foo]" );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == u8"bar" );
		const auto lit = doc->labeledLinks().find( l->url() );
		REQUIRE( lit != doc->labeledLinks().cend() );
#ifdef MD4QT_QT_SUPPORT
		REQUIRE( (*lit)->url() == u8"/url" );
#else
		REQUIRE( lit->second->url() == u8"/url" );
#endif
	}
}

TEST_CASE( "542" )
{
	const auto doc = load_test( 542 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 2 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"[foo]" );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == u8"bar" );
		const auto lit = doc->labeledLinks().find( l->url() );
		REQUIRE( lit != doc->labeledLinks().cend() );
#ifdef MD4QT_QT_SUPPORT
		REQUIRE( (*lit)->url() == u8"/url" );
#else
		REQUIRE( lit->second->url() == u8"/url" );
#endif
	}
}

TEST_CASE( "543" )
{
	const auto doc = load_test( 543 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == u8"bar" );
		const auto lit = doc->labeledLinks().find( l->url() );
		REQUIRE( lit != doc->labeledLinks().cend() );
#ifdef MD4QT_QT_SUPPORT
		REQUIRE( (*lit)->url() == u8"/url1" );
#else
		REQUIRE( lit->second->url() == u8"/url1" );
#endif
	}
}

TEST_CASE( "544" )
{
	const auto doc = load_test( 544 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"[bar][foo!]" );
	}
}

TEST_CASE( "545" )
{
	const auto doc = load_test( 545 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"[foo][ref[]" );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"[ref[]: /uri" );
	}
}

TEST_CASE( "546" )
{
	const auto doc = load_test( 546 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"[foo][ref[bar]]" );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"[ref[bar]]: /uri" );
	}
}

TEST_CASE( "547" )
{
	const auto doc = load_test( 547 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"[[[foo]]]" );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"[[[foo]]]: /url" );
	}
}

TEST_CASE( "548" )
{
	const auto doc = load_test( 548 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == u8"foo" );
		const auto lit = doc->labeledLinks().find( l->url() );
		REQUIRE( lit != doc->labeledLinks().cend() );
#ifdef MD4QT_QT_SUPPORT
		REQUIRE( (*lit)->url() == u8"/uri" );
#else
		REQUIRE( lit->second->url() == u8"/uri" );
#endif
	}
}

TEST_CASE( "549" )
{
	const auto doc = load_test( 549 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == u8"bar\\" );
		const auto lit = doc->labeledLinks().find( l->url() );
		REQUIRE( lit != doc->labeledLinks().cend() );
#ifdef MD4QT_QT_SUPPORT
		REQUIRE( (*lit)->url() == u8"/uri" );
#else
		REQUIRE( lit->second->url() == u8"/uri" );
#endif
	}
}

TEST_CASE( "550" )
{
	const auto doc = load_test( 550 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"[]" );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"[]: /uri" );
	}
}

TEST_CASE( "551" )
{
	const auto doc = load_test( 551 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"[ ]" );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"[ ]: /uri" );
	}
}

TEST_CASE( "552" )
{
	const auto doc = load_test( 552 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == u8"foo" );
		const auto lit = doc->labeledLinks().find( l->url() );
		REQUIRE( lit != doc->labeledLinks().cend() );
#ifdef MD4QT_QT_SUPPORT
		REQUIRE( (*lit)->url() == u8"/url" );
#else
		REQUIRE( lit->second->url() == u8"/url" );
#endif
	}
}

TEST_CASE( "553" )
{
	const auto doc = load_test( 553 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == u8"*foo* bar" );
		const auto lit = doc->labeledLinks().find( l->url() );
		REQUIRE( lit != doc->labeledLinks().cend() );
#ifdef MD4QT_QT_SUPPORT
		REQUIRE( (*lit)->url() == u8"/url" );
#else
		REQUIRE( lit->second->url() == u8"/url" );
#endif
	}
}

TEST_CASE( "554" )
{
	const auto doc = load_test( 554 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == u8"Foo" );
		const auto lit = doc->labeledLinks().find( l->url() );
		REQUIRE( lit != doc->labeledLinks().cend() );
#ifdef MD4QT_QT_SUPPORT
		REQUIRE( (*lit)->url() == u8"/url" );
#else
		REQUIRE( lit->second->url() == u8"/url" );
#endif
	}
}

TEST_CASE( "555" )
{
	const auto doc = load_test( 555 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 2 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == u8"foo" );
		const auto lit = doc->labeledLinks().find( l->url() );
		REQUIRE( lit != doc->labeledLinks().cend() );
#ifdef MD4QT_QT_SUPPORT
		REQUIRE( (*lit)->url() == u8"/url" );
#else
		REQUIRE( lit->second->url() == u8"/url" );
#endif
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"[]" );
	}
}

TEST_CASE( "556" )
{
	const auto doc = load_test( 556 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == u8"foo" );
		const auto lit = doc->labeledLinks().find( l->url() );
		REQUIRE( lit != doc->labeledLinks().cend() );
#ifdef MD4QT_QT_SUPPORT
		REQUIRE( (*lit)->url() == u8"/url" );
#else
		REQUIRE( lit->second->url() == u8"/url" );
#endif
	}
}

TEST_CASE( "557" )
{
	const auto doc = load_test( 557 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == u8"*foo* bar" );
		const auto lit = doc->labeledLinks().find( l->url() );
		REQUIRE( lit != doc->labeledLinks().cend() );
#ifdef MD4QT_QT_SUPPORT
		REQUIRE( (*lit)->url() == u8"/url" );
#else
		REQUIRE( lit->second->url() == u8"/url" );
#endif
	}
}

TEST_CASE( "558" )
{
	const auto doc = load_test( 558 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 3 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"[" );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == u8"*foo* bar" );
		const auto lit = doc->labeledLinks().find( l->url() );
		REQUIRE( lit != doc->labeledLinks().cend() );
#ifdef MD4QT_QT_SUPPORT
		REQUIRE( (*lit)->url() == u8"/url" );
#else
		REQUIRE( lit->second->url() == u8"/url" );
#endif
	}

	{
		REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 2 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"]" );
	}
}

TEST_CASE( "559" )
{
	const auto doc = load_test( 559 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 2 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"[[bar" );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == u8"foo" );
		const auto lit = doc->labeledLinks().find( l->url() );
		REQUIRE( lit != doc->labeledLinks().cend() );
#ifdef MD4QT_QT_SUPPORT
		REQUIRE( (*lit)->url() == u8"/url" );
#else
		REQUIRE( lit->second->url() == u8"/url" );
#endif
	}
}

TEST_CASE( "560" )
{
	const auto doc = load_test( 560 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == u8"Foo" );
		const auto lit = doc->labeledLinks().find( l->url() );
		REQUIRE( lit != doc->labeledLinks().cend() );
#ifdef MD4QT_QT_SUPPORT
		REQUIRE( (*lit)->url() == u8"/url" );
#else
		REQUIRE( lit->second->url() == u8"/url" );
#endif
	}
}

TEST_CASE( "561" )
{
	const auto doc = load_test( 561 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 2 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == u8"foo" );
		const auto lit = doc->labeledLinks().find( l->url() );
		REQUIRE( lit != doc->labeledLinks().cend() );
#ifdef MD4QT_QT_SUPPORT
		REQUIRE( (*lit)->url() == u8"/url" );
#else
		REQUIRE( lit->second->url() == u8"/url" );
#endif
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"bar" );
	}
}

TEST_CASE( "562" )
{
	const auto doc = load_test( 562 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"[foo]" );
	}
}

TEST_CASE( "563" )
{
	const auto doc = load_test( 563 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 2 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"*" );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == u8"foo*" );
		const auto lit = doc->labeledLinks().find( l->url() );
		REQUIRE( lit != doc->labeledLinks().cend() );
#ifdef MD4QT_QT_SUPPORT
		REQUIRE( (*lit)->url() == u8"/url" );
#else
		REQUIRE( lit->second->url() == u8"/url" );
#endif
	}
}

TEST_CASE( "564" )
{
	const auto doc = load_test( 564 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == u8"foo" );
		const auto lit = doc->labeledLinks().find( l->url() );
		REQUIRE( lit != doc->labeledLinks().cend() );
#ifdef MD4QT_QT_SUPPORT
		REQUIRE( (*lit)->url() == u8"/url2" );
#else
		REQUIRE( lit->second->url() == u8"/url2" );
#endif
	}
}

TEST_CASE( "565" )
{
	const auto doc = load_test( 565 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == u8"foo" );
		const auto lit = doc->labeledLinks().find( l->url() );
		REQUIRE( lit != doc->labeledLinks().cend() );
#ifdef MD4QT_QT_SUPPORT
		REQUIRE( (*lit)->url() == u8"/url1" );
#else
		REQUIRE( lit->second->url() == u8"/url1" );
#endif
	}
}

TEST_CASE( "566" )
{
	const auto doc = load_test( 566 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == u8"foo" );
		REQUIRE( l->url().isEmpty() );
	}
}

TEST_CASE( "567" )
{
	const auto doc = load_test( 567 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 2 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == u8"foo" );
		const auto lit = doc->labeledLinks().find( l->url() );
		REQUIRE( lit != doc->labeledLinks().cend() );
#ifdef MD4QT_QT_SUPPORT
		REQUIRE( (*lit)->url() == u8"/url1" );
#else
		REQUIRE( lit->second->url() == u8"/url1" );
#endif
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"(not a link)" );
	}
}

TEST_CASE( "568" )
{
	const auto doc = load_test( 568 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 2 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"[foo]" );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == u8"bar" );
		const auto lit = doc->labeledLinks().find( l->url() );
		REQUIRE( lit != doc->labeledLinks().cend() );
#ifdef MD4QT_QT_SUPPORT
		REQUIRE( (*lit)->url() == u8"/url" );
#else
		REQUIRE( lit->second->url() == u8"/url" );
#endif
	}
}

TEST_CASE( "569" )
{
	const auto doc = load_test( 569 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 2 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == u8"foo" );
		const auto lit = doc->labeledLinks().find( l->url() );
		REQUIRE( lit != doc->labeledLinks().cend() );
#ifdef MD4QT_QT_SUPPORT
		REQUIRE( (*lit)->url() == u8"/url2" );
#else
		REQUIRE( lit->second->url() == u8"/url2" );
#endif
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == u8"baz" );
		const auto lit = doc->labeledLinks().find( l->url() );
		REQUIRE( lit != doc->labeledLinks().cend() );
#ifdef MD4QT_QT_SUPPORT
		REQUIRE( (*lit)->url() == u8"/url1" );
#else
		REQUIRE( lit->second->url() == u8"/url1" );
#endif
	}
}

TEST_CASE( "570" )
{
	const auto doc = load_test( 570 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 2 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"[foo]" );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == u8"bar" );
		const auto lit = doc->labeledLinks().find( l->url() );
		REQUIRE( lit != doc->labeledLinks().cend() );
#ifdef MD4QT_QT_SUPPORT
		REQUIRE( (*lit)->url() == u8"/url1" );
#else
		REQUIRE( lit->second->url() == u8"/url1" );
#endif
	}
}
