
/*
	SPDX-FileCopyrightText: 2022-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

// doctest include.
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

// 5 Container blocks

// 5.1 Block quotes

TEST_CASE( "228" )
{
	const auto doc = load_test( 228 );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );
	auto b = static_cast< MD::Blockquote< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( b->items().size() == 2 );

	{
		REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( b->items().at( 0 ).get() );
		REQUIRE( h->text().get() );
		auto p = h->text().get();
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( h->level() == 1 );
		REQUIRE( t->text() == u8"Foo" );
	}

	{
		REQUIRE( b->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( b->items().at( 1 ).get() );
		REQUIRE( p->items().size() == 2 );

		{
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"bar" );
		}

		{
			REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"baz" );
		}
	}
}

TEST_CASE( "229" )
{
	const auto doc = load_test( 229 );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );
	auto b = static_cast< MD::Blockquote< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( b->items().size() == 2 );

	{
		REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( b->items().at( 0 ).get() );
		REQUIRE( h->text().get() );
		auto p = h->text().get();
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( h->level() == 1 );
		REQUIRE( t->text() == u8"Foo" );
	}

	{
		REQUIRE( b->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( b->items().at( 1 ).get() );
		REQUIRE( p->items().size() == 2 );

		{
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"bar" );
		}

		{
			REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"baz" );
		}
	}
}

TEST_CASE( "230" )
{
	const auto doc = load_test( 230 );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );
	auto b = static_cast< MD::Blockquote< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( b->items().size() == 2 );

	{
		REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( b->items().at( 0 ).get() );
		REQUIRE( h->text().get() );
		auto p = h->text().get();
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( h->level() == 1 );
		REQUIRE( t->text() == u8"Foo" );
	}

	{
		REQUIRE( b->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( b->items().at( 1 ).get() );
		REQUIRE( p->items().size() == 2 );

		{
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"bar" );
		}

		{
			REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"baz" );
		}
	}
}

TEST_CASE( "231" )
{
	const auto doc = load_test( 231 );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( !c->isInlined() );
	REQUIRE( c->text() == u8"> # Foo\n> bar\n> baz" );
}

TEST_CASE( "232" )
{
	const auto doc = load_test( 232 );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );
	auto b = static_cast< MD::Blockquote< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( b->items().size() == 2 );

	{
		REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( b->items().at( 0 ).get() );
		REQUIRE( h->text().get() );
		auto p = h->text().get();
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( h->level() == 1 );
		REQUIRE( t->text() == u8"Foo" );
	}

	{
		REQUIRE( b->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( b->items().at( 1 ).get() );
		REQUIRE( p->items().size() == 2 );

		{
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"bar" );
		}

		{
			REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"baz" );
		}
	}
}

TEST_CASE( "233" )
{
	const auto doc = load_test( 233 );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );
	auto b = static_cast< MD::Blockquote< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( b->items().size() == 1 );

	{
		REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( b->items().at( 0 ).get() );
		REQUIRE( p->items().size() == 3 );

		{
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"bar" );
		}

		{
			REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"baz" );
		}

		{
			REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 2 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"foo" );
		}
	}
}

TEST_CASE( "234" )
{
	const auto doc = load_test( 234 );

	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );
	auto b = static_cast< MD::Blockquote< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( b->items().size() == 1 );

	{
		REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( b->items().at( 0 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"foo" );
	}

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::HorizontalLine );
}

TEST_CASE( "235" )
{
	const auto doc = load_test( 235 );

	REQUIRE( doc->items().size() == 3 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );
		auto b = static_cast< MD::Blockquote< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( b->items().size() == 1 );
		REQUIRE( b->items().at( 0 )->type() == MD::ItemType::List );
		auto l = static_cast< MD::List< TRAIT >* > ( b->items().at( 0 ).get() );
		REQUIRE( l->items().size() == 1 );
		REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"foo" );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::List );
		auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( l->items().size() == 1 );
		REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"bar" );
	}
}

TEST_CASE( "236" )
{
	const auto doc = load_test( 236 );

	REQUIRE( doc->items().size() == 3 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );
		auto b = static_cast< MD::Blockquote< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( b->items().size() == 1 );
		REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code< TRAIT >* > ( b->items().at( 0 ).get() );
		REQUIRE( !c->isInlined() );
		REQUIRE( c->text() == u8"foo" );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( !c->isInlined() );
		REQUIRE( c->text() == u8"bar" );
	}
}

TEST_CASE( "237" )
{
	const auto doc = load_test( 237 );

	REQUIRE( doc->items().size() == 4 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );
		auto b = static_cast< MD::Blockquote< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( b->items().size() == 1 );
		REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code< TRAIT >* > ( b->items().at( 0 ).get() );
		REQUIRE( !c->isInlined() );
		REQUIRE( c->text().size() == 0 );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"foo" );
	}

	{
		REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 3 ).get() );
		REQUIRE( !c->isInlined() );
		REQUIRE( c->text().size() == 0 );
	}
}

TEST_CASE( "238" )
{
	const auto doc = load_test( 238 );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );
	auto b = static_cast< MD::Blockquote< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( b->items().size() == 1 );

	{
		REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( b->items().at( 0 ).get() );
		REQUIRE( p->items().size() == 2 );

		{
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"foo" );
		}

		{
			REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"- bar" );
		}
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
	auto b = static_cast< MD::Blockquote< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( b->items().size() == 1 );

	{
		REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( b->items().at( 0 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"foo" );
	}
}

TEST_CASE( "242" )
{
	const auto doc = load_test( 242 );

	REQUIRE( doc->items().size() == 3 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );
		auto b = static_cast< MD::Blockquote< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( b->items().size() == 1 );

		{
			REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph< TRAIT >* > ( b->items().at( 0 ).get() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"foo" );
		}
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Blockquote );
		auto b = static_cast< MD::Blockquote< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( b->items().size() == 1 );

		{
			REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph< TRAIT >* > ( b->items().at( 0 ).get() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"bar" );
		}
	}
}

TEST_CASE( "243" )
{
	const auto doc = load_test( 243 );

	REQUIRE( doc->items().size() == 2 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );
		auto b = static_cast< MD::Blockquote< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( b->items().size() == 1 );

		{
			REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph< TRAIT >* > ( b->items().at( 0 ).get() );
			REQUIRE( p->items().size() == 2 );

			{
				REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
				auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
				REQUIRE( t->opts() == MD::TextWithoutFormat );
				REQUIRE( t->text() == u8"foo" );
			}

			{
				REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
				auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );
				REQUIRE( t->opts() == MD::TextWithoutFormat );
				REQUIRE( t->text() == u8"bar" );
			}
		}
	}
}

TEST_CASE( "244" )
{
	const auto doc = load_test( 244 );

	REQUIRE( doc->items().size() == 2 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );
		auto b = static_cast< MD::Blockquote< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( b->items().size() == 2 );

		{
			REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph< TRAIT >* > ( b->items().at( 0 ).get() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"foo" );
		}

		{
			REQUIRE( b->items().at( 1 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph< TRAIT >* > ( b->items().at( 1 ).get() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"bar" );
		}
	}
}

TEST_CASE( "245" )
{
	const auto doc = load_test( 245 );

	REQUIRE( doc->items().size() == 3 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"foo" );
	}

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Blockquote );
	auto b = static_cast< MD::Blockquote< TRAIT >* > ( doc->items().at( 2 ).get() );
	REQUIRE( b->items().size() == 1 );

	{
		REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( b->items().at( 0 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"bar" );
	}
}

TEST_CASE( "246" )
{
	const auto doc = load_test( 246 );

	REQUIRE( doc->items().size() == 4 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );
		auto b = static_cast< MD::Blockquote< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( b->items().size() == 1 );

		{
			REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph< TRAIT >* > ( b->items().at( 0 ).get() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"aaa" );
		}
	}

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::HorizontalLine );

	{
		REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Blockquote );
		auto b = static_cast< MD::Blockquote< TRAIT >* > ( doc->items().at( 3 ).get() );
		REQUIRE( b->items().size() == 1 );

		{
			REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph< TRAIT >* > ( b->items().at( 0 ).get() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"bbb" );
		}
	}
}

TEST_CASE( "247" )
{
	const auto doc = load_test( 247 );

	REQUIRE( doc->items().size() == 2 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );
		auto b = static_cast< MD::Blockquote< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( b->items().size() == 1 );

		{
			REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph< TRAIT >* > ( b->items().at( 0 ).get() );
			REQUIRE( p->items().size() == 2 );

			{
				REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
				auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
				REQUIRE( t->opts() == MD::TextWithoutFormat );
				REQUIRE( t->text() == u8"bar" );
			}

			{
				REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
				auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );
				REQUIRE( t->opts() == MD::TextWithoutFormat );
				REQUIRE( t->text() == u8"baz" );
			}
		}
	}
}

TEST_CASE( "248" )
{
	const auto doc = load_test( 248 );

	REQUIRE( doc->items().size() == 3 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );
		auto b = static_cast< MD::Blockquote< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( b->items().size() == 1 );

		{
			REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph< TRAIT >* > ( b->items().at( 0 ).get() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"bar" );
		}
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"baz" );
	}
}

TEST_CASE( "249" )
{
	const auto doc = load_test( 249 );

	REQUIRE( doc->items().size() == 3 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );
		auto b = static_cast< MD::Blockquote< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( b->items().size() == 1 );

		{
			REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph< TRAIT >* > ( b->items().at( 0 ).get() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"bar" );
		}
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"baz" );
	}
}

TEST_CASE( "250" )
{
	const auto doc = load_test( 250 );

	REQUIRE( doc->items().size() == 2 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );
		auto b1 = static_cast< MD::Blockquote< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( b1->items().size() == 1 );

		REQUIRE( b1->items().at( 0 )->type() == MD::ItemType::Blockquote );
		auto b2 = static_cast< MD::Blockquote< TRAIT >* > ( b1->items().at( 0 ).get() );

		REQUIRE( b2->items().size() == 1 );
		REQUIRE( b2->items().at( 0 )->type() == MD::ItemType::Blockquote );

		auto b3 = static_cast< MD::Blockquote< TRAIT >* > ( b2->items().at( 0 ).get() );
		REQUIRE( b3->items().size() == 1 );

		{
			REQUIRE( b3->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph< TRAIT >* > ( b3->items().at( 0 ).get() );
			REQUIRE( p->items().size() == 2 );

			{
				REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
				auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
				REQUIRE( t->opts() == MD::TextWithoutFormat );
				REQUIRE( t->text() == u8"foo" );
			}

			{
				REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
				auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );
				REQUIRE( t->opts() == MD::TextWithoutFormat );
				REQUIRE( t->text() == u8"bar" );
			}
		}
	}
}

TEST_CASE( "251" )
{
	const auto doc = load_test( 251 );

	REQUIRE( doc->items().size() == 2 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );
		auto b1 = static_cast< MD::Blockquote< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( b1->items().size() == 1 );

		REQUIRE( b1->items().at( 0 )->type() == MD::ItemType::Blockquote );
		auto b2 = static_cast< MD::Blockquote< TRAIT >* > ( b1->items().at( 0 ).get() );

		REQUIRE( b2->items().size() == 1 );
		REQUIRE( b2->items().at( 0 )->type() == MD::ItemType::Blockquote );

		auto b3 = static_cast< MD::Blockquote< TRAIT >* > ( b2->items().at( 0 ).get() );
		REQUIRE( b3->items().size() == 1 );

		{
			REQUIRE( b3->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph< TRAIT >* > ( b3->items().at( 0 ).get() );
			REQUIRE( p->items().size() == 3 );

			{
				REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
				auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
				REQUIRE( t->opts() == MD::TextWithoutFormat );
				REQUIRE( t->text() == u8"foo" );
			}

			{
				REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
				auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );
				REQUIRE( t->opts() == MD::TextWithoutFormat );
				REQUIRE( t->text() == u8"bar" );
			}

			{
				REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );
				auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 2 ).get() );
				REQUIRE( t->opts() == MD::TextWithoutFormat );
				REQUIRE( t->text() == u8"baz" );
			}
		}
	}
}

TEST_CASE( "252" )
{
	const auto doc = load_test( 252 );

	REQUIRE( doc->items().size() == 3 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );
		auto b = static_cast< MD::Blockquote< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( b->items().size() == 1 );

		REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code< TRAIT >* > ( b->items().at( 0 ).get() );
		REQUIRE( !c->isInlined() );
		REQUIRE( c->text() == u8"code" );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Blockquote );
		auto b = static_cast< MD::Blockquote< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( b->items().size() == 1 );

		REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( b->items().at( 0 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"not code" );
	}
}
