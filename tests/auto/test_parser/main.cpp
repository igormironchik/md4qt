
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

#include <md4qt/parser.hpp>
#include <md4qt/traits.hpp>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
// doctest include.
#include <doctest/doctest.h>

#include <fstream>

#ifdef MD4QT_QT_SUPPORT
#include <QDir>
#include <QTextStream>
#include <QFile>
#else
#include <filesystem>
#endif

inline typename TRAIT::String
to_string( int i )
{
#ifdef MD4QT_QT_SUPPORT
	return QString::number( i );
#else
	return std::to_string( i );
#endif
}


TEST_CASE( "001" )
{
	MD::Parser< TRAIT > p;
	auto doc = p.parse( "tests/parser/data/001.md" );
	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 1 );
	REQUIRE( doc->items().at( 0 )->type() == MD::ItemType::Anchor );
}

TEST_CASE( "002" )
{
	MD::Parser< TRAIT > p;
	auto doc = p.parse( "tests/parser/data/002.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );
	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto dp = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( dp->items().size() == 1 );
	REQUIRE( dp->items().first()->type() == MD::ItemType::Text );

	auto dt = static_cast< MD::Text< TRAIT >* > ( dp->items().first().get() );

	REQUIRE( dt->opts() == MD::TextOption::TextWithoutFormat );
	REQUIRE( dt->text() == u8"This is just a text!" );
}

TEST_CASE( "003" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/003.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

		auto dp = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );

		REQUIRE( dp->items().size() == 1 );
		REQUIRE( dp->items().first()->type() == MD::ItemType::Text );

		auto dt = static_cast< MD::Text< TRAIT >* > ( dp->items().first().get() );

		REQUIRE( dt->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( dt->text() == u8"Paragraph 1." );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );

		auto dp = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).get() );

		REQUIRE( dp->items().size() == 1 );
		REQUIRE( dp->items().first()->type() == MD::ItemType::Text );

		auto dt = static_cast< MD::Text< TRAIT >* > ( dp->items().first().get() );

		REQUIRE( dt->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( dt->text() == u8"Paragraph 2." );
	}
}

TEST_CASE( "004" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/004.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto dp = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( dp->items().size() == 1 );

	REQUIRE( dp->items().at( 0 )->type() == MD::ItemType::Text );

	auto dt = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 0 ).get() );

	REQUIRE( dt->opts() == MD::TextOption::TextWithoutFormat );
	REQUIRE( dt->text() == u8"Line 1... Line 2... Line 3..." );
}

TEST_CASE( "005" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/005.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto dp = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( dp->items().size() == 3 );

	{
		REQUIRE( dp->items().at( 0 )->type() == MD::ItemType::Text );

		auto dt = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 0 ).get() );

		REQUIRE( dt->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( dt->text() == u8"Line 1..." );
	}

	REQUIRE( dp->items().at( 1 )->type() == MD::ItemType::LineBreak );

	{
		REQUIRE( dp->items().at( 2 )->type() == MD::ItemType::Text );

		auto dt = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 2 ).get() );

		REQUIRE( dt->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( dt->text() == u8"Line 2... Line 3..." );
	}
}

TEST_CASE( "006" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/006.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto dp = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( dp->items().size() == 3 );

	{
		REQUIRE( dp->items().at( 0 )->type() == MD::ItemType::Text );

		auto dt = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 0 ).get() );

		REQUIRE( dt->opts() == MD::TextOption::ItalicText );
		REQUIRE( dt->text() == u8"Line 1..." );
	}

	{
		REQUIRE( dp->items().at( 1 )->type() == MD::ItemType::Text );

		auto dt = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 1 ).get() );

		REQUIRE( dt->opts() == MD::TextOption::BoldText );
		REQUIRE( dt->text() == u8"Line 2..." );
	}

	{
		REQUIRE( dp->items().at( 2 )->type() == MD::ItemType::Text );

		auto dt = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 2 ).get() );

		REQUIRE( dt->opts() == MD::TextOption::StrikethroughText );
		REQUIRE( dt->text() == u8"Line 3..." );
	}
}

TEST_CASE( "007" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/007.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto dp = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( dp->items().size() == 1 );

	REQUIRE( dp->items().at( 0 )->type() == MD::ItemType::Text );

	auto dt = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 0 ).get() );

	REQUIRE( dt->opts() == ( MD::TextOption::ItalicText | MD::TextOption::BoldText ) );
	REQUIRE( dt->text() == u8"Line 1... Line 2... Line 3..." );
}

TEST_CASE( "008" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/008.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto dp = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( dp->items().size() == 1 );

	REQUIRE( dp->items().at( 0 )->type() == MD::ItemType::Text );

	auto dt = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 0 ).get() );

	REQUIRE( dt->opts() == ( MD::TextOption::ItalicText | MD::TextOption::BoldText |
		MD::TextOption::StrikethroughText ) );
	REQUIRE( dt->text() == u8"Line 1... Line 2... Line 3..." );
}

TEST_CASE( "009" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/009.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto dp = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( dp->items().size() == 2 );

	{
		REQUIRE( dp->items().at( 0 )->type() == MD::ItemType::Text );

		auto dt = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 0 ).get() );

		REQUIRE( dt->opts() == MD::TextOption::StrikethroughText );
		REQUIRE( dt->text() == u8"__*Line 1... Line 2..." );
	}

	{
		REQUIRE( dp->items().at( 1 )->type() == MD::ItemType::Text );

		auto dt = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 1 ).get() );

		REQUIRE( dt->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( dt->text() == u8"Line 3...*__" );
	}
}

TEST_CASE( "010" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/010.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto dp = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( dp->items().size() == 1 );

	{
		REQUIRE( dp->items().at( 0 )->type() == MD::ItemType::Text );

		auto dt = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 0 ).get() );

		REQUIRE( dt->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( dt->text() == u8"~~__*Line 1... Line 2...~~ Line 3...*__" );
	}
}

TEST_CASE( "011" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/011.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto dp = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( dp->items().size() == 1 );

	REQUIRE( dp->items().at( 0 )->type() == MD::ItemType::Code );

	auto c = static_cast< MD::Code< TRAIT >* > ( dp->items().at( 0 ).get() );

	REQUIRE( c->isInlined() == true );
	REQUIRE( c->text() == u8"code" );
}

TEST_CASE( "012" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/012.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto dp = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( dp->items().size() == 3 );

	REQUIRE( dp->items().at( 0 )->type() == MD::ItemType::Text );

	auto t1 = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 0 ).get() );

	REQUIRE( t1->text() == u8"Code in the" );

	REQUIRE( dp->items().at( 1 )->type() == MD::ItemType::Code );

	auto c = static_cast< MD::Code< TRAIT >* > ( dp->items().at( 1 ).get() );

	REQUIRE( c->isInlined() == true );
	REQUIRE( c->text() == u8"text" );

	REQUIRE( dp->items().at( 2 )->type() == MD::ItemType::Text );

	auto t2 = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 2 ).get() );

	REQUIRE( t2->text() == u8"." );
}

TEST_CASE( "013" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/013.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto dp = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( dp->items().size() == 1 );

	REQUIRE( dp->items().at( 0 )->type() == MD::ItemType::Code );

	auto c = static_cast< MD::Code< TRAIT >* > ( dp->items().at( 0 ).get() );

	REQUIRE( c->isInlined() == true );
	REQUIRE( c->text() == u8"Use this `code` in the code" );
}

TEST_CASE( "014" )
{
	MD::Parser< TRAIT > parser;

	std::ofstream file( "tests/parser/data/014.md", std::ios::out | std::ios::trunc );

	if( file.good() )
	{
		file << "Line 1...\rLine 2...\r\nLine 3...\n";
		file.close();

		auto doc = parser.parse( "tests/parser/data/014.md" );

		REQUIRE( doc->isEmpty() == false );
		REQUIRE( doc->items().size() == 2 );

		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

		auto dp = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );

		REQUIRE( dp->items().size() == 1 );

		{
			REQUIRE( dp->items().at( 0 )->type() == MD::ItemType::Text );

			auto dt = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 0 ).get() );

			REQUIRE( dt->opts() == MD::TextOption::TextWithoutFormat );
			REQUIRE( dt->text() == u8"Line 1... Line 2... Line 3..." );
		}
	}
	else
		REQUIRE( true == false );
}

TEST_CASE( "015" )
{
	MD::Parser< TRAIT > parser;

	std::ofstream file( "tests/parser/data/015.md", std::ios::out | std::ios::trunc );

	if( file.good() )
	{
		file << "Line 1...\r\rLine 2...\r\rLine 3...\r";
		file.close();

		auto doc = parser.parse( "tests/parser/data/015.md" );

		REQUIRE( doc->isEmpty() == false );
		REQUIRE( doc->items().size() == 4 );

		{
			REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

			auto dp = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );

			REQUIRE( dp->items().size() == 1 );

			REQUIRE( dp->items().at( 0 )->type() == MD::ItemType::Text );

			auto t = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 0 ).get() );

			REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
			REQUIRE( t->text() == u8"Line 1..." );
		}

		{
			REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );

			auto dp = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).get() );

			REQUIRE( dp->items().size() == 1 );

			REQUIRE( dp->items().at( 0 )->type() == MD::ItemType::Text );

			auto t = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 0 ).get() );

			REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
			REQUIRE( t->text() == u8"Line 2..." );
		}

		{
			REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Paragraph );

			auto dp = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 3 ).get() );

			REQUIRE( dp->items().size() == 1 );

			REQUIRE( dp->items().at( 0 )->type() == MD::ItemType::Text );

			auto t = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 0 ).get() );

			REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
			REQUIRE( t->text() == u8"Line 3..." );
		}
	}
	else
		REQUIRE( true == false );
}

TEST_CASE( "016" )
{
	MD::Parser< TRAIT > parser;

	std::ofstream file( "tests/parser/data/016.md", std::ios::out | std::ios::trunc );

	if( file.good() )
	{
		file << "Line 1...\r\nLine 2...\r\nLine 3...\r\n";
		file.close();

		auto doc = parser.parse( "tests/parser/data/016.md" );

		REQUIRE( doc->isEmpty() == false );
		REQUIRE( doc->items().size() == 2 );

		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

		auto dp = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );

		REQUIRE( dp->items().size() == 1 );

		{
			REQUIRE( dp->items().at( 0 )->type() == MD::ItemType::Text );

			auto dt = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 0 ).get() );

			REQUIRE( dt->opts() == MD::TextOption::TextWithoutFormat );
			REQUIRE( dt->text() == u8"Line 1... Line 2... Line 3..." );
		}
	}
	else
		REQUIRE( true == false );
}

TEST_CASE( "017" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/017.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );

	auto bq = static_cast< MD::Blockquote< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( !bq->isEmpty() );
	REQUIRE( bq->items().size() == 3 );

	{
		REQUIRE( bq->items().at( 0 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( bq->items().at( 0 ).get() );

		REQUIRE( !p->isEmpty() );
		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

		REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( t->text() == u8"Quote paragraph 1." );
	}

	{
		REQUIRE( bq->items().at( 1 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( bq->items().at( 1 ).get() );

		REQUIRE( !p->isEmpty() );
		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

		REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( t->text() == u8"Quote paragraph 2." );
	}

	REQUIRE( bq->items().at( 2 )->type() == MD::ItemType::Blockquote );

	auto nbq = static_cast< MD::Blockquote< TRAIT >* > ( bq->items().at( 2 ).get() );

	REQUIRE( !nbq->isEmpty() );
	REQUIRE( nbq->items().size() == 1 );

	REQUIRE( nbq->items().at( 0 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph< TRAIT >* > ( nbq->items().at( 0 ).get() );

	REQUIRE( !p->isEmpty() );
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

	REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
	REQUIRE( t->text() == u8"Nested quote" );
}

TEST_CASE( "018" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/018.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );

	auto bq = static_cast< MD::Blockquote< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( !bq->isEmpty() );
	REQUIRE( bq->items().size() == 3 );

	{
		REQUIRE( bq->items().at( 0 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( bq->items().at( 0 ).get() );

		REQUIRE( !p->isEmpty() );
		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

		REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( t->text() == u8"Quote paragraph 1." );
	}

	{
		REQUIRE( bq->items().at( 1 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( bq->items().at( 1 ).get() );

		REQUIRE( !p->isEmpty() );
		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

		REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( t->text() == u8"Quote paragraph 2." );
	}

	REQUIRE( bq->items().at( 2 )->type() == MD::ItemType::Blockquote );

	auto nbq = static_cast< MD::Blockquote< TRAIT >* > ( bq->items().at( 2 ).get() );

	REQUIRE( !nbq->isEmpty() );
	REQUIRE( nbq->items().size() == 1 );

	REQUIRE( nbq->items().at( 0 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph< TRAIT >* > ( nbq->items().at( 0 ).get() );

	REQUIRE( !p->isEmpty() );
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

	REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
	REQUIRE( t->text() == u8"Nested quote" );
}

TEST_CASE( "019" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/019.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	for( int i = 1; i < 3; ++i )
	{
		REQUIRE( doc->items().at( i )->type() == MD::ItemType::Blockquote );

		auto bq = static_cast< MD::Blockquote< TRAIT >* > ( doc->items().at( i ).get() );

		REQUIRE( !bq->isEmpty() );
		REQUIRE( bq->items().size() == 3 );

		{
			REQUIRE( bq->items().at( 0 )->type() == MD::ItemType::Paragraph );

			auto p = static_cast< MD::Paragraph< TRAIT >* > ( bq->items().at( 0 ).get() );

			REQUIRE( !p->isEmpty() );
			REQUIRE( p->items().size() == 1 );

			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

			REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
			REQUIRE( t->text() == u8"Quote paragraph 1." );
		}

		{
			REQUIRE( bq->items().at( 1 )->type() == MD::ItemType::Paragraph );

			auto p = static_cast< MD::Paragraph< TRAIT >* > ( bq->items().at( 1 ).get() );

			REQUIRE( !p->isEmpty() );
			REQUIRE( p->items().size() == 1 );

			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

			REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
			REQUIRE( t->text() == u8"Quote paragraph 2." );
		}

		REQUIRE( bq->items().at( 2 )->type() == MD::ItemType::Blockquote );

		auto nbq = static_cast< MD::Blockquote< TRAIT >* > ( bq->items().at( 2 ).get() );

		REQUIRE( !nbq->isEmpty() );
		REQUIRE( nbq->items().size() == 1 );

		REQUIRE( nbq->items().at( 0 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( nbq->items().at( 0 ).get() );

		REQUIRE( !p->isEmpty() );
		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

		REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( t->text() == u8"Nested quote" );
	}
}

TEST_CASE( "020" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/020.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );

	auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( c->isInlined() == false );
	REQUIRE( c->text() ==
		u8"if( a > b )\n  do_something();\nelse\n  dont_do_anything();" );
	REQUIRE( c->syntax() == u8"cpp" );
}

TEST_CASE( "021" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/021.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );

	auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( c->isInlined() == false );
	REQUIRE( c->text() ==
		u8"if( a > b )\n  do_something();\nelse\n  dont_do_anything();" );
}

TEST_CASE( "022" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/022.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );

	auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( c->isInlined() == false );
	REQUIRE( c->text() ==
		u8"if( a > b )\n  do_something();\nelse\n  dont_do_anything();" );
}

TEST_CASE( "023" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/023.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( l->items().size() == 3 );

	for( int i = 0; i < 3; ++i )
	{
		REQUIRE( l->items().at( i )->type() == MD::ItemType::ListItem );

		auto item = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( i ).get() );

		REQUIRE( item->listType() == MD::ListItem< TRAIT >::Unordered );

		REQUIRE( item->items().size() == 1 );

		REQUIRE( item->items().at( 0 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( item->items().at( 0 ).get() );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

		REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( t->text() == u8"Item " + to_string( i + 1 ) );
	}
}

TEST_CASE( "024" )
{
	auto checkItem = [] ( MD::ListItem< TRAIT > * item, int i )
	{
		REQUIRE( item->items().at( 0 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( item->items().at( 0 ).get() );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

		REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( t->text() == u8"Item " + to_string( i + 1 ) );
	};

	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/024.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( l->items().size() == 3 );

	for( int i = 0; i < 3; ++i )
	{
		REQUIRE( l->items().at( i )->type() == MD::ItemType::ListItem );

		auto item = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( i ).get() );

		REQUIRE( item->listType() == MD::ListItem< TRAIT >::Unordered );

		REQUIRE( item->items().size() == 2 );

		checkItem( item, i );

		REQUIRE( item->items().at( 1 )->type() == MD::ItemType::List );

		auto nl = static_cast< MD::List< TRAIT >* > ( item->items().at( 1 ).get() );

		REQUIRE( nl->items().size() == 2 );

		for( int j = 0; j < 2; ++j )
		{
			REQUIRE( nl->items().at( j )->type() == MD::ItemType::ListItem );

			auto nitem = static_cast< MD::ListItem< TRAIT >* > ( nl->items().at( j ).get() );

			checkItem( nitem, j );
		}
	}
}

TEST_CASE( "025" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/025.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( l->items().size() == 3 );

	for( int i = 0; i < 3; ++i )
	{
		REQUIRE( l->items().at( i )->type() == MD::ItemType::ListItem );

		auto item = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( i ).get() );

		REQUIRE( item->listType() == MD::ListItem< TRAIT >::Unordered );

		REQUIRE( item->items().size() == 2 );

		{
			REQUIRE( item->items().at( 0 )->type() == MD::ItemType::Paragraph );

			auto p = static_cast< MD::Paragraph< TRAIT >* > ( item->items().at( 0 ).get() );

			REQUIRE( p->items().size() == 1 );

			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

			REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
			REQUIRE( t->text() == ( u8"Item " + to_string( i + 1 ) ) );
		}

		{
			REQUIRE( item->items().at( 1 )->type() == MD::ItemType::Paragraph );

			auto p = static_cast< MD::Paragraph< TRAIT >* > ( item->items().at( 1 ).get() );

			REQUIRE( p->items().size() == 1 );

			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

			REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
			REQUIRE( t->text() == u8"Paragraph in list" );
		}
	}
}

TEST_CASE( "026" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/026.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( l->items().size() == 3 );

	for( int i = 0; i < 3; ++i )
	{
		REQUIRE( l->items().at( i )->type() == MD::ItemType::ListItem );

		auto item = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( i ).get() );

		REQUIRE( item->listType() == MD::ListItem< TRAIT >::Unordered );

		REQUIRE( item->items().size() == 3 );

		{
			REQUIRE( item->items().at( 0 )->type() == MD::ItemType::Paragraph );

			auto p = static_cast< MD::Paragraph< TRAIT >* > ( item->items().at( 0 ).get() );

			REQUIRE( p->items().size() == 1 );

			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

			REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
			REQUIRE( t->text() == ( u8"Item " + to_string( i + 1 ) ) );
		}

		{
			REQUIRE( item->items().at( 1 )->type() == MD::ItemType::Paragraph );

			auto p = static_cast< MD::Paragraph< TRAIT >* > ( item->items().at( 1 ).get() );

			REQUIRE( p->items().size() == 1 );

			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

			REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
			REQUIRE( t->text() == u8"Paragraph in list" );
		}

		{
			REQUIRE( item->items().at( 2 )->type() == MD::ItemType::List );

			auto nl = static_cast< MD::List< TRAIT >* > ( item->items().at( 2 ).get() );

			REQUIRE( nl->items().at( 0 )->type() == MD::ItemType::ListItem );

			auto item = static_cast< MD::ListItem< TRAIT >* > ( nl->items().at( 0 ).get() );

			REQUIRE( item->listType() == MD::ListItem< TRAIT >::Unordered );

			REQUIRE( item->items().size() == 2 );

			{
				REQUIRE( item->items().at( 0 )->type() == MD::ItemType::Paragraph );

				auto p = static_cast< MD::Paragraph< TRAIT >* > ( item->items().at( 0 ).get() );

				REQUIRE( p->items().size() == 1 );

				REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

				auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

				REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
				REQUIRE( t->text() == u8"Nested" );
			}

			{
				REQUIRE( item->items().at( 1 )->type() == MD::ItemType::Paragraph );

				auto p = static_cast< MD::Paragraph< TRAIT >* > ( item->items().at( 1 ).get() );

				REQUIRE( p->items().size() == 1 );

				REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

				auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

				REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
				REQUIRE( t->text() == u8"Paragraph in list" );
			}
		}
	}
}

TEST_CASE( "027" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/027.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( l->items().size() == 3 );

	for( int i = 0; i < 3; ++i )
	{
		REQUIRE( l->items().at( i )->type() == MD::ItemType::ListItem );

		auto item = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( i ).get() );

		REQUIRE( item->listType() == MD::ListItem< TRAIT >::Unordered );

		REQUIRE( item->items().size() == 2 );

		{
			REQUIRE( item->items().at( 0 )->type() == MD::ItemType::Paragraph );

			auto p = static_cast< MD::Paragraph< TRAIT >* > ( item->items().at( 0 ).get() );

			REQUIRE( p->items().size() == 1 );

			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

			REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
			REQUIRE( t->text() == u8"Item " + to_string( i + 1 ) );
		}

		{
			REQUIRE( item->items().at( 1 )->type() == MD::ItemType::Code );

			auto c = static_cast< MD::Code< TRAIT >* > ( item->items().at( 1 ).get() );

			REQUIRE( c->isInlined() == false );
			REQUIRE( c->text() == u8"code" );
		}
	}
}

TEST_CASE( "028" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/028.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( l->items().size() == 3 );

	for( int i = 0; i < 3; ++i )
	{
		REQUIRE( l->items().at( i )->type() == MD::ItemType::ListItem );

		auto item = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( i ).get() );

		REQUIRE( item->listType() == MD::ListItem< TRAIT >::Unordered );

		REQUIRE( item->items().size() == 2 );

		{
			REQUIRE( item->items().at( 0 )->type() == MD::ItemType::Paragraph );

			auto p = static_cast< MD::Paragraph< TRAIT >* > ( item->items().at( 0 ).get() );

			REQUIRE( p->items().size() == 1 );

			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

			REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
			REQUIRE( t->text() == u8"Item " + to_string( i + 1 ) );
		}

		{
			REQUIRE( item->items().at( 1 )->type() == MD::ItemType::Code );

			auto c = static_cast< MD::Code< TRAIT >* > ( item->items().at( 1 ).get() );

			REQUIRE( c->isInlined() == false );
			REQUIRE( c->text() == u8"code" );
		}
	}
}

TEST_CASE( "029" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/029.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( l->items().size() == 3 );

	for( int i = 0; i < 3; ++i )
	{
		REQUIRE( l->items().at( i )->type() == MD::ItemType::ListItem );

		auto item = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( i ).get() );

		REQUIRE( item->listType() == MD::ListItem< TRAIT >::Unordered );

		REQUIRE( item->items().size() == 3 );

		{
			REQUIRE( item->items().at( 0 )->type() == MD::ItemType::Paragraph );

			auto p = static_cast< MD::Paragraph< TRAIT >* > ( item->items().at( 0 ).get() );

			REQUIRE( p->items().size() == 1 );

			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

			REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
			REQUIRE( t->text() == u8"Item " + to_string( i + 1 ) );
		}

		{
			REQUIRE( item->items().at( 1 )->type() == MD::ItemType::Paragraph );

			auto p = static_cast< MD::Paragraph< TRAIT >* > ( item->items().at( 1 ).get() );

			REQUIRE( p->items().size() == 1 );

			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

			REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
			REQUIRE( t->text() == u8"Paragraph in list" );
		}

		{
			REQUIRE( item->items().at( 2 )->type() == MD::ItemType::List );

			auto nl = static_cast< MD::List< TRAIT >* > ( item->items().at( 2 ).get() );

			REQUIRE( nl->items().at( 0 )->type() == MD::ItemType::ListItem );

			auto item = static_cast< MD::ListItem< TRAIT >* > ( nl->items().at( 0 ).get() );

			REQUIRE( item->listType() == MD::ListItem< TRAIT >::Unordered );

			REQUIRE( item->items().size() == 2 );

			{
				REQUIRE( item->items().at( 0 )->type() == MD::ItemType::Paragraph );

				auto p = static_cast< MD::Paragraph< TRAIT >* > ( item->items().at( 0 ).get() );

				REQUIRE( p->items().size() == 1 );

				REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

				auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

				REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
				REQUIRE( t->text() == u8"Nested" );
			}

			{
				REQUIRE( item->items().at( 1 )->type() == MD::ItemType::Paragraph );

				auto p = static_cast< MD::Paragraph< TRAIT >* > ( item->items().at( 1 ).get() );

				REQUIRE( p->items().size() == 1 );

				REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

				auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

				REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
				REQUIRE( t->text() == u8"Paragraph in list" );
			}
		}
	}

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).get() );

	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

	REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
	REQUIRE( t->text() == u8"Standalone paragraph" );
}

TEST_CASE( "030" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/030.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( p->items().size() == 6 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

	auto t1 = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

	REQUIRE( t1->text() == u8"Text" );

	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Image );

	auto i1 = static_cast< MD::Image< TRAIT >* > ( p->items().at( 1 ).get() );

	const typename TRAIT::String wd =
#ifdef MD4QT_QT_SUPPORT
		QDir().absolutePath()
#else
		std::filesystem::current_path().u8string()
#endif
		+ u8"/tests/parser/data/";

	REQUIRE( i1->text() == u8"Image 1" );
	REQUIRE( i1->url() == wd + u8"a.jpg" );

	REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );

	auto t2 = static_cast< MD::Text< TRAIT >* > ( p->items().at( 2 ).get() );

	REQUIRE( t2->text() == u8"continue" );

	REQUIRE( p->items().at( 3 )->type() == MD::ItemType::Image );

	auto i2 = static_cast< MD::Image< TRAIT >* > ( p->items().at( 3 ).get() );

	REQUIRE( i2->text() == u8"Image 2" );
	REQUIRE( i2->url() == wd + u8"b.png" );

	REQUIRE( p->items().at( 4 )->type() == MD::ItemType::Text );

	auto t3 = static_cast< MD::Text< TRAIT >* > ( p->items().at( 4 ).get() );

	REQUIRE( t3->text() == u8"and" );

	REQUIRE( p->items().at( 5 )->type() == MD::ItemType::Image );

	auto i3 = static_cast< MD::Image< TRAIT >* > ( p->items().at( 5 ).get() );

	REQUIRE( i3->text() == u8"Image 3" );
	REQUIRE( i3->url() == u8"http://www.where.com/c.jpeg" );
}

TEST_CASE( "031" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/031.md" );

	const typename TRAIT::String wd =
#ifdef MD4QT_QT_SUPPORT
		QDir().absolutePath()
#else
		std::filesystem::current_path().u8string()
#endif
		+ u8"/tests/parser/data";

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 9 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( p->items().size() == 5 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

	auto t0 = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

	REQUIRE( t0->text() == u8"[link 0][wrong-label]" );
	REQUIRE( t0->opts() == MD::TextWithoutFormat );

	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Link );

	auto l1 = static_cast< MD::Link< TRAIT >* > ( p->items().at( 1 ).get() );

	REQUIRE( l1->text() == u8"link 1" );
	REQUIRE( l1->url() == wd + u8"/a.md" );

	REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Link );

	auto l2 = static_cast< MD::Link< TRAIT >* > ( p->items().at( 2 ).get() );

	REQUIRE( l2->url() == wd + u8"/b.md" );
	REQUIRE( l2->opts() == MD::TextOption::TextWithoutFormat );

	REQUIRE( !l2->img().isNull() );
	REQUIRE( l2->img()->text() == u8"image 1" );
	REQUIRE( l2->img()->url() == wd + u8"/" + u8"a.png" );

	REQUIRE( p->items().at( 3 )->type() == MD::ItemType::Link );

	auto l3 = static_cast< MD::Link< TRAIT >* > ( p->items().at( 3 ).get() );

	REQUIRE( l3->text() == u8"link 3" );

	const TRAIT::String label = u8"#LABEL/" + wd + u8"/031.md";

	REQUIRE( l3->url() == label );

	REQUIRE( p->items().at( 4 )->type() == MD::ItemType::FootnoteRef );

	auto f1 = static_cast< MD::FootnoteRef< TRAIT >* > ( p->items().at( 4 ).get() );

	REQUIRE( f1->id() == u8"#^REF/" + wd + u8"/031.md" );

	REQUIRE( !doc->labeledLinks().isEmpty() );
	REQUIRE( doc->labeledLinks().contains( label ) );
	REQUIRE( doc->labeledLinks()[ label ]->url() == u8"http://www.where.com/a.md" );


	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );

		p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).get() );

		REQUIRE( p->items().size() == 2 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::FootnoteRef );

		f1 = static_cast< MD::FootnoteRef< TRAIT >* > ( p->items().at( 0 ).get() );

		REQUIRE( f1->id() == u8"#^REF/" + wd + u8"/031.md" );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );

		REQUIRE( t->text() == u8"text" );

		REQUIRE( doc->labeledLinks().size() == 2 );

		const auto lit = doc->labeledLinks().find( u8"#1/" + wd + u8"/031.md" );
		REQUIRE( lit != doc->labeledLinks().cend() );

#ifdef MD4QT_QT_SUPPORT
		REQUIRE( (*lit)->url() == wd + u8"/a.md" );
#else
		REQUIRE( lit->second->url() == wd + u8"/a.md" );
#endif
	}

	{
		REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Paragraph );

		p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 3 ).get() );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );

		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );

		REQUIRE( l->url() == u8"#label/" + wd + u8"/031.md" );
	}

	REQUIRE( doc->items().at( 4 )->type() == MD::ItemType::PageBreak );
}

TEST_CASE( "032" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/032.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );

	auto q = static_cast< MD::Blockquote< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( q->items().size() == 1 );

	REQUIRE( q->items().at( 0 )->type() == MD::ItemType::Code );

	auto c = static_cast< MD::Code< TRAIT >* > ( q->items().at( 0 ).get() );

	REQUIRE( c->isInlined() == false );
	REQUIRE( c->text() == u8"if( a < b )\n  do_something();" );
}

TEST_CASE( "033" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/033.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );

	auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );

	REQUIRE( l->url() == u8"http://www.google.com" );
	REQUIRE( l->text().isEmpty() );
}

TEST_CASE( "034" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/034.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );

	auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );

	REQUIRE( l->url() == u8"https://www.google.com" );
	REQUIRE( l->text() == u8"Google" );
	REQUIRE( l->opts() == MD::TextOption::BoldText );
}

TEST_CASE( "035" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/035.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( l->items().size() == 3 );

	{
		REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );

		auto i1 = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );

		REQUIRE( i1->listType() == MD::ListItem< TRAIT >::Ordered );
		REQUIRE( i1->orderedListPreState() == MD::ListItem< TRAIT >::Start );
		REQUIRE( i1->items().size() == 1 );
		REQUIRE( i1->items().at( 0 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( i1->items().at( 0 ).get() );

		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		REQUIRE( static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() )->text() == u8"1" );
	}

	REQUIRE( l->items().size() == 3 );

	{
		REQUIRE( l->items().at( 1 )->type() == MD::ItemType::ListItem );

		auto i1 = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 1 ).get() );

		REQUIRE( i1->listType() == MD::ListItem< TRAIT >::Ordered );
		REQUIRE( i1->orderedListPreState() == MD::ListItem< TRAIT >::Continue );
		REQUIRE( i1->items().size() == 2 );
		REQUIRE( i1->items().at( 0 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( i1->items().at( 0 ).get() );

		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		REQUIRE( static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() )->text() == u8"2" );

		REQUIRE( i1->items().at( 1 )->type() == MD::ItemType::List );

		auto nl = static_cast< MD::List< TRAIT >* > ( i1->items().at( 1 ).get() );

		REQUIRE( nl->items().size() == 2 );

		{
			REQUIRE( nl->items().at( 0 )->type() == MD::ItemType::ListItem );

			auto i1 = static_cast< MD::ListItem< TRAIT >* > ( nl->items().at( 0 ).get() );

			REQUIRE( i1->listType() == MD::ListItem< TRAIT >::Ordered );
			REQUIRE( i1->orderedListPreState() == MD::ListItem< TRAIT >::Start );
			REQUIRE( i1->items().size() == 1 );
			REQUIRE( i1->items().at( 0 )->type() == MD::ItemType::Paragraph );

			auto p = static_cast< MD::Paragraph< TRAIT >* > ( i1->items().at( 0 ).get() );

			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			REQUIRE( static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() )->text() == u8"1" );
		}

		{
			REQUIRE( nl->items().at( 1 )->type() == MD::ItemType::ListItem );

			auto i1 = static_cast< MD::ListItem< TRAIT >* > ( nl->items().at( 1 ).get() );

			REQUIRE( i1->listType() == MD::ListItem< TRAIT >::Ordered );
			REQUIRE( i1->orderedListPreState() == MD::ListItem< TRAIT >::Continue );
			REQUIRE( i1->items().size() == 1 );
			REQUIRE( i1->items().at( 0 )->type() == MD::ItemType::Paragraph );

			auto p = static_cast< MD::Paragraph< TRAIT >* > ( i1->items().at( 0 ).get() );

			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			REQUIRE( static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() )->text() == u8"2" );
		}
	}

	{
		REQUIRE( l->items().at( 2 )->type() == MD::ItemType::ListItem );

		auto i1 = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 2 ).get() );

		REQUIRE( i1->listType() == MD::ListItem< TRAIT >::Ordered );
		REQUIRE( i1->orderedListPreState() == MD::ListItem< TRAIT >::Continue );
		REQUIRE( i1->items().size() == 1 );
		REQUIRE( i1->items().at( 0 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( i1->items().at( 0 ).get() );

		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		REQUIRE( static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() )->text() == u8"3" );
	}
}

TEST_CASE( "036" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/036.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );

	auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );

	REQUIRE( l->url() == u8"www.google.com" );
	REQUIRE( l->text() == u8"Google" );
}

TEST_CASE( "037" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/037.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 17 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

		REQUIRE( t->text() == u8"[Google] ( www.google.com Google Shmoogle..." );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).get() );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

		REQUIRE( t->text() == u8"[Google] (" );
	}

	{
		REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 3 ).get() );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

		REQUIRE( t->text() == u8"[Google" );
	}

	{
		REQUIRE( doc->items().at( 4 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 4 ).get() );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

		REQUIRE( t->text() == u8"[Google]" );
	}

	{
		REQUIRE( doc->items().at( 5 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 5 ).get() );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

		REQUIRE( t->text() == u8"[![Google](" );
	}

	{
		REQUIRE( doc->items().at( 6 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 6 ).get() );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

		REQUIRE( t->text() == u8"![Google](" );
	}

	{
		REQUIRE( doc->items().at( 7 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 7 ).get() );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

		REQUIRE( t->text() == u8"[Google] ( www.google.com \"Google Shmoogle...\"" );
	}

	{
		REQUIRE( doc->items().at( 8 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 8 ).get() );

		REQUIRE( p->items().size() == 2 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

		REQUIRE( t->text() == u8"[" );

		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Image );

		auto i = static_cast< MD::Image< TRAIT >* > ( p->items().at( 1 ).get() );

		REQUIRE( i->url() == u8"https://www.google.com/logo.png" );
		REQUIRE( i->text() == u8"Google" );
	}

	{
		REQUIRE( doc->items().at( 9 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 9 ).get() );

		REQUIRE( p->items().size() == 3 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

		REQUIRE( t->text() == u8"text" );

		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::FootnoteRef );

		REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );

		t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 2 ).get() );

		REQUIRE( t->text() == u8":" );
	}

	{
		REQUIRE( doc->items().at( 10 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 10 ).get() );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

		REQUIRE( t->text() == u8"[^ref" );
	}

	{
		REQUIRE( doc->items().at( 11 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 11 ).get() );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

		REQUIRE( t->text() == u8"[" );
	}

	{
		REQUIRE( doc->items().at( 12 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 12 ).get() );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

		REQUIRE( t->text() == u8"[1]:" );
	}

	{
		REQUIRE( doc->items().at( 13 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 13 ).get() );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

		REQUIRE( t->text() == u8"[1]:" );
	}

	{
		REQUIRE( doc->items().at( 14 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 14 ).get() );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

		REQUIRE( t->text() == u8"[text][link" );
	}

	{
		REQUIRE( doc->items().at( 15 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 15 ).get() );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"[text]#" );
	}

	{
		REQUIRE( doc->items().at( 16 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 16 ).get() );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

		REQUIRE( t->text() == u8"<www.google.com" );
	}
}

TEST_CASE( "038" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/038.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( l->items().size() == 3 );

	{
		REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );

		auto i1 = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );

		REQUIRE( i1->listType() == MD::ListItem< TRAIT >::Ordered );
		REQUIRE( i1->orderedListPreState() == MD::ListItem< TRAIT >::Start );
		REQUIRE( i1->items().size() == 1 );
		REQUIRE( i1->items().at( 0 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( i1->items().at( 0 ).get() );

		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		REQUIRE( static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() )->text() == u8"1" );
	}

	REQUIRE( l->items().size() == 3 );

	{
		REQUIRE( l->items().at( 1 )->type() == MD::ItemType::ListItem );

		auto i1 = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 1 ).get() );

		REQUIRE( i1->listType() == MD::ListItem< TRAIT >::Ordered );
		REQUIRE( i1->orderedListPreState() == MD::ListItem< TRAIT >::Continue );
		REQUIRE( i1->items().size() == 2 );
		REQUIRE( i1->items().at( 0 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( i1->items().at( 0 ).get() );

		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		REQUIRE( static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() )->text() == u8"2" );

		REQUIRE( i1->items().at( 1 )->type() == MD::ItemType::List );

		auto nl = static_cast< MD::List< TRAIT >* > ( i1->items().at( 1 ).get() );

		REQUIRE( nl->items().size() == 2 );

		{
			REQUIRE( nl->items().at( 0 )->type() == MD::ItemType::ListItem );

			auto i1 = static_cast< MD::ListItem< TRAIT >* > ( nl->items().at( 0 ).get() );

			REQUIRE( i1->listType() == MD::ListItem< TRAIT >::Ordered );
			REQUIRE( i1->orderedListPreState() == MD::ListItem< TRAIT >::Start );
			REQUIRE( i1->items().size() == 1 );
			REQUIRE( i1->items().at( 0 )->type() == MD::ItemType::Paragraph );

			auto p = static_cast< MD::Paragraph< TRAIT >* > ( i1->items().at( 0 ).get() );

			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			REQUIRE( static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() )->text() == u8"1" );
		}

		{
			REQUIRE( nl->items().at( 1 )->type() == MD::ItemType::ListItem );

			auto i1 = static_cast< MD::ListItem< TRAIT >* > ( nl->items().at( 1 ).get() );

			REQUIRE( i1->listType() == MD::ListItem< TRAIT >::Ordered );
			REQUIRE( i1->orderedListPreState() == MD::ListItem< TRAIT >::Continue );
			REQUIRE( i1->items().size() == 1 );
			REQUIRE( i1->items().at( 0 )->type() == MD::ItemType::Paragraph );

			auto p = static_cast< MD::Paragraph< TRAIT >* > ( i1->items().at( 0 ).get() );

			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			REQUIRE( static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() )->text() == u8"2" );
		}
	}

	{
		REQUIRE( l->items().at( 2 )->type() == MD::ItemType::ListItem );

		auto i1 = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 2 ).get() );

		REQUIRE( i1->listType() == MD::ListItem< TRAIT >::Ordered );
		REQUIRE( i1->orderedListPreState() == MD::ListItem< TRAIT >::Continue );
		REQUIRE( i1->items().size() == 1 );
		REQUIRE( i1->items().at( 0 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( i1->items().at( 0 ).get() );

		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		REQUIRE( static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() )->text() == u8"3" );
	}
}

TEST_CASE( "039" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/039.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

	REQUIRE( t->opts() == MD::TextOption::BoldText );
	REQUIRE( t->text() == u8"text" );
}

TEST_CASE( "040" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/040.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto dp = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( dp->items().size() == 3 );

	REQUIRE( dp->items().at( 0 )->type() == MD::ItemType::Text );

	auto t1 = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 0 ).get() );

	REQUIRE( t1->text() == u8"``Use this" );

	REQUIRE( dp->items().at( 1 )->type() == MD::ItemType::Code );

	auto c1 = static_cast< MD::Code< TRAIT >* > ( dp->items().at( 1 ).get() );

	REQUIRE( c1->text() == u8"code" );

	REQUIRE( dp->items().at( 2 )->type() == MD::ItemType::Text );

	auto t2 = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 2 ).get() );

	REQUIRE( t2->text() == u8"in the code" );
}

TEST_CASE( "041" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/041.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( l->items().size() == 3 );

	for( int i = 0; i < 3; ++i )
	{
		REQUIRE( l->items().at( i )->type() == MD::ItemType::ListItem );

		auto item = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( i ).get() );

		REQUIRE( item->listType() == MD::ListItem< TRAIT >::Unordered );

		REQUIRE( item->items().size() == 2 );

		{
			REQUIRE( item->items().at( 0 )->type() == MD::ItemType::Paragraph );

			auto p = static_cast< MD::Paragraph< TRAIT >* > ( item->items().at( 0 ).get() );

			REQUIRE( p->items().size() == 1 );

			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

			REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
			REQUIRE( t->text() == u8"Item " + to_string( i + 1 ) );
		}

		{
			REQUIRE( item->items().at( 1 )->type() == MD::ItemType::Paragraph );

			auto p = static_cast< MD::Paragraph< TRAIT >* > ( item->items().at( 1 ).get() );

			REQUIRE( p->items().size() == 1 );

			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

			REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
			REQUIRE( t->text() == u8"Paragraph in list" );
		}
	}
}

TEST_CASE( "042" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/042.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 5 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( l->items().size() == 2 );

	const typename TRAIT::String wd =
#ifdef MD4QT_QT_SUPPORT
		QDir().absolutePath()
#else
		std::filesystem::current_path().u8string()
#endif
		+ u8"/tests/parser/data";

	for( int i = 0; i < 2; ++i )
	{
		REQUIRE( l->items().at( i )->type() == MD::ItemType::ListItem );

		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( i ).get() );

		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );

		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );

		auto lnk = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );

		REQUIRE( lnk->text() == u8"Chapter 1" );
		REQUIRE( lnk->url() == wd + u8"/042-1.md" );
	}

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::PageBreak );

	REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Anchor );

	REQUIRE( static_cast< MD::Anchor< TRAIT >* > ( doc->items().at( 3 ).get() )->label() ==
		wd + u8"/042-1.md" );

	REQUIRE( doc->items().at( 4 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 4 ).get() );

	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

	REQUIRE( t->text() == u8"Paragraph 1" );
}

TEST_CASE( "042-1" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/042.md", false );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( l->items().size() == 2 );

	const typename TRAIT::String wd =
#ifdef MD4QT_QT_SUPPORT
		QDir().absolutePath()
#else
		std::filesystem::current_path().u8string()
#endif
		+ u8"/tests/parser/data";

	for( int i = 0; i < 2; ++i )
	{
		REQUIRE( l->items().at( i )->type() == MD::ItemType::ListItem );

		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( i ).get() );

		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );

		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );

		auto lnk = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );

		REQUIRE( lnk->text() == u8"Chapter 1" );
		REQUIRE( lnk->url() == wd + u8"/042-1.md" );
	}
}

TEST_CASE( "044" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/044.md", false );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( l->items().size() == 1 );

	REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );

	auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );

	REQUIRE( li->items().size() == 2 );
	REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );

	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

	REQUIRE( t->text() == u8"Item" );

	REQUIRE( li->items().at( 1 )->type() == MD::ItemType::Blockquote );

	auto bq = static_cast< MD::Blockquote< TRAIT >* > ( li->items().at( 1 ).get() );

	REQUIRE( bq->items().size() == 1 );

	REQUIRE( bq->items().at( 0 )->type() == MD::ItemType::Paragraph );

	p = static_cast< MD::Paragraph< TRAIT >* > ( bq->items().at( 0 ).get() );

	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

	t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

	REQUIRE( t->text() == u8"Quote" );
}

TEST_CASE( "045" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/045.md", false );

	REQUIRE( !doc->isEmpty() );
	REQUIRE( doc->items().size() == 1 );
	REQUIRE( doc->items().at( 0 )->type() == MD::ItemType::Anchor );

	REQUIRE( doc->footnotesMap().size() == 1 );

	const typename TRAIT::String wd =
#ifdef MD4QT_QT_SUPPORT
		QDir().absolutePath()
#else
		std::filesystem::current_path().u8string()
#endif
		+ u8"/tests/parser/data/";

	const typename TRAIT::String label = u8"#^footnote/" + wd + u8"045.md";

	REQUIRE( doc->footnotesMap().contains( label ) );

	auto f = doc->footnotesMap()[ label ];

	REQUIRE( f->items().size() == 3 );

	for( int i = 0; i < 3; ++i )
	{
		REQUIRE( f->items().at( i )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( f->items().at( i ).get() );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

		REQUIRE( t->text() == u8"Paragraph in footnote" );
	}
}

TEST_CASE( "046" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/046.md" );

	REQUIRE( !doc->isEmpty() );
	REQUIRE( doc->items().size() == 14 );

	int idx = 1;

	for( int i = 1; i < 4; ++i )
	{
		for( int j = 1; j < 3; ++j )
		{
			REQUIRE( doc->items().at( idx )->type() == MD::ItemType::Heading );

			auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( idx ).get() );

			{
				REQUIRE( h->level() == j );
				REQUIRE( !h->text().isNull() );
				auto p = h->text().get();
				REQUIRE( p->items().size() == 1 );
				REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
				auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
				REQUIRE( t->text() == u8"Heading " + to_string( j ) );
			}

			++idx;

			REQUIRE( doc->items().at( idx )->type() == MD::ItemType::Paragraph );

			auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( idx ).get() );

			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			REQUIRE( static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() )->text() ==
				u8"Paragraph " + to_string( j ) );

			++idx;
		}
	}

	REQUIRE( doc->items().at( idx )->type() == MD::ItemType::Heading );

	auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( idx ).get() );

	REQUIRE( h->level() == 3 );
	REQUIRE( !h->text().isNull() );
	auto p = h->text().get();
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->text() == u8"Heading 3" );
	REQUIRE( h->isLabeled() );

	const typename TRAIT::String wd =
#ifdef MD4QT_QT_SUPPORT
		QDir().absolutePath()
#else
		std::filesystem::current_path().u8string()
#endif
		+ u8"/tests/parser/data/";

	const typename TRAIT::String label = u8"#heading-3/" + wd + u8"046.md";

	REQUIRE( h->label() == label );

	REQUIRE( doc->labeledHeadings().size() == 3 );
	REQUIRE( doc->labeledHeadings().contains( label ) );
	REQUIRE( doc->labeledHeadings()[ label ].get() == h );
}

TEST_CASE( "047" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/047.md" );

	REQUIRE( !doc->isEmpty() );
	REQUIRE( doc->items().size() == 3 );

	for( int i = 1; i < 3; ++i )
	{
		REQUIRE( doc->items().at( i )->type() == MD::ItemType::Table );

		auto t = static_cast< MD::Table< TRAIT >* > ( doc->items().at( i ).get() );

		REQUIRE( t->columnsCount() == 2 );
		REQUIRE( t->rows().size() == 2 );

		auto r0 = t->rows().at( 0 );

		REQUIRE( r0->type() == MD::ItemType::TableRow );

		REQUIRE( r0->cells().size() == 2 );

		{
			REQUIRE( r0->cells().at( 0 )->type() == MD::ItemType::TableCell );
			auto c0 = static_cast< MD::TableCell< TRAIT >* > ( r0->cells().at( 0 ).get() );

			REQUIRE( c0->items().size() == 1 );
			REQUIRE( c0->items().at( 0 )->type() == MD::ItemType::Text );

			auto t0 = static_cast< MD::Text< TRAIT >* > ( c0->items().at( 0 ).get() );

			REQUIRE( t0->text() == u8"Column 1" );
		}

		{
			auto c1 = static_cast< MD::TableCell< TRAIT >* > ( r0->cells().at( 1 ).get() );

			REQUIRE( c1->items().size() == 1 );
			REQUIRE( c1->items().at( 0 )->type() == MD::ItemType::Text );

			auto t1 = static_cast< MD::Text< TRAIT >* > ( c1->items().at( 0 ).get() );

			REQUIRE( t1->text() == u8"Column 2" );
		}

		auto r1 = t->rows().at( 1 );

		REQUIRE( r1->cells().size() == 2 );

		{
			auto c0 = static_cast< MD::TableCell< TRAIT >* > ( r1->cells().at( 0 ).get() );

			REQUIRE( c0->items().size() == 1 );
			REQUIRE( c0->items().at( 0 )->type() == MD::ItemType::Text );

			auto t0 = static_cast< MD::Text< TRAIT >* > ( c0->items().at( 0 ).get() );

			REQUIRE( t0->text() == u8"Cell 1" );
		}

		{
			auto c1 = static_cast< MD::TableCell< TRAIT >* > ( r1->cells().at( 1 ).get() );

			REQUIRE( c1->items().size() == 1 );
			REQUIRE( c1->items().at( 0 )->type() == MD::ItemType::Text );

			auto t1 = static_cast< MD::Text< TRAIT >* > ( c1->items().at( 0 ).get() );

			REQUIRE( t1->text() == u8"Cell 2" );
		}
	}

	auto table = static_cast< MD::Table< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( table->columnAlignment( 0 ) == MD::Table< TRAIT >::AlignLeft );
	REQUIRE( table->columnAlignment( 1 ) == MD::Table< TRAIT >::AlignLeft );

	table = static_cast< MD::Table< TRAIT >* > ( doc->items().at( 2 ).get() );

	REQUIRE( table->columnAlignment( 0 ) == MD::Table< TRAIT >::AlignCenter );
	REQUIRE( table->columnAlignment( 1 ) == MD::Table< TRAIT >::AlignRight );
}

TEST_CASE( "048" )
{
	MD::Parser< TRAIT > parser;
	auto doc = parser.parse( "tests/parser/data/048.md" );
	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 4 );
	REQUIRE( doc->items().at( 0 )->type() == MD::ItemType::Anchor );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( !h->text().isNull() );
		auto p = h->text().get();
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->text() == u8"Heading" );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );
		auto * p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		REQUIRE( static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() )->text() ==
			u8"Paragraph" );
	}

	REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Heading );
	auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( !h->text().isNull() );
	auto p = h->text().get();
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->text() == u8"Heading" );
}

TEST_CASE( "049" )
{
	MD::Parser< TRAIT > parser;
	auto doc = parser.parse( "tests/parser/data/049.md" );
	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 0 )->type() == MD::ItemType::Anchor );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<!-- --> 1<!-- -->" );
}

TEST_CASE( "050" )
{
	MD::Parser< TRAIT > parser;
	auto doc = parser.parse( "tests/parser/data/050.md" );
	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );
	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto * p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 4 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
	auto l0 = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( l0->text() == u8"a]" );

	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Link );
	auto l1 = static_cast< MD::Link< TRAIT >* > ( p->items().at( 1 ).get() );
	REQUIRE( l1->text() == u8"b\\" );

	REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Link );
	auto l2 = static_cast< MD::Link< TRAIT >* > ( p->items().at( 2 ).get() );
	REQUIRE( l2->text() == u8"c-d" );

	REQUIRE( p->items().at( 3 )->type() == MD::ItemType::Link );
	auto l3 = static_cast< MD::Link< TRAIT >* > ( p->items().at( 3 ).get() );
	REQUIRE( l3->text() == u8"\\" );
}

TEST_CASE( "051" )
{
	MD::Parser< TRAIT > parser;
	auto doc = parser.parse( "tests/parser/data/051.md" );

	const typename TRAIT::String wd =
#ifdef MD4QT_QT_SUPPORT
		QDir().absolutePath()
#else
		std::filesystem::current_path().u8string()
#endif
		+ u8"/tests/parser/data";

	REQUIRE( doc->items().size() == 8 );

	REQUIRE( static_cast< MD::Anchor< TRAIT >* > ( doc->items().at( 0 ).get() )->label() ==
		wd + u8"/051.md" );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::PageBreak );

	REQUIRE( static_cast< MD::Anchor< TRAIT >* > ( doc->items().at( 3 ).get() )->label() ==
		wd + u8"/051-1.md" );

	REQUIRE( doc->items().at( 4 )->type() == MD::ItemType::Paragraph );
	REQUIRE( doc->items().at( 5 )->type() == MD::ItemType::PageBreak );

	REQUIRE( static_cast< MD::Anchor< TRAIT >* > ( doc->items().at( 6 ).get() )->label() ==
		wd + u8"/051-2.md" );

	REQUIRE( doc->items().at( 7 )->type() == MD::ItemType::Paragraph );
}

TEST_CASE( "052" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/052.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );

		auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );

		REQUIRE( c->isInlined() == false );
		REQUIRE( c->text() == u8"_```` *bold* _italic" );
		REQUIRE( c->syntax() == u8"code" );
	}
}

TEST_CASE( "053" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/053.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( !c->isInlined() );
		REQUIRE( c->text().isEmpty() );
	}
}

TEST_CASE( "054" )
{
	MD::Parser< TRAIT > parser;

	const auto doc = parser.parse( "tests/parser/data/054.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( !c->isInlined() );
		REQUIRE( c->text().isEmpty() );
	}
}

TEST_CASE( "055" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/055.md", false );

	REQUIRE( doc->type() == MD::ItemType::Document );

	REQUIRE( !doc->isEmpty() );
	REQUIRE( doc->items().size() == 2 );
	REQUIRE( doc->items().at( 0 )->type() == MD::ItemType::Anchor );

	REQUIRE( doc->footnotesMap().size() == 1 );

	const typename TRAIT::String wd =
#ifdef MD4QT_QT_SUPPORT
		QDir().absolutePath()
#else
		std::filesystem::current_path().u8string()
#endif
		+ u8"/tests/parser/data/";

	const typename TRAIT::String label = u8"#^footnote/" + wd + u8"055.md";

	REQUIRE( doc->footnotesMap().contains( label ) );

	auto f = doc->footnotesMap()[ label ];

	REQUIRE( f->type() == MD::ItemType::Footnote );

	REQUIRE( f->items().size() == 3 );

	for( int i = 0; i < 3; ++i )
	{
		REQUIRE( f->items().at( i )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( f->items().at( i ).get() );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

		REQUIRE( t->text() == u8"Paragraph in footnote" );
	}

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

	REQUIRE( t->text() == u8"Text" );
}

TEST_CASE( "056" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/056.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );

	auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( c->isInlined() == false );
	REQUIRE( c->text() ==
		u8"if( a > b )\n\n  do_something();\n\nelse\n\n  dont_do_anything();" );
}

TEST_CASE( "057" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/057.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );

	auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( c->isInlined() == false );
	REQUIRE( c->text() == u8"code\n\ncode" );
}

TEST_CASE( "058" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/058.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( l->items().size() == 2 );

	REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
	REQUIRE( l->items().at( 1 )->type() == MD::ItemType::ListItem );

	auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 1 ).get() );

	REQUIRE( li->items().size() == 2 );
	REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
	REQUIRE( li->items().at( 1 )->type() == MD::ItemType::Paragraph );
}

TEST_CASE( "059" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/059.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );

	auto b = static_cast< MD::Blockquote< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( b->items().size() == 4 );

	REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Heading );
	REQUIRE( b->items().at( 1 )->type() == MD::ItemType::List );
	REQUIRE( b->items().at( 2 )->type() == MD::ItemType::Paragraph );
	REQUIRE( b->items().at( 3 )->type() == MD::ItemType::Code );
}

TEST_CASE( "060" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/060.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( l->items().size() == 1 );

	REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );

	auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );

	REQUIRE( li->items().size() == 2 );

	REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
	REQUIRE( li->items().at( 1 )->type() == MD::ItemType::List );
}

TEST_CASE( "061" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/061.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( l->items().size() == 1 );

	REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );

	auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );

	REQUIRE( li->items().size() == 2 );

	REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"A list item with a code block:" );

	REQUIRE( li->items().at( 1 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code< TRAIT >* > ( li->items().at( 1 ).get() );
	REQUIRE( c->isInlined() == false );
	REQUIRE( c->syntax().isEmpty() );
	REQUIRE( c->text() == u8"<code goes here>" );
}

TEST_CASE( "062" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/062.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 5 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );

		REQUIRE( p->items().size() == 2 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t1 = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t1->text() == u8"*" );

		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Code );
		auto c1 = static_cast< MD::Code< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( c1->text() == u8"**" );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).get() );

		REQUIRE( p->items().size() == 2 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t1 = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t1->text() == u8"*" );

		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Code );
		auto c1 = static_cast< MD::Code< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( c1->text() == u8"**" );
	}

	{
		REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 3 ).get() );

		REQUIRE( p->items().size() == 2 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t1 = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t1->text() == u8"*" );

		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Code );
		auto c1 = static_cast< MD::Code< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( c1->text() == u8"**`**" );
	}

	{
		REQUIRE( doc->items().at( 4 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 4 ).get() );

		REQUIRE( p->items().size() == 2 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Code );
		auto c1 = static_cast< MD::Code< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( c1->text() == u8"*" );

		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
		auto t1 = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( t1->text() == u8"`*" );
	}
}

TEST_CASE( "063" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/063.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 2 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == ( MD::ItalicText | MD::BoldText ) );
		REQUIRE( t->text() == u8"Lorem." );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Aenean" );
	}
}

TEST_CASE( "064" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/064.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"\\`" );
}

TEST_CASE( "065" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/065.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 6 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"When" );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Math );
		auto m = static_cast< MD::Math< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( m->expr() == u8"a \\ne 0" );
		REQUIRE( m->isInline() );
	}

	{
		REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 2 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8", there are two solutions to" );
	}

	{
		REQUIRE( p->items().at( 3 )->type() == MD::ItemType::Math );
		auto m = static_cast< MD::Math< TRAIT >* > ( p->items().at( 3 ).get() );
		REQUIRE( m->expr() == u8"(ax^2 + bx + c = 0)" );
		REQUIRE( m->isInline() );
	}

	{
		REQUIRE( p->items().at( 4 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 4 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"and they are" );
	}

	{
		REQUIRE( p->items().at( 5 )->type() == MD::ItemType::Math );
		auto m = static_cast< MD::Math< TRAIT >* > ( p->items().at( 5 ).get() );
		REQUIRE( m->expr() == u8" x = {-b \\pm \\sqrt{b^2-4ac} \\over 2a} " );
		REQUIRE( !m->isInline() );
	}
}

TEST_CASE( "066" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/066.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Math );
	auto m = static_cast< MD::Math< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( m->expr() == u8"x = {-b \\pm \\sqrt{b^2-4ac} \\over 2a}" );
	REQUIRE( !m->isInline() );
}

TEST_CASE( "067" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/067.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

		REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( t->text() == u8"Paragraph" );
	}

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 2 ).get() );

	REQUIRE( l->items().size() == 1 );

	REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );

	auto item = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );

	REQUIRE( item->listType() == MD::ListItem< TRAIT >::Unordered );

	REQUIRE( item->items().size() == 2 );

	{
		REQUIRE( item->items().at( 0 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( item->items().at( 0 ).get() );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

		REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( t->text() == u8"List item" );
	}

	{
		REQUIRE( item->items().at( 1 )->type() == MD::ItemType::Code );

		auto c = static_cast< MD::Code< TRAIT >* > ( item->items().at( 1 ).get() );

		REQUIRE( c->text() == u8"Code" );
	}
}

TEST_CASE( "068" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/068.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"<!-- Not finished HTML comment" );
}

TEST_CASE( "069" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/069.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<pre>\n\ndata\n\n</pre>" );
}

TEST_CASE( "070" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/070.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 2 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"Text" );

	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<pre>\n\ndata\n\n</pre>" );
}

TEST_CASE( "071" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/071.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<pre>\n\ndata\n\n</pre> Text" );
}

TEST_CASE( "072" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/072.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<pre>\n\ndata\n\n</pre>" );

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"Text" );
}

TEST_CASE( "073" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/073.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<pre>abc</pre>" );

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"Text" );
}

TEST_CASE( "074" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/074.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"Text" );

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 2 ).get() );
	REQUIRE( h->text() == u8"<pre>abc</pre>" );
}

TEST_CASE( "075" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/075.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 3 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Text" );
	}

	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<pre>\n</pre>" );

	{
		REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 2 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Text" );
	}
}

TEST_CASE( "076" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/076.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 4 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Text" );
	}

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 2 ).get() );
	REQUIRE( h->text() == u8"<pre>\n</pre>" );

	{
		REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 3 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Text" );
	}
}

TEST_CASE( "077" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/077.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<!-- Comment -->" );
}

TEST_CASE( "078" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/078.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 2 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"Text" );

	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<!-- Comment -->" );
}

TEST_CASE( "079" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/079.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<!-- Comment --> Text" );
}

TEST_CASE( "080" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/080.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<!-- Comment -->" );

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"Text" );
}

TEST_CASE( "081" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/081.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"Text" );

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 2 ).get() );
	REQUIRE( h->text() == u8"<!-- Comment -->" );
}

TEST_CASE( "082" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/082.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 3 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Text" );
	}

	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<!-- Comment -->" );

	{
		REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 2 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Text" );
	}
}

TEST_CASE( "083" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/083.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 4 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Text" );
	}

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 2 ).get() );
	REQUIRE( h->text() == u8"<!-- Comment -->" );

	{
		REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 3 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Text" );
	}
}

TEST_CASE( "084" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/084.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<?php\n\necho '>';\n\n?>" );
}

TEST_CASE( "085" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/085.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 2 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"Text" );

	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<?php\n\necho '>';\n\n?>" );
}

TEST_CASE( "086" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/086.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<?php\n\necho '>';\n\n?> Text" );
}

TEST_CASE( "087" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/087.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<?php\n\necho '>';\n\n?>" );

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"Text" );
}

TEST_CASE( "088" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/088.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"Text" );

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 2 ).get() );
	REQUIRE( h->text() == u8"<?php\n\necho '>';\n\n?>" );
}

TEST_CASE( "089" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/089.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 3 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Text" );
	}

	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<?php\n\necho '>';\n\n?>" );

	{
		REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 2 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Text" );
	}
}

TEST_CASE( "090" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/090.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 4 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Text" );
	}

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 2 ).get() );
	REQUIRE( h->text() == u8"<?php\n\necho '>';\n\n?>" );

	{
		REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 3 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Text" );
	}
}

TEST_CASE( "091" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/091.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<!DOCTYPE html>" );
}

TEST_CASE( "092" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/092.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 2 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"Text" );

	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<!DOCTYPE html>" );
}

TEST_CASE( "093" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/093.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<!DOCTYPE html> Text" );
}

TEST_CASE( "094" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/094.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<!DOCTYPE html>" );

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"Text" );
}

TEST_CASE( "095" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/095.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"Text" );

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 2 ).get() );
	REQUIRE( h->text() == u8"<!DOCTYPE html>" );
}

TEST_CASE( "096" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/096.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 3 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Text" );
	}

	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<!DOCTYPE html>" );

	{
		REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 2 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Text" );
	}
}

TEST_CASE( "097" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/097.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 4 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Text" );
	}

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 2 ).get() );
	REQUIRE( h->text() == u8"<!DOCTYPE html>" );

	{
		REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 3 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Text" );
	}
}

TEST_CASE( "098" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/098.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<![CDATA[\n"
										  "function matchwo(a,b)\n"
										  "{\n"
										  "  if (a < b && a < 0) then {\n"
										  "    return 1;\n\n"
										  "  } else {\n\n"
										  "    return 0;\n"
										  "  }\n"
										  "}\n"
										  "]]>" );
}

TEST_CASE( "099" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/099.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 2 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"Text" );

	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<![CDATA[\n"
										  "function matchwo(a,b)\n"
										  "{\n"
										  "  if (a < b && a < 0) then {\n"
										  "    return 1;\n\n"
										  "  } else {\n\n"
										  "    return 0;\n"
										  "  }\n"
										  "}\n"
										  "]]>" );
}

TEST_CASE( "100" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/100.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<![CDATA[\n"
										  "function matchwo(a,b)\n"
										  "{\n"
										  "  if (a < b && a < 0) then {\n"
										  "    return 1;\n\n"
										  "  } else {\n\n"
										  "    return 0;\n"
										  "  }\n"
										  "}\n"
										  "]]> Text" );
}

TEST_CASE( "101" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/101.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<![CDATA[\n"
										  "function matchwo(a,b)\n"
										  "{\n"
										  "  if (a < b && a < 0) then {\n"
										  "    return 1;\n\n"
										  "  } else {\n\n"
										  "    return 0;\n"
										  "  }\n"
										  "}\n"
										  "]]>" );

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"Text" );
}

TEST_CASE( "102" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/102.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"Text" );

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 2 ).get() );
	REQUIRE( h->text() == u8"<![CDATA[\n"
										  "function matchwo(a,b)\n"
										  "{\n"
										  "  if (a < b && a < 0) then {\n"
										  "    return 1;\n\n"
										  "  } else {\n\n"
										  "    return 0;\n"
										  "  }\n"
										  "}\n"
										  "]]>" );
}

TEST_CASE( "103" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/103.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 3 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Text" );
	}

	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<![CDATA[\n"
										  "function matchwo(a,b)\n"
										  "{\n"
										  "  if (a < b && a < 0) then {\n"
										  "    return 1;\n\n"
										  "  } else {\n\n"
										  "    return 0;\n"
										  "  }\n"
										  "}\n"
										  "]]>" );

	{
		REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 2 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Text" );
	}
}

TEST_CASE( "104" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/104.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 4 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Text" );
	}

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 2 ).get() );
	REQUIRE( h->text() == u8"<![CDATA[\n"
										  "function matchwo(a,b)\n"
										  "{\n"
										  "  if (a < b && a < 0) then {\n"
										  "    return 1;\n\n"
										  "  } else {\n\n"
										  "    return 0;\n"
										  "  }\n"
										  "}\n"
										  "]]>" );

	{
		REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 3 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Text" );
	}
}

TEST_CASE( "105" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/105.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<form>" );
}

TEST_CASE( "106" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/106.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 2 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"Text" );

	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<form>" );
}

TEST_CASE( "107" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/107.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<form> Text" );
}

TEST_CASE( "108" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/108.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<form>\nText" );
}

TEST_CASE( "109" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/109.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"Text" );

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 2 ).get() );
	REQUIRE( h->text() == u8"<form>" );
}

TEST_CASE( "110" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/110.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 2 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Text" );
	}

	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<form>\nText" );
}

TEST_CASE( "111" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/111.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Text" );
	}

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 2 ).get() );
	REQUIRE( h->text() == u8"<form>\nText" );
}

TEST_CASE( "112" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/112.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<a href=\"www.google.com\">\nGoogle\n</a>" );
}

TEST_CASE( "113" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/113.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 4 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Text" );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::RawHtml );
		auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( h->text() == u8"<a href=\"www.google.com\">" );
	}

	{
		REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 2 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Google" );
	}

	{
		REQUIRE( p->items().at( 3 )->type() == MD::ItemType::RawHtml );
		auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 3 ).get() );
		REQUIRE( h->text() == u8"</a>" );
	}
}

TEST_CASE( "114" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/114.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 4 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::RawHtml );
		auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( h->text() == u8"<a href=\"www.google.com\">" );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Google" );
	}

	{
		REQUIRE( p->items().at( 2 )->type() == MD::ItemType::RawHtml );
		auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 2 ).get() );
		REQUIRE( h->text() == u8"</a>" );
	}

	{
		REQUIRE( p->items().at( 3 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 3 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Text" );
	}
}

TEST_CASE( "115" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/115.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<a href=\"www.google.com\">\nGoogle\n</a>\nText" );
}

TEST_CASE( "116" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/116.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 4 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Text" );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::RawHtml );
		auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( h->text() == u8"<a href=\"www.google.com\">" );
	}

	{
		REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 2 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Google" );
	}

	{
		REQUIRE( p->items().at( 3 )->type() == MD::ItemType::RawHtml );
		auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 3 ).get() );
		REQUIRE( h->text() == u8"</a>" );
	}
}

TEST_CASE( "117" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/117.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 5 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Text" );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::RawHtml );
		auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( h->text() == u8"<a href=\"www.google.com\">" );
	}

	{
		REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 2 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Google" );
	}

	{
		REQUIRE( p->items().at( 3 )->type() == MD::ItemType::RawHtml );
		auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 3 ).get() );
		REQUIRE( h->text() == u8"</a>" );
	}

	{
		REQUIRE( p->items().at( 4 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 4 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Text" );
	}
}
TEST_CASE( "118" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/118.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 5 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Text" );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::RawHtml );
		auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( h->text() == u8"<a href=\"www.google.com\">" );
	}

	{
		REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 2 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Google" );
	}

	{
		REQUIRE( p->items().at( 3 )->type() == MD::ItemType::RawHtml );
		auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 3 ).get() );
		REQUIRE( h->text() == u8"</a>" );
	}

	{
		REQUIRE( p->items().at( 4 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 4 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Text" );
	}
}

TEST_CASE( "118-1" )
{
	const auto fileName = "tests/parser/data/118.md";

#ifdef MD4QT_QT_SUPPORT
	QFile file( fileName );

	if( file.open( QIODeviceBase::ReadOnly ) )
	{
		QTextStream stream( file.readAll() );
		file.close();

#else
	std::ifstream stream( filename, std::ios::in );

	if( stream.good() )
	{
#endif

		MD::Parser< TRAIT > parser;

		auto doc = parser.parse( stream, fileName );

#ifndef MD4QT_QT_SUPPORT
		stream.close();
#endif

		REQUIRE( doc->isEmpty() == false );
		REQUIRE( doc->items().size() == 2 );

		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( p->items().size() == 5 );

		{
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"Text" );
		}

		{
			REQUIRE( p->items().at( 1 )->type() == MD::ItemType::RawHtml );
			auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 1 ).get() );
			REQUIRE( h->text() == u8"<a href=\"www.google.com\">" );
		}

		{
			REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 2 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"Google" );
		}

		{
			REQUIRE( p->items().at( 3 )->type() == MD::ItemType::RawHtml );
			auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 3 ).get() );
			REQUIRE( h->text() == u8"</a>" );
		}

		{
			REQUIRE( p->items().at( 4 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 4 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"Text" );
		}
	}
	else
		REQUIRE( false );
}

TEST_CASE( "119" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/119.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );
		auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( h->text() == u8"<a href=\"www.google.com\">\nGoogle\n</a>" );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::RawHtml );
		auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( h->text() == u8"<pre>\n</pre>" );
	}
}

TEST_CASE( "120" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/120.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Math );
	auto m = static_cast< MD::Math< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( !m->isInline() );
	REQUIRE( m->expr() == u8"\n\\[\\mathrm{\\mathbf{M}}(\\alpha) =\n"
		"   \\left(\n"
		"      \\begin{matrix}\n"
		"         \\cos(\\alpha)+n_x^2\\cdot (1-\\cos(\\alpha))  &  "
		"n_x\\cdot n_y\\cdot (1-\\cos(\\alpha))-n_z\\cdot \\sin(\\alpha) &  "
		"n_x\\cdot n_z\\cdot (1-\\cos(\\alpha))+n_y\\cdot \\sin(\\alpha)\\\\\n"
		"         n_x\\cdot n_y\\cdot (1-\\cos(\\alpha))+n_z\\cdot \\sin(\\alpha) & "
		"\\cos(\\alpha)+n_y^2\\cdot (1-\\cos(\\alpha))  &   "
		"n_y\\cdot n_z\\cdot (1-\\cos(\\alpha))-n_x\\cdot \\sin(\\alpha)\\\\\n"
		"         n_z\\cdot n_x\\cdot (1-\\cos(\\alpha))-n_y\\cdot \\sin(\\alpha) & "
		"n_z\\cdot n_y\\cdot (1-\\cos(\\alpha))+n_x\\cdot \\sin(\\alpha)  & "
		"\\cos(\\alpha)+n_z^2\\cdot (1-\\cos(\\alpha))\n"
		"      \\end{matrix}\n"
		"   \\right)\n"
		"\\]\n" );
}

TEST_CASE( "121" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/121.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );
		auto b = static_cast< MD::Blockquote< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( b->items().size() == 1 );
		REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( b->items().at( 0 ).get() );
		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"foo bar --" );
	}
}

TEST_CASE( "122" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/122.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Math );
	auto m = static_cast< MD::Math< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( !m->isInline() );
	REQUIRE( m->expr() == u8"\\[\\mathrm{\\mathbf{M}}(\\alpha) =\n"
		"   \\left(\n"
		"      \\begin{matrix}\n"
		"         \\cos(\\alpha)+n_x^2\\cdot (1-\\cos(\\alpha))  &  "
		"n_x\\cdot n_y\\cdot (1-\\cos(\\alpha))-n_z\\cdot \\sin(\\alpha) &  "
		"n_x\\cdot n_z\\cdot (1-\\cos(\\alpha))+n_y\\cdot \\sin(\\alpha)\\\\\n"
		"         n_x\\cdot n_y\\cdot (1-\\cos(\\alpha))+n_z\\cdot \\sin(\\alpha) & "
		"\\cos(\\alpha)+n_y^2\\cdot (1-\\cos(\\alpha))  &   "
		"n_y\\cdot n_z\\cdot (1-\\cos(\\alpha))-n_x\\cdot \\sin(\\alpha)\\\\\n"
		"         n_z\\cdot n_x\\cdot (1-\\cos(\\alpha))-n_y\\cdot \\sin(\\alpha) & "
		"n_z\\cdot n_y\\cdot (1-\\cos(\\alpha))+n_x\\cdot \\sin(\\alpha)  & "
		"\\cos(\\alpha)+n_z^2\\cdot (1-\\cos(\\alpha))\n"
		"      \\end{matrix}\n"
		"   \\right)\n"
		"\\]" );
}

TEST_CASE( "123" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/123.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 4 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->text() == u8"Text" );

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 2 ).get() );

		auto p = h->text().get();
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->text() == u8"Heading" );
	}

	{
		REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::List );
		auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 3 ).get() );

		REQUIRE( l->items().size() == 1 );

		REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
		auto item = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
		REQUIRE( item->listType() == MD::ListItem< TRAIT >::Unordered );
		REQUIRE( item->items().size() == 1 );
		REQUIRE( item->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( item->items().at( 0 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( t->text() == u8"List" );
	}
}
