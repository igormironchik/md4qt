
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

// 5 Container blocks

// 5.1 Block quotes

TEST_CASE( "228" )
{
	const auto doc = load_test( 228 );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );
	auto b = static_cast< MD::Blockquote* > ( doc->items().at( 1 ).data() );
	REQUIRE( b->items().size() == 2 );

	{
		REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading* > ( b->items().at( 0 ).data() );
		REQUIRE( !h->text().isNull() );
		auto p = h->text().data();
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
		REQUIRE( h->level() == 1 );
		REQUIRE( t->text() == QStringLiteral( "Foo" ) );
	}

	{
		REQUIRE( b->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph* > ( b->items().at( 1 ).data() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "bar baz" ) );
	}
}

TEST_CASE( "229" )
{
	const auto doc = load_test( 229 );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );
	auto b = static_cast< MD::Blockquote* > ( doc->items().at( 1 ).data() );
	REQUIRE( b->items().size() == 2 );

	{
		REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading* > ( b->items().at( 0 ).data() );
		REQUIRE( !h->text().isNull() );
		auto p = h->text().data();
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
		REQUIRE( h->level() == 1 );
		REQUIRE( t->text() == QStringLiteral( "Foo" ) );
	}

	{
		REQUIRE( b->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph* > ( b->items().at( 1 ).data() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "bar baz" ) );
	}
}

TEST_CASE( "230" )
{
	const auto doc = load_test( 230 );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );
	auto b = static_cast< MD::Blockquote* > ( doc->items().at( 1 ).data() );
	REQUIRE( b->items().size() == 2 );

	{
		REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading* > ( b->items().at( 0 ).data() );
		REQUIRE( !h->text().isNull() );
		auto p = h->text().data();
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
		REQUIRE( h->level() == 1 );
		REQUIRE( t->text() == QStringLiteral( "Foo" ) );
	}

	{
		REQUIRE( b->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph* > ( b->items().at( 1 ).data() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "bar baz" ) );
	}
}

TEST_CASE( "231" )
{
	const auto doc = load_test( 231 );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code* > ( doc->items().at( 1 ).data() );
	REQUIRE( !c->isInlined() );
	REQUIRE( c->text() == QStringLiteral( "> # Foo\n> bar\n> baz" ) );
}

TEST_CASE( "232" )
{
	const auto doc = load_test( 232 );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );
	auto b = static_cast< MD::Blockquote* > ( doc->items().at( 1 ).data() );
	REQUIRE( b->items().size() == 2 );

	{
		REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading* > ( b->items().at( 0 ).data() );
		REQUIRE( !h->text().isNull() );
		auto p = h->text().data();
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
		REQUIRE( h->level() == 1 );
		REQUIRE( t->text() == QStringLiteral( "Foo" ) );
	}

	{
		REQUIRE( b->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph* > ( b->items().at( 1 ).data() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "bar baz" ) );
	}
}

TEST_CASE( "233" )
{
	const auto doc = load_test( 233 );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );
	auto b = static_cast< MD::Blockquote* > ( doc->items().at( 1 ).data() );
	REQUIRE( b->items().size() == 1 );

	{
		REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph* > ( b->items().at( 0 ).data() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "bar baz foo" ) );
	}
}

TEST_CASE( "234" )
{
	const auto doc = load_test( 234 );

	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );
	auto b = static_cast< MD::Blockquote* > ( doc->items().at( 1 ).data() );
	REQUIRE( b->items().size() == 1 );

	{
		REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph* > ( b->items().at( 0 ).data() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "foo" ) );
	}

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::HorizontalLine );
}

TEST_CASE( "235" )
{
	const auto doc = load_test( 235 );

	REQUIRE( doc->items().size() == 3 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );
		auto b = static_cast< MD::Blockquote* > ( doc->items().at( 1 ).data() );
		REQUIRE( b->items().size() == 1 );
		REQUIRE( b->items().at( 0 )->type() == MD::ItemType::List );
		auto l = static_cast< MD::List* > ( b->items().at( 0 ).data() );
		REQUIRE( l->items().size() == 1 );
		REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem* > ( l->items().at( 0 ).data() );
		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph* > ( li->items().at( 0 ).data() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "foo" ) );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::List );
		auto l = static_cast< MD::List* > ( doc->items().at( 2 ).data() );
		REQUIRE( l->items().size() == 1 );
		REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem* > ( l->items().at( 0 ).data() );
		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph* > ( li->items().at( 0 ).data() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "bar" ) );
	}
}

TEST_CASE( "236" )
{
	const auto doc = load_test( 236 );

	REQUIRE( doc->items().size() == 3 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );
		auto b = static_cast< MD::Blockquote* > ( doc->items().at( 1 ).data() );
		REQUIRE( b->items().size() == 1 );
		REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code* > ( b->items().at( 0 ).data() );
		REQUIRE( !c->isInlined() );
		REQUIRE( c->text() == QStringLiteral( "foo" ) );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code* > ( doc->items().at( 2 ).data() );
		REQUIRE( !c->isInlined() );
		REQUIRE( c->text() == QStringLiteral( "bar" ) );
	}
}

TEST_CASE( "237" )
{
	const auto doc = load_test( 237 );

	REQUIRE( doc->items().size() == 4 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );
		auto b = static_cast< MD::Blockquote* > ( doc->items().at( 1 ).data() );
		REQUIRE( b->items().size() == 1 );
		REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code* > ( b->items().at( 0 ).data() );
		REQUIRE( !c->isInlined() );
		REQUIRE( c->text().isEmpty() );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph* > ( doc->items().at( 2 ).data() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "foo" ) );
	}

	{
		REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code* > ( doc->items().at( 3 ).data() );
		REQUIRE( !c->isInlined() );
		REQUIRE( c->text().isEmpty() );
	}
}

TEST_CASE( "238" )
{
	const auto doc = load_test( 238 );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );
	auto b = static_cast< MD::Blockquote* > ( doc->items().at( 1 ).data() );
	REQUIRE( b->items().size() == 1 );

	{
		REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph* > ( b->items().at( 0 ).data() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "foo - bar" ) );
	}
}

TEST_CASE( "239" )
{
	const auto doc = load_test( 239 );

	REQUIRE( doc->items().size() == 1 );

	// I dont add empty blockquote...
}

TEST_CASE( "240" )
{
	const auto doc = load_test( 240 );

	REQUIRE( doc->items().size() == 1 );

	// I dont add empty blockquote...
}

TEST_CASE( "241" )
{
	const auto doc = load_test( 241 );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );
	auto b = static_cast< MD::Blockquote* > ( doc->items().at( 1 ).data() );
	REQUIRE( b->items().size() == 1 );

	{
		REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph* > ( b->items().at( 0 ).data() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "foo" ) );
	}
}

TEST_CASE( "242" )
{
	const auto doc = load_test( 242 );

	REQUIRE( doc->items().size() == 3 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );
		auto b = static_cast< MD::Blockquote* > ( doc->items().at( 1 ).data() );
		REQUIRE( b->items().size() == 1 );

		{
			REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph* > ( b->items().at( 0 ).data() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "foo" ) );
		}
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Blockquote );
		auto b = static_cast< MD::Blockquote* > ( doc->items().at( 2 ).data() );
		REQUIRE( b->items().size() == 1 );

		{
			REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph* > ( b->items().at( 0 ).data() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "bar" ) );
		}
	}
}

TEST_CASE( "243" )
{
	const auto doc = load_test( 243 );

	REQUIRE( doc->items().size() == 2 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );
		auto b = static_cast< MD::Blockquote* > ( doc->items().at( 1 ).data() );
		REQUIRE( b->items().size() == 1 );

		{
			REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph* > ( b->items().at( 0 ).data() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "foo bar" ) );
		}
	}
}

TEST_CASE( "244" )
{
	const auto doc = load_test( 244 );

	REQUIRE( doc->items().size() == 2 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );
		auto b = static_cast< MD::Blockquote* > ( doc->items().at( 1 ).data() );
		REQUIRE( b->items().size() == 2 );

		{
			REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph* > ( b->items().at( 0 ).data() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "foo" ) );
		}

		{
			REQUIRE( b->items().at( 1 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph* > ( b->items().at( 1 ).data() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "bar" ) );
		}
	}
}

TEST_CASE( "245" )
{
	const auto doc = load_test( 245 );

	REQUIRE( doc->items().size() == 3 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "foo" ) );
	}

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Blockquote );
	auto b = static_cast< MD::Blockquote* > ( doc->items().at( 2 ).data() );
	REQUIRE( b->items().size() == 1 );

	{
		REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph* > ( b->items().at( 0 ).data() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "bar" ) );
	}
}

TEST_CASE( "246" )
{
	const auto doc = load_test( 246 );

	REQUIRE( doc->items().size() == 4 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );
		auto b = static_cast< MD::Blockquote* > ( doc->items().at( 1 ).data() );
		REQUIRE( b->items().size() == 1 );

		{
			REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph* > ( b->items().at( 0 ).data() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "aaa" ) );
		}
	}

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::HorizontalLine );

	{
		REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Blockquote );
		auto b = static_cast< MD::Blockquote* > ( doc->items().at( 3 ).data() );
		REQUIRE( b->items().size() == 1 );

		{
			REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph* > ( b->items().at( 0 ).data() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "bbb" ) );
		}
	}
}

TEST_CASE( "247" )
{
	const auto doc = load_test( 247 );

	REQUIRE( doc->items().size() == 2 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );
		auto b = static_cast< MD::Blockquote* > ( doc->items().at( 1 ).data() );
		REQUIRE( b->items().size() == 1 );

		{
			REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph* > ( b->items().at( 0 ).data() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "bar baz" ) );
		}
	}
}

TEST_CASE( "248" )
{
	const auto doc = load_test( 248 );

	REQUIRE( doc->items().size() == 3 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );
		auto b = static_cast< MD::Blockquote* > ( doc->items().at( 1 ).data() );
		REQUIRE( b->items().size() == 1 );

		{
			REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph* > ( b->items().at( 0 ).data() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "bar" ) );
		}
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph* > ( doc->items().at( 2 ).data() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "baz" ) );
	}
}

TEST_CASE( "249" )
{
	const auto doc = load_test( 249 );

	REQUIRE( doc->items().size() == 3 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );
		auto b = static_cast< MD::Blockquote* > ( doc->items().at( 1 ).data() );
		REQUIRE( b->items().size() == 1 );

		{
			REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph* > ( b->items().at( 0 ).data() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "bar" ) );
		}
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph* > ( doc->items().at( 2 ).data() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "baz" ) );
	}
}

TEST_CASE( "250" )
{
	const auto doc = load_test( 250 );

	REQUIRE( doc->items().size() == 2 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );
		auto b1 = static_cast< MD::Blockquote* > ( doc->items().at( 1 ).data() );
		REQUIRE( b1->items().size() == 1 );

		REQUIRE( b1->items().at( 0 )->type() == MD::ItemType::Blockquote );
		auto b2 = static_cast< MD::Blockquote* > ( b1->items().at( 0 ).data() );

		REQUIRE( b2->items().size() == 1 );
		REQUIRE( b2->items().at( 0 )->type() == MD::ItemType::Blockquote );

		auto b3 = static_cast< MD::Blockquote* > ( b2->items().at( 0 ).data() );
		REQUIRE( b3->items().size() == 1 );

		{
			REQUIRE( b3->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph* > ( b3->items().at( 0 ).data() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "foo bar" ) );
		}
	}
}

TEST_CASE( "251" )
{
	const auto doc = load_test( 251 );

	REQUIRE( doc->items().size() == 2 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );
		auto b1 = static_cast< MD::Blockquote* > ( doc->items().at( 1 ).data() );
		REQUIRE( b1->items().size() == 1 );

		REQUIRE( b1->items().at( 0 )->type() == MD::ItemType::Blockquote );
		auto b2 = static_cast< MD::Blockquote* > ( b1->items().at( 0 ).data() );

		REQUIRE( b2->items().size() == 1 );
		REQUIRE( b2->items().at( 0 )->type() == MD::ItemType::Blockquote );

		auto b3 = static_cast< MD::Blockquote* > ( b2->items().at( 0 ).data() );
		REQUIRE( b3->items().size() == 1 );

		{
			REQUIRE( b3->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph* > ( b3->items().at( 0 ).data() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "foo bar baz" ) );
		}
	}
}

TEST_CASE( "252" )
{
	const auto doc = load_test( 252 );

	REQUIRE( doc->items().size() == 3 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );
		auto b = static_cast< MD::Blockquote* > ( doc->items().at( 1 ).data() );
		REQUIRE( b->items().size() == 1 );

		REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code* > ( b->items().at( 0 ).data() );
		REQUIRE( !c->isInlined() );
		REQUIRE( c->text() == QStringLiteral( "code" ) );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Blockquote );
		auto b = static_cast< MD::Blockquote* > ( doc->items().at( 2 ).data() );
		REQUIRE( b->items().size() == 1 );

		REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph* > ( b->items().at( 0 ).data() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "not code" ) );
	}
}
