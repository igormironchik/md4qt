
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2022 Igor Mironchik

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "load.hpp"

#include <doctest/doctest.h>

// 2.2 Tabs

TEST_CASE( "001" )
{
	const auto doc = load_test( 1 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );
	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );

	auto c = static_cast< MD::Code* > ( doc->items().at( 1 ).data() );
	REQUIRE( c->isInlined() == false );
	REQUIRE( c->syntax().isEmpty() );
	REQUIRE( c->text() == QStringLiteral( "foo    baz        bim" ) );
}

TEST_CASE( "002" ) // Not strict to CommonMark.
{
	MESSAGE( "This test is not strict to CommonMark 0.30." );

	const auto doc = load_test( 2 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );
	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );

	auto c = static_cast< MD::Code* > ( doc->items().at( 1 ).data() );
	REQUIRE( c->isInlined() == false );
	REQUIRE( c->syntax().isEmpty() );
	// Extra spaces.
	REQUIRE( c->text() == QStringLiteral( "  foo    baz        bim" ) );
}

TEST_CASE( "003" )
{
	const auto doc = load_test( 3 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );
	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );

	auto c = static_cast< MD::Code* > ( doc->items().at( 1 ).data() );
	REQUIRE( c->isInlined() == false );
	REQUIRE( c->syntax().isEmpty() );
	REQUIRE( c->text() == QStringLiteral( "a    a\nὐ    a" ) );
}

TEST_CASE( "004" )
{
	const auto doc = load_test( 4 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );
	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List* > ( doc->items().at( 1 ).data() );
	REQUIRE( l->items().size() == 1 );

	auto li = static_cast< MD::ListItem* > ( l->items().at( 0 ).data() );
	REQUIRE( li->listType() == MD::ListItem::Unordered );
	REQUIRE( li->items().size() == 2 );

	REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
	auto p1 = static_cast< MD::Paragraph* > ( li->items().at( 0 ).data() );
	REQUIRE( p1->items().size() == 1 );
	REQUIRE( p1->items().at( 0 )->type() == MD::ItemType::Text );
	auto t1 = static_cast< MD::Text* > ( p1->items().at( 0 ).data() );
	REQUIRE( t1->opts() == MD::TextWithoutFormat );
	REQUIRE( t1->text() == QStringLiteral( "foo" ) );

	REQUIRE( li->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p2 = static_cast< MD::Paragraph* > ( li->items().at( 1 ).data() );
	REQUIRE( p2->items().size() == 1 );
	REQUIRE( p2->items().at( 0 )->type() == MD::ItemType::Text );
	auto t2 = static_cast< MD::Text* > ( p2->items().at( 0 ).data() );
	REQUIRE( t2->opts() == MD::TextWithoutFormat );
	REQUIRE( t2->text() == QStringLiteral( "bar" ) );
}

TEST_CASE( "005" )
{
	const auto doc = load_test( 5 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );
	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List* > ( doc->items().at( 1 ).data() );
	REQUIRE( l->items().size() == 1 );

	auto li = static_cast< MD::ListItem* > ( l->items().at( 0 ).data() );
	REQUIRE( li->listType() == MD::ListItem::Unordered );
	REQUIRE( li->items().size() == 2 );

	REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
	auto p1 = static_cast< MD::Paragraph* > ( li->items().at( 0 ).data() );
	REQUIRE( p1->items().size() == 1 );
	REQUIRE( p1->items().at( 0 )->type() == MD::ItemType::Text );
	auto t1 = static_cast< MD::Text* > ( p1->items().at( 0 ).data() );
	REQUIRE( t1->opts() == MD::TextWithoutFormat );
	REQUIRE( t1->text() == QStringLiteral( "foo" ) );

	REQUIRE( li->items().at( 1 )->type() == MD::ItemType::Code );
	auto c2 = static_cast< MD::Code* > ( li->items().at( 1 ).data() );
	REQUIRE( c2->isInlined() == false );
	REQUIRE( c2->syntax().isEmpty() );
	REQUIRE( c2->text() == QStringLiteral( "  bar" ) );
}

TEST_CASE( "006" ) // Not strict to CommonMark.
{
	MESSAGE( "This test is not strict to CommonMark 0.30." );

	const auto doc = load_test( 6 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );
	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );

	auto q = static_cast< MD::Blockquote* > ( doc->items().at( 1 ).data() );
	REQUIRE( q->items().size() == 1 );

	REQUIRE( q->items().at( 0 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code* > ( q->items().at( 0 ).data() );
	REQUIRE( c->isInlined() == false );
	REQUIRE( c->syntax().isEmpty() );
	// Extra spaces.
	REQUIRE( c->text() == QStringLiteral( "   foo" ) );
}

TEST_CASE( "007" ) // Not strict to CommonMark.
{
	MESSAGE( "This test is not strict to CommonMark 0.30." );

	const auto doc = load_test( 7 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );
	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List* > ( doc->items().at( 1 ).data() );
	REQUIRE( l->items().size() == 1 );

	auto li = static_cast< MD::ListItem* > ( l->items().at( 0 ).data() );
	REQUIRE( li->listType() == MD::ListItem::Unordered );
	REQUIRE( li->items().size() == 1 );

	REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code* > ( li->items().at( 0 ).data() );
	REQUIRE( c->isInlined() == false );
	REQUIRE( c->syntax().isEmpty() );
	// Extra spaces.
	REQUIRE( c->text() == QStringLiteral( "   foo" ) );
}

TEST_CASE( "008" )
{
	const auto doc = load_test( 8 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );
	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );

	auto c = static_cast< MD::Code* > ( doc->items().at( 1 ).data() );
	REQUIRE( c->isInlined() == false );
	REQUIRE( c->syntax().isEmpty() );
	REQUIRE( c->text() == QStringLiteral( "foo\nbar" ) );
}

TEST_CASE( "009" )
{
	const auto doc = load_test( 9 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );
	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	auto l1 = static_cast< MD::List* > ( doc->items().at( 1 ).data() );
	REQUIRE( l1->items().size() == 1 );
	REQUIRE( l1->items().at( 0 )->type() == MD::ItemType::ListItem );
	auto li1 = static_cast< MD::ListItem* > ( l1->items().at( 0 ).data() );
	REQUIRE( li1->items().size() == 2 );
	REQUIRE( li1->items().at( 0 )->type() == MD::ItemType::Paragraph );
	auto p1 = static_cast< MD::Paragraph* > ( li1->items().at( 0 ).data() );
	REQUIRE( p1->items().size() == 1 );
	REQUIRE( p1->items().at( 0 )->type() == MD::ItemType::Text );
	auto t1 = static_cast< MD::Text* > ( p1->items().at( 0 ).data() );
	REQUIRE( t1->opts() == MD::TextWithoutFormat );
	REQUIRE( t1->text() == QStringLiteral( "foo" ) );

	REQUIRE( li1->items().at( 1 )->type() == MD::ItemType::List );
	auto l2 = static_cast< MD::List* > ( li1->items().at( 1 ).data() );
	REQUIRE( l2->items().size() == 1 );
	REQUIRE( l2->items().at( 0 )->type() == MD::ItemType::ListItem );
	auto li2 = static_cast< MD::ListItem* > ( l2->items().at( 0 ).data() );
	REQUIRE( li2->items().size() == 2 );
	REQUIRE( li2->items().at( 0 )->type() == MD::ItemType::Paragraph );
	auto p2 = static_cast< MD::Paragraph* > ( li2->items().at( 0 ).data() );
	REQUIRE( p2->items().size() == 1 );
	REQUIRE( p2->items().at( 0 )->type() == MD::ItemType::Text );
	auto t2 = static_cast< MD::Text* > ( p2->items().at( 0 ).data() );
	REQUIRE( t2->opts() == MD::TextWithoutFormat );
	REQUIRE( t2->text() == QStringLiteral( "bar" ) );

	REQUIRE( li2->items().at( 1 )->type() == MD::ItemType::List );
	auto l3 = static_cast< MD::List* > ( li2->items().at( 1 ).data() );
	REQUIRE( l3->items().size() == 1 );
	REQUIRE( l3->items().at( 0 )->type() == MD::ItemType::ListItem );
	auto li3 = static_cast< MD::ListItem* > ( l3->items().at( 0 ).data() );
	REQUIRE( li3->items().size() == 1 );
	REQUIRE( li3->items().at( 0 )->type() == MD::ItemType::Paragraph );
	auto p3 = static_cast< MD::Paragraph* > ( li3->items().at( 0 ).data() );
	REQUIRE( p3->items().size() == 1 );
	REQUIRE( p3->items().at( 0 )->type() == MD::ItemType::Text );
	auto t3 = static_cast< MD::Text* > ( p3->items().at( 0 ).data() );
	REQUIRE( t3->opts() == MD::TextWithoutFormat );
	REQUIRE( t3->text() == QStringLiteral( "baz" ) );
}

TEST_CASE( "010" )
{
	const auto doc = load_test( 10 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );
	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Heading );

	auto h = static_cast< MD::Heading* > ( doc->items().at( 1 ).data() );
	REQUIRE( h->isLabeled() );
	const QString fn = QStringLiteral( "/" ) + QDir().absolutePath() +
		QStringLiteral( "/tests/commonmark/0.30/010.md" );
	REQUIRE( !h->text().isNull() );
	auto p = h->text().data();
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
	REQUIRE( h->label() == QStringLiteral( "#" ) + t->text().toLower() + fn );
	REQUIRE( h->level() == 1 );
	REQUIRE( t->text() == QStringLiteral( "Foo" ) );
}

TEST_CASE( "011" )
{
	const auto doc = load_test( 11 );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::HorizontalLine );
}
