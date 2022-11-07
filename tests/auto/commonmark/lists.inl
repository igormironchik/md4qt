
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

// 5.3 Lists

TEST_CASE( "301" )
{
	const auto doc = load_test( 301 );

	REQUIRE( doc->items().size() == 3 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
		auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).data() );
		REQUIRE( l->items().size() == 2 );

		{
			REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
			auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).data() );
			REQUIRE( li->items().size() == 1 );
			REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );
			REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).data() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "foo" ) );
		}

		{
			REQUIRE( l->items().at( 1 )->type() == MD::ItemType::ListItem );
			auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 1 ).data() );
			REQUIRE( li->items().size() == 1 );
			REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );
			REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).data() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "bar" ) );
		}
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::List );
		auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 2 ).data() );
		REQUIRE( l->items().size() == 1 );

		{
			REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
			auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).data() );
			REQUIRE( li->items().size() == 1 );
			REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );
			REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).data() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "baz" ) );
		}
	}
}

TEST_CASE( "302" )
{
	const auto doc = load_test( 302 );

	REQUIRE( doc->items().size() == 3 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
		auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).data() );
		REQUIRE( l->items().size() == 2 );

		{
			REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
			auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).data() );
			REQUIRE( li->items().size() == 1 );
			REQUIRE( li->listType() == MD::ListItem< TRAIT >::Ordered );
			REQUIRE( li->startNumber() == 1 );
			REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).data() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "foo" ) );
		}

		{
			REQUIRE( l->items().at( 1 )->type() == MD::ItemType::ListItem );
			auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 1 ).data() );
			REQUIRE( li->items().size() == 1 );
			REQUIRE( li->listType() == MD::ListItem< TRAIT >::Ordered );
			REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).data() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "bar" ) );
		}
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::List );
		auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 2 ).data() );
		REQUIRE( l->items().size() == 1 );

		{
			REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
			auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).data() );
			REQUIRE( li->items().size() == 1 );
			REQUIRE( li->listType() == MD::ListItem< TRAIT >::Ordered );
			REQUIRE( li->startNumber() == 3 );
			REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).data() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "baz" ) );
		}
	}
}

TEST_CASE( "303" )
{
	const auto doc = load_test( 303 );

	REQUIRE( doc->items().size() == 3 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).data() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "Foo" ) );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::List );
		auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 2 ).data() );
		REQUIRE( l->items().size() == 2 );

		{
			REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
			auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).data() );
			REQUIRE( li->items().size() == 1 );
			REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );
			REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).data() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "bar" ) );
		}

		{
			REQUIRE( l->items().at( 1 )->type() == MD::ItemType::ListItem );
			auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 1 ).data() );
			REQUIRE( li->items().size() == 1 );
			REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );
			REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).data() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "baz" ) );
		}
	}
}

TEST_CASE( "304" )
{
	const auto doc = load_test( 304 );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == QStringLiteral( "The number of windows in my house is 14. "
		"The number of doors is 6." ) );
}

TEST_CASE( "305" )
{
	const auto doc = load_test( 305 );

	REQUIRE( doc->items().size() == 3 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).data() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "The number of windows in my house is" ) );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::List );
		auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 2 ).data() );
		REQUIRE( l->items().size() == 1 );

		{
			REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
			auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).data() );
			REQUIRE( li->items().size() == 1 );
			REQUIRE( li->listType() == MD::ListItem< TRAIT >::Ordered );
			REQUIRE( li->startNumber() == 1 );
			REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).data() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "The number of doors is 6." ) );
		}
	}
}

TEST_CASE( "306" )
{
	const auto doc = load_test( 306 );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( l->items().size() == 3 );

	{
		REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).data() );
		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).data() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "foo" ) );
	}

	{
		REQUIRE( l->items().at( 1 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 1 ).data() );
		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).data() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "bar" ) );
	}

	{
		REQUIRE( l->items().at( 2 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 2 ).data() );
		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).data() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "baz" ) );
	}
}

TEST_CASE( "307" )
{
	const auto doc = load_test( 307 );

	REQUIRE( doc->items().size() == 2 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
		auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).data() );
		REQUIRE( l->items().size() == 1 );

		{
			REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
			auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).data() );
			REQUIRE( li->items().size() == 2 );
			REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );
			REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).data() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "foo" ) );

			REQUIRE( li->items().at( 1 )->type() == MD::ItemType::List );
			auto l = static_cast< MD::List< TRAIT >* > ( li->items().at( 1 ).data() );
			REQUIRE( l->items().size() == 1 );

			{
				REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
				auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).data() );
				REQUIRE( li->items().size() == 2 );
				REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );
				REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
				auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).data() );
				REQUIRE( p->items().size() == 1 );
				REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
				auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
				REQUIRE( t->opts() == MD::TextWithoutFormat );
				REQUIRE( t->text() == QStringLiteral( "bar" ) );

				REQUIRE( li->items().at( 1 )->type() == MD::ItemType::List );
				auto l = static_cast< MD::List< TRAIT >* > ( li->items().at( 1 ).data() );
				REQUIRE( l->items().size() == 1 );

				{
					REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
					auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).data() );
					REQUIRE( li->items().size() == 2 );
					REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );

					{
						REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
						auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).data() );
						REQUIRE( p->items().size() == 1 );
						REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
						auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
						REQUIRE( t->opts() == MD::TextWithoutFormat );
						REQUIRE( t->text() == QStringLiteral( "baz" ) );
					}

					{
						REQUIRE( li->items().at( 1 )->type() == MD::ItemType::Paragraph );
						auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 1 ).data() );
						REQUIRE( p->items().size() == 1 );
						REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
						auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
						REQUIRE( t->opts() == MD::TextWithoutFormat );
						REQUIRE( t->text() == QStringLiteral( "bim" ) );
					}
				}
			}
		}
	}
}

TEST_CASE( "308" )
{
	const auto doc = load_test( 308 );

	REQUIRE( doc->items().size() == 4 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
		auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).data() );
		REQUIRE( l->items().size() == 2 );

		{
			REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
			auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).data() );
			REQUIRE( li->items().size() == 1 );
			REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );
			REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).data() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "foo" ) );
		}

		{
			REQUIRE( l->items().at( 1 )->type() == MD::ItemType::ListItem );
			auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 1 ).data() );
			REQUIRE( li->items().size() == 1 );
			REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );
			REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).data() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "bar" ) );
		}
	}

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 2 ).data() );
	REQUIRE( h->text() == QStringLiteral( "<!-- -->" ) );

	{
		REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::List );
		auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 3 ).data() );
		REQUIRE( l->items().size() == 2 );

		{
			REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
			auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).data() );
			REQUIRE( li->items().size() == 1 );
			REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );
			REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).data() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "baz" ) );
		}

		{
			REQUIRE( l->items().at( 1 )->type() == MD::ItemType::ListItem );
			auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 1 ).data() );
			REQUIRE( li->items().size() == 1 );
			REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );
			REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).data() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "bim" ) );
		}
	}
}

TEST_CASE( "309" )
{
	const auto doc = load_test( 309 );

	REQUIRE( doc->items().size() == 4 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
		auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).data() );
		REQUIRE( l->items().size() == 2 );

		{
			REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
			auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).data() );
			REQUIRE( li->items().size() == 2 );
			REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );

			{
				REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
				auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).data() );
				REQUIRE( p->items().size() == 1 );
				REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
				auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
				REQUIRE( t->opts() == MD::TextWithoutFormat );
				REQUIRE( t->text() == QStringLiteral( "foo" ) );
			}

			{
				REQUIRE( li->items().at( 1 )->type() == MD::ItemType::Paragraph );
				auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 1 ).data() );
				REQUIRE( p->items().size() == 1 );
				REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
				auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
				REQUIRE( t->opts() == MD::TextWithoutFormat );
				REQUIRE( t->text() == QStringLiteral( "notcode" ) );
			}
		}

		{
			REQUIRE( l->items().at( 1 )->type() == MD::ItemType::ListItem );
			auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 1 ).data() );
			REQUIRE( li->items().size() == 1 );
			REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );
			REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).data() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "foo" ) );
		}
	}

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 2 ).data() );
	REQUIRE( h->text() == QStringLiteral( "<!-- -->" ) );

	REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 3 ).data() );
	REQUIRE( !c->isInlined() );
	REQUIRE( c->text() == QStringLiteral( "code" ) );
}

TEST_CASE( "310" )
{
	const auto doc = load_test( 310 );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( l->items().size() == 7 );

	for( int i = 0; i < 7; ++i )
	{
		REQUIRE( l->items().at( i )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( i ).data() );
		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).data() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QString( ( char ) ( 97 + i ) ) );
	}
}

TEST_CASE( "311" )
{
	const auto doc = load_test( 311 );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( l->items().size() == 3 );

	for( int i = 0; i < 3; ++i )
	{
		REQUIRE( l->items().at( i )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( i ).data() );
		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->listType() == MD::ListItem< TRAIT >::Ordered );
		REQUIRE( li->startNumber() == 1 + i );
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).data() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QString( ( char ) ( 97 + i ) ) );
	}
}

TEST_CASE( "312" )
{
	const auto doc = load_test( 312 );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( l->items().size() == 4 );

	for( int i = 0; i < 3; ++i )
	{
		REQUIRE( l->items().at( i )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( i ).data() );
		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).data() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QString( ( char ) ( 97 + i ) ) );
	}

	REQUIRE( l->items().at( 3 )->type() == MD::ItemType::ListItem );
	auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 3 ).data() );
	REQUIRE( li->items().size() == 1 );
	REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );
	REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).data() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == QStringLiteral( "d - e" ) );
}

TEST_CASE( "313" )
{
	const auto doc = load_test( 313 );

	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( l->items().size() == 2 );

	for( int i = 0; i < 2; ++i )
	{
		REQUIRE( l->items().at( i )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( i ).data() );
		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->listType() == MD::ListItem< TRAIT >::Ordered );
		REQUIRE( li->startNumber() == i + 1 );
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).data() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QString( ( char ) ( 97 + i ) ) );
	}

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 2 ).data() );
	REQUIRE( !c->isInlined() );
	REQUIRE( c->text() == QStringLiteral( "3. c" ) );
}

TEST_CASE( "314" )
{
	const auto doc = load_test( 314 );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( l->items().size() == 3 );

	for( int i = 0; i < 3; ++i )
	{
		REQUIRE( l->items().at( i )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( i ).data() );
		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).data() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QString( ( char ) ( 97 + i ) ) );
	}
}

TEST_CASE( "315" )
{
	const auto doc = load_test( 315 );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( l->items().size() == 2 );

	for( int i = 0; i < 2; ++i )
	{
		REQUIRE( l->items().at( i )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( i ).data() );
		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).data() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QString( ( char ) ( 97 + i + ( i % 2 > 0 ? 1 : 0 ) ) ) );
	}
}

TEST_CASE( "316" )
{
	const auto doc = load_test( 316 );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( l->items().size() == 3 );

	for( int i = 0; i < 3; i += 2 )
	{
		REQUIRE( l->items().at( i )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( i ).data() );
		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).data() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QString( ( char ) ( 97 + i + ( i > 0 ? 1 : 0 ) ) ) );
	}

	{
		REQUIRE( l->items().at( 1 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 1 ).data() );
		REQUIRE( li->items().size() == 2 );
		REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );

		{
			REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).data() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "b" ) );
		}

		{
			REQUIRE( li->items().at( 1 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 1 ).data() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "c" ) );
		}
	}
}

TEST_CASE( "317" )
{
	const auto doc = load_test( 317 );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( l->items().size() == 3 );

	{
		REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).data() );
		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).data() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "a" ) );
	}

	{
		REQUIRE( l->items().at( 1 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 1 ).data() );
		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).data() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "b" ) );
	}

	{
		REQUIRE( l->items().at( 2 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 2 ).data() );
		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).data() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "d" ) );
	}

	REQUIRE( !doc->labeledLinks().isEmpty() );
}

TEST_CASE( "318" )
{
	const auto doc = load_test( 318 );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( l->items().size() == 3 );

	{
		REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).data() );
		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).data() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "a" ) );
	}

	{
		REQUIRE( l->items().at( 1 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 1 ).data() );
		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code< TRAIT >* > ( li->items().at( 0 ).data() );
		REQUIRE( c->text() == QStringLiteral( "b\n\n" ) );
	}

	{
		REQUIRE( l->items().at( 2 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 2 ).data() );
		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).data() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "c" ) );
	}
}

TEST_CASE( "319" )
{
	const auto doc = load_test( 319 );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( l->items().size() == 2 );

	{
		REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).data() );
		REQUIRE( li->items().size() == 2 );
		REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).data() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "a" ) );

		REQUIRE( li->items().at( 1 )->type() == MD::ItemType::List );
		auto l = static_cast< MD::List< TRAIT >* > ( li->items().at( 1 ).data() );
		REQUIRE( l->items().size() == 1 );

		{
			REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
			auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).data() );
			REQUIRE( li->items().size() == 2 );
			REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );

			{
				REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
				auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).data() );
				REQUIRE( p->items().size() == 1 );
				REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
				auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
				REQUIRE( t->opts() == MD::TextWithoutFormat );
				REQUIRE( t->text() == QStringLiteral( "b" ) );
			}

			{
				REQUIRE( li->items().at( 1 )->type() == MD::ItemType::Paragraph );
				auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 1 ).data() );
				REQUIRE( p->items().size() == 1 );
				REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
				auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
				REQUIRE( t->opts() == MD::TextWithoutFormat );
				REQUIRE( t->text() == QStringLiteral( "c" ) );
			}
		}
	}

	{
		REQUIRE( l->items().at( 1 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 1 ).data() );
		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).data() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "d" ) );
	}
}

TEST_CASE( "320" )
{
	const auto doc = load_test( 320 );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( l->items().size() == 2 );

	{
		REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).data() );
		REQUIRE( li->items().size() == 2 );
		REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );

		{
			REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).data() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "a" ) );
		}

		REQUIRE( li->items().at( 1 )->type() == MD::ItemType::Blockquote );
		auto b = static_cast< MD::Blockquote< TRAIT >* > ( li->items().at( 1 ).data() );
		REQUIRE( b->items().size() == 1 );
		REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( b->items().at( 0 ).data() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "b" ) );
	}

	{
		REQUIRE( l->items().at( 1 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 1 ).data() );
		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );

		{
			REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).data() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "c" ) );
		}
	}
}

TEST_CASE( "321" )
{
	const auto doc = load_test( 321 );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( l->items().size() == 2 );

	{
		REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).data() );
		REQUIRE( li->items().size() == 3 );
		REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );

		{
			REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).data() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "a" ) );
		}

		REQUIRE( li->items().at( 1 )->type() == MD::ItemType::Blockquote );
		auto b = static_cast< MD::Blockquote< TRAIT >* > ( li->items().at( 1 ).data() );
		REQUIRE( b->items().size() == 1 );
		REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( b->items().at( 0 ).data() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "b" ) );

		REQUIRE( li->items().at( 2 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code< TRAIT >* > ( li->items().at( 2 ).data() );
		REQUIRE( c->text() == QStringLiteral( "c" ) );
	}

	{
		REQUIRE( l->items().at( 1 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 1 ).data() );
		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );

		{
			REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).data() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "d" ) );
		}
	}
}

TEST_CASE( "322" )
{
	const auto doc = load_test( 322 );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( l->items().size() == 1 );

	{
		REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).data() );
		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );

		{
			REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).data() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "a" ) );
		}
	}
}

TEST_CASE( "323" )
{
	const auto doc = load_test( 323 );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( l->items().size() == 1 );

	{
		REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).data() );
		REQUIRE( li->items().size() == 2 );
		REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );

		{
			REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).data() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "a" ) );
		}

		REQUIRE( li->items().at( 1 )->type() == MD::ItemType::List );
		auto l = static_cast< MD::List< TRAIT >* > ( li->items().at( 1 ).data() );
		REQUIRE( l->items().size() == 1 );

		{
			REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
			auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).data() );
			REQUIRE( li->items().size() == 1 );
			REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );

			{
				REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
				auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).data() );
				REQUIRE( p->items().size() == 1 );
				REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
				auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
				REQUIRE( t->opts() == MD::TextWithoutFormat );
				REQUIRE( t->text() == QStringLiteral( "b" ) );
			}
		}
	}
}

TEST_CASE( "324" )
{
	const auto doc = load_test( 324 );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( l->items().size() == 1 );

	{
		REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).data() );
		REQUIRE( li->items().size() == 2 );
		REQUIRE( li->listType() == MD::ListItem< TRAIT >::Ordered );
		REQUIRE( li->startNumber() == 1 );

		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code< TRAIT >* > ( li->items().at( 0 ).data() );
		REQUIRE( c->text() == QStringLiteral( "foo" ) );

		{
			REQUIRE( li->items().at( 1 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 1 ).data() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "bar" ) );
		}
	}
}

TEST_CASE( "325" )
{
	const auto doc = load_test( 325 );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( l->items().size() == 1 );

	{
		REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).data() );
		REQUIRE( li->items().size() == 3 );
		REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );

		{
			REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).data() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "foo" ) );
		}

		{
			REQUIRE( li->items().at( 1 )->type() == MD::ItemType::List );
			auto l = static_cast< MD::List< TRAIT >* > ( li->items().at( 1 ).data() );
			REQUIRE( l->items().size() == 1 );

			{
				REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
				auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).data() );
				REQUIRE( li->items().size() == 1 );
				REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );

				{
					REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
					auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).data() );
					REQUIRE( p->items().size() == 1 );
					REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
					auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
					REQUIRE( t->opts() == MD::TextWithoutFormat );
					REQUIRE( t->text() == QStringLiteral( "bar" ) );
				}
			}
		}

		{
			REQUIRE( li->items().at( 2 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 2 ).data() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "baz" ) );
		}
	}
}

TEST_CASE( "326" )
{
	const auto doc = load_test( 326 );

	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( l->items().size() == 2 );

	for( int i = 0; i < 2; ++i )
	{
		REQUIRE( l->items().at( i )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( i ).data() );
		REQUIRE( li->items().size() == 2 );
		REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );

		{
			REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
			auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).data() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QString( (char) ( 97 + i * 3 ) ) );
		}

		{
			REQUIRE( li->items().at( 1 )->type() == MD::ItemType::List );
			auto l = static_cast< MD::List< TRAIT >* > ( li->items().at( 1 ).data() );
			REQUIRE( l->items().size() == 2 );

			for( int j = 0; j < 2; ++j )
			{
				REQUIRE( l->items().at( j )->type() == MD::ItemType::ListItem );
				auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( j ).data() );
				REQUIRE( li->items().size() == 1 );
				REQUIRE( li->listType() == MD::ListItem< TRAIT >::Unordered );

				{
					REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
					auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).data() );
					REQUIRE( p->items().size() == 1 );
					REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
					auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
					REQUIRE( t->opts() == MD::TextWithoutFormat );
					REQUIRE( t->text() == QString( (char) ( 98 + i * 3 + j ) ) );
				}
			}
		}
	}
}
