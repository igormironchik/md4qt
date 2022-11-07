
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

#define MD4QT_QT_SUPPORT
#define TRAIT MD::QStringTrait

#include <md4qt/parser.hpp>
#include <md4qt/traits.hpp>

#include <QDir>


inline typename TRAIT::template SharedPointer< MD::Document< TRAIT > >
load_test( int n )
{
	auto fileName = QString::number( n );

	if( fileName.size() < 3 )
		fileName.prepend( QString( 3 - fileName.size(), QLatin1Char( '0' ) ) );

	fileName.prepend( QStringLiteral( "tests/gfm/data/" ) );
	fileName.append( QStringLiteral( ".md" ) );

	MD::Parser< TRAIT > p;

	return p.parse( fileName, false );
}


TEST_CASE( "198" )
{
	const auto doc = load_test( 198 );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Table );
	const auto t = static_cast< MD::Table< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( t->columnsCount() == 2 );
	REQUIRE( t->rows().size() == 2 );

	REQUIRE( t->columnAlignment( 0 ) == MD::Table< TRAIT >::AlignLeft );
	REQUIRE( t->columnAlignment( 1 ) == MD::Table< TRAIT >::AlignLeft );

	{
		REQUIRE( t->rows().at( 0 )->type() == MD::ItemType::TableRow );
		const auto r = static_cast< MD::TableRow< TRAIT >* > ( t->rows().at( 0 ).data() );
		REQUIRE( r->cells().size() == 2 );

		{
			REQUIRE( r->cells().at( 0 )->type() == MD::ItemType::TableCell );
			const auto c = static_cast< MD::TableCell< TRAIT >* > ( r->cells().at( 0 ).data() );
			REQUIRE( c->items().size() == 1 );
			REQUIRE( c->items().at( 0 )->type() == MD::ItemType::Text );
			const auto t = static_cast< MD::Text< TRAIT >* > ( c->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "foo" ) );
		}

		{
			REQUIRE( r->cells().at( 1 )->type() == MD::ItemType::TableCell );
			const auto c = static_cast< MD::TableCell< TRAIT >* > ( r->cells().at( 1 ).data() );
			REQUIRE( c->items().size() == 1 );
			REQUIRE( c->items().at( 0 )->type() == MD::ItemType::Text );
			const auto t = static_cast< MD::Text< TRAIT >* > ( c->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "bar" ) );
		}
	}

	{
		REQUIRE( t->rows().at( 1 )->type() == MD::ItemType::TableRow );
		const auto r = static_cast< MD::TableRow< TRAIT >* > ( t->rows().at( 1 ).data() );
		REQUIRE( r->cells().size() == 2 );

		{
			REQUIRE( r->cells().at( 0 )->type() == MD::ItemType::TableCell );
			const auto c = static_cast< MD::TableCell< TRAIT >* > ( r->cells().at( 0 ).data() );
			REQUIRE( c->items().size() == 1 );
			REQUIRE( c->items().at( 0 )->type() == MD::ItemType::Text );
			const auto t = static_cast< MD::Text< TRAIT >* > ( c->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "baz" ) );
		}

		{
			REQUIRE( r->cells().at( 1 )->type() == MD::ItemType::TableCell );
			const auto c = static_cast< MD::TableCell< TRAIT >* > ( r->cells().at( 1 ).data() );
			REQUIRE( c->items().size() == 1 );
			REQUIRE( c->items().at( 0 )->type() == MD::ItemType::Text );
			const auto t = static_cast< MD::Text< TRAIT >* > ( c->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "bim" ) );
		}
	}
}

TEST_CASE( "199" )
{
	const auto doc = load_test( 199 );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Table );
	const auto t = static_cast< MD::Table< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( t->columnsCount() == 2 );
	REQUIRE( t->rows().size() == 2 );

	REQUIRE( t->columnAlignment( 0 ) == MD::Table< TRAIT >::AlignCenter );
	REQUIRE( t->columnAlignment( 1 ) == MD::Table< TRAIT >::AlignRight );

	{
		REQUIRE( t->rows().at( 0 )->type() == MD::ItemType::TableRow );
		const auto r = static_cast< MD::TableRow< TRAIT >* > ( t->rows().at( 0 ).data() );
		REQUIRE( r->cells().size() == 2 );

		{
			REQUIRE( r->cells().at( 0 )->type() == MD::ItemType::TableCell );
			const auto c = static_cast< MD::TableCell< TRAIT >* > ( r->cells().at( 0 ).data() );
			REQUIRE( c->items().size() == 1 );
			REQUIRE( c->items().at( 0 )->type() == MD::ItemType::Text );
			const auto t = static_cast< MD::Text< TRAIT >* > ( c->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "abc" ) );
		}

		{
			REQUIRE( r->cells().at( 1 )->type() == MD::ItemType::TableCell );
			const auto c = static_cast< MD::TableCell< TRAIT >* > ( r->cells().at( 1 ).data() );
			REQUIRE( c->items().size() == 1 );
			REQUIRE( c->items().at( 0 )->type() == MD::ItemType::Text );
			const auto t = static_cast< MD::Text< TRAIT >* > ( c->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "defghi" ) );
		}
	}

	{
		REQUIRE( t->rows().at( 1 )->type() == MD::ItemType::TableRow );
		const auto r = static_cast< MD::TableRow< TRAIT >* > ( t->rows().at( 1 ).data() );
		REQUIRE( r->cells().size() == 2 );

		{
			REQUIRE( r->cells().at( 0 )->type() == MD::ItemType::TableCell );
			const auto c = static_cast< MD::TableCell< TRAIT >* > ( r->cells().at( 0 ).data() );
			REQUIRE( c->items().size() == 1 );
			REQUIRE( c->items().at( 0 )->type() == MD::ItemType::Text );
			const auto t = static_cast< MD::Text< TRAIT >* > ( c->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "bar" ) );
		}

		{
			REQUIRE( r->cells().at( 1 )->type() == MD::ItemType::TableCell );
			const auto c = static_cast< MD::TableCell< TRAIT >* > ( r->cells().at( 1 ).data() );
			REQUIRE( c->items().size() == 1 );
			REQUIRE( c->items().at( 0 )->type() == MD::ItemType::Text );
			const auto t = static_cast< MD::Text< TRAIT >* > ( c->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "baz" ) );
		}
	}
}

TEST_CASE( "200" )
{
	const auto doc = load_test( 200 );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Table );
	const auto t = static_cast< MD::Table< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( t->columnsCount() == 1 );
	REQUIRE( t->rows().size() == 3 );

	REQUIRE( t->columnAlignment( 0 ) == MD::Table< TRAIT >::AlignLeft );

	{
		REQUIRE( t->rows().at( 0 )->type() == MD::ItemType::TableRow );
		const auto r = static_cast< MD::TableRow< TRAIT >* > ( t->rows().at( 0 ).data() );
		REQUIRE( r->cells().size() == 1 );

		REQUIRE( r->cells().at( 0 )->type() == MD::ItemType::TableCell );
		const auto c = static_cast< MD::TableCell< TRAIT >* > ( r->cells().at( 0 ).data() );
		REQUIRE( c->items().size() == 1 );
		REQUIRE( c->items().at( 0 )->type() == MD::ItemType::Text );
		const auto t = static_cast< MD::Text< TRAIT >* > ( c->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "f|oo" ) );
	}

	{
		REQUIRE( t->rows().at( 1 )->type() == MD::ItemType::TableRow );
		const auto r = static_cast< MD::TableRow< TRAIT >* > ( t->rows().at( 1 ).data() );
		REQUIRE( r->cells().size() == 1 );

		REQUIRE( r->cells().at( 0 )->type() == MD::ItemType::TableCell );
		const auto c = static_cast< MD::TableCell< TRAIT >* > ( r->cells().at( 0 ).data() );
		REQUIRE( c->items().size() == 3 );

		{
			REQUIRE( c->items().at( 0 )->type() == MD::ItemType::Text );
			const auto t = static_cast< MD::Text< TRAIT >* > ( c->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "b" ) );
		}

		{
			REQUIRE( c->items().at( 1 )->type() == MD::ItemType::Code );
			const auto cc = static_cast< MD::Code< TRAIT >* > ( c->items().at( 1 ).data() );
			REQUIRE( cc->text() == QStringLiteral( "\\|" ) );
		}

		{
			REQUIRE( c->items().at( 2 )->type() == MD::ItemType::Text );
			const auto t = static_cast< MD::Text< TRAIT >* > ( c->items().at( 2 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "az" ) );
		}
	}

	{
		REQUIRE( t->rows().at( 2 )->type() == MD::ItemType::TableRow );
		const auto r = static_cast< MD::TableRow< TRAIT >* > ( t->rows().at( 2 ).data() );
		REQUIRE( r->cells().size() == 1 );

		REQUIRE( r->cells().at( 0 )->type() == MD::ItemType::TableCell );
		const auto c = static_cast< MD::TableCell< TRAIT >* > ( r->cells().at( 0 ).data() );
		REQUIRE( c->items().size() == 3 );

		{
			REQUIRE( c->items().at( 0 )->type() == MD::ItemType::Text );
			const auto t = static_cast< MD::Text< TRAIT >* > ( c->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "b" ) );
		}

		{
			REQUIRE( c->items().at( 1 )->type() == MD::ItemType::Text );
			const auto t = static_cast< MD::Text< TRAIT >* > ( c->items().at( 1 ).data() );
			REQUIRE( t->opts() == MD::BoldText );
			REQUIRE( t->text() == QStringLiteral( "|" ) );
		}

		{
			REQUIRE( c->items().at( 2 )->type() == MD::ItemType::Text );
			const auto t = static_cast< MD::Text< TRAIT >* > ( c->items().at( 2 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "im" ) );
		}
	}
}

TEST_CASE( "201" )
{
	const auto doc = load_test( 201 );
	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Table );
	const auto t = static_cast< MD::Table< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( t->columnsCount() == 2 );
	REQUIRE( t->rows().size() == 2 );

	REQUIRE( t->columnAlignment( 0 ) == MD::Table< TRAIT >::AlignLeft );
	REQUIRE( t->columnAlignment( 1 ) == MD::Table< TRAIT >::AlignLeft );

	{
		REQUIRE( t->rows().at( 0 )->type() == MD::ItemType::TableRow );
		const auto r = static_cast< MD::TableRow< TRAIT >* > ( t->rows().at( 0 ).data() );
		REQUIRE( r->cells().size() == 2 );

		{
			REQUIRE( r->cells().at( 0 )->type() == MD::ItemType::TableCell );
			const auto c = static_cast< MD::TableCell< TRAIT >* > ( r->cells().at( 0 ).data() );
			REQUIRE( c->items().size() == 1 );
			REQUIRE( c->items().at( 0 )->type() == MD::ItemType::Text );
			const auto t = static_cast< MD::Text< TRAIT >* > ( c->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "abc" ) );
		}

		{
			REQUIRE( r->cells().at( 1 )->type() == MD::ItemType::TableCell );
			const auto c = static_cast< MD::TableCell< TRAIT >* > ( r->cells().at( 1 ).data() );
			REQUIRE( c->items().size() == 1 );
			REQUIRE( c->items().at( 0 )->type() == MD::ItemType::Text );
			const auto t = static_cast< MD::Text< TRAIT >* > ( c->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "def" ) );
		}
	}

	{
		REQUIRE( t->rows().at( 1 )->type() == MD::ItemType::TableRow );
		const auto r = static_cast< MD::TableRow< TRAIT >* > ( t->rows().at( 1 ).data() );
		REQUIRE( r->cells().size() == 2 );

		{
			REQUIRE( r->cells().at( 0 )->type() == MD::ItemType::TableCell );
			const auto c = static_cast< MD::TableCell< TRAIT >* > ( r->cells().at( 0 ).data() );
			REQUIRE( c->items().size() == 1 );
			REQUIRE( c->items().at( 0 )->type() == MD::ItemType::Text );
			const auto t = static_cast< MD::Text< TRAIT >* > ( c->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "bar" ) );
		}

		{
			REQUIRE( r->cells().at( 1 )->type() == MD::ItemType::TableCell );
			const auto c = static_cast< MD::TableCell< TRAIT >* > ( r->cells().at( 1 ).data() );
			REQUIRE( c->items().size() == 1 );
			REQUIRE( c->items().at( 0 )->type() == MD::ItemType::Text );
			const auto t = static_cast< MD::Text< TRAIT >* > ( c->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "baz" ) );
		}
	}

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Blockquote );
	const auto b = static_cast< MD::Blockquote< TRAIT >* > ( doc->items().at( 2 ).data() );
	REQUIRE( b->items().size() == 1 );
	REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Paragraph );
	const auto p = static_cast< MD::Paragraph< TRAIT >* > ( b->items().at( 0 ).data() );
	REQUIRE( p->items().size() == 1 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		const auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "bar" ) );
	}
}

TEST_CASE( "202" )
{
	const auto doc = load_test( 202 );
	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Table );
	const auto t = static_cast< MD::Table< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( t->columnsCount() == 2 );
	REQUIRE( t->rows().size() == 3 );

	REQUIRE( t->columnAlignment( 0 ) == MD::Table< TRAIT >::AlignLeft );
	REQUIRE( t->columnAlignment( 1 ) == MD::Table< TRAIT >::AlignLeft );

	{
		REQUIRE( t->rows().at( 0 )->type() == MD::ItemType::TableRow );
		const auto r = static_cast< MD::TableRow< TRAIT >* > ( t->rows().at( 0 ).data() );
		REQUIRE( r->cells().size() == 2 );

		{
			REQUIRE( r->cells().at( 0 )->type() == MD::ItemType::TableCell );
			const auto c = static_cast< MD::TableCell< TRAIT >* > ( r->cells().at( 0 ).data() );
			REQUIRE( c->items().size() == 1 );
			REQUIRE( c->items().at( 0 )->type() == MD::ItemType::Text );
			const auto t = static_cast< MD::Text< TRAIT >* > ( c->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "abc" ) );
		}

		{
			REQUIRE( r->cells().at( 1 )->type() == MD::ItemType::TableCell );
			const auto c = static_cast< MD::TableCell< TRAIT >* > ( r->cells().at( 1 ).data() );
			REQUIRE( c->items().size() == 1 );
			REQUIRE( c->items().at( 0 )->type() == MD::ItemType::Text );
			const auto t = static_cast< MD::Text< TRAIT >* > ( c->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "def" ) );
		}
	}

	{
		REQUIRE( t->rows().at( 1 )->type() == MD::ItemType::TableRow );
		const auto r = static_cast< MD::TableRow< TRAIT >* > ( t->rows().at( 1 ).data() );
		REQUIRE( r->cells().size() == 2 );

		{
			REQUIRE( r->cells().at( 0 )->type() == MD::ItemType::TableCell );
			const auto c = static_cast< MD::TableCell< TRAIT >* > ( r->cells().at( 0 ).data() );
			REQUIRE( c->items().size() == 1 );
			REQUIRE( c->items().at( 0 )->type() == MD::ItemType::Text );
			const auto t = static_cast< MD::Text< TRAIT >* > ( c->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "bar" ) );
		}

		{
			REQUIRE( r->cells().at( 1 )->type() == MD::ItemType::TableCell );
			const auto c = static_cast< MD::TableCell< TRAIT >* > ( r->cells().at( 1 ).data() );
			REQUIRE( c->items().size() == 1 );
			REQUIRE( c->items().at( 0 )->type() == MD::ItemType::Text );
			const auto t = static_cast< MD::Text< TRAIT >* > ( c->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "baz" ) );
		}
	}

	{
		REQUIRE( t->rows().at( 2 )->type() == MD::ItemType::TableRow );
		const auto r = static_cast< MD::TableRow< TRAIT >* > ( t->rows().at( 2 ).data() );
		REQUIRE( r->cells().size() == 1 );

		{
			REQUIRE( r->cells().at( 0 )->type() == MD::ItemType::TableCell );
			const auto c = static_cast< MD::TableCell< TRAIT >* > ( r->cells().at( 0 ).data() );
			REQUIRE( c->items().size() == 1 );
			REQUIRE( c->items().at( 0 )->type() == MD::ItemType::Text );
			const auto t = static_cast< MD::Text< TRAIT >* > ( c->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "bar" ) );
		}
	}

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );
	const auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).data() );
	REQUIRE( p->items().size() == 1 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		const auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "bar" ) );
	}
}

TEST_CASE( "203" )
{
	const auto doc = load_test( 203 );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	const auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	const auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == QStringLiteral( "| abc | def | | --- | | bar |" ) );
}

TEST_CASE( "204" )
{
	const auto doc = load_test( 204 );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Table );
	const auto t = static_cast< MD::Table< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( t->columnsCount() == 2 );
	REQUIRE( t->rows().size() == 3 );

	REQUIRE( t->columnAlignment( 0 ) == MD::Table< TRAIT >::AlignLeft );
	REQUIRE( t->columnAlignment( 1 ) == MD::Table< TRAIT >::AlignLeft );

	{
		REQUIRE( t->rows().at( 0 )->type() == MD::ItemType::TableRow );
		const auto r = static_cast< MD::TableRow< TRAIT >* > ( t->rows().at( 0 ).data() );
		REQUIRE( r->cells().size() == 2 );

		{
			REQUIRE( r->cells().at( 0 )->type() == MD::ItemType::TableCell );
			const auto c = static_cast< MD::TableCell< TRAIT >* > ( r->cells().at( 0 ).data() );
			REQUIRE( c->items().size() == 1 );
			REQUIRE( c->items().at( 0 )->type() == MD::ItemType::Text );
			const auto t = static_cast< MD::Text< TRAIT >* > ( c->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "abc" ) );
		}

		{
			REQUIRE( r->cells().at( 1 )->type() == MD::ItemType::TableCell );
			const auto c = static_cast< MD::TableCell< TRAIT >* > ( r->cells().at( 1 ).data() );
			REQUIRE( c->items().size() == 1 );
			REQUIRE( c->items().at( 0 )->type() == MD::ItemType::Text );
			const auto t = static_cast< MD::Text< TRAIT >* > ( c->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "def" ) );
		}
	}

	{
		REQUIRE( t->rows().at( 1 )->type() == MD::ItemType::TableRow );
		const auto r = static_cast< MD::TableRow< TRAIT >* > ( t->rows().at( 1 ).data() );
		REQUIRE( r->cells().size() == 1 );

		{
			REQUIRE( r->cells().at( 0 )->type() == MD::ItemType::TableCell );
			const auto c = static_cast< MD::TableCell< TRAIT >* > ( r->cells().at( 0 ).data() );
			REQUIRE( c->items().size() == 1 );
			REQUIRE( c->items().at( 0 )->type() == MD::ItemType::Text );
			const auto t = static_cast< MD::Text< TRAIT >* > ( c->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "bar" ) );
		}
	}

	{
		REQUIRE( t->rows().at( 2 )->type() == MD::ItemType::TableRow );
		const auto r = static_cast< MD::TableRow< TRAIT >* > ( t->rows().at( 2 ).data() );
		REQUIRE( r->cells().size() == 2 );

		{
			REQUIRE( r->cells().at( 0 )->type() == MD::ItemType::TableCell );
			const auto c = static_cast< MD::TableCell< TRAIT >* > ( r->cells().at( 0 ).data() );
			REQUIRE( c->items().size() == 1 );
			REQUIRE( c->items().at( 0 )->type() == MD::ItemType::Text );
			const auto t = static_cast< MD::Text< TRAIT >* > ( c->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "bar" ) );
		}

		{
			REQUIRE( r->cells().at( 1 )->type() == MD::ItemType::TableCell );
			const auto c = static_cast< MD::TableCell< TRAIT >* > ( r->cells().at( 1 ).data() );
			REQUIRE( c->items().size() == 1 );
			REQUIRE( c->items().at( 0 )->type() == MD::ItemType::Text );
			const auto t = static_cast< MD::Text< TRAIT >* > ( c->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "baz" ) );
		}
	}
}

TEST_CASE( "205" )
{
	const auto doc = load_test( 205 );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Table );
	const auto t = static_cast< MD::Table< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( t->columnsCount() == 2 );
	REQUIRE( t->rows().size() == 1 );

	REQUIRE( t->columnAlignment( 0 ) == MD::Table< TRAIT >::AlignLeft );
	REQUIRE( t->columnAlignment( 1 ) == MD::Table< TRAIT >::AlignLeft );

	{
		REQUIRE( t->rows().at( 0 )->type() == MD::ItemType::TableRow );
		const auto r = static_cast< MD::TableRow< TRAIT >* > ( t->rows().at( 0 ).data() );
		REQUIRE( r->cells().size() == 2 );

		{
			REQUIRE( r->cells().at( 0 )->type() == MD::ItemType::TableCell );
			const auto c = static_cast< MD::TableCell< TRAIT >* > ( r->cells().at( 0 ).data() );
			REQUIRE( c->items().size() == 1 );
			REQUIRE( c->items().at( 0 )->type() == MD::ItemType::Text );
			const auto t = static_cast< MD::Text< TRAIT >* > ( c->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "abc" ) );
		}

		{
			REQUIRE( r->cells().at( 1 )->type() == MD::ItemType::TableCell );
			const auto c = static_cast< MD::TableCell< TRAIT >* > ( r->cells().at( 1 ).data() );
			REQUIRE( c->items().size() == 1 );
			REQUIRE( c->items().at( 0 )->type() == MD::ItemType::Text );
			const auto t = static_cast< MD::Text< TRAIT >* > ( c->items().at( 0 ).data() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "def" ) );
		}
	}
}

TEST_CASE( "279" )
{
	const auto doc = load_test( 279 );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	const auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( l->items().size() == 2 );

	{
		REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
		const auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).data() );
		REQUIRE( li->isTaskList() );
		REQUIRE( !li->isChecked() );
		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		const auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).data() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		const auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "foo" ) );
	}

	{
		REQUIRE( l->items().at( 1 )->type() == MD::ItemType::ListItem );
		const auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 1 ).data() );
		REQUIRE( li->isTaskList() );
		REQUIRE( li->isChecked() );
		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		const auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).data() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		const auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "bar" ) );
	}
}

TEST_CASE( "280" )
{
	const auto doc = load_test( 280 );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	const auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( l->items().size() == 2 );

	{
		REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
		const auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).data() );
		REQUIRE( li->isTaskList() );
		REQUIRE( li->isChecked() );
		REQUIRE( li->items().size() == 2 );
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		const auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).data() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		const auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "foo" ) );

		{
			REQUIRE( li->items().at( 1 )->type() == MD::ItemType::List );
			const auto l = static_cast< MD::List< TRAIT >* > ( li->items().at( 1 ).data() );
			REQUIRE( l->items().size() == 2 );

			{
				REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
				const auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).data() );
				REQUIRE( li->isTaskList() );
				REQUIRE( !li->isChecked() );
				REQUIRE( li->items().size() == 1 );
				REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
				const auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).data() );
				REQUIRE( p->items().size() == 1 );
				REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
				const auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
				REQUIRE( t->opts() == MD::TextWithoutFormat );
				REQUIRE( t->text() == QStringLiteral( "bar" ) );
			}

			{
				REQUIRE( l->items().at( 1 )->type() == MD::ItemType::ListItem );
				const auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 1 ).data() );
				REQUIRE( li->isTaskList() );
				REQUIRE( li->isChecked() );
				REQUIRE( li->items().size() == 1 );
				REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
				const auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).data() );
				REQUIRE( p->items().size() == 1 );
				REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
				const auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
				REQUIRE( t->opts() == MD::TextWithoutFormat );
				REQUIRE( t->text() == QStringLiteral( "baz" ) );
			}
		}
	}

	{
		REQUIRE( l->items().at( 1 )->type() == MD::ItemType::ListItem );
		const auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 1 ).data() );
		REQUIRE( li->isTaskList() );
		REQUIRE( !li->isChecked() );
		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
		const auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).data() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		const auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "bim" ) );
	}
}

TEST_CASE( "491" )
{
	const auto doc = load_test( 491 );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	const auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 2 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		const auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::StrikethroughText );
		REQUIRE( t->text() == QStringLiteral( "Hi" ) );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
		const auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "Hello, world!" ) );
	}
}

TEST_CASE( "492" )
{
	const auto doc = load_test( 492 );
	REQUIRE( doc->items().size() == 3 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
		const auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).data() );
		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		const auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "This ~~has a" ) );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );
		const auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).data() );
		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		const auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "new paragraph~~." ) );
	}
}
