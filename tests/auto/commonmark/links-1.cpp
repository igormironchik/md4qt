
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2022-2023 Igor Mironchik

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

// doctest include.
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#ifndef MD4QT_QT_SUPPORT
#include <filesystem>
#endif

// 6.3 Links

TEST_CASE( "481" )
{
	const auto doc = load_test( 481 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
	auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( l->img()->isEmpty() );
	REQUIRE( l->opts() == MD::TextWithoutFormat );
	REQUIRE( l->text() == u8"link" );
	REQUIRE( l->url() == u8"/uri" );
}

TEST_CASE( "482" )
{
	const auto doc = load_test( 482 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
	auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( l->img()->isEmpty() );
	REQUIRE( l->opts() == MD::TextWithoutFormat );
	REQUIRE( l->text() == u8"link" );
	REQUIRE( l->url() == u8"/uri" );
}

TEST_CASE( "483" )
{
	const auto doc = load_test( 483 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
	auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( l->img()->isEmpty() );
	REQUIRE( l->opts() == MD::TextWithoutFormat );
	REQUIRE( l->text().isEmpty() );
	REQUIRE( l->url() == u8"./target.md" );
}

TEST_CASE( "484" )
{
	const auto doc = load_test( 484 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
	auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( l->img()->isEmpty() );
	REQUIRE( l->opts() == MD::TextWithoutFormat );
	REQUIRE( l->text() == u8"link" );
	REQUIRE( l->url().isEmpty() );
}

TEST_CASE( "485" )
{
	const auto doc = load_test( 485 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
	auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( l->img()->isEmpty() );
	REQUIRE( l->opts() == MD::TextWithoutFormat );
	REQUIRE( l->text() == u8"link" );
	REQUIRE( l->url().isEmpty() );
}

TEST_CASE( "486" )
{
	const auto doc = load_test( 486 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
	auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( l->img()->isEmpty() );
	REQUIRE( l->opts() == MD::TextWithoutFormat );
	REQUIRE( l->text().isEmpty() );
	REQUIRE( l->url().isEmpty() );
}

TEST_CASE( "487" )
{
	const auto doc = load_test( 487 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"[link](/my uri)" );
}

TEST_CASE( "488" )
{
	const auto doc = load_test( 488 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
	auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( l->img()->isEmpty() );
	REQUIRE( l->opts() == MD::TextWithoutFormat );
	REQUIRE( l->text() == u8"link" );
	REQUIRE( l->url() == u8"/my uri" );
}

TEST_CASE( "489" )
{
	const auto doc = load_test( 489 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 2 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"[link](foo" );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"bar)" );
	}
}

TEST_CASE( "490" )
{
	const auto doc = load_test( 490 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 3 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"[link](" );
	}

	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<foo\nbar>" );

	{
		REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 2 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8")" );
	}
}

TEST_CASE( "491" )
{
	const auto doc = load_test( 491 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
	auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( l->img()->isEmpty() );
	REQUIRE( l->opts() == MD::TextWithoutFormat );
	REQUIRE( l->text() == u8"a" );
	REQUIRE( l->url() == u8"b)c" );
}

TEST_CASE( "492" )
{
	const auto doc = load_test( 492 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"[link](<foo>)" );
}

TEST_CASE( "493" )
{
	const auto doc = load_test( 493 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 5 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"[a](<b)c" );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"[a](<b)c>" );
	}

	{
		REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 2 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"[a](" );
	}

	REQUIRE( p->items().at( 3 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 3 ).get() );
	REQUIRE( h->text() == u8"<b>" );

	{
		REQUIRE( p->items().at( 4 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 4 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"c)" );
	}
}

TEST_CASE( "494" )
{
	const auto doc = load_test( 494 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
	auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( l->img()->isEmpty() );
	REQUIRE( l->opts() == MD::TextWithoutFormat );
	REQUIRE( l->text() == u8"link" );
	REQUIRE( l->url() == u8"(foo)" );
}

TEST_CASE( "495" )
{
	const auto doc = load_test( 495 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
	auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( l->img()->isEmpty() );
	REQUIRE( l->opts() == MD::TextWithoutFormat );
	REQUIRE( l->text() == u8"link" );
	REQUIRE( l->url() == u8"foo(and(bar))" );
}

TEST_CASE( "496" )
{
	const auto doc = load_test( 496 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"[link](foo(and(bar))" );
}

TEST_CASE( "497" )
{
	const auto doc = load_test( 497 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
	auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( l->img()->isEmpty() );
	REQUIRE( l->opts() == MD::TextWithoutFormat );
	REQUIRE( l->text() == u8"link" );
	REQUIRE( l->url() == u8"foo(and(bar)" );
}

TEST_CASE( "498" )
{
	const auto doc = load_test( 498 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
	auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( l->img()->isEmpty() );
	REQUIRE( l->opts() == MD::TextWithoutFormat );
	REQUIRE( l->text() == u8"link" );
	REQUIRE( l->url() == u8"foo(and(bar)" );
}

TEST_CASE( "499" )
{
	const auto doc = load_test( 499 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
	auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( l->img()->isEmpty() );
	REQUIRE( l->opts() == MD::TextWithoutFormat );
	REQUIRE( l->text() == u8"link" );
	REQUIRE( l->url() == u8"foo):" );
}

TEST_CASE( "500" )
{
	const auto doc = load_test( 500 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 4 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == u8"link" );

		typename TRAIT::String fn = u8"/" +
#ifdef MD4QT_QT_SUPPORT
				QDir().absolutePath()
#else
				std::filesystem::canonical( std::filesystem::current_path() ).u8string()
#endif
				+ u8"/tests/commonmark/0.30/500.md";

#ifndef MD4QT_QT_SUPPORT
		std::string tmp;
		fn.toUTF8String( tmp );
		std::replace( tmp.begin(), tmp.end(), '\\', '/' );
		fn = icu::UnicodeString::fromUTF8( tmp );
#endif

		REQUIRE( l->url() == u8"#fragment" + fn );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == u8"link" );
		REQUIRE( l->url() == u8"http://example.com#fragment" );
	}

	{
		REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 3 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == u8"link" );
		REQUIRE( l->url() == u8"http://example.com?foo=3#frag" );
	}
}

TEST_CASE( "501" )
{
	const auto doc = load_test( 501 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
	auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( l->img()->isEmpty() );
	REQUIRE( l->opts() == MD::TextWithoutFormat );
	REQUIRE( l->text() == u8"link" );
	REQUIRE( l->url() == u8"foo\\bar" );
}

TEST_CASE( "502" )
{
	const auto doc = load_test( 502 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
	auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( l->img()->isEmpty() );
	REQUIRE( l->opts() == MD::TextWithoutFormat );
	REQUIRE( l->text() == u8"link" );
	REQUIRE( l->url() == TRAIT::utf16ToString( u"foo%20b\x00E4" ) );
}

TEST_CASE( "503" )
{
	const auto doc = load_test( 503 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
	auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( l->img()->isEmpty() );
	REQUIRE( l->opts() == MD::TextWithoutFormat );
	REQUIRE( l->text() == u8"link" );
	REQUIRE( l->url() == u8"\"title\"" );
}

TEST_CASE( "504" )
{
	const auto doc = load_test( 504 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 3 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == u8"link" );
		REQUIRE( l->url() == u8"/url" );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == u8"link" );
		REQUIRE( l->url() == u8"/url" );
	}

	{
		REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 2 ).get() );
		REQUIRE( l->img()->isEmpty() );
		REQUIRE( l->opts() == MD::TextWithoutFormat );
		REQUIRE( l->text() == u8"link" );
		REQUIRE( l->url() == u8"/url" );
	}
}

TEST_CASE( "505" )
{
	const auto doc = load_test( 505 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
	auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( l->img()->isEmpty() );
	REQUIRE( l->opts() == MD::TextWithoutFormat );
	REQUIRE( l->text() == u8"link" );
	REQUIRE( l->url() == u8"/url" );
}

TEST_CASE( "506" )
{
	const auto doc = load_test( 506 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
	auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( l->img()->isEmpty() );
	REQUIRE( l->opts() == MD::TextWithoutFormat );
	REQUIRE( l->text() == u8"link" );
	REQUIRE( l->url() == TRAIT::utf16ToString( u"/url\x00A0\"title\"" ) );
}

TEST_CASE( "507" )
{
	const auto doc = load_test( 507 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"[link](/url \"title \"and\" title\")" );
}

TEST_CASE( "508" )
{
	const auto doc = load_test( 508 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
	auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( l->img()->isEmpty() );
	REQUIRE( l->opts() == MD::TextWithoutFormat );
	REQUIRE( l->text() == u8"link" );
	REQUIRE( l->url() == u8"/url" );
}

TEST_CASE( "509" )
{
	const auto doc = load_test( 509 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
	auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( l->img()->isEmpty() );
	REQUIRE( l->opts() == MD::TextWithoutFormat );
	REQUIRE( l->text() == u8"link" );
	REQUIRE( l->url() == u8"/uri" );
}

TEST_CASE( "510" )
{
	const auto doc = load_test( 510 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"[link] (/uri)" );
}

TEST_CASE( "511" )
{
	const auto doc = load_test( 511 );

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
	REQUIRE( l->url() == u8"/uri" );
}

TEST_CASE( "512" )
{
	const auto doc = load_test( 512 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"[link] bar](/uri)" );
}

TEST_CASE( "513" )
{
	const auto doc = load_test( 513 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 2 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"[link" );

	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Link );
	auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 1 ).get() );
	REQUIRE( l->img()->isEmpty() );
	REQUIRE( l->opts() == MD::TextWithoutFormat );
	REQUIRE( l->text() == u8"bar" );
	REQUIRE( l->url() == u8"/uri" );
}

TEST_CASE( "514" )
{
	const auto doc = load_test( 514 );

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
	REQUIRE( l->url() == u8"/uri" );
}

TEST_CASE( "515" )
{
	const auto doc = load_test( 515 );

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
	REQUIRE( l->url() == u8"/uri" );
}

TEST_CASE( "516" )
{
	const auto doc = load_test( 516 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
	auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( l->opts() == MD::TextWithoutFormat );
	REQUIRE( l->text() == u8"![moon](moon.jpg)" );
	REQUIRE( l->url() == u8"/uri" );
	REQUIRE( l->img().get() );
	REQUIRE( !l->img()->isEmpty() );
	REQUIRE( l->img()->text() == u8"moon" );
	REQUIRE( l->img()->url() == u8"moon.jpg" );
}

TEST_CASE( "517" )
{
	const auto doc = load_test( 517 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 3 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"[foo" );
	}

	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Link );
	auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 1 ).get() );
	REQUIRE( l->img()->isEmpty() );
	REQUIRE( l->opts() == MD::TextWithoutFormat );
	REQUIRE( l->text() == u8"bar" );
	REQUIRE( l->url() == u8"/uri" );

	{
		REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 2 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"](/uri)" );
	}
}

TEST_CASE( "518" )
{
	const auto doc = load_test( 518 );

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
		REQUIRE( t->text() == u8"[bar" );
	}

	REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Link );
	auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 2 ).get() );
	REQUIRE( l->img()->isEmpty() );
	REQUIRE( l->opts() == MD::ItalicText );
	REQUIRE( l->text() == u8"baz" );
	REQUIRE( l->url() == u8"/uri" );

	{
		REQUIRE( p->items().at( 3 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 3 ).get() );
		REQUIRE( t->opts() == MD::ItalicText );
		REQUIRE( t->text() == u8"](/uri)" );
	}

	{
		REQUIRE( p->items().at( 4 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 4 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"](/uri)" );
	}
}

TEST_CASE( "519" )
{
	MESSAGE( "This test is not strict to CommonMark 0.30." );

	const auto doc = load_test( 519 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Image );
	auto i = static_cast< MD::Image< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( !i->isEmpty() );

	// I don't parse alt text of the image.
	REQUIRE( i->text() == u8"[[foo](uri1)](uri2)" );
	REQUIRE( i->url() == u8"uri3" );
}

TEST_CASE( "520" )
{
	const auto doc = load_test( 520 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 2 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"*" );

	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Link );
	auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 1 ).get() );
	REQUIRE( l->img()->isEmpty() );
	REQUIRE( l->opts() == MD::TextWithoutFormat );
	REQUIRE( l->text() == u8"foo*" );
	REQUIRE( l->url() == u8"/uri" );
}

TEST_CASE( "521" )
{
	const auto doc = load_test( 521 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
	auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( l->img()->isEmpty() );
	REQUIRE( l->opts() == MD::TextWithoutFormat );
	REQUIRE( l->text() == u8"foo *bar" );
	REQUIRE( l->url() == u8"baz*" );
}

TEST_CASE( "522" )
{
	const auto doc = load_test( 522 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 2 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::ItalicText );
		REQUIRE( t->text() == u8"foo [bar" );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"baz]" );
	}
}

TEST_CASE( "523" )
{
	const auto doc = load_test( 523 );

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
	REQUIRE( h->text() == u8"<bar attr=\"](baz)\">" );
}

TEST_CASE( "524" )
{
	const auto doc = load_test( 524 );

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

	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code< TRAIT >* > ( p->items().at( 1 ).get() );
	REQUIRE( c->text() == u8"](/uri)" );
}

template< class Trait >
void checkTest525( std::shared_ptr< MD::Document< Trait > > doc );

#ifdef MD4QT_ICU_STL_SUPPORT

template<>
void checkTest525< MD::UnicodeStringTrait >( std::shared_ptr< MD::Document< MD::UnicodeStringTrait > > doc )
{
	MESSAGE( "This test is not strict to CommonMark 0.30 as uriparse library wrong with this URL." );
	MESSAGE( "Skip for now." );
}


#endif // MD4QT_ICU_STL_SUPPORT

#ifdef MD4QT_QT_SUPPORT

template<>
void checkTest525< MD::QStringTrait >( std::shared_ptr< MD::Document< MD::QStringTrait > > doc )
{
	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 2 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"[foo" );

	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Link );
	auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 1 ).get() );
	REQUIRE( l->img()->isEmpty() );
	REQUIRE( l->opts() == MD::TextWithoutFormat );
	REQUIRE( l->text().isEmpty() );
	REQUIRE( l->url() == u8"http://example.com/?search=](uri)" );
}

#endif // MD4QT_QT_SUPPORT

TEST_CASE( "525" )
{
	checkTest525< TRAIT >( load_test( 525 ) );
}
