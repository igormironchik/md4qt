
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

// 2.4 Backslash escapes

TEST_CASE( "012" )
{
	const auto doc = load_test( 12 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );
	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).data() );

	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == QStringLiteral( "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~" ) );
}

TEST_CASE( "013" )
{
	const auto doc = load_test( 13 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );
	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).data() );

	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == QStringLiteral( "\\ \\A\\a\\ \\3\\φ\\«" ) );
}

TEST_CASE( "014" )
{
	const auto doc = load_test( 14 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );
	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).data() );

	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t1 = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
	REQUIRE( t1->opts() == MD::TextWithoutFormat );
	REQUIRE( t1->text() == QStringLiteral( "*not emphasized* <br/> not a tag [not a link](/foo) "
		"`not code` 1. not a list * not a list # not a heading [foo]: /url \"not a reference\" "
		"&ouml; not a character entity" ) );
}

TEST_CASE( "015" )
{
	const auto doc = load_test( 15 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );
	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).data() );

	REQUIRE( p->items().size() == 2 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t1 = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
	REQUIRE( t1->opts() == MD::TextWithoutFormat );
	REQUIRE( t1->text() == QStringLiteral( "\\" ) );

	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
	auto t2 = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).data() );
	REQUIRE( t2->opts() == MD::ItalicText );
	REQUIRE( t2->text() == QStringLiteral( "emphasis" ) );
}

TEST_CASE( "016" )
{
	const auto doc = load_test( 16 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );
	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).data() );

	REQUIRE( p->items().size() == 3 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t1 = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
	REQUIRE( t1->text() == QStringLiteral( "foo" ) );

	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::LineBreak );

	REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );
	auto t2 = static_cast< MD::Text< TRAIT >* > ( p->items().at( 2 ).data() );
	REQUIRE( t2->text() == QStringLiteral( "bar" ) );
}

TEST_CASE( "017" )
{
	const auto doc = load_test( 17 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).data() );

	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code< TRAIT >* > ( p->items().at( 0 ).data() );
	REQUIRE( c->isInlined() == true );
	REQUIRE( c->syntax().isEmpty() );
	REQUIRE( c->text() == QStringLiteral( "\\[\\`" ) );
}

TEST_CASE( "018" )
{
	const auto doc = load_test( 18 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );

	auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( c->isInlined() == false );
	REQUIRE( c->syntax().isEmpty() );
	REQUIRE( c->text() == QStringLiteral( "\\[\\]" ) );
}

TEST_CASE( "019" )
{
	const auto doc = load_test( 19 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );

	auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( c->isInlined() == false );
	REQUIRE( c->syntax().isEmpty() );
	REQUIRE( c->text() == QStringLiteral( "\\[\\]" ) );
}

TEST_CASE( "020" )
{
	const auto doc = load_test( 20 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
	auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).data() );
	REQUIRE( l->opts() == MD::TextWithoutFormat );
	REQUIRE( l->img()->isEmpty() );
	REQUIRE( l->text().isEmpty() );
	REQUIRE( l->url() == QStringLiteral( "http://example.com?find=\\*" ) );
}

TEST_CASE( "021" )
{
	const auto doc = load_test( 21 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( h->text() == QStringLiteral( "<a href=\"/bar\\/)\">" ) );
}

TEST_CASE( "022" )
{
	const auto doc = load_test( 22 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
	auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).data() );
	REQUIRE( l->opts() == MD::TextWithoutFormat );
	REQUIRE( l->img()->isEmpty() );
	REQUIRE( l->text() == QStringLiteral( "foo" ) );
	REQUIRE( l->url() == QStringLiteral( "/bar*" ) );
}

TEST_CASE( "023" )
{
	const auto doc = load_test( 23 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
	auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).data() );
	REQUIRE( l->opts() == MD::TextWithoutFormat );
	REQUIRE( l->img()->isEmpty() );
	REQUIRE( l->text() == QStringLiteral( "foo" ) );
	REQUIRE( doc->labeledLinks().size() == 1 );
	REQUIRE( doc->labeledLinks()[ l->url() ]->url() == QStringLiteral( "/bar*" ) );
}

TEST_CASE( "024" )
{
	const auto doc = load_test( 24 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );

	auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).data() );

	REQUIRE( c->isInlined() == false );
	REQUIRE( c->syntax() == QStringLiteral( "foo+bar" ) );
	REQUIRE( c->text() == QStringLiteral( "foo" ) );
}
