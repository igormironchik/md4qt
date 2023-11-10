
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

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <md4qt/parser.hpp>
#include <md4qt/traits.hpp>


inline std::shared_ptr< MD::Document< TRAIT > >
load_test( int n )
{
	auto fileName = std::to_string( n );

	if( fileName.size() < 3 )
		fileName.insert( 0, 3 - fileName.size(), '0' );

	fileName.insert( 0, "tests/gfm/data/" );
	fileName.append( ".md" );

	MD::Parser< TRAIT > p;

	return p.parse(
#ifdef MD4QT_QT_SUPPORT
		QString::fromStdString( fileName ),
#else
		fileName,
#endif
		false );
}


TEST_CASE( "198" )
{
	const auto doc = load_test( 198 );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Table );
	const auto t = static_cast< MD::Table< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( t->columnsCount() == 2 );
	REQUIRE( t->rows().size() == 2 );

	REQUIRE( t->columnAlignment( 0 ) == MD::Table< TRAIT >::AlignLeft );
	REQUIRE( t->columnAlignment( 1 ) == MD::Table< TRAIT >::AlignLeft );

	{
		REQUIRE( t->rows().at( 0 )->type() == MD::ItemType::TableRow );
		const auto r = static_cast< MD::TableRow< TRAIT >* > ( t->rows().at( 0 ).get() );
		REQUIRE( r->cells().size() == 2 );

		{
			REQUIRE( r->cells().at( 0 )->type() == MD::ItemType::TableCell );
			const auto c = static_cast< MD::TableCell< TRAIT >* > ( r->cells().at( 0 ).get() );
			REQUIRE( c->items().size() == 1 );
			REQUIRE( c->items().at( 0 )->type() == MD::ItemType::Text );
			const auto t = static_cast< MD::Text< TRAIT >* > ( c->items().at( 0 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"foo" );
		}

		{
			REQUIRE( r->cells().at( 1 )->type() == MD::ItemType::TableCell );
			const auto c = static_cast< MD::TableCell< TRAIT >* > ( r->cells().at( 1 ).get() );
			REQUIRE( c->items().size() == 1 );
			REQUIRE( c->items().at( 0 )->type() == MD::ItemType::Text );
			const auto t = static_cast< MD::Text< TRAIT >* > ( c->items().at( 0 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"bar" );
		}
	}

	{
		REQUIRE( t->rows().at( 1 )->type() == MD::ItemType::TableRow );
		const auto r = static_cast< MD::TableRow< TRAIT >* > ( t->rows().at( 1 ).get() );
		REQUIRE( r->cells().size() == 2 );

		{
			REQUIRE( r->cells().at( 0 )->type() == MD::ItemType::TableCell );
			const auto c = static_cast< MD::TableCell< TRAIT >* > ( r->cells().at( 0 ).get() );
			REQUIRE( c->items().size() == 1 );
			REQUIRE( c->items().at( 0 )->type() == MD::ItemType::Text );
			const auto t = static_cast< MD::Text< TRAIT >* > ( c->items().at( 0 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"baz" );
		}

		{
			REQUIRE( r->cells().at( 1 )->type() == MD::ItemType::TableCell );
			const auto c = static_cast< MD::TableCell< TRAIT >* > ( r->cells().at( 1 ).get() );
			REQUIRE( c->items().size() == 1 );
			REQUIRE( c->items().at( 0 )->type() == MD::ItemType::Text );
			const auto t = static_cast< MD::Text< TRAIT >* > ( c->items().at( 0 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"bim" );
		}
	}
}

TEST_CASE( "199" )
{
	const auto doc = load_test( 199 );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Table );
	const auto t = static_cast< MD::Table< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( t->columnsCount() == 2 );
	REQUIRE( t->rows().size() == 2 );

	REQUIRE( t->columnAlignment( 0 ) == MD::Table< TRAIT >::AlignCenter );
	REQUIRE( t->columnAlignment( 1 ) == MD::Table< TRAIT >::AlignRight );

	{
		REQUIRE( t->rows().at( 0 )->type() == MD::ItemType::TableRow );
		const auto r = static_cast< MD::TableRow< TRAIT >* > ( t->rows().at( 0 ).get() );
		REQUIRE( r->cells().size() == 2 );

		{
			REQUIRE( r->cells().at( 0 )->type() == MD::ItemType::TableCell );
			const auto c = static_cast< MD::TableCell< TRAIT >* > ( r->cells().at( 0 ).get() );
			REQUIRE( c->items().size() == 1 );
			REQUIRE( c->items().at( 0 )->type() == MD::ItemType::Text );
			const auto t = static_cast< MD::Text< TRAIT >* > ( c->items().at( 0 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"abc" );
		}

		{
			REQUIRE( r->cells().at( 1 )->type() == MD::ItemType::TableCell );
			const auto c = static_cast< MD::TableCell< TRAIT >* > ( r->cells().at( 1 ).get() );
			REQUIRE( c->items().size() == 1 );
			REQUIRE( c->items().at( 0 )->type() == MD::ItemType::Text );
			const auto t = static_cast< MD::Text< TRAIT >* > ( c->items().at( 0 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"defghi" );
		}
	}

	{
		REQUIRE( t->rows().at( 1 )->type() == MD::ItemType::TableRow );
		const auto r = static_cast< MD::TableRow< TRAIT >* > ( t->rows().at( 1 ).get() );
		REQUIRE( r->cells().size() == 2 );

		{
			REQUIRE( r->cells().at( 0 )->type() == MD::ItemType::TableCell );
			const auto c = static_cast< MD::TableCell< TRAIT >* > ( r->cells().at( 0 ).get() );
			REQUIRE( c->items().size() == 1 );
			REQUIRE( c->items().at( 0 )->type() == MD::ItemType::Text );
			const auto t = static_cast< MD::Text< TRAIT >* > ( c->items().at( 0 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"bar" );
		}

		{
			REQUIRE( r->cells().at( 1 )->type() == MD::ItemType::TableCell );
			const auto c = static_cast< MD::TableCell< TRAIT >* > ( r->cells().at( 1 ).get() );
			REQUIRE( c->items().size() == 1 );
			REQUIRE( c->items().at( 0 )->type() == MD::ItemType::Text );
			const auto t = static_cast< MD::Text< TRAIT >* > ( c->items().at( 0 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"baz" );
		}
	}
}

TEST_CASE( "200" )
{
	const auto doc = load_test( 200 );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Table );
	const auto t = static_cast< MD::Table< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( t->columnsCount() == 1 );
	REQUIRE( t->rows().size() == 3 );

	REQUIRE( t->columnAlignment( 0 ) == MD::Table< TRAIT >::AlignLeft );

	{
		REQUIRE( t->rows().at( 0 )->type() == MD::ItemType::TableRow );
		const auto r = static_cast< MD::TableRow< TRAIT >* > ( t->rows().at( 0 ).get() );
		REQUIRE( r->cells().size() == 1 );

		REQUIRE( r->cells().at( 0 )->type() == MD::ItemType::TableCell );
		const auto c = static_cast< MD::TableCell< TRAIT >* > ( r->cells().at( 0 ).get() );
		REQUIRE( c->items().size() == 1 );
		REQUIRE( c->items().at( 0 )->type() == MD::ItemType::Text );
		const auto t = static_cast< MD::Text< TRAIT >* > ( c->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"f|oo" );
	}

	{
		REQUIRE( t->rows().at( 1 )->type() == MD::ItemType::TableRow );
		const auto r = static_cast< MD::TableRow< TRAIT >* > ( t->rows().at( 1 ).get() );
		REQUIRE( r->cells().size() == 1 );

		REQUIRE( r->cells().at( 0 )->type() == MD::ItemType::TableCell );
		const auto c = static_cast< MD::TableCell< TRAIT >* > ( r->cells().at( 0 ).get() );
		REQUIRE( c->items().size() == 3 );

		{
			REQUIRE( c->items().at( 0 )->type() == MD::ItemType::Text );
			const auto t = static_cast< MD::Text< TRAIT >* > ( c->items().at( 0 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"b" );
		}

		{
			REQUIRE( c->items().at( 1 )->type() == MD::ItemType::Code );
			const auto cc = static_cast< MD::Code< TRAIT >* > ( c->items().at( 1 ).get() );
			REQUIRE( cc->text() == u8"|" );
		}

		{
			REQUIRE( c->items().at( 2 )->type() == MD::ItemType::Text );
			const auto t = static_cast< MD::Text< TRAIT >* > ( c->items().at( 2 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"az" );
		}
	}

	{
		REQUIRE( t->rows().at( 2 )->type() == MD::ItemType::TableRow );
		const auto r = static_cast< MD::TableRow< TRAIT >* > ( t->rows().at( 2 ).get() );
		REQUIRE( r->cells().size() == 1 );

		REQUIRE( r->cells().at( 0 )->type() == MD::ItemType::TableCell );
		const auto c = static_cast< MD::TableCell< TRAIT >* > ( r->cells().at( 0 ).get() );
		REQUIRE( c->items().size() == 3 );

		{
			REQUIRE( c->items().at( 0 )->type() == MD::ItemType::Text );
			const auto t = static_cast< MD::Text< TRAIT >* > ( c->items().at( 0 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"b" );
		}

		{
			REQUIRE( c->items().at( 1 )->type() == MD::ItemType::Text );
			const auto t = static_cast< MD::Text< TRAIT >* > ( c->items().at( 1 ).get() );
			REQUIRE( t->opts() == MD::BoldText );
			REQUIRE( t->text() == u8"|" );
		}

		{
			REQUIRE( c->items().at( 2 )->type() == MD::ItemType::Text );
			const auto t = static_cast< MD::Text< TRAIT >* > ( c->items().at( 2 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"im" );
		}
	}
}

TEST_CASE( "201" )
{
	const auto doc = load_test( 201 );
	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Table );
	const auto t = static_cast< MD::Table< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( t->columnsCount() == 2 );
	REQUIRE( t->rows().size() == 2 );

	REQUIRE( t->columnAlignment( 0 ) == MD::Table< TRAIT >::AlignLeft );
	REQUIRE( t->columnAlignment( 1 ) == MD::Table< TRAIT >::AlignLeft );

	{
		REQUIRE( t->rows().at( 0 )->type() == MD::ItemType::TableRow );
		const auto r = static_cast< MD::TableRow< TRAIT >* > ( t->rows().at( 0 ).get() );
		REQUIRE( r->cells().size() == 2 );

		{
			REQUIRE( r->cells().at( 0 )->type() == MD::ItemType::TableCell );
			const auto c = static_cast< MD::TableCell< TRAIT >* > ( r->cells().at( 0 ).get() );
			REQUIRE( c->items().size() == 1 );
			REQUIRE( c->items().at( 0 )->type() == MD::ItemType::Text );
			const auto t = static_cast< MD::Text< TRAIT >* > ( c->items().at( 0 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"abc" );
		}

		{
			REQUIRE( r->cells().at( 1 )->type() == MD::ItemType::TableCell );
			const auto c = static_cast< MD::TableCell< TRAIT >* > ( r->cells().at( 1 ).get() );
			REQUIRE( c->items().size() == 1 );
			REQUIRE( c->items().at( 0 )->type() == MD::ItemType::Text );
			const auto t = static_cast< MD::Text< TRAIT >* > ( c->items().at( 0 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"def" );
		}
	}

	{
		REQUIRE( t->rows().at( 1 )->type() == MD::ItemType::TableRow );
		const auto r = static_cast< MD::TableRow< TRAIT >* > ( t->rows().at( 1 ).get() );
		REQUIRE( r->cells().size() == 2 );

		{
			REQUIRE( r->cells().at( 0 )->type() == MD::ItemType::TableCell );
			const auto c = static_cast< MD::TableCell< TRAIT >* > ( r->cells().at( 0 ).get() );
			REQUIRE( c->items().size() == 1 );
			REQUIRE( c->items().at( 0 )->type() == MD::ItemType::Text );
			const auto t = static_cast< MD::Text< TRAIT >* > ( c->items().at( 0 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"bar" );
		}

		{
			REQUIRE( r->cells().at( 1 )->type() == MD::ItemType::TableCell );
			const auto c = static_cast< MD::TableCell< TRAIT >* > ( r->cells().at( 1 ).get() );
			REQUIRE( c->items().size() == 1 );
			REQUIRE( c->items().at( 0 )->type() == MD::ItemType::Text );
			const auto t = static_cast< MD::Text< TRAIT >* > ( c->items().at( 0 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"baz" );
		}
	}

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Blockquote );
	const auto b = static_cast< MD::Blockquote< TRAIT >* > ( doc->items().at( 2 ).get() );
	REQUIRE( b->items().size() == 1 );
	REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Paragraph );
	const auto p = static_cast< MD::Paragraph< TRAIT >* > ( b->items().at( 0 ).get() );
	REQUIRE( p->items().size() == 1 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		const auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"bar" );
	}
}

TEST_CASE( "202" )
{
	const auto doc = load_test( 202 );
	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Table );
	const auto t = static_cast< MD::Table< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( t->columnsCount() == 2 );
	REQUIRE( t->rows().size() == 3 );

	REQUIRE( t->columnAlignment( 0 ) == MD::Table< TRAIT >::AlignLeft );
	REQUIRE( t->columnAlignment( 1 ) == MD::Table< TRAIT >::AlignLeft );

	{
		REQUIRE( t->rows().at( 0 )->type() == MD::ItemType::TableRow );
		const auto r = static_cast< MD::TableRow< TRAIT >* > ( t->rows().at( 0 ).get() );
		REQUIRE( r->cells().size() == 2 );

		{
			REQUIRE( r->cells().at( 0 )->type() == MD::ItemType::TableCell );
			const auto c = static_cast< MD::TableCell< TRAIT >* > ( r->cells().at( 0 ).get() );
			REQUIRE( c->items().size() == 1 );
			REQUIRE( c->items().at( 0 )->type() == MD::ItemType::Text );
			const auto t = static_cast< MD::Text< TRAIT >* > ( c->items().at( 0 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"abc" );
		}

		{
			REQUIRE( r->cells().at( 1 )->type() == MD::ItemType::TableCell );
			const auto c = static_cast< MD::TableCell< TRAIT >* > ( r->cells().at( 1 ).get() );
			REQUIRE( c->items().size() == 1 );
			REQUIRE( c->items().at( 0 )->type() == MD::ItemType::Text );
			const auto t = static_cast< MD::Text< TRAIT >* > ( c->items().at( 0 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"def" );
		}
	}

	{
		REQUIRE( t->rows().at( 1 )->type() == MD::ItemType::TableRow );
		const auto r = static_cast< MD::TableRow< TRAIT >* > ( t->rows().at( 1 ).get() );
		REQUIRE( r->cells().size() == 2 );

		{
			REQUIRE( r->cells().at( 0 )->type() == MD::ItemType::TableCell );
			const auto c = static_cast< MD::TableCell< TRAIT >* > ( r->cells().at( 0 ).get() );
			REQUIRE( c->items().size() == 1 );
			REQUIRE( c->items().at( 0 )->type() == MD::ItemType::Text );
			const auto t = static_cast< MD::Text< TRAIT >* > ( c->items().at( 0 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"bar" );
		}

		{
			REQUIRE( r->cells().at( 1 )->type() == MD::ItemType::TableCell );
			const auto c = static_cast< MD::TableCell< TRAIT >* > ( r->cells().at( 1 ).get() );
			REQUIRE( c->items().size() == 1 );
			REQUIRE( c->items().at( 0 )->type() == MD::ItemType::Text );
			const auto t = static_cast< MD::Text< TRAIT >* > ( c->items().at( 0 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"baz" );
		}
	}

	{
		REQUIRE( t->rows().at( 2 )->type() == MD::ItemType::TableRow );
		const auto r = static_cast< MD::TableRow< TRAIT >* > ( t->rows().at( 2 ).get() );
		REQUIRE( r->cells().size() == 1 );

		{
			REQUIRE( r->cells().at( 0 )->type() == MD::ItemType::TableCell );
			const auto c = static_cast< MD::TableCell< TRAIT >* > ( r->cells().at( 0 ).get() );
			REQUIRE( c->items().size() == 1 );
			REQUIRE( c->items().at( 0 )->type() == MD::ItemType::Text );
			const auto t = static_cast< MD::Text< TRAIT >* > ( c->items().at( 0 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"bar" );
		}
	}

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );
	const auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).get() );
	REQUIRE( p->items().size() == 1 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		const auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"bar" );
	}
}

TEST_CASE( "203" )
{
	const auto doc = load_test( 203 );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	const auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 3 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		const auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"| abc | def |" );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
		const auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"| --- |" );
	}

	{
		REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );
		const auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 2 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"| bar |" );
	}
}

TEST_CASE( "204" )
{
	const auto doc = load_test( 204 );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Table );
	const auto t = static_cast< MD::Table< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( t->columnsCount() == 2 );
	REQUIRE( t->rows().size() == 3 );

	REQUIRE( t->columnAlignment( 0 ) == MD::Table< TRAIT >::AlignLeft );
	REQUIRE( t->columnAlignment( 1 ) == MD::Table< TRAIT >::AlignLeft );

	{
		REQUIRE( t->rows().at( 0 )->type() == MD::ItemType::TableRow );
		const auto r = static_cast< MD::TableRow< TRAIT >* > ( t->rows().at( 0 ).get() );
		REQUIRE( r->cells().size() == 2 );

		{
			REQUIRE( r->cells().at( 0 )->type() == MD::ItemType::TableCell );
			const auto c = static_cast< MD::TableCell< TRAIT >* > ( r->cells().at( 0 ).get() );
			REQUIRE( c->items().size() == 1 );
			REQUIRE( c->items().at( 0 )->type() == MD::ItemType::Text );
			const auto t = static_cast< MD::Text< TRAIT >* > ( c->items().at( 0 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"abc" );
		}

		{
			REQUIRE( r->cells().at( 1 )->type() == MD::ItemType::TableCell );
			const auto c = static_cast< MD::TableCell< TRAIT >* > ( r->cells().at( 1 ).get() );
			REQUIRE( c->items().size() == 1 );
			REQUIRE( c->items().at( 0 )->type() == MD::ItemType::Text );
			const auto t = static_cast< MD::Text< TRAIT >* > ( c->items().at( 0 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"def" );
		}
	}

	{
		REQUIRE( t->rows().at( 1 )->type() == MD::ItemType::TableRow );
		const auto r = static_cast< MD::TableRow< TRAIT >* > ( t->rows().at( 1 ).get() );
		REQUIRE( r->cells().size() == 1 );

		{
			REQUIRE( r->cells().at( 0 )->type() == MD::ItemType::TableCell );
			const auto c = static_cast< MD::TableCell< TRAIT >* > ( r->cells().at( 0 ).get() );
			REQUIRE( c->items().size() == 1 );
			REQUIRE( c->items().at( 0 )->type() == MD::ItemType::Text );
			const auto t = static_cast< MD::Text< TRAIT >* > ( c->items().at( 0 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"bar" );
		}
	}

	{
		REQUIRE( t->rows().at( 2 )->type() == MD::ItemType::TableRow );
		const auto r = static_cast< MD::TableRow< TRAIT >* > ( t->rows().at( 2 ).get() );
		REQUIRE( r->cells().size() == 2 );

		{
			REQUIRE( r->cells().at( 0 )->type() == MD::ItemType::TableCell );
			const auto c = static_cast< MD::TableCell< TRAIT >* > ( r->cells().at( 0 ).get() );
			REQUIRE( c->items().size() == 1 );
			REQUIRE( c->items().at( 0 )->type() == MD::ItemType::Text );
			const auto t = static_cast< MD::Text< TRAIT >* > ( c->items().at( 0 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"bar" );
		}

		{
			REQUIRE( r->cells().at( 1 )->type() == MD::ItemType::TableCell );
			const auto c = static_cast< MD::TableCell< TRAIT >* > ( r->cells().at( 1 ).get() );
			REQUIRE( c->items().size() == 1 );
			REQUIRE( c->items().at( 0 )->type() == MD::ItemType::Text );
			const auto t = static_cast< MD::Text< TRAIT >* > ( c->items().at( 0 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"baz" );
		}
	}
}

TEST_CASE( "205" )
{
	const auto doc = load_test( 205 );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Table );
	const auto t = static_cast< MD::Table< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( t->columnsCount() == 2 );
	REQUIRE( t->rows().size() == 1 );

	REQUIRE( t->columnAlignment( 0 ) == MD::Table< TRAIT >::AlignLeft );
	REQUIRE( t->columnAlignment( 1 ) == MD::Table< TRAIT >::AlignLeft );

	{
		REQUIRE( t->rows().at( 0 )->type() == MD::ItemType::TableRow );
		const auto r = static_cast< MD::TableRow< TRAIT >* > ( t->rows().at( 0 ).get() );
		REQUIRE( r->cells().size() == 2 );

		{
			REQUIRE( r->cells().at( 0 )->type() == MD::ItemType::TableCell );
			const auto c = static_cast< MD::TableCell< TRAIT >* > ( r->cells().at( 0 ).get() );
			REQUIRE( c->items().size() == 1 );
			REQUIRE( c->items().at( 0 )->type() == MD::ItemType::Text );
			const auto t = static_cast< MD::Text< TRAIT >* > ( c->items().at( 0 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"abc" );
		}

		{
			REQUIRE( r->cells().at( 1 )->type() == MD::ItemType::TableCell );
			const auto c = static_cast< MD::TableCell< TRAIT >* > ( r->cells().at( 1 ).get() );
			REQUIRE( c->items().size() == 1 );
			REQUIRE( c->items().at( 0 )->type() == MD::ItemType::Text );
			const auto t = static_cast< MD::Text< TRAIT >* > ( c->items().at( 0 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"def" );
		}
	}
}

TEST_CASE( "279" )
{
	const auto doc = load_test( 279 );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	const auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->items().size() == 2 );

	{
		REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
		const auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
		REQUIRE( li->isTaskList() );
		REQUIRE( !li->isChecked() );
		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		const auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		const auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"foo" );
	}

	{
		REQUIRE( l->items().at( 1 )->type() == MD::ItemType::ListItem );
		const auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 1 ).get() );
		REQUIRE( li->isTaskList() );
		REQUIRE( li->isChecked() );
		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		const auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		const auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"bar" );
	}
}

TEST_CASE( "280" )
{
	const auto doc = load_test( 280 );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	const auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->items().size() == 2 );

	{
		REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
		const auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
		REQUIRE( li->isTaskList() );
		REQUIRE( li->isChecked() );
		REQUIRE( li->items().size() == 2 );
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		const auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		const auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"foo" );

		{
			REQUIRE( li->items().at( 1 )->type() == MD::ItemType::List );
			const auto l = static_cast< MD::List< TRAIT >* > ( li->items().at( 1 ).get() );
			REQUIRE( l->items().size() == 2 );

			{
				REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
				const auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
				REQUIRE( li->isTaskList() );
				REQUIRE( !li->isChecked() );
				REQUIRE( li->items().size() == 1 );
				REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
				const auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
				REQUIRE( p->items().size() == 1 );
				REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
				const auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
				REQUIRE( t->opts() == MD::TextWithoutFormat );
				REQUIRE( t->text() == u8"bar" );
			}

			{
				REQUIRE( l->items().at( 1 )->type() == MD::ItemType::ListItem );
				const auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 1 ).get() );
				REQUIRE( li->isTaskList() );
				REQUIRE( li->isChecked() );
				REQUIRE( li->items().size() == 1 );
				REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
				const auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
				REQUIRE( p->items().size() == 1 );
				REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
				const auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
				REQUIRE( t->opts() == MD::TextWithoutFormat );
				REQUIRE( t->text() == u8"baz" );
			}
		}
	}

	{
		REQUIRE( l->items().at( 1 )->type() == MD::ItemType::ListItem );
		const auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 1 ).get() );
		REQUIRE( li->isTaskList() );
		REQUIRE( !li->isChecked() );
		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		const auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		const auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"bim" );
	}
}

TEST_CASE( "491" )
{
	const auto doc = load_test( 491 );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	const auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 2 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		const auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::StrikethroughText );
		REQUIRE( t->text() == u8"Hi" );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
		const auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Hello, world!" );
	}
}

TEST_CASE( "492" )
{
	const auto doc = load_test( 492 );
	REQUIRE( doc->items().size() == 3 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
		const auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		const auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"This ~~has a" );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );
		const auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		const auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"new paragraph~~." );
	}
}
