
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
#include <cstring>

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

/*
This is just a text!

*/
TEST_CASE( "002" )
{
	MD::Parser< TRAIT > p;
	auto doc = p.parse( "tests/parser/data/002.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );
	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto dp = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( dp->startColumn() == 0 );
	REQUIRE( dp->startLine() == 0 );
	REQUIRE( dp->endColumn() == 19 );
	REQUIRE( dp->endLine() == 0 );

	REQUIRE( dp->items().size() == 1 );
	REQUIRE( dp->items().front()->type() == MD::ItemType::Text );

	auto dt = static_cast< MD::Text< TRAIT >* > ( dp->items().front().get() );

	REQUIRE( dt->opts() == MD::TextOption::TextWithoutFormat );
	REQUIRE( dt->text() == u8"This is just a text!" );
	REQUIRE( dt->startColumn() == 0 );
	REQUIRE( dt->startLine() == 0 );
	REQUIRE( dt->endColumn() == 19 );
	REQUIRE( dt->endLine() == 0 );
}

/*

Paragraph 1.

Paragraph 2.

*/
TEST_CASE( "003" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/003.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

		auto dp = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( dp->startColumn() == 0 );
		REQUIRE( dp->startLine() == 1 );
		REQUIRE( dp->endColumn() == 11 );
		REQUIRE( dp->endLine() == 1 );

		REQUIRE( dp->items().size() == 1 );
		REQUIRE( dp->items().front()->type() == MD::ItemType::Text );

		auto dt = static_cast< MD::Text< TRAIT >* > ( dp->items().front().get() );

		REQUIRE( dt->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( dt->text() == u8"Paragraph 1." );
		REQUIRE( dt->startColumn() == 0 );
		REQUIRE( dt->startLine() == 1 );
		REQUIRE( dt->endColumn() == 11 );
		REQUIRE( dt->endLine() == 1 );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );

		auto dp = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( dp->startColumn() == 0 );
		REQUIRE( dp->startLine() == 3 );
		REQUIRE( dp->endColumn() == 11 );
		REQUIRE( dp->endLine() == 3 );

		REQUIRE( dp->items().size() == 1 );
		REQUIRE( dp->items().front()->type() == MD::ItemType::Text );

		auto dt = static_cast< MD::Text< TRAIT >* > ( dp->items().front().get() );

		REQUIRE( dt->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( dt->text() == u8"Paragraph 2." );
		REQUIRE( dt->startColumn() == 0 );
		REQUIRE( dt->startLine() == 3 );
		REQUIRE( dt->endColumn() == 11 );
		REQUIRE( dt->endLine() == 3 );
	}
}

/*
Line 1...
Line 2...
Line 3...

*/
TEST_CASE( "004" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/004.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto dp = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( dp->startColumn() == 0 );
	REQUIRE( dp->startLine() == 0 );
	REQUIRE( dp->endColumn() == 8 );
	REQUIRE( dp->endLine() == 2 );

	REQUIRE( dp->items().size() == 1 );

	REQUIRE( dp->items().at( 0 )->type() == MD::ItemType::Text );

	auto dt = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 0 ).get() );

	REQUIRE( dt->opts() == MD::TextOption::TextWithoutFormat );
	REQUIRE( dt->text() == u8"Line 1... Line 2... Line 3..." );
	REQUIRE( dt->startColumn() == 0 );
	REQUIRE( dt->startLine() == 0 );
	REQUIRE( dt->endColumn() == 8 );
	REQUIRE( dt->endLine() == 2 );
}

/*
Line 1...<space><space>
Line 2...
Line 3...

*/
TEST_CASE( "005" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/005.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto dp = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( dp->startColumn() == 0 );
	REQUIRE( dp->startLine() == 0 );
	REQUIRE( dp->endColumn() == 8 );
	REQUIRE( dp->endLine() == 2 );

	REQUIRE( dp->items().size() == 3 );

	{
		REQUIRE( dp->items().at( 0 )->type() == MD::ItemType::Text );

		auto dt = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 0 ).get() );
		REQUIRE( dt->startColumn() == 0 );
		REQUIRE( dt->startLine() == 0 );
		REQUIRE( dt->endColumn() == 8 );
		REQUIRE( dt->endLine() == 0 );

		REQUIRE( dt->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( dt->text() == u8"Line 1..." );
	}

	REQUIRE( dp->items().at( 1 )->type() == MD::ItemType::LineBreak );
	auto lb = static_cast< MD::LineBreak< TRAIT >* > ( dp->items().at( 1 ).get() );
	REQUIRE( lb->startColumn() == 9 );
	REQUIRE( lb->startLine() == 0 );
	REQUIRE( lb->endColumn() == 10 );
	REQUIRE( lb->endLine() == 0 );

	{
		REQUIRE( dp->items().at( 2 )->type() == MD::ItemType::Text );

		auto dt = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 2 ).get() );

		REQUIRE( dt->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( dt->text() == u8"Line 2... Line 3..." );
		REQUIRE( dt->startColumn() == 0 );
		REQUIRE( dt->startLine() == 1 );
		REQUIRE( dt->endColumn() == 8 );
		REQUIRE( dt->endLine() == 2 );
	}
}

/*
*Line 1...*
__Line 2...__
~~Line 3...~~

*/
TEST_CASE( "006" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/006.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto dp = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( dp->startColumn() == 0 );
	REQUIRE( dp->startLine() == 0 );
	REQUIRE( dp->endColumn() == 12 );
	REQUIRE( dp->endLine() == 2 );

	REQUIRE( dp->items().size() == 3 );

	{
		REQUIRE( dp->items().at( 0 )->type() == MD::ItemType::Text );

		auto dt = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 0 ).get() );

		REQUIRE( dt->opts() == MD::TextOption::ItalicText );
		REQUIRE( dt->text() == u8"Line 1..." );
		REQUIRE( dt->startColumn() == 1 );
		REQUIRE( dt->startLine() == 0 );
		REQUIRE( dt->endColumn() == 9 );
		REQUIRE( dt->endLine() == 0 );
	}

	{
		REQUIRE( dp->items().at( 1 )->type() == MD::ItemType::Text );

		auto dt = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 1 ).get() );

		REQUIRE( dt->opts() == MD::TextOption::BoldText );
		REQUIRE( dt->text() == u8"Line 2..." );
		REQUIRE( dt->startColumn() == 2 );
		REQUIRE( dt->startLine() == 1 );
		REQUIRE( dt->endColumn() == 10 );
		REQUIRE( dt->endLine() == 1 );
	}

	{
		REQUIRE( dp->items().at( 2 )->type() == MD::ItemType::Text );

		auto dt = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 2 ).get() );

		REQUIRE( dt->opts() == MD::TextOption::StrikethroughText );
		REQUIRE( dt->text() == u8"Line 3..." );
		REQUIRE( dt->startColumn() == 2 );
		REQUIRE( dt->startLine() == 2 );
		REQUIRE( dt->endColumn() == 10 );
		REQUIRE( dt->endLine() == 2 );
	}
}

/*
__*Line 1...
Line 2...
Line 3...*__

*/
TEST_CASE( "007" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/007.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto dp = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( dp->startColumn() == 0 );
	REQUIRE( dp->startLine() == 0 );
	REQUIRE( dp->endColumn() == 11 );
	REQUIRE( dp->endLine() == 2 );

	REQUIRE( dp->items().size() == 1 );

	REQUIRE( dp->items().at( 0 )->type() == MD::ItemType::Text );

	auto dt = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 0 ).get() );

	REQUIRE( dt->opts() == ( MD::TextOption::ItalicText | MD::TextOption::BoldText ) );
	REQUIRE( dt->text() == u8"Line 1... Line 2... Line 3..." );
	REQUIRE( dt->startColumn() == 3 );
	REQUIRE( dt->startLine() == 0 );
	REQUIRE( dt->endColumn() == 8 );
	REQUIRE( dt->endLine() == 2 );
}

/*
~~__*Line 1...
Line 2...
Line 3...*__~~

*/
TEST_CASE( "008" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/008.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto dp = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( dp->startColumn() == 0 );
	REQUIRE( dp->startLine() == 0 );
	REQUIRE( dp->endColumn() == 13 );
	REQUIRE( dp->endLine() == 2 );

	REQUIRE( dp->items().size() == 1 );

	REQUIRE( dp->items().at( 0 )->type() == MD::ItemType::Text );

	auto dt = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 0 ).get() );

	REQUIRE( dt->opts() == ( MD::TextOption::ItalicText | MD::TextOption::BoldText |
		MD::TextOption::StrikethroughText ) );
	REQUIRE( dt->text() == u8"Line 1... Line 2... Line 3..." );
	REQUIRE( dt->startColumn() == 5 );
	REQUIRE( dt->startLine() == 0 );
	REQUIRE( dt->endColumn() == 8 );
	REQUIRE( dt->endLine() == 2 );
}

/*
~~__*Line 1...
Line 2...~~
Line 3...*__

*/
TEST_CASE( "009" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/009.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto dp = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( dp->startColumn() == 0 );
	REQUIRE( dp->startLine() == 0 );
	REQUIRE( dp->endColumn() == 11 );
	REQUIRE( dp->endLine() == 2 );

	REQUIRE( dp->items().size() == 2 );

	{
		REQUIRE( dp->items().at( 0 )->type() == MD::ItemType::Text );

		auto dt = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 0 ).get() );

		REQUIRE( dt->opts() == MD::TextOption::StrikethroughText );
		REQUIRE( dt->text() == u8"__*Line 1... Line 2..." );
		REQUIRE( dt->startColumn() == 2 );
		REQUIRE( dt->startLine() == 0 );
		REQUIRE( dt->endColumn() == 8 );
		REQUIRE( dt->endLine() == 1 );
	}

	{
		REQUIRE( dp->items().at( 1 )->type() == MD::ItemType::Text );

		auto dt = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 1 ).get() );

		REQUIRE( dt->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( dt->text() == u8"Line 3...*__" );
		REQUIRE( dt->startColumn() == 0 );
		REQUIRE( dt->startLine() == 2 );
		REQUIRE( dt->endColumn() == 11 );
		REQUIRE( dt->endLine() == 2 );
	}
}

/*
\~\~\_\_\*Line 1...
Line 2...\~\~
Line 3...\*\_\_

*/
TEST_CASE( "010" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/010.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto dp = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( dp->startColumn() == 0 );
	REQUIRE( dp->startLine() == 0 );
	REQUIRE( dp->endColumn() == 14 );
	REQUIRE( dp->endLine() == 2 );

	REQUIRE( dp->items().size() == 1 );

	{
		REQUIRE( dp->items().at( 0 )->type() == MD::ItemType::Text );

		auto dt = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 0 ).get() );

		REQUIRE( dt->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( dt->text() == u8"~~__*Line 1... Line 2...~~ Line 3...*__" );
		REQUIRE( dt->startColumn() == 0 );
		REQUIRE( dt->startLine() == 0 );
		REQUIRE( dt->endColumn() == 14 );
		REQUIRE( dt->endLine() == 2 );
	}
}

/*
`code`

*/
TEST_CASE( "011" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/011.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto dp = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( dp->startColumn() == 0 );
	REQUIRE( dp->startLine() == 0 );
	REQUIRE( dp->endColumn() == 5 );
	REQUIRE( dp->endLine() == 0 );

	REQUIRE( dp->items().size() == 1 );

	REQUIRE( dp->items().at( 0 )->type() == MD::ItemType::Code );

	auto c = static_cast< MD::Code< TRAIT >* > ( dp->items().at( 0 ).get() );

	REQUIRE( c->isInlined() == true );
	REQUIRE( c->text() == u8"code" );
	REQUIRE( c->startColumn() == 1 );
	REQUIRE( c->startLine() == 0 );
	REQUIRE( c->endColumn() == 4 );
	REQUIRE( c->endLine() == 0 );
}

/*
Code in the `text`.

*/
TEST_CASE( "012" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/012.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto dp = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( dp->startColumn() == 0 );
	REQUIRE( dp->startLine() == 0 );
	REQUIRE( dp->endColumn() == 18 );
	REQUIRE( dp->endLine() == 0 );

	REQUIRE( dp->items().size() == 3 );

	REQUIRE( dp->items().at( 0 )->type() == MD::ItemType::Text );

	auto t1 = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 0 ).get() );
	REQUIRE( t1->startColumn() == 0 );
	REQUIRE( t1->startLine() == 0 );
	REQUIRE( t1->endColumn() == 11 );
	REQUIRE( t1->endLine() == 0 );

	REQUIRE( t1->text() == u8"Code in the" );

	REQUIRE( dp->items().at( 1 )->type() == MD::ItemType::Code );

	auto c = static_cast< MD::Code< TRAIT >* > ( dp->items().at( 1 ).get() );
	REQUIRE( c->startColumn() == 13 );
	REQUIRE( c->startLine() == 0 );
	REQUIRE( c->endColumn() == 16 );
	REQUIRE( c->endLine() == 0 );

	REQUIRE( c->isInlined() == true );
	REQUIRE( c->text() == u8"text" );

	REQUIRE( dp->items().at( 2 )->type() == MD::ItemType::Text );

	auto t2 = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 2 ).get() );
	REQUIRE( t2->startColumn() == 18 );
	REQUIRE( t2->startLine() == 0 );
	REQUIRE( t2->endColumn() == 18 );
	REQUIRE( t2->endLine() == 0 );

	REQUIRE( t2->text() == u8"." );
}

/*
``Use this `code`
in the code``

*/
TEST_CASE( "013" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/013.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto dp = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( dp->startColumn() == 0 );
	REQUIRE( dp->startLine() == 0 );
	REQUIRE( dp->endColumn() == 12 );
	REQUIRE( dp->endLine() == 1 );

	REQUIRE( dp->items().size() == 1 );

	REQUIRE( dp->items().at( 0 )->type() == MD::ItemType::Code );

	auto c = static_cast< MD::Code< TRAIT >* > ( dp->items().at( 0 ).get() );

	REQUIRE( c->isInlined() == true );
	REQUIRE( c->text() == u8"Use this `code` in the code" );
	REQUIRE( c->startColumn() == 2 );
	REQUIRE( c->startLine() == 0 );
	REQUIRE( c->endColumn() == 10 );
	REQUIRE( c->endLine() == 1 );
}

TEST_CASE( "014" )
{
	MD::Parser< TRAIT > parser;

	std::ofstream file( "tests/parser/data/014.md",
		std::ios::out | std::ios::trunc | std::ios::binary );

	if( file.good() )
	{
		const char * str = "Line 1...\rLine 2...\r\nLine 3...\n";
		file.write( str, strlen( str ) );
		file.close();

		auto doc = parser.parse( "tests/parser/data/014.md" );

		REQUIRE( doc->isEmpty() == false );
		REQUIRE( doc->items().size() == 2 );

		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

		auto dp = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( dp->startColumn() == 0 );
		REQUIRE( dp->startLine() == 0 );
		REQUIRE( dp->endColumn() == 8 );
		REQUIRE( dp->endLine() == 2 );

		REQUIRE( dp->items().size() == 1 );

		{
			REQUIRE( dp->items().at( 0 )->type() == MD::ItemType::Text );

			auto dt = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 0 ).get() );

			REQUIRE( dt->opts() == MD::TextOption::TextWithoutFormat );
			REQUIRE( dt->text() == u8"Line 1... Line 2... Line 3..." );
			REQUIRE( dt->startColumn() == 0 );
			REQUIRE( dt->startLine() == 0 );
			REQUIRE( dt->endColumn() == 8 );
			REQUIRE( dt->endLine() == 2 );
		}
	}
	else
		REQUIRE( true == false );
}

TEST_CASE( "015" )
{
	MD::Parser< TRAIT > parser;

	std::ofstream file( "tests/parser/data/015.md",
		std::ios::out | std::ios::trunc | std::ios::binary );

	if( file.good() )
	{
		const char * str = "Line 1...\r\rLine 2...\r\rLine 3...\r";
		file.write( str, strlen( str )  );
		file.close();

		auto doc = parser.parse( "tests/parser/data/015.md" );

		REQUIRE( doc->isEmpty() == false );
		REQUIRE( doc->items().size() == 4 );

		{
			REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

			auto dp = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
			REQUIRE( dp->startColumn() == 0 );
			REQUIRE( dp->startLine() == 0 );
			REQUIRE( dp->endColumn() == 8 );
			REQUIRE( dp->endLine() == 0 );

			REQUIRE( dp->items().size() == 1 );

			REQUIRE( dp->items().at( 0 )->type() == MD::ItemType::Text );

			auto t = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 0 ).get() );

			REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
			REQUIRE( t->text() == u8"Line 1..." );
			REQUIRE( t->startColumn() == 0 );
			REQUIRE( t->startLine() == 0 );
			REQUIRE( t->endColumn() == 8 );
			REQUIRE( t->endLine() == 0 );
		}

		{
			REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );

			auto dp = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).get() );
			REQUIRE( dp->startColumn() == 0 );
			REQUIRE( dp->startLine() == 2 );
			REQUIRE( dp->endColumn() == 8 );
			REQUIRE( dp->endLine() == 2 );

			REQUIRE( dp->items().size() == 1 );

			REQUIRE( dp->items().at( 0 )->type() == MD::ItemType::Text );

			auto t = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 0 ).get() );

			REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
			REQUIRE( t->text() == u8"Line 2..." );
			REQUIRE( t->startColumn() == 0 );
			REQUIRE( t->startLine() == 2 );
			REQUIRE( t->endColumn() == 8 );
			REQUIRE( t->endLine() == 2 );
		}

		{
			REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Paragraph );

			auto dp = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 3 ).get() );
			REQUIRE( dp->startColumn() == 0 );
			REQUIRE( dp->startLine() == 4 );
			REQUIRE( dp->endColumn() == 8 );
			REQUIRE( dp->endLine() == 4 );

			REQUIRE( dp->items().size() == 1 );

			REQUIRE( dp->items().at( 0 )->type() == MD::ItemType::Text );

			auto t = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 0 ).get() );

			REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
			REQUIRE( t->text() == u8"Line 3..." );
			REQUIRE( t->startColumn() == 0 );
			REQUIRE( t->startLine() == 4 );
			REQUIRE( t->endColumn() == 8 );
			REQUIRE( t->endLine() == 4 );
		}
	}
	else
		REQUIRE( true == false );
}

TEST_CASE( "016" )
{
	MD::Parser< TRAIT > parser;

	std::ofstream file( "tests/parser/data/016.md",
		std::ios::out | std::ios::trunc | std::ios::binary );

	if( file.good() )
	{
		const char * str = "Line 1...\r\nLine 2...\r\nLine 3...\r\n";
		file.write( str, strlen( str ) );
		file.close();

		auto doc = parser.parse( "tests/parser/data/016.md" );

		REQUIRE( doc->isEmpty() == false );
		REQUIRE( doc->items().size() == 2 );

		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

		auto dp = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( dp->startColumn() == 0 );
		REQUIRE( dp->startLine() == 0 );
		REQUIRE( dp->endColumn() == 8 );
		REQUIRE( dp->endLine() == 2 );

		REQUIRE( dp->items().size() == 1 );

		{
			REQUIRE( dp->items().at( 0 )->type() == MD::ItemType::Text );

			auto dt = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 0 ).get() );

			REQUIRE( dt->opts() == MD::TextOption::TextWithoutFormat );
			REQUIRE( dt->text() == u8"Line 1... Line 2... Line 3..." );
			REQUIRE( dt->startColumn() == 0 );
			REQUIRE( dt->startLine() == 0 );
			REQUIRE( dt->endColumn() == 8 );
			REQUIRE( dt->endLine() == 2 );
		}
	}
	else
		REQUIRE( true == false );
}

/*
> Quote paragraph 1.
>
> Quote paragraph 2.
>
>> Nested quote

*/
TEST_CASE( "017" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/017.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );

	auto bq = static_cast< MD::Blockquote< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( bq->startColumn() == 0 );
	REQUIRE( bq->startLine() == 0 );
	REQUIRE( bq->endColumn() == 14 );
	REQUIRE( bq->endLine() == 4 );

	REQUIRE( !bq->isEmpty() );
	REQUIRE( bq->items().size() == 3 );

	{
		REQUIRE( bq->items().at( 0 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( bq->items().at( 0 ).get() );
		REQUIRE( p->startColumn() == 2 );
		REQUIRE( p->startLine() == 0 );
		REQUIRE( p->endColumn() == 19 );
		REQUIRE( p->endLine() == 0 );

		REQUIRE( !p->isEmpty() );
		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

		REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( t->text() == u8"Quote paragraph 1." );
		REQUIRE( t->startColumn() == 2 );
		REQUIRE( t->startLine() == 0 );
		REQUIRE( t->endColumn() == 19 );
		REQUIRE( t->endLine() == 0 );
	}

	{
		REQUIRE( bq->items().at( 1 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( bq->items().at( 1 ).get() );
		REQUIRE( p->startColumn() == 2 );
		REQUIRE( p->startLine() == 2 );
		REQUIRE( p->endColumn() == 19 );
		REQUIRE( p->endLine() == 2 );

		REQUIRE( !p->isEmpty() );
		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

		REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( t->text() == u8"Quote paragraph 2." );
		REQUIRE( t->startColumn() == 2 );
		REQUIRE( t->startLine() == 2 );
		REQUIRE( t->endColumn() == 19 );
		REQUIRE( t->endLine() == 2 );
	}

	REQUIRE( bq->items().at( 2 )->type() == MD::ItemType::Blockquote );

	auto nbq = static_cast< MD::Blockquote< TRAIT >* > ( bq->items().at( 2 ).get() );
	REQUIRE( nbq->startColumn() == 1 );
	REQUIRE( nbq->startLine() == 4 );
	REQUIRE( nbq->endColumn() == 14 );
	REQUIRE( nbq->endLine() == 4 );

	REQUIRE( !nbq->isEmpty() );
	REQUIRE( nbq->items().size() == 1 );

	REQUIRE( nbq->items().at( 0 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph< TRAIT >* > ( nbq->items().at( 0 ).get() );
	REQUIRE( p->startColumn() == 3 );
	REQUIRE( p->startLine() == 4 );
	REQUIRE( p->endColumn() == 14 );
	REQUIRE( p->endLine() == 4 );

	REQUIRE( !p->isEmpty() );
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

	REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
	REQUIRE( t->text() == u8"Nested quote" );
	REQUIRE( t->startColumn() == 3 );
	REQUIRE( t->startLine() == 4 );
	REQUIRE( t->endColumn() == 14 );
	REQUIRE( t->endLine() == 4 );
}

/*
> Quote paragraph 1.
>
> Quote paragraph 2.
>
> > Nested quote

*/
TEST_CASE( "018" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/018.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );

	auto bq = static_cast< MD::Blockquote< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( bq->startColumn() == 0 );
	REQUIRE( bq->startLine() == 0 );
	REQUIRE( bq->endColumn() == 15 );
	REQUIRE( bq->endLine() == 4 );

	REQUIRE( !bq->isEmpty() );
	REQUIRE( bq->items().size() == 3 );

	{
		REQUIRE( bq->items().at( 0 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( bq->items().at( 0 ).get() );
		REQUIRE( p->startColumn() == 2 );
		REQUIRE( p->startLine() == 0 );
		REQUIRE( p->endColumn() == 19 );
		REQUIRE( p->endLine() == 0 );

		REQUIRE( !p->isEmpty() );
		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

		REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( t->text() == u8"Quote paragraph 1." );
		REQUIRE( t->startColumn() == 2 );
		REQUIRE( t->startLine() == 0 );
		REQUIRE( t->endColumn() == 19 );
		REQUIRE( t->endLine() == 0 );
	}

	{
		REQUIRE( bq->items().at( 1 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( bq->items().at( 1 ).get() );
		REQUIRE( p->startColumn() == 2 );
		REQUIRE( p->startLine() == 2 );
		REQUIRE( p->endColumn() == 19 );
		REQUIRE( p->endLine() == 2 );

		REQUIRE( !p->isEmpty() );
		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

		REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( t->text() == u8"Quote paragraph 2." );
		REQUIRE( t->startColumn() == 2 );
		REQUIRE( t->startLine() == 2 );
		REQUIRE( t->endColumn() == 19 );
		REQUIRE( t->endLine() == 2 );
	}

	REQUIRE( bq->items().at( 2 )->type() == MD::ItemType::Blockquote );

	auto nbq = static_cast< MD::Blockquote< TRAIT >* > ( bq->items().at( 2 ).get() );
	REQUIRE( nbq->startColumn() == 2 );
	REQUIRE( nbq->startLine() == 4 );
	REQUIRE( nbq->endColumn() == 15 );
	REQUIRE( nbq->endLine() == 4 );

	REQUIRE( !nbq->isEmpty() );
	REQUIRE( nbq->items().size() == 1 );

	REQUIRE( nbq->items().at( 0 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph< TRAIT >* > ( nbq->items().at( 0 ).get() );
	REQUIRE( p->startColumn() == 4 );
	REQUIRE( p->startLine() == 4 );
	REQUIRE( p->endColumn() == 15 );
	REQUIRE( p->endLine() == 4 );

	REQUIRE( !p->isEmpty() );
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

	REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
	REQUIRE( t->text() == u8"Nested quote" );
	REQUIRE( t->startColumn() == 4 );
	REQUIRE( t->startLine() == 4 );
	REQUIRE( t->endColumn() == 15 );
	REQUIRE( t->endLine() == 4 );
}

/*
> Quote paragraph 1.
>
> Quote paragraph 2.
>
> > Nested quote

> Quote paragraph 1.
>
> Quote paragraph 2.
>
> > Nested quote

*/
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
		REQUIRE( bq->startColumn() == 0 );
		REQUIRE( bq->startLine() == 0 + 6 * ( i - 1 ) );
		REQUIRE( bq->endColumn() == 15 );
		REQUIRE( bq->endLine() == 4 + 6 * ( i - 1 ) );

		REQUIRE( !bq->isEmpty() );
		REQUIRE( bq->items().size() == 3 );

		{
			REQUIRE( bq->items().at( 0 )->type() == MD::ItemType::Paragraph );

			auto p = static_cast< MD::Paragraph< TRAIT >* > ( bq->items().at( 0 ).get() );
			REQUIRE( p->startColumn() == 2 );
			REQUIRE( p->startLine() == 0 + 6 * ( i - 1 ) );
			REQUIRE( p->endColumn() == 19 );
			REQUIRE( p->endLine() == 0 + 6 * ( i - 1 ) );

			REQUIRE( !p->isEmpty() );
			REQUIRE( p->items().size() == 1 );

			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

			REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
			REQUIRE( t->text() == u8"Quote paragraph 1." );
			REQUIRE( t->startColumn() == 2 );
			REQUIRE( t->startLine() == 0 + 6 * ( i - 1 ) );
			REQUIRE( t->endColumn() == 19 );
			REQUIRE( t->endLine() == 0 + 6 * ( i - 1 ) );
		}

		{
			REQUIRE( bq->items().at( 1 )->type() == MD::ItemType::Paragraph );

			auto p = static_cast< MD::Paragraph< TRAIT >* > ( bq->items().at( 1 ).get() );
			REQUIRE( p->startColumn() == 2 );
			REQUIRE( p->startLine() == 2 + 6 * ( i - 1 ) );
			REQUIRE( p->endColumn() == 19 );
			REQUIRE( p->endLine() == 2 + 6 * ( i - 1 ) );

			REQUIRE( !p->isEmpty() );
			REQUIRE( p->items().size() == 1 );

			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

			REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
			REQUIRE( t->text() == u8"Quote paragraph 2." );
			REQUIRE( t->startColumn() == 2 );
			REQUIRE( t->startLine() == 2 + 6 * ( i - 1 ) );
			REQUIRE( t->endColumn() == 19 );
			REQUIRE( t->endLine() == 2 + 6 * ( i - 1 ) );
		}

		REQUIRE( bq->items().at( 2 )->type() == MD::ItemType::Blockquote );

		auto nbq = static_cast< MD::Blockquote< TRAIT >* > ( bq->items().at( 2 ).get() );
		REQUIRE( nbq->startColumn() == 2 );
		REQUIRE( nbq->startLine() == 4 + 6 * ( i - 1 ) );
		REQUIRE( nbq->endColumn() == 15 );
		REQUIRE( nbq->endLine() == 4 + 6 * ( i - 1 ) );

		REQUIRE( !nbq->isEmpty() );
		REQUIRE( nbq->items().size() == 1 );

		REQUIRE( nbq->items().at( 0 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( nbq->items().at( 0 ).get() );
		REQUIRE( p->startColumn() == 4 );
		REQUIRE( p->startLine() == 4 + 6 * ( i - 1 ) );
		REQUIRE( p->endColumn() == 15 );
		REQUIRE( p->endLine() == 4 + 6 * ( i - 1 ) );

		REQUIRE( !p->isEmpty() );
		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

		REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( t->text() == u8"Nested quote" );
		REQUIRE( t->startColumn() == 4 );
		REQUIRE( t->startLine() == 4 + 6 * ( i - 1 ) );
		REQUIRE( t->endColumn() == 15 );
		REQUIRE( t->endLine() == 4 + 6 * ( i - 1 ) );
	}
}

/*
```cpp
if( a > b )
  do_something();
else
  dont_do_anything();
```

*/
TEST_CASE( "020" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/020.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );

	auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( c->startColumn() == 0 );
	REQUIRE( c->startLine() == 1 );
	REQUIRE( c->endColumn() == 20 );
	REQUIRE( c->endLine() == 4 );

	REQUIRE( c->isInlined() == false );
	REQUIRE( c->text() ==
		u8"if( a > b )\n  do_something();\nelse\n  dont_do_anything();" );
	REQUIRE( c->syntax() == u8"cpp" );
}

/*
	if( a > b )
      do_something();
    else
      dont_do_anything();
*/
TEST_CASE( "021" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/021.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );

	auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( c->startColumn() == 4 );
	REQUIRE( c->startLine() == 0 );
	REQUIRE( c->endColumn() == 24 );
	REQUIRE( c->endLine() == 3 );

	REQUIRE( c->isInlined() == false );
	REQUIRE( c->text() ==
		u8"if( a > b )\n  do_something();\nelse\n  dont_do_anything();" );
}

/*
	if( a > b )
	  do_something();
	else
	  dont_do_anything();

*/
TEST_CASE( "022" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/022.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );

	auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( c->startColumn() == 1 );
	REQUIRE( c->startLine() == 0 );
	REQUIRE( c->endColumn() == 21 );
	REQUIRE( c->endLine() == 3 );

	REQUIRE( c->isInlined() == false );
	REQUIRE( c->text() ==
		u8"if( a > b )\n  do_something();\nelse\n  dont_do_anything();" );
}

/*
* Item 1
* Item 2
* Item 3

*/
TEST_CASE( "023" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/023.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->startColumn() == 0 );
	REQUIRE( l->startLine() == 0 );
	REQUIRE( l->endColumn() == 7 );
	REQUIRE( l->endLine() == 2 );

	REQUIRE( l->items().size() == 3 );

	for( int i = 0; i < 3; ++i )
	{
		REQUIRE( l->items().at( i )->type() == MD::ItemType::ListItem );

		auto item = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( i ).get() );
		REQUIRE( item->startColumn() == 0 );
		REQUIRE( item->startLine() == i );
		REQUIRE( item->endColumn() == 7 );
		REQUIRE( item->endLine() == i );

		REQUIRE( item->listType() == MD::ListItem< TRAIT >::Unordered );

		REQUIRE( item->items().size() == 1 );

		REQUIRE( item->items().at( 0 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( item->items().at( 0 ).get() );
		REQUIRE( p->startColumn() == 2 );
		REQUIRE( p->startLine() == i );
		REQUIRE( p->endColumn() == 7 );
		REQUIRE( p->endLine() == i );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

		REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( t->text() == u8"Item " + to_string( i + 1 ) );
		REQUIRE( t->startColumn() == 2 );
		REQUIRE( t->startLine() == i );
		REQUIRE( t->endColumn() == 7 );
		REQUIRE( t->endLine() == i );
	}
}

/*
* Item 1
  * Item 1
  * Item 2
* Item 2
  * Item 1
  * Item 2
* Item 3
  * Item 1
  * Item 2

*/
TEST_CASE( "024" )
{
	auto checkItem = [] ( MD::ListItem< TRAIT > * item, int i, int indent, int line )
	{
		REQUIRE( item->items().at( 0 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( item->items().at( 0 ).get() );
		REQUIRE( p->startColumn() == 2 + indent );
		REQUIRE( p->startLine() == line );
		REQUIRE( p->endColumn() == 7 + indent );
		REQUIRE( p->endLine() == line );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

		REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( t->text() == u8"Item " + to_string( i + 1 ) );
		REQUIRE( t->startColumn() == 2 + indent );
		REQUIRE( t->startLine() == line );
		REQUIRE( t->endColumn() == 7 + indent );
		REQUIRE( t->endLine() == line );
	};

	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/024.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->startColumn() == 0 );
	REQUIRE( l->startLine() == 0 );
	REQUIRE( l->endColumn() == 9 );
	REQUIRE( l->endLine() == 8 );

	REQUIRE( l->items().size() == 3 );

	for( int i = 0; i < 3; ++i )
	{
		REQUIRE( l->items().at( i )->type() == MD::ItemType::ListItem );

		auto item = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( i ).get() );
		REQUIRE( item->startColumn() == 0 );
		REQUIRE( item->startLine() == i * 3 );
		REQUIRE( item->endColumn() == 9 );
		REQUIRE( item->endLine() == 2 + i * 3 );

		REQUIRE( item->listType() == MD::ListItem< TRAIT >::Unordered );

		REQUIRE( item->items().size() == 2 );

		checkItem( item, i, 0, i * 3 );

		REQUIRE( item->items().at( 1 )->type() == MD::ItemType::List );

		auto nl = static_cast< MD::List< TRAIT >* > ( item->items().at( 1 ).get() );

		REQUIRE( nl->items().size() == 2 );

		for( int j = 0; j < 2; ++j )
		{
			REQUIRE( nl->items().at( j )->type() == MD::ItemType::ListItem );

			auto nitem = static_cast< MD::ListItem< TRAIT >* > ( nl->items().at( j ).get() );

			checkItem( nitem, j, 2, j + i * 3 + 1 );
		}
	}
}

/*
* Item 1

  Paragraph in list

* Item 2

  Paragraph in list

* Item 3

  Paragraph in list

*/
TEST_CASE( "025" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/025.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->startColumn() == 0 );
	REQUIRE( l->startLine() == 0 );
	REQUIRE( l->endColumn() == 18 );
	REQUIRE( l->endLine() == 10 );

	REQUIRE( l->items().size() == 3 );

	for( int i = 0; i < 3; ++i )
	{
		REQUIRE( l->items().at( i )->type() == MD::ItemType::ListItem );

		auto item = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( i ).get() );
		REQUIRE( item->startColumn() == 0 );
		REQUIRE( item->startLine() == i * 4 );
		REQUIRE( item->endColumn() == 18 );
		REQUIRE( item->endLine() == 2 + i * 4 );

		REQUIRE( item->listType() == MD::ListItem< TRAIT >::Unordered );

		REQUIRE( item->items().size() == 2 );

		{
			REQUIRE( item->items().at( 0 )->type() == MD::ItemType::Paragraph );

			auto p = static_cast< MD::Paragraph< TRAIT >* > ( item->items().at( 0 ).get() );
			REQUIRE( p->startColumn() == 2 );
			REQUIRE( p->startLine() == 0 + i * 4 );
			REQUIRE( p->endColumn() == 7 );
			REQUIRE( p->endLine() == 0 + i * 4 );

			REQUIRE( p->items().size() == 1 );

			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

			REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
			REQUIRE( t->text() == ( u8"Item " + to_string( i + 1 ) ) );
			REQUIRE( t->startColumn() == 2 );
			REQUIRE( t->startLine() == 0 + i * 4 );
			REQUIRE( t->endColumn() == 7 );
			REQUIRE( t->endLine() == 0 + i * 4 );
		}

		{
			REQUIRE( item->items().at( 1 )->type() == MD::ItemType::Paragraph );

			auto p = static_cast< MD::Paragraph< TRAIT >* > ( item->items().at( 1 ).get() );
			REQUIRE( p->startColumn() == 2 );
			REQUIRE( p->startLine() == 2 + i * 4 );
			REQUIRE( p->endColumn() == 18 );
			REQUIRE( p->endLine() == 2 + i * 4 );

			REQUIRE( p->items().size() == 1 );

			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

			REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
			REQUIRE( t->text() == u8"Paragraph in list" );
			REQUIRE( t->startColumn() == 2 );
			REQUIRE( t->startLine() == 2 + i * 4 );
			REQUIRE( t->endColumn() == 18 );
			REQUIRE( t->endLine() == 2 + i * 4 );
		}
	}
}

/*
* Item 1

  Paragraph in list

  * Nested

    Paragraph in list

* Item 2

  Paragraph in list

  * Nested

    Paragraph in list

* Item 3

  Paragraph in list

  * Nested

    Paragraph in list

*/
TEST_CASE( "026" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/026.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->startColumn() == 0 );
	REQUIRE( l->startLine() == 0 );
	REQUIRE( l->endColumn() == 20 );
	REQUIRE( l->endLine() == 22 );

	REQUIRE( l->items().size() == 3 );

	for( int i = 0; i < 3; ++i )
	{
		REQUIRE( l->items().at( i )->type() == MD::ItemType::ListItem );

		auto item = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( i ).get() );
		REQUIRE( item->startColumn() == 0 );
		REQUIRE( item->startLine() == 0 + 8 * i );
		REQUIRE( item->endColumn() == 20 );
		REQUIRE( item->endLine() == 6 + 8 * i );

		REQUIRE( item->listType() == MD::ListItem< TRAIT >::Unordered );

		REQUIRE( item->items().size() == 3 );

		{
			REQUIRE( item->items().at( 0 )->type() == MD::ItemType::Paragraph );

			auto p = static_cast< MD::Paragraph< TRAIT >* > ( item->items().at( 0 ).get() );
			REQUIRE( p->startColumn() == 2 );
			REQUIRE( p->startLine() == 0 + 8 * i );
			REQUIRE( p->endColumn() == 7 );
			REQUIRE( p->endLine() == 0 + 8 * i );

			REQUIRE( p->items().size() == 1 );

			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

			REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
			REQUIRE( t->text() == ( u8"Item " + to_string( i + 1 ) ) );
			REQUIRE( t->startColumn() == 2 );
			REQUIRE( t->startLine() == 0 + 8 * i );
			REQUIRE( t->endColumn() == 7 );
			REQUIRE( t->endLine() == 0 + 8 * i );
		}

		{
			REQUIRE( item->items().at( 1 )->type() == MD::ItemType::Paragraph );

			auto p = static_cast< MD::Paragraph< TRAIT >* > ( item->items().at( 1 ).get() );
			REQUIRE( p->startColumn() == 2 );
			REQUIRE( p->startLine() == 2 + 8 * i );
			REQUIRE( p->endColumn() == 18 );
			REQUIRE( p->endLine() == 2 + 8 * i );

			REQUIRE( p->items().size() == 1 );

			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

			REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
			REQUIRE( t->text() == u8"Paragraph in list" );
			REQUIRE( t->startColumn() == 2 );
			REQUIRE( t->startLine() == 2 + 8 * i );
			REQUIRE( t->endColumn() == 18 );
			REQUIRE( t->endLine() == 2 + 8 * i );
		}

		{
			REQUIRE( item->items().at( 2 )->type() == MD::ItemType::List );

			auto nl = static_cast< MD::List< TRAIT >* > ( item->items().at( 2 ).get() );
			REQUIRE( nl->startColumn() == 2 );
			REQUIRE( nl->startLine() == 4 + 8 * i );
			REQUIRE( nl->endColumn() == 20 );
			REQUIRE( nl->endLine() == 6 + 8 * i );

			REQUIRE( nl->items().at( 0 )->type() == MD::ItemType::ListItem );

			auto item = static_cast< MD::ListItem< TRAIT >* > ( nl->items().at( 0 ).get() );
			REQUIRE( item->startColumn() == 2 );
			REQUIRE( item->startLine() == 4 + 8 * i );
			REQUIRE( item->endColumn() == 20 );
			REQUIRE( item->endLine() == 6 + 8 * i );

			REQUIRE( item->listType() == MD::ListItem< TRAIT >::Unordered );

			REQUIRE( item->items().size() == 2 );

			{
				REQUIRE( item->items().at( 0 )->type() == MD::ItemType::Paragraph );

				auto p = static_cast< MD::Paragraph< TRAIT >* > ( item->items().at( 0 ).get() );
				REQUIRE( p->startColumn() == 4 );
				REQUIRE( p->startLine() == 4 + 8 * i );
				REQUIRE( p->endColumn() == 9 );
				REQUIRE( p->endLine() == 4 + 8 * i );

				REQUIRE( p->items().size() == 1 );

				REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

				auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

				REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
				REQUIRE( t->text() == u8"Nested" );
				REQUIRE( t->startColumn() == 4 );
				REQUIRE( t->startLine() == 4 + 8 * i );
				REQUIRE( t->endColumn() == 9 );
				REQUIRE( t->endLine() == 4 + 8 * i );
			}

			{
				REQUIRE( item->items().at( 1 )->type() == MD::ItemType::Paragraph );

				auto p = static_cast< MD::Paragraph< TRAIT >* > ( item->items().at( 1 ).get() );
				REQUIRE( p->startColumn() == 4 );
				REQUIRE( p->startLine() == 6 + 8 * i );
				REQUIRE( p->endColumn() == 20 );
				REQUIRE( p->endLine() == 6 + 8 * i );

				REQUIRE( p->items().size() == 1 );

				REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

				auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

				REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
				REQUIRE( t->text() == u8"Paragraph in list" );
				REQUIRE( t->startColumn() == 4 );
				REQUIRE( t->startLine() == 6 + 8 * i );
				REQUIRE( t->endColumn() == 20 );
				REQUIRE( t->endLine() == 6 + 8 * i );
			}
		}
	}
}

/*
* Item 1

      code

* Item 2

      code

* Item 3

      code

*/
TEST_CASE( "027" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/027.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->startColumn() == 0 );
	REQUIRE( l->startLine() == 0 );
	REQUIRE( l->endColumn() == 9 );
	REQUIRE( l->endLine() == 10 );

	REQUIRE( l->items().size() == 3 );

	for( int i = 0; i < 3; ++i )
	{
		REQUIRE( l->items().at( i )->type() == MD::ItemType::ListItem );

		auto item = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( i ).get() );
		REQUIRE( item->startColumn() == 0 );
		REQUIRE( item->startLine() == 0 + 4 * i );
		REQUIRE( item->endColumn() == 9 );
		REQUIRE( item->endLine() == 2 + 4 * i );

		REQUIRE( item->listType() == MD::ListItem< TRAIT >::Unordered );

		REQUIRE( item->items().size() == 2 );

		{
			REQUIRE( item->items().at( 0 )->type() == MD::ItemType::Paragraph );

			auto p = static_cast< MD::Paragraph< TRAIT >* > ( item->items().at( 0 ).get() );
			REQUIRE( p->startColumn() == 2 );
			REQUIRE( p->startLine() == 0 + 4 * i );
			REQUIRE( p->endColumn() == 7 );
			REQUIRE( p->endLine() == 0 + 4 * i );

			REQUIRE( p->items().size() == 1 );

			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

			REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
			REQUIRE( t->text() == u8"Item " + to_string( i + 1 ) );
			REQUIRE( t->startColumn() == 2 );
			REQUIRE( t->startLine() == 0 + 4 * i );
			REQUIRE( t->endColumn() == 7 );
			REQUIRE( t->endLine() == 0 + 4 * i );
		}

		{
			REQUIRE( item->items().at( 1 )->type() == MD::ItemType::Code );

			auto c = static_cast< MD::Code< TRAIT >* > ( item->items().at( 1 ).get() );
			REQUIRE( c->startColumn() == 6 );
			REQUIRE( c->startLine() == 2 + 4 * i );
			REQUIRE( c->endColumn() == 9 );
			REQUIRE( c->endLine() == 2 + 4 * i );

			REQUIRE( c->isInlined() == false );
			REQUIRE( c->text() == u8"code" );
		}
	}
}

/*
* Item 1

    ```
    code
    ```

* Item 2

    ```
    code
    ```

* Item 3

    ```
    code
    ```

*/
TEST_CASE( "028" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/028.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->startColumn() == 0 );
	REQUIRE( l->startLine() == 0 );
	REQUIRE( l->endColumn() == 6 );
	REQUIRE( l->endLine() == 16 );

	REQUIRE( l->items().size() == 3 );

	for( int i = 0; i < 3; ++i )
	{
		REQUIRE( l->items().at( i )->type() == MD::ItemType::ListItem );

		auto item = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( i ).get() );
		REQUIRE( item->startColumn() == 0 );
		REQUIRE( item->startLine() == 0 + 6 * i );
		REQUIRE( item->endColumn() == 6 );
		REQUIRE( item->endLine() == 4 + 6 * i );

		REQUIRE( item->listType() == MD::ListItem< TRAIT >::Unordered );

		REQUIRE( item->items().size() == 2 );

		{
			REQUIRE( item->items().at( 0 )->type() == MD::ItemType::Paragraph );

			auto p = static_cast< MD::Paragraph< TRAIT >* > ( item->items().at( 0 ).get() );
			REQUIRE( p->startColumn() == 2 );
			REQUIRE( p->startLine() == 0 + 6 * i );
			REQUIRE( p->endColumn() == 7 );
			REQUIRE( p->endLine() == 0 + 6 * i );

			REQUIRE( p->items().size() == 1 );

			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

			REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
			REQUIRE( t->text() == u8"Item " + to_string( i + 1 ) );
			REQUIRE( t->startColumn() == 2 );
			REQUIRE( t->startLine() == 0 + 6 * i );
			REQUIRE( t->endColumn() == 7 );
			REQUIRE( t->endLine() == 0 + 6 * i );
		}

		{
			REQUIRE( item->items().at( 1 )->type() == MD::ItemType::Code );

			auto c = static_cast< MD::Code< TRAIT >* > ( item->items().at( 1 ).get() );
			REQUIRE( c->startColumn() == 4 );
			REQUIRE( c->startLine() == 3 + 6 * i );
			REQUIRE( c->endColumn() == 7 );
			REQUIRE( c->endLine() == 3 + 6 * i );

			REQUIRE( c->isInlined() == false );
			REQUIRE( c->text() == u8"code" );
		}
	}
}

/*
* Item 1

    Paragraph in list

    * Nested

        Paragraph in list

* Item 2

    Paragraph in list

    * Nested

        Paragraph in list

* Item 3

    Paragraph in list

    * Nested

        Paragraph in list

Standalone paragraph

*/
TEST_CASE( "029" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/029.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->startColumn() == 0 );
	REQUIRE( l->startLine() == 0 );
	REQUIRE( l->endColumn() == 24 );
	REQUIRE( l->endLine() == 22 );

	REQUIRE( l->items().size() == 3 );

	for( int i = 0; i < 3; ++i )
	{
		REQUIRE( l->items().at( i )->type() == MD::ItemType::ListItem );

		auto item = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( i ).get() );
		REQUIRE( item->startColumn() == 0 );
		REQUIRE( item->startLine() == 8 * i );
		REQUIRE( item->endColumn() == 24 );
		REQUIRE( item->endLine() == 6 + 8 * i );

		REQUIRE( item->listType() == MD::ListItem< TRAIT >::Unordered );

		REQUIRE( item->items().size() == 3 );

		{
			REQUIRE( item->items().at( 0 )->type() == MD::ItemType::Paragraph );

			auto p = static_cast< MD::Paragraph< TRAIT >* > ( item->items().at( 0 ).get() );
			REQUIRE( p->startColumn() == 2 );
			REQUIRE( p->startLine() == 8 * i );
			REQUIRE( p->endColumn() == 7 );
			REQUIRE( p->endLine() == 8 * i );

			REQUIRE( p->items().size() == 1 );

			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

			REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
			REQUIRE( t->text() == u8"Item " + to_string( i + 1 ) );
			REQUIRE( t->startColumn() == 2 );
			REQUIRE( t->startLine() == 8 * i );
			REQUIRE( t->endColumn() == 7 );
			REQUIRE( t->endLine() == 8 * i );
		}

		{
			REQUIRE( item->items().at( 1 )->type() == MD::ItemType::Paragraph );

			auto p = static_cast< MD::Paragraph< TRAIT >* > ( item->items().at( 1 ).get() );
			REQUIRE( p->startColumn() == 2 );
			REQUIRE( p->startLine() == 2 + 8 * i );
			REQUIRE( p->endColumn() == 20 );
			REQUIRE( p->endLine() == 2 + 8 * i );

			REQUIRE( p->items().size() == 1 );

			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

			REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
			REQUIRE( t->text() == u8"Paragraph in list" );
			REQUIRE( t->startColumn() == 2 );
			REQUIRE( t->startLine() == 2 + 8 * i );
			REQUIRE( t->endColumn() == 20 );
			REQUIRE( t->endLine() == 2 + 8 * i );
		}

		{
			REQUIRE( item->items().at( 2 )->type() == MD::ItemType::List );

			auto nl = static_cast< MD::List< TRAIT >* > ( item->items().at( 2 ).get() );
			REQUIRE( nl->startColumn() == 4 );
			REQUIRE( nl->startLine() == 4 + 8 * i );
			REQUIRE( nl->endColumn() == 24 );
			REQUIRE( nl->endLine() == 6 + 8 * i );

			REQUIRE( nl->items().at( 0 )->type() == MD::ItemType::ListItem );

			auto item = static_cast< MD::ListItem< TRAIT >* > ( nl->items().at( 0 ).get() );
			REQUIRE( item->startColumn() == 4 );
			REQUIRE( item->startLine() == 4 + 8 * i );
			REQUIRE( item->endColumn() == 24 );
			REQUIRE( item->endLine() == 6 + 8 * i );

			REQUIRE( item->listType() == MD::ListItem< TRAIT >::Unordered );

			REQUIRE( item->items().size() == 2 );

			{
				REQUIRE( item->items().at( 0 )->type() == MD::ItemType::Paragraph );

				auto p = static_cast< MD::Paragraph< TRAIT >* > ( item->items().at( 0 ).get() );
				REQUIRE( p->startColumn() == 6 );
				REQUIRE( p->startLine() == 4 + 8 * i );
				REQUIRE( p->endColumn() == 11 );
				REQUIRE( p->endLine() == 4 + 8 * i );

				REQUIRE( p->items().size() == 1 );

				REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

				auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

				REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
				REQUIRE( t->text() == u8"Nested" );
				REQUIRE( t->startColumn() == 6 );
				REQUIRE( t->startLine() == 4 + 8 * i );
				REQUIRE( t->endColumn() == 11 );
				REQUIRE( t->endLine() == 4 + 8 * i );
			}

			{
				REQUIRE( item->items().at( 1 )->type() == MD::ItemType::Paragraph );

				auto p = static_cast< MD::Paragraph< TRAIT >* > ( item->items().at( 1 ).get() );
				REQUIRE( p->startColumn() == 6 );
				REQUIRE( p->startLine() == 6 + 8 * i );
				REQUIRE( p->endColumn() == 24 );
				REQUIRE( p->endLine() == 6 + 8 * i );

				REQUIRE( p->items().size() == 1 );

				REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

				auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

				REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
				REQUIRE( t->text() == u8"Paragraph in list" );
				REQUIRE( t->startColumn() == 6 );
				REQUIRE( t->startLine() == 6 + 8 * i );
				REQUIRE( t->endColumn() == 24 );
				REQUIRE( t->endLine() == 6 + 8 * i );
			}
		}
	}

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).get() );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 24 );
	REQUIRE( p->endColumn() == 19 );
	REQUIRE( p->endLine() == 24 );

	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

	REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
	REQUIRE( t->text() == u8"Standalone paragraph" );
	REQUIRE( t->startColumn() == 0 );
	REQUIRE( t->startLine() == 24 );
	REQUIRE( t->endColumn() == 19 );
	REQUIRE( t->endLine() == 24 );
}

/*
Text ![Image 1](a.jpg) continue ![ Image 2 ](b.png) and ![ Image 3]( http://www.where.com/c.jpeg "description" )

*/
TEST_CASE( "030" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/030.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 111 );
	REQUIRE( p->endLine() == 0 );

	REQUIRE( p->items().size() == 6 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

	auto t1 = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t1->startColumn() == 0 );
	REQUIRE( t1->startLine() == 0 );
	REQUIRE( t1->endColumn() == 4 );
	REQUIRE( t1->endLine() == 0 );

	REQUIRE( t1->text() == u8"Text" );

	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Image );

	auto i1 = static_cast< MD::Image< TRAIT >* > ( p->items().at( 1 ).get() );
	REQUIRE( i1->startColumn() == 5 );
	REQUIRE( i1->startLine() == 0 );
	REQUIRE( i1->endColumn() == 21 );
	REQUIRE( i1->endLine() == 0 );

	typename TRAIT::String wd =
#ifdef MD4QT_QT_SUPPORT
		QDir().absolutePath()
#else
		std::filesystem::canonical( std::filesystem::current_path() ).u8string()
#endif
		+ u8"/tests/parser/data/";

#ifndef MD4QT_QT_SUPPORT
	std::string tmp;
	wd.toUTF8String( tmp );
	std::replace( tmp.begin(), tmp.end(), '\\', '/' );
	wd = icu::UnicodeString::fromUTF8( tmp );
#endif

	REQUIRE( i1->text() == u8"Image 1" );
	REQUIRE( i1->url() == wd + u8"a.jpg" );

	REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );

	auto t2 = static_cast< MD::Text< TRAIT >* > ( p->items().at( 2 ).get() );
	REQUIRE( t2->startColumn() == 22 );
	REQUIRE( t2->startLine() == 0 );
	REQUIRE( t2->endColumn() == 31 );
	REQUIRE( t2->endLine() == 0 );

	REQUIRE( t2->text() == u8"continue" );

	REQUIRE( p->items().at( 3 )->type() == MD::ItemType::Image );

	auto i2 = static_cast< MD::Image< TRAIT >* > ( p->items().at( 3 ).get() );
	REQUIRE( i2->startColumn() == 32 );
	REQUIRE( i2->startLine() == 0 );
	REQUIRE( i2->endColumn() == 50 );
	REQUIRE( i2->endLine() == 0 );

	REQUIRE( i2->text() == u8"Image 2" );
	REQUIRE( i2->url() == wd + u8"b.png" );

	REQUIRE( p->items().at( 4 )->type() == MD::ItemType::Text );

	auto t3 = static_cast< MD::Text< TRAIT >* > ( p->items().at( 4 ).get() );
	REQUIRE( t3->startColumn() == 51 );
	REQUIRE( t3->startLine() == 0 );
	REQUIRE( t3->endColumn() == 55 );
	REQUIRE( t3->endLine() == 0 );

	REQUIRE( t3->text() == u8"and" );

	REQUIRE( p->items().at( 5 )->type() == MD::ItemType::Image );

	auto i3 = static_cast< MD::Image< TRAIT >* > ( p->items().at( 5 ).get() );
	REQUIRE( i3->startColumn() == 56 );
	REQUIRE( i3->startLine() == 0 );
	REQUIRE( i3->endColumn() == 111 );
	REQUIRE( i3->endLine() == 0 );

	REQUIRE( i3->text() == u8"Image 3" );
	REQUIRE( i3->url() == u8"http://www.where.com/c.jpeg" );
}

/*
[link 0][wrong-label] [link 1](a.md) [![image 1](a.png)](b.md) [link 3][label] [^ref]

[label]: http://www.where.com/a.md (caption)

[^ref] text

[1]: a.md 'title'

[link 4](#label)

*/
TEST_CASE( "031" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/031.md" );

	typename TRAIT::String wd =
#ifdef MD4QT_QT_SUPPORT
		QDir().absolutePath()
#else
		std::filesystem::canonical( std::filesystem::current_path() ).u8string()
#endif
		+ u8"/tests/parser/data";

#ifndef MD4QT_QT_SUPPORT
	std::string tmp;
	wd.toUTF8String( tmp );
	std::replace( tmp.begin(), tmp.end(), '\\', '/' );
	wd = icu::UnicodeString::fromUTF8( tmp );
#endif

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 8 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 84 );
	REQUIRE( p->endLine() == 0 );

	REQUIRE( p->items().size() == 5 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

	auto t0 = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t0->startColumn() == 0 );
	REQUIRE( t0->startLine() == 0 );
	REQUIRE( t0->endColumn() == 20 );
	REQUIRE( t0->endLine() == 0 );
	REQUIRE( t0->isSpaceAfter() );

	REQUIRE( t0->text() == u8"[link 0][wrong-label]" );
	REQUIRE( t0->opts() == MD::TextWithoutFormat );

	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Link );

	auto l1 = static_cast< MD::Link< TRAIT >* > ( p->items().at( 1 ).get() );
	REQUIRE( l1->startColumn() == 22 );
	REQUIRE( l1->startLine() == 0 );
	REQUIRE( l1->endColumn() == 35 );
	REQUIRE( l1->endLine() == 0 );

	REQUIRE( l1->text() == u8"link 1" );
	REQUIRE( l1->url() == wd + u8"/a.md" );

	REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Link );

	auto l2 = static_cast< MD::Link< TRAIT >* > ( p->items().at( 2 ).get() );
	REQUIRE( l2->startColumn() == 37 );
	REQUIRE( l2->startLine() == 0 );
	REQUIRE( l2->endColumn() == 61 );
	REQUIRE( l2->endLine() == 0 );

	REQUIRE( l2->url() == wd + u8"/b.md" );
	REQUIRE( l2->opts() == MD::TextOption::TextWithoutFormat );

	REQUIRE( l2->img().get() );
	REQUIRE( l2->img()->text() == u8"image 1" );
	REQUIRE( l2->img()->url() == wd + u8"/" + u8"a.png" );

	REQUIRE( p->items().at( 3 )->type() == MD::ItemType::Link );

	auto l3 = static_cast< MD::Link< TRAIT >* > ( p->items().at( 3 ).get() );
	REQUIRE( l3->startColumn() == 63 );
	REQUIRE( l3->startLine() == 0 );
	REQUIRE( l3->endColumn() == 77 );
	REQUIRE( l3->endLine() == 0 );

	REQUIRE( l3->text() == u8"link 3" );

	const TRAIT::String label = u8"#LABEL/" + wd + u8"/031.md";

	REQUIRE( l3->url() == label );

	REQUIRE( p->items().at( 4 )->type() == MD::ItemType::FootnoteRef );

	auto f1 = static_cast< MD::FootnoteRef< TRAIT >* > ( p->items().at( 4 ).get() );
	REQUIRE( f1->startColumn() == 79 );
	REQUIRE( f1->startLine() == 0 );
	REQUIRE( f1->endColumn() == 84 );
	REQUIRE( f1->endLine() == 0 );

	REQUIRE( f1->id() == u8"#^REF/" + wd + u8"/031.md" );

	REQUIRE( !doc->labeledLinks().empty() );
	const auto lit = doc->labeledLinks().find( label );
	REQUIRE( lit != doc->labeledLinks().cend() );
	REQUIRE( lit->second->url() == u8"http://www.where.com/a.md" );


	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );

		p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( p->startColumn() == 0 );
		REQUIRE( p->startLine() == 4 );
		REQUIRE( p->endColumn() == 10 );
		REQUIRE( p->endLine() == 4 );

		REQUIRE( p->items().size() == 2 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::FootnoteRef );

		f1 = static_cast< MD::FootnoteRef< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( f1->startColumn() == 0 );
		REQUIRE( f1->startLine() == 4 );
		REQUIRE( f1->endColumn() == 5 );
		REQUIRE( f1->endLine() == 4 );

		REQUIRE( f1->id() == u8"#^REF/" + wd + u8"/031.md" );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( t->isSpaceBefore() );
		REQUIRE( t->startColumn() == 6 );
		REQUIRE( t->startLine() == 4 );
		REQUIRE( t->endColumn() == 10 );
		REQUIRE( t->endLine() == 4 );

		REQUIRE( t->text() == u8"text" );

		REQUIRE( doc->labeledLinks().size() == 2 );

		const auto lit = doc->labeledLinks().find( u8"#1/" + wd + u8"/031.md" );
		REQUIRE( lit != doc->labeledLinks().cend() );

		REQUIRE( lit->second->url() == wd + u8"/a.md" );
	}

	{
		REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Paragraph );

		p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 3 ).get() );
		REQUIRE( p->startColumn() == 0 );
		REQUIRE( p->startLine() == 8 );
		REQUIRE( p->endColumn() == 15 );
		REQUIRE( p->endLine() == 8 );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );

		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( l->startColumn() == 0 );
		REQUIRE( l->startLine() == 8 );
		REQUIRE( l->endColumn() == 15 );
		REQUIRE( l->endLine() == 8 );

		REQUIRE( l->url() == u8"#label/" + wd + u8"/031.md" );
	}

	REQUIRE( doc->items().at( 4 )->type() == MD::ItemType::PageBreak );
}

/*
> ```cpp
> if( a < b )
>   do_something();
> ```

*/
TEST_CASE( "032" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/032.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );

	auto q = static_cast< MD::Blockquote< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( q->startColumn() == 0 );
	REQUIRE( q->startLine() == 0 );
	REQUIRE( q->endColumn() == 4 );
	REQUIRE( q->endLine() == 3 );

	REQUIRE( q->items().size() == 1 );

	REQUIRE( q->items().at( 0 )->type() == MD::ItemType::Code );

	auto c = static_cast< MD::Code< TRAIT >* > ( q->items().at( 0 ).get() );
	REQUIRE( c->startColumn() == 2 );
	REQUIRE( c->startLine() == 1 );
	REQUIRE( c->endColumn() == 18 );
	REQUIRE( c->endLine() == 2 );

	REQUIRE( c->isInlined() == false );
	REQUIRE( c->text() == u8"if( a < b )\n  do_something();" );
}

/*
<http://www.google.com>

*/
TEST_CASE( "033" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/033.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 22 );
	REQUIRE( p->endLine() == 0 );

	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );

	auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( l->startColumn() == 0 );
	REQUIRE( l->startLine() == 0 );
	REQUIRE( l->endColumn() == 22 );
	REQUIRE( l->endLine() == 0 );

	REQUIRE( l->url() == u8"http://www.google.com" );
	REQUIRE( l->text().isEmpty() );
}

/*
**[Google](https://www.google.com)**

*/
TEST_CASE( "034" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/034.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 35 );
	REQUIRE( p->endLine() == 0 );

	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );

	auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( l->startColumn() == 2 );
	REQUIRE( l->startLine() == 0 );
	REQUIRE( l->endColumn() == 33 );
	REQUIRE( l->endLine() == 0 );

	REQUIRE( l->url() == u8"https://www.google.com" );
	REQUIRE( l->text() == u8"Google" );
	REQUIRE( l->opts() == MD::TextOption::BoldText );
}

/*
1. 1
2. 2
   1. 1
   2. 2
3. 3

*/
TEST_CASE( "035" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/035.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->startColumn() == 0 );
	REQUIRE( l->startLine() == 0 );
	REQUIRE( l->endColumn() == 3 );
	REQUIRE( l->endLine() == 4 );

	REQUIRE( l->items().size() == 3 );

	{
		REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );

		auto i1 = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
		REQUIRE( i1->startColumn() == 0 );
		REQUIRE( i1->startLine() == 0 );
		REQUIRE( i1->endColumn() == 3 );
		REQUIRE( i1->endLine() == 0 );

		REQUIRE( i1->listType() == MD::ListItem< TRAIT >::Ordered );
		REQUIRE( i1->orderedListPreState() == MD::ListItem< TRAIT >::Start );
		REQUIRE( i1->items().size() == 1 );
		REQUIRE( i1->items().at( 0 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( i1->items().at( 0 ).get() );
		REQUIRE( p->startColumn() == 3 );
		REQUIRE( p->startLine() == 0 );
		REQUIRE( p->endColumn() == 3 );
		REQUIRE( p->endLine() == 0 );

		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		REQUIRE( static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() )->text() == u8"1" );
	}

	REQUIRE( l->items().size() == 3 );

	{
		REQUIRE( l->items().at( 1 )->type() == MD::ItemType::ListItem );

		auto i1 = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 1 ).get() );
		REQUIRE( i1->startColumn() == 0 );
		REQUIRE( i1->startLine() == 1 );
		REQUIRE( i1->endColumn() == 6 );
		REQUIRE( i1->endLine() == 3 );

		REQUIRE( i1->listType() == MD::ListItem< TRAIT >::Ordered );
		REQUIRE( i1->orderedListPreState() == MD::ListItem< TRAIT >::Continue );
		REQUIRE( i1->items().size() == 2 );
		REQUIRE( i1->items().at( 0 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( i1->items().at( 0 ).get() );
		REQUIRE( p->startColumn() == 3 );
		REQUIRE( p->startLine() == 1 );
		REQUIRE( p->endColumn() == 3 );
		REQUIRE( p->endLine() == 1 );

		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		REQUIRE( static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() )->text() == u8"2" );

		REQUIRE( i1->items().at( 1 )->type() == MD::ItemType::List );

		auto nl = static_cast< MD::List< TRAIT >* > ( i1->items().at( 1 ).get() );
		REQUIRE( nl->startColumn() == 3 );
		REQUIRE( nl->startLine() == 2 );
		REQUIRE( nl->endColumn() == 6 );
		REQUIRE( nl->endLine() == 3 );

		REQUIRE( nl->items().size() == 2 );

		{
			REQUIRE( nl->items().at( 0 )->type() == MD::ItemType::ListItem );

			auto i1 = static_cast< MD::ListItem< TRAIT >* > ( nl->items().at( 0 ).get() );
			REQUIRE( i1->startColumn() == 3 );
			REQUIRE( i1->startLine() == 2 );
			REQUIRE( i1->endColumn() == 6 );
			REQUIRE( i1->endLine() == 2 );

			REQUIRE( i1->listType() == MD::ListItem< TRAIT >::Ordered );
			REQUIRE( i1->orderedListPreState() == MD::ListItem< TRAIT >::Start );
			REQUIRE( i1->items().size() == 1 );
			REQUIRE( i1->items().at( 0 )->type() == MD::ItemType::Paragraph );

			auto p = static_cast< MD::Paragraph< TRAIT >* > ( i1->items().at( 0 ).get() );
			REQUIRE( p->startColumn() == 6 );
			REQUIRE( p->startLine() == 2 );
			REQUIRE( p->endColumn() == 6 );
			REQUIRE( p->endLine() == 2 );

			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			REQUIRE( static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() )->text() == u8"1" );
		}

		{
			REQUIRE( nl->items().at( 1 )->type() == MD::ItemType::ListItem );

			auto i1 = static_cast< MD::ListItem< TRAIT >* > ( nl->items().at( 1 ).get() );
			REQUIRE( i1->startColumn() == 3 );
			REQUIRE( i1->startLine() == 3 );
			REQUIRE( i1->endColumn() == 6 );
			REQUIRE( i1->endLine() == 3 );

			REQUIRE( i1->listType() == MD::ListItem< TRAIT >::Ordered );
			REQUIRE( i1->orderedListPreState() == MD::ListItem< TRAIT >::Continue );
			REQUIRE( i1->items().size() == 1 );
			REQUIRE( i1->items().at( 0 )->type() == MD::ItemType::Paragraph );

			auto p = static_cast< MD::Paragraph< TRAIT >* > ( i1->items().at( 0 ).get() );
			REQUIRE( p->startColumn() == 6 );
			REQUIRE( p->startLine() == 3 );
			REQUIRE( p->endColumn() == 6 );
			REQUIRE( p->endLine() == 3 );

			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			REQUIRE( static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() )->text() == u8"2" );
		}
	}

	{
		REQUIRE( l->items().at( 2 )->type() == MD::ItemType::ListItem );

		auto i1 = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 2 ).get() );
		REQUIRE( i1->startColumn() == 0 );
		REQUIRE( i1->startLine() == 4 );
		REQUIRE( i1->endColumn() == 3 );
		REQUIRE( i1->endLine() == 4 );

		REQUIRE( i1->listType() == MD::ListItem< TRAIT >::Ordered );
		REQUIRE( i1->orderedListPreState() == MD::ListItem< TRAIT >::Continue );
		REQUIRE( i1->items().size() == 1 );
		REQUIRE( i1->items().at( 0 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( i1->items().at( 0 ).get() );
		REQUIRE( p->startColumn() == 3 );
		REQUIRE( p->startLine() == 4 );
		REQUIRE( p->endColumn() == 3 );
		REQUIRE( p->endLine() == 4 );

		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		REQUIRE( static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() )->text() == u8"3" );
	}
}

/*
[Google]( www.google.com "Google Shmoogle..." )

*/
TEST_CASE( "036" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/036.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 46 );
	REQUIRE( p->endLine() == 0 );

	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );

	auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( l->startColumn() == 0 );
	REQUIRE( l->startLine() == 0 );
	REQUIRE( l->endColumn() == 46 );
	REQUIRE( l->endLine() == 0 );

	REQUIRE( l->url() == u8"www.google.com" );
	REQUIRE( l->text() == u8"Google" );
}

/*
[Google] ( www.google.com Google Shmoogle...

[Google] (

[Google

[Google]

[![Google](

![Google](

[Google] ( www.google.com "Google Shmoogle..."

[![Google](https://www.google.com/logo.png)

text [^ref]:

[^ref

[

[1]:

[1]:

[text][link

[text]#

<www.google.com

*/
TEST_CASE( "037" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/037.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 17 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( p->startColumn() == 0 );
		REQUIRE( p->startLine() == 0 );
		REQUIRE( p->endColumn() == 43 );
		REQUIRE( p->endLine() == 0 );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 0 );
		REQUIRE( t->endColumn() == 43 );
		REQUIRE( t->endLine() == 0 );

		REQUIRE( t->text() == u8"[Google] ( www.google.com Google Shmoogle..." );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( p->startColumn() == 0 );
		REQUIRE( p->startLine() == 2 );
		REQUIRE( p->endColumn() == 9 );
		REQUIRE( p->endLine() == 2 );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 2 );
		REQUIRE( t->endColumn() == 9 );
		REQUIRE( t->endLine() == 2 );

		REQUIRE( t->text() == u8"[Google] (" );
	}

	{
		REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 3 ).get() );
		REQUIRE( p->startColumn() == 0 );
		REQUIRE( p->startLine() == 4 );
		REQUIRE( p->endColumn() == 6 );
		REQUIRE( p->endLine() == 4 );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 4 );
		REQUIRE( t->endColumn() == 6 );
		REQUIRE( t->endLine() == 4 );

		REQUIRE( t->text() == u8"[Google" );
	}

	{
		REQUIRE( doc->items().at( 4 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 4 ).get() );
		REQUIRE( p->startColumn() == 0 );
		REQUIRE( p->startLine() == 6 );
		REQUIRE( p->endColumn() == 7 );
		REQUIRE( p->endLine() == 6 );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

		REQUIRE( t->text() == u8"[Google]" );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 6 );
		REQUIRE( t->endColumn() == 7 );
		REQUIRE( t->endLine() == 6 );
	}

	{
		REQUIRE( doc->items().at( 5 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 5 ).get() );
		REQUIRE( p->startColumn() == 0 );
		REQUIRE( p->startLine() == 8 );
		REQUIRE( p->endColumn() == 10 );
		REQUIRE( p->endLine() == 8 );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

		REQUIRE( t->text() == u8"[![Google](" );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 8 );
		REQUIRE( t->endColumn() == 10 );
		REQUIRE( t->endLine() == 8 );
	}

	{
		REQUIRE( doc->items().at( 6 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 6 ).get() );
		REQUIRE( p->startColumn() == 0 );
		REQUIRE( p->startLine() == 10 );
		REQUIRE( p->endColumn() == 9 );
		REQUIRE( p->endLine() == 10 );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

		REQUIRE( t->text() == u8"![Google](" );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 10 );
		REQUIRE( t->endColumn() == 9 );
		REQUIRE( t->endLine() == 10 );
	}

	{
		REQUIRE( doc->items().at( 7 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 7 ).get() );
		REQUIRE( p->startColumn() == 0 );
		REQUIRE( p->startLine() == 12 );
		REQUIRE( p->endColumn() == 45 );
		REQUIRE( p->endLine() == 12 );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 12 );
		REQUIRE( t->endColumn() == 45 );
		REQUIRE( t->endLine() == 12 );

		REQUIRE( t->text() == u8"[Google] ( www.google.com \"Google Shmoogle...\"" );
	}

	{
		REQUIRE( doc->items().at( 8 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 8 ).get() );
		REQUIRE( p->startColumn() == 0 );
		REQUIRE( p->startLine() == 14 );
		REQUIRE( p->endColumn() == 42 );
		REQUIRE( p->endLine() == 14 );

		REQUIRE( p->items().size() == 2 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 14 );
		REQUIRE( t->endColumn() == 0 );
		REQUIRE( t->endLine() == 14 );

		REQUIRE( t->text() == u8"[" );

		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Image );

		auto i = static_cast< MD::Image< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( i->startColumn() == 1 );
		REQUIRE( i->startLine() == 14 );
		REQUIRE( i->endColumn() == 42 );
		REQUIRE( i->endLine() == 14 );

		REQUIRE( i->url() == u8"https://www.google.com/logo.png" );
		REQUIRE( i->text() == u8"Google" );
	}

	{
		REQUIRE( doc->items().at( 9 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 9 ).get() );
		REQUIRE( p->startColumn() == 0 );
		REQUIRE( p->startLine() == 16 );
		REQUIRE( p->endColumn() == 11 );
		REQUIRE( p->endLine() == 16 );

		REQUIRE( p->items().size() == 3 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 16 );
		REQUIRE( t->endColumn() == 4 );
		REQUIRE( t->endLine() == 16 );

		REQUIRE( t->text() == u8"text" );

		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::FootnoteRef );

		REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );

		t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 2 ).get() );
		REQUIRE( t->startColumn() == 11 );
		REQUIRE( t->startLine() == 16 );
		REQUIRE( t->endColumn() == 11 );
		REQUIRE( t->endLine() == 16 );

		REQUIRE( t->text() == u8":" );
	}

	{
		REQUIRE( doc->items().at( 10 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 10 ).get() );
		REQUIRE( p->startColumn() == 0 );
		REQUIRE( p->startLine() == 18 );
		REQUIRE( p->endColumn() == 4 );
		REQUIRE( p->endLine() == 18 );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 18 );
		REQUIRE( t->endColumn() == 4 );
		REQUIRE( t->endLine() == 18 );

		REQUIRE( t->text() == u8"[^ref" );
	}

	{
		REQUIRE( doc->items().at( 11 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 11 ).get() );
		REQUIRE( p->startColumn() == 0 );
		REQUIRE( p->startLine() == 20 );
		REQUIRE( p->endColumn() == 0 );
		REQUIRE( p->endLine() == 20 );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 20 );
		REQUIRE( t->endColumn() == 0 );
		REQUIRE( t->endLine() == 20 );

		REQUIRE( t->text() == u8"[" );
	}

	{
		REQUIRE( doc->items().at( 12 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 12 ).get() );
		REQUIRE( p->startColumn() == 0 );
		REQUIRE( p->startLine() == 22 );
		REQUIRE( p->endColumn() == 3 );
		REQUIRE( p->endLine() == 22 );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 22 );
		REQUIRE( t->endColumn() == 3 );
		REQUIRE( t->endLine() == 22 );

		REQUIRE( t->text() == u8"[1]:" );
	}

	{
		REQUIRE( doc->items().at( 13 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 13 ).get() );
		REQUIRE( p->startColumn() == 0 );
		REQUIRE( p->startLine() == 24 );
		REQUIRE( p->endColumn() == 4 );
		REQUIRE( p->endLine() == 24 );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 24 );
		REQUIRE( t->endColumn() == 4 );
		REQUIRE( t->endLine() == 24 );
		REQUIRE( t->isSpaceAfter() );
		REQUIRE( t->isSpaceBefore() );

		REQUIRE( t->text() == u8"[1]:" );
	}

	{
		REQUIRE( doc->items().at( 14 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 14 ).get() );
		REQUIRE( p->startColumn() == 0 );
		REQUIRE( p->startLine() == 26 );
		REQUIRE( p->endColumn() == 10 );
		REQUIRE( p->endLine() == 26 );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 26 );
		REQUIRE( t->endColumn() == 10 );
		REQUIRE( t->endLine() == 26 );

		REQUIRE( t->text() == u8"[text][link" );
	}

	{
		REQUIRE( doc->items().at( 15 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 15 ).get() );
		REQUIRE( p->startColumn() == 0 );
		REQUIRE( p->startLine() == 28 );
		REQUIRE( p->endColumn() == 6 );
		REQUIRE( p->endLine() == 28 );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 28 );
		REQUIRE( t->endColumn() == 6 );
		REQUIRE( t->endLine() == 28 );

		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"[text]#" );
	}

	{
		REQUIRE( doc->items().at( 16 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 16 ).get() );
		REQUIRE( p->startColumn() == 0 );
		REQUIRE( p->startLine() == 30 );
		REQUIRE( p->endColumn() == 14 );
		REQUIRE( p->endLine() == 30 );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 30 );
		REQUIRE( t->endColumn() == 14 );
		REQUIRE( t->endLine() == 30 );

		REQUIRE( t->text() == u8"<www.google.com" );
	}
}
/*
1. 1
2. 2
	1. 1
	2. 2
3. 3

*/
TEST_CASE( "038" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/038.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->startColumn() == 0 );
	REQUIRE( l->startLine() == 0 );
	REQUIRE( l->endColumn() == 3 );
	REQUIRE( l->endLine() == 4 );

	REQUIRE( l->items().size() == 3 );

	{
		REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );

		auto i1 = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
		REQUIRE( i1->startColumn() == 0 );
		REQUIRE( i1->startLine() == 0 );
		REQUIRE( i1->endColumn() == 3 );
		REQUIRE( i1->endLine() == 0 );

		REQUIRE( i1->listType() == MD::ListItem< TRAIT >::Ordered );
		REQUIRE( i1->orderedListPreState() == MD::ListItem< TRAIT >::Start );
		REQUIRE( i1->items().size() == 1 );
		REQUIRE( i1->items().at( 0 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( i1->items().at( 0 ).get() );
		REQUIRE( p->startColumn() == 3 );
		REQUIRE( p->startLine() == 0 );
		REQUIRE( p->endColumn() == 3 );
		REQUIRE( p->endLine() == 0 );

		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		REQUIRE( static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() )->text() == u8"1" );
	}

	REQUIRE( l->items().size() == 3 );

	{
		REQUIRE( l->items().at( 1 )->type() == MD::ItemType::ListItem );

		auto i1 = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 1 ).get() );
		REQUIRE( i1->startColumn() == 0 );
		REQUIRE( i1->startLine() == 1 );
		REQUIRE( i1->endColumn() == 4 );
		REQUIRE( i1->endLine() == 3 );

		REQUIRE( i1->listType() == MD::ListItem< TRAIT >::Ordered );
		REQUIRE( i1->orderedListPreState() == MD::ListItem< TRAIT >::Continue );
		REQUIRE( i1->items().size() == 2 );
		REQUIRE( i1->items().at( 0 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( i1->items().at( 0 ).get() );
		REQUIRE( p->startColumn() == 3 );
		REQUIRE( p->startLine() == 1 );
		REQUIRE( p->endColumn() == 3 );
		REQUIRE( p->endLine() == 1 );

		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		REQUIRE( static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() )->text() == u8"2" );

		REQUIRE( i1->items().at( 1 )->type() == MD::ItemType::List );

		auto nl = static_cast< MD::List< TRAIT >* > ( i1->items().at( 1 ).get() );
		REQUIRE( nl->startColumn() == 1 );
		REQUIRE( nl->startLine() == 2 );
		REQUIRE( nl->endColumn() == 4 );
		REQUIRE( nl->endLine() == 3 );

		REQUIRE( nl->items().size() == 2 );

		{
			REQUIRE( nl->items().at( 0 )->type() == MD::ItemType::ListItem );

			auto i1 = static_cast< MD::ListItem< TRAIT >* > ( nl->items().at( 0 ).get() );
			REQUIRE( i1->startColumn() == 1 );
			REQUIRE( i1->startLine() == 2 );
			REQUIRE( i1->endColumn() == 4 );
			REQUIRE( i1->endLine() == 2 );

			REQUIRE( i1->listType() == MD::ListItem< TRAIT >::Ordered );
			REQUIRE( i1->orderedListPreState() == MD::ListItem< TRAIT >::Start );
			REQUIRE( i1->items().size() == 1 );
			REQUIRE( i1->items().at( 0 )->type() == MD::ItemType::Paragraph );

			auto p = static_cast< MD::Paragraph< TRAIT >* > ( i1->items().at( 0 ).get() );
			REQUIRE( p->startColumn() == 4 );
			REQUIRE( p->startLine() == 2 );
			REQUIRE( p->endColumn() == 4 );
			REQUIRE( p->endLine() == 2 );

			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			REQUIRE( static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() )->text() == u8"1" );
		}

		{
			REQUIRE( nl->items().at( 1 )->type() == MD::ItemType::ListItem );

			auto i1 = static_cast< MD::ListItem< TRAIT >* > ( nl->items().at( 1 ).get() );
			REQUIRE( i1->startColumn() == 1 );
			REQUIRE( i1->startLine() == 3 );
			REQUIRE( i1->endColumn() == 4 );
			REQUIRE( i1->endLine() == 3 );

			REQUIRE( i1->listType() == MD::ListItem< TRAIT >::Ordered );
			REQUIRE( i1->orderedListPreState() == MD::ListItem< TRAIT >::Continue );
			REQUIRE( i1->items().size() == 1 );
			REQUIRE( i1->items().at( 0 )->type() == MD::ItemType::Paragraph );

			auto p = static_cast< MD::Paragraph< TRAIT >* > ( i1->items().at( 0 ).get() );
			REQUIRE( p->startColumn() == 4 );
			REQUIRE( p->startLine() == 3 );
			REQUIRE( p->endColumn() == 4 );
			REQUIRE( p->endLine() == 3 );

			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			REQUIRE( static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() )->text() == u8"2" );
		}
	}

	{
		REQUIRE( l->items().at( 2 )->type() == MD::ItemType::ListItem );

		auto i1 = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 2 ).get() );
		REQUIRE( i1->startColumn() == 0 );
		REQUIRE( i1->startLine() == 4 );
		REQUIRE( i1->endColumn() == 3 );
		REQUIRE( i1->endLine() == 4 );

		REQUIRE( i1->listType() == MD::ListItem< TRAIT >::Ordered );
		REQUIRE( i1->orderedListPreState() == MD::ListItem< TRAIT >::Continue );
		REQUIRE( i1->items().size() == 1 );
		REQUIRE( i1->items().at( 0 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( i1->items().at( 0 ).get() );
		REQUIRE( p->startColumn() == 3 );
		REQUIRE( p->startLine() == 4 );
		REQUIRE( p->endColumn() == 3 );
		REQUIRE( p->endLine() == 4 );

		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		REQUIRE( static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() )->text() == u8"3" );
	}
}

/*
****text****

*/
TEST_CASE( "039" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/039.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 11 );
	REQUIRE( p->endLine() == 0 );

	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->startColumn() == 4 );
	REQUIRE( t->startLine() == 0 );
	REQUIRE( t->endColumn() == 7 );
	REQUIRE( t->endLine() == 0 );

	REQUIRE( t->opts() == MD::TextOption::BoldText );
	REQUIRE( t->text() == u8"text" );
}

/*
``Use this `code`
in the code

*/
TEST_CASE( "040" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/040.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto dp = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( dp->startColumn() == 0 );
	REQUIRE( dp->startLine() == 0 );
	REQUIRE( dp->endColumn() == 10 );
	REQUIRE( dp->endLine() == 1 );

	REQUIRE( dp->items().size() == 3 );

	REQUIRE( dp->items().at( 0 )->type() == MD::ItemType::Text );

	auto t1 = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 0 ).get() );

	REQUIRE( t1->text() == u8"``Use this" );
	REQUIRE( t1->startColumn() == 0 );
	REQUIRE( t1->startLine() == 0 );
	REQUIRE( t1->endColumn() == 10 );
	REQUIRE( t1->endLine() == 0 );
	REQUIRE( t1->isSpaceAfter() );

	REQUIRE( dp->items().at( 1 )->type() == MD::ItemType::Code );

	auto c1 = static_cast< MD::Code< TRAIT >* > ( dp->items().at( 1 ).get() );
	REQUIRE( c1->startColumn() == 12 );
	REQUIRE( c1->startLine() == 0 );
	REQUIRE( c1->endColumn() == 15 );
	REQUIRE( c1->endLine() == 0 );

	REQUIRE( c1->text() == u8"code" );

	REQUIRE( dp->items().at( 2 )->type() == MD::ItemType::Text );

	auto t2 = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 2 ).get() );
	REQUIRE( t2->startColumn() == 0 );
	REQUIRE( t2->startLine() == 1 );
	REQUIRE( t2->endColumn() == 10 );
	REQUIRE( t2->endLine() == 1 );

	REQUIRE( t2->text() == u8"in the code" );
}

/*
* Item 1

	Paragraph in list

* Item 2

	Paragraph in list

* Item 3

	Paragraph in list

*/
TEST_CASE( "041" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/041.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->startColumn() == 0 );
	REQUIRE( l->startLine() == 0 );
	REQUIRE( l->endColumn() == 17 );
	REQUIRE( l->endLine() == 10 );

	REQUIRE( l->items().size() == 3 );

	for( int i = 0; i < 3; ++i )
	{
		REQUIRE( l->items().at( i )->type() == MD::ItemType::ListItem );

		auto item = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( i ).get() );
		REQUIRE( item->startColumn() == 0 );
		REQUIRE( item->startLine() == 4 * i );
		REQUIRE( item->endColumn() == 17 );
		REQUIRE( item->endLine() == 2 + 4 * i );

		REQUIRE( item->listType() == MD::ListItem< TRAIT >::Unordered );

		REQUIRE( item->items().size() == 2 );

		{
			REQUIRE( item->items().at( 0 )->type() == MD::ItemType::Paragraph );

			auto p = static_cast< MD::Paragraph< TRAIT >* > ( item->items().at( 0 ).get() );
			REQUIRE( p->startColumn() == 2 );
			REQUIRE( p->startLine() == 4 * i );
			REQUIRE( p->endColumn() == 7 );
			REQUIRE( p->endLine() == 4 * i );

			REQUIRE( p->items().size() == 1 );

			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
			REQUIRE( t->startColumn() == 2 );
			REQUIRE( t->startLine() == 4 * i );
			REQUIRE( t->endColumn() == 7 );
			REQUIRE( t->endLine() == 4 * i );

			REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
			REQUIRE( t->text() == u8"Item " + to_string( i + 1 ) );
		}

		{
			REQUIRE( item->items().at( 1 )->type() == MD::ItemType::Paragraph );

			auto p = static_cast< MD::Paragraph< TRAIT >* > ( item->items().at( 1 ).get() );
			REQUIRE( p->startColumn() == 0 );
			REQUIRE( p->startLine() == 2 + 4 * i );
			REQUIRE( p->endColumn() == 17 );
			REQUIRE( p->endLine() == 2 + 4 * i );

			REQUIRE( p->items().size() == 1 );

			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
			REQUIRE( t->startColumn() == 0 );
			REQUIRE( t->startLine() == 2 + 4 * i );
			REQUIRE( t->endColumn() == 17 );
			REQUIRE( t->endLine() == 2 + 4 * i );

			REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
			REQUIRE( t->text() == u8"Paragraph in list" );
		}
	}
}

/*
* [Chapter 1](042-1.md)
* [Chapter 1](042-1.md)

*/
TEST_CASE( "042" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/042.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 5 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->startColumn() == 0 );
	REQUIRE( l->startLine() == 0 );
	REQUIRE( l->endColumn() == 22 );
	REQUIRE( l->endLine() == 1 );

	REQUIRE( l->items().size() == 2 );

	typename TRAIT::String wd =
#ifdef MD4QT_QT_SUPPORT
		QDir().absolutePath()
#else
		std::filesystem::canonical( std::filesystem::current_path() ).u8string()
#endif
		+ u8"/tests/parser/data";

#ifndef MD4QT_QT_SUPPORT
	std::string tmp;
	wd.toUTF8String( tmp );
	std::replace( tmp.begin(), tmp.end(), '\\', '/' );
	wd = icu::UnicodeString::fromUTF8( tmp );
#endif

	for( int i = 0; i < 2; ++i )
	{
		REQUIRE( l->items().at( i )->type() == MD::ItemType::ListItem );

		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( i ).get() );
		REQUIRE( li->startColumn() == 0 );
		REQUIRE( li->startLine() == i );
		REQUIRE( li->endColumn() == 22 );
		REQUIRE( li->endLine() == i );

		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
		REQUIRE( p->startColumn() == 2 );
		REQUIRE( p->startLine() == i );
		REQUIRE( p->endColumn() == 22 );
		REQUIRE( p->endLine() == i );

		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );

		auto lnk = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( lnk->startColumn() == 2 );
		REQUIRE( lnk->startLine() == i );
		REQUIRE( lnk->endColumn() == 22 );
		REQUIRE( lnk->endLine() == i );

		REQUIRE( lnk->text() == u8"Chapter 1" );
		REQUIRE( lnk->url() == wd + u8"/042-1.md" );
	}

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::PageBreak );

	REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Anchor );

	REQUIRE( static_cast< MD::Anchor< TRAIT >* > ( doc->items().at( 3 ).get() )->label() ==
		wd + u8"/042-1.md" );

	REQUIRE( doc->items().at( 4 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 4 ).get() );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 10 );
	REQUIRE( p->endLine() == 0 );

	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->startColumn() == 0 );
	REQUIRE( t->startLine() == 0 );
	REQUIRE( t->endColumn() == 10 );
	REQUIRE( t->endLine() == 0 );

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
	REQUIRE( l->startColumn() == 0 );
	REQUIRE( l->startLine() == 0 );
	REQUIRE( l->endColumn() == 22 );
	REQUIRE( l->endLine() == 1 );

	REQUIRE( l->items().size() == 2 );

	typename TRAIT::String wd =
#ifdef MD4QT_QT_SUPPORT
		QDir().absolutePath()
#else
		std::filesystem::canonical( std::filesystem::current_path() ).u8string()
#endif
		+ u8"/tests/parser/data";

#ifndef MD4QT_QT_SUPPORT
	std::string tmp;
	wd.toUTF8String( tmp );
	std::replace( tmp.begin(), tmp.end(), '\\', '/' );
	wd = icu::UnicodeString::fromUTF8( tmp );
#endif

	for( int i = 0; i < 2; ++i )
	{
		REQUIRE( l->items().at( i )->type() == MD::ItemType::ListItem );

		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( i ).get() );
		REQUIRE( li->startColumn() == 0 );
		REQUIRE( li->startLine() == i );
		REQUIRE( li->endColumn() == 22 );
		REQUIRE( li->endLine() == i );

		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
		REQUIRE( p->startColumn() == 2 );
		REQUIRE( p->startLine() == i );
		REQUIRE( p->endColumn() == 22 );
		REQUIRE( p->endLine() == i );

		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );

		auto lnk = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( lnk->startColumn() == 2 );
		REQUIRE( lnk->startLine() == i );
		REQUIRE( lnk->endColumn() == 22 );
		REQUIRE( lnk->endLine() == i );

		REQUIRE( lnk->text() == u8"Chapter 1" );
		REQUIRE( lnk->url() == wd + u8"/042-1.md" );
	}
}

/*
* Item

    > Quote


*/
TEST_CASE( "044" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/044.md", false );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->startColumn() == 0 );
	REQUIRE( l->startLine() == 0 );
	REQUIRE( l->endColumn() == 10 );
	REQUIRE( l->endLine() == 2 );

	REQUIRE( l->items().size() == 1 );

	REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );

	auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
	REQUIRE( li->startColumn() == 0 );
	REQUIRE( li->startLine() == 0 );
	REQUIRE( li->endColumn() == 10 );
	REQUIRE( li->endLine() == 2 );

	REQUIRE( li->items().size() == 2 );
	REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
	REQUIRE( p->startColumn() == 2 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 5 );
	REQUIRE( p->endLine() == 0 );

	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->startColumn() == 2 );
	REQUIRE( t->startLine() == 0 );
	REQUIRE( t->endColumn() == 5 );
	REQUIRE( t->endLine() == 0 );

	REQUIRE( t->text() == u8"Item" );

	REQUIRE( li->items().at( 1 )->type() == MD::ItemType::Blockquote );

	auto bq = static_cast< MD::Blockquote< TRAIT >* > ( li->items().at( 1 ).get() );
	REQUIRE( bq->startColumn() == 2 );
	REQUIRE( bq->startLine() == 2 );
	REQUIRE( bq->endColumn() == 10 );
	REQUIRE( bq->endLine() == 2 );

	REQUIRE( bq->items().size() == 1 );

	REQUIRE( bq->items().at( 0 )->type() == MD::ItemType::Paragraph );

	p = static_cast< MD::Paragraph< TRAIT >* > ( bq->items().at( 0 ).get() );
	REQUIRE( p->startColumn() == 6 );
	REQUIRE( p->startLine() == 2 );
	REQUIRE( p->endColumn() == 10 );
	REQUIRE( p->endLine() == 2 );

	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

	t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->startColumn() == 6 );
	REQUIRE( t->startLine() == 2 );
	REQUIRE( t->endColumn() == 10 );
	REQUIRE( t->endLine() == 2 );

	REQUIRE( t->text() == u8"Quote" );
}

/*
  [^footnote]: Paragraph in footnote

    Paragraph in footnote

	Paragraph in footnote

*/
TEST_CASE( "045" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/045.md", false );

	REQUIRE( !doc->isEmpty() );
	REQUIRE( doc->items().size() == 1 );
	REQUIRE( doc->items().at( 0 )->type() == MD::ItemType::Anchor );

	REQUIRE( doc->footnotesMap().size() == 1 );

	typename TRAIT::String wd =
#ifdef MD4QT_QT_SUPPORT
		QDir().absolutePath()
#else
		std::filesystem::canonical( std::filesystem::current_path() ).u8string()
#endif
		+ u8"/tests/parser/data/";

#ifndef MD4QT_QT_SUPPORT
	std::string tmp;
	wd.toUTF8String( tmp );
	std::replace( tmp.begin(), tmp.end(), '\\', '/' );
	wd = icu::UnicodeString::fromUTF8( tmp );
#endif

	const typename TRAIT::String label = u8"#^footnote/" + wd + u8"045.md";

	const auto fit = doc->footnotesMap().find( label );

	REQUIRE( fit != doc->footnotesMap().cend() );

	auto f = fit->second;

	REQUIRE( f->items().size() == 3 );
	REQUIRE( f->startColumn() == 0 );
	REQUIRE( f->startLine() == 0 );
	REQUIRE( f->endColumn() == 21 );
	REQUIRE( f->endLine() == 4 );

	static const std::vector< long long int > start = { 14, 4, 1 };

	for( int i = 0; i < 3; ++i )
	{
		REQUIRE( f->items().at( i )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( f->items().at( i ).get() );
		REQUIRE( p->startColumn() == start.at( i ) );
		REQUIRE( p->startLine() == 2 * i );
		REQUIRE( p->endColumn() == start.at( i ) + 20 );
		REQUIRE( p->endLine() == 2 * i );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->startColumn() == start.at( i ) );
		REQUIRE( t->startLine() == 2 * i );
		REQUIRE( t->endColumn() == start.at( i ) + 20 );
		REQUIRE( t->endLine() == 2 * i );

		REQUIRE( t->text() == u8"Paragraph in footnote" );
	}
}

/*
Heading 1
=========
Paragraph 1

Heading 2
---------
Paragraph 2

  # Heading 1

Paragraph 1

## Heading 2

Paragraph 2

Heading 1
=========

Paragraph 1

Heading 2
---------

Paragraph 2

### Heading 3 {#heading-3}

*/
TEST_CASE( "046" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/046.md" );

	REQUIRE( !doc->isEmpty() );
	REQUIRE( doc->items().size() == 14 );

	static const std::vector< long long int > hcolumn = { 0, 0, 2, 0, 0, 0 };
	static const std::vector< long long int > hecolumn = { 8, 8, 12, 11, 8, 8 };
	static const std::vector< long long int > sline = { 0, 4, 8, 12, 16, 21 };
	static const std::vector< long long int > eline = { 1, 5, 8, 12, 17, 22 };
	static const std::vector< long long int > pcolumn = { 0, 0, 4, 3, 0, 0 };
	static const std::vector< long long int > pline = { 2, 6, 10, 14, 19, 24 };

	int didx = 1;

	for( int i = 1; i < 4; ++i )
	{
		for( int j = 1; j < 3; ++j )
		{
			REQUIRE( doc->items().at( didx )->type() == MD::ItemType::Heading );

			auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( didx ).get() );
			const size_t idx = ( ( i - 1 ) * 2 ) + j - 1;
			REQUIRE( h->startColumn() == hcolumn.at( idx ) );
			REQUIRE( h->startLine() == sline.at( idx ) );
			REQUIRE( h->endColumn() == hecolumn.at( idx ) );
			REQUIRE( h->endLine() == eline.at( idx ) );

			{
				REQUIRE( h->level() == j );
				REQUIRE( h->text().get() );
				auto p = h->text().get();
				REQUIRE( p->startColumn() == pcolumn.at( idx ) );
				REQUIRE( p->startLine() == sline.at( idx ) );
				REQUIRE( p->endColumn() == pcolumn.at( idx ) + 8 );
				REQUIRE( p->endLine() == p->startLine() );
				REQUIRE( p->items().size() == 1 );
				REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
				auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
				REQUIRE( t->text() == u8"Heading " + to_string( j ) );
				REQUIRE( t->startColumn() == pcolumn.at( idx ) );
				REQUIRE( t->startLine() == sline.at( idx ) );
				REQUIRE( t->endColumn() == pcolumn.at( idx ) + 8 );
				REQUIRE( t->endLine() == t->startLine() );
			}

			++didx;

			REQUIRE( doc->items().at( didx )->type() == MD::ItemType::Paragraph );

			auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( didx ).get() );
			REQUIRE( p->startColumn() == 0 );
			REQUIRE( p->startLine() == pline.at( idx ) );
			REQUIRE( p->endColumn() == 10 );
			REQUIRE( p->endLine() == p->startLine() );

			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			REQUIRE( static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() )->text() ==
				u8"Paragraph " + to_string( j ) );

			++didx;
		}
	}

	REQUIRE( doc->items().at( didx )->type() == MD::ItemType::Heading );

	auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( didx ).get() );
	REQUIRE( h->startColumn() == 0 );
	REQUIRE( h->startLine() == 26 );
	REQUIRE( h->endColumn() == 25 );
	REQUIRE( h->endLine() == 26 );

	REQUIRE( h->level() == 3 );
	REQUIRE( h->text().get() );
	auto p = h->text().get();
	REQUIRE( p->startColumn() == 4 );
	REQUIRE( p->startLine() == 26 );
	REQUIRE( p->endColumn() == 12 );
	REQUIRE( p->endLine() == 26 );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->startColumn() == 4 );
	REQUIRE( t->startLine() == 26 );
	REQUIRE( t->endColumn() == 12 );
	REQUIRE( t->endLine() == 26 );
	REQUIRE( t->text() == u8"Heading 3" );
	REQUIRE( h->isLabeled() );

	typename TRAIT::String wd =
#ifdef MD4QT_QT_SUPPORT
		QDir().absolutePath()
#else
		std::filesystem::canonical( std::filesystem::current_path() ).u8string()
#endif
		+ u8"/tests/parser/data/";

#ifndef MD4QT_QT_SUPPORT
	std::string tmp;
	wd.toUTF8String( tmp );
	std::replace( tmp.begin(), tmp.end(), '\\', '/' );
	wd = icu::UnicodeString::fromUTF8( tmp );
#endif

	const typename TRAIT::String label = u8"#heading-3/" + wd + u8"046.md";

	REQUIRE( h->label() == label );

	REQUIRE( doc->labeledHeadings().size() == 3 );
	const auto hit = doc->labeledHeadings().find( label );
	REQUIRE( hit != doc->labeledHeadings().cend() );
	REQUIRE( hit->second.get() == h );
}

/*

Column 1 | Column 2
---------|---------
Cell 1   | Cell 2

| Column 1 | Column 2 |
|:--------:|---------:|
| Cell 1   | Cell 2   |

*/
TEST_CASE( "047" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/047.md" );

	REQUIRE( !doc->isEmpty() );
	REQUIRE( doc->items().size() == 3 );

	static const std::vector< long long int > scolumn = { 0, 2 };
	static const std::vector< long long int > ecolumn1 = { 18, 22 };
	static const std::vector< long long int > ecolumn2 = { 16, 22 };

	for( int i = 1; i < 3; ++i )
	{
		REQUIRE( doc->items().at( i )->type() == MD::ItemType::Table );

		auto t = static_cast< MD::Table< TRAIT >* > ( doc->items().at( i ).get() );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == ( ( i - 1 ) * 4 ) + 1 );
		REQUIRE( t->endColumn() == ecolumn2.at( i - 1 ) );
		REQUIRE( t->endLine() == ( ( i - 1 ) * 4 ) + 3 );

		REQUIRE( t->columnsCount() == 2 );
		REQUIRE( t->rows().size() == 2 );

		auto r0 = t->rows().at( 0 );
		REQUIRE( r0->startColumn() == 0 );
		REQUIRE( r0->startLine() == ( ( i - 1 ) * 4 ) + 1 );
		REQUIRE( r0->endColumn() == ecolumn1.at( i - 1 ) ) ;
		REQUIRE( r0->endLine() == r0->startLine() );

		REQUIRE( r0->type() == MD::ItemType::TableRow );

		REQUIRE( r0->cells().size() == 2 );

		{
			REQUIRE( r0->cells().at( 0 )->type() == MD::ItemType::TableCell );
			auto c0 = static_cast< MD::TableCell< TRAIT >* > ( r0->cells().at( 0 ).get() );
			REQUIRE( c0->startColumn() == 0 );
			REQUIRE( c0->startLine() == r0->startLine() );
			REQUIRE( c0->endColumn() == c0->startColumn() + 9 + scolumn.at( i - 1 ) ) ;
			REQUIRE( c0->endLine() == c0->startLine() );

			REQUIRE( c0->items().size() == 1 );
			REQUIRE( c0->items().at( 0 )->type() == MD::ItemType::Text );

			auto t0 = static_cast< MD::Text< TRAIT >* > ( c0->items().at( 0 ).get() );
			REQUIRE( t0->startColumn() == scolumn.at( i - 1 ) );
			REQUIRE( t0->startLine() == c0->startLine() );
			REQUIRE( t0->endColumn() == t0->startColumn() + 7 );
			REQUIRE( t0->endLine() == t0->startLine() );

			REQUIRE( t0->text() == u8"Column 1" );
		}

		{
			auto c1 = static_cast< MD::TableCell< TRAIT >* > ( r0->cells().at( 1 ).get() );
			REQUIRE( c1->startColumn() == scolumn.at( i - 1 ) + 9 );
			REQUIRE( c1->startLine() == r0->startLine() );
			REQUIRE( c1->endColumn() == ecolumn1.at( i - 1 ) );
			REQUIRE( c1->endLine() == c1->startLine() );

			REQUIRE( c1->items().size() == 1 );
			REQUIRE( c1->items().at( 0 )->type() == MD::ItemType::Text );

			auto t1 = static_cast< MD::Text< TRAIT >* > ( c1->items().at( 0 ).get() );
			REQUIRE( t1->startColumn() == scolumn.at( i - 1 ) + 9 + 2 );
			REQUIRE( t1->startLine() == r0->startLine() );
			REQUIRE( t1->endColumn() == c1->startColumn() + 9 );
			REQUIRE( t1->endLine() == c1->startLine() );

			REQUIRE( t1->text() == u8"Column 2" );
		}

		auto r1 = t->rows().at( 1 );
		REQUIRE( r1->startColumn() == 0 );
		REQUIRE( r1->startLine() == 3 + ( ( i - 1 ) * 4 ) );
		REQUIRE( r1->endColumn() == ecolumn2.at( i - 1 ) );
		REQUIRE( r1->endLine() == r1->startLine() );

		REQUIRE( r1->cells().size() == 2 );

		{
			auto c0 = static_cast< MD::TableCell< TRAIT >* > ( r1->cells().at( 0 ).get() );
			REQUIRE( c0->startColumn() == 0 );
			REQUIRE( c0->startLine() == r1->startLine() );
			REQUIRE( c0->endColumn() == c0->startColumn() + 9 + scolumn.at( i - 1 ) );
			REQUIRE( c0->endLine() == c0->startLine() );

			REQUIRE( c0->items().size() == 1 );
			REQUIRE( c0->items().at( 0 )->type() == MD::ItemType::Text );

			auto t0 = static_cast< MD::Text< TRAIT >* > ( c0->items().at( 0 ).get() );
			REQUIRE( t0->startColumn() == scolumn.at( i -1 ) );
			REQUIRE( t0->startLine() == r1->startLine() );
			REQUIRE( t0->endColumn() == t0->startColumn() + 5 );
			REQUIRE( t0->endLine() == t0->startLine() );

			REQUIRE( t0->text() == u8"Cell 1" );
		}

		{
			auto c1 = static_cast< MD::TableCell< TRAIT >* > ( r1->cells().at( 1 ).get() );
			REQUIRE( c1->startColumn() == scolumn.at( i - 1 ) + 9 );
			REQUIRE( c1->startLine() == r1->startLine() );
			REQUIRE( c1->endColumn() == ecolumn2.at( i - 1 ) );
			REQUIRE( c1->endLine() == c1->startLine() );

			REQUIRE( c1->items().size() == 1 );
			REQUIRE( c1->items().at( 0 )->type() == MD::ItemType::Text );

			auto t1 = static_cast< MD::Text< TRAIT >* > ( c1->items().at( 0 ).get() );
			REQUIRE( t1->startColumn() == scolumn.at( i - 1 ) + 9 + 2 );
			REQUIRE( t1->startLine() == r1->startLine() );
			REQUIRE( t1->endColumn() == t1->startColumn() + 5 );
			REQUIRE( t1->endLine() == t1->startLine() );

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

/*
# Heading
Paragraph
# Heading

*/
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
		REQUIRE( h->startColumn() == 0 );
		REQUIRE( h->startLine() == 0 );
		REQUIRE( h->endColumn() == 8 );
		REQUIRE( h->endLine() == 0 );

		REQUIRE( h->text().get() );
		auto p = h->text().get();
		REQUIRE( p->startColumn() == 2 );
		REQUIRE( p->startLine() == 0 );
		REQUIRE( p->endColumn() == 8 );
		REQUIRE( p->endLine() == 0 );

		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->text() == u8"Heading" );
		REQUIRE( t->startColumn() == 2 );
		REQUIRE( t->startLine() == 0 );
		REQUIRE( t->endColumn() == 8 );
		REQUIRE( t->endLine() == 0 );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );
		auto * p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( p->startColumn() == 0 );
		REQUIRE( p->startLine() == 1 );
		REQUIRE( p->endColumn() == 8 );
		REQUIRE( p->endLine() == 1 );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		REQUIRE( static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() )->text() ==
			u8"Paragraph" );
	}

	REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Heading );
	auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 3 ).get() );
	REQUIRE( h->startColumn() == 0 );
	REQUIRE( h->startLine() == 2 );
	REQUIRE( h->endColumn() == 8 );
	REQUIRE( h->endLine() == 2 );

	REQUIRE( h->text().get() );
	auto p = h->text().get();
	REQUIRE( p->startColumn() == 2 );
	REQUIRE( p->startLine() == 2 );
	REQUIRE( p->endColumn() == 8 );
	REQUIRE( p->endLine() == 2 );

	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->startColumn() == 2 );
	REQUIRE( t->startLine() == 2 );
	REQUIRE( t->endColumn() == 8 );
	REQUIRE( t->endLine() == 2 );
	REQUIRE( t->text() == u8"Heading" );
}

/*
<!-- --> 1<!-- -->

*/
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
	REQUIRE( h->startColumn() == 0 );
	REQUIRE( h->startLine() == 0 );
	REQUIRE( h->endColumn() == 17 );
	REQUIRE( h->endLine() == 0 );
}

/*
[a\]](#1)
[b\\](#2)
[c\-d](#3)
[\\](#4)

*/
TEST_CASE( "050" )
{
	MD::Parser< TRAIT > parser;
	auto doc = parser.parse( "tests/parser/data/050.md" );
	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );
	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto * p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 4 );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 7 );
	REQUIRE( p->endLine() == 3 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
	auto l0 = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( l0->startColumn() == 0 );
	REQUIRE( l0->startLine() == 0 );
	REQUIRE( l0->endColumn() == 8 );
	REQUIRE( l0->endLine() == 0 );
	REQUIRE( l0->text() == u8"a]" );

	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Link );
	auto l1 = static_cast< MD::Link< TRAIT >* > ( p->items().at( 1 ).get() );
	REQUIRE( l1->startColumn() == 0 );
	REQUIRE( l1->startLine() == 1 );
	REQUIRE( l1->endColumn() == 8 );
	REQUIRE( l1->endLine() == 1 );
	REQUIRE( l1->text() == u8"b\\" );

	REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Link );
	auto l2 = static_cast< MD::Link< TRAIT >* > ( p->items().at( 2 ).get() );
	REQUIRE( l2->startColumn() == 0 );
	REQUIRE( l2->startLine() == 2 );
	REQUIRE( l2->endColumn() == 9 );
	REQUIRE( l2->endLine() == 2 );
	REQUIRE( l2->text() == u8"c-d" );

	REQUIRE( p->items().at( 3 )->type() == MD::ItemType::Link );
	auto l3 = static_cast< MD::Link< TRAIT >* > ( p->items().at( 3 ).get() );
	REQUIRE( l3->startColumn() == 0 );
	REQUIRE( l3->startLine() == 3 );
	REQUIRE( l3->endColumn() == 7 );
	REQUIRE( l3->endLine() == 3 );
	REQUIRE( l3->text() == u8"\\" );
}

/*
[link1](051-1.md)
[link2](051-2.md)

*/
TEST_CASE( "051" )
{
	MD::Parser< TRAIT > parser;
	auto doc = parser.parse( "tests/parser/data/051.md" );

	typename TRAIT::String wd =
#ifdef MD4QT_QT_SUPPORT
		QDir().absolutePath()
#else
		std::filesystem::canonical( std::filesystem::current_path() ).u8string()
#endif
		+ u8"/tests/parser/data";

#ifndef MD4QT_QT_SUPPORT
	std::string tmp;
	wd.toUTF8String( tmp );
	std::replace( tmp.begin(), tmp.end(), '\\', '/' );
	wd = icu::UnicodeString::fromUTF8( tmp );
#endif

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

/*
```code *bold _italic
_```` *bold* _italic
```

*/
TEST_CASE( "052" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/052.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );

		auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( c->startColumn() == 0 );
		REQUIRE( c->startLine() == 1 );
		REQUIRE( c->endColumn() == 19 );
		REQUIRE( c->endLine() == 1 );

		REQUIRE( c->isInlined() == false );
		REQUIRE( c->text() == u8"_```` *bold* _italic" );
		REQUIRE( c->syntax() == u8"code" );
	}
}

/*
```cpp
```

*/
TEST_CASE( "053" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/053.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( c->startColumn() == 6 );
		REQUIRE( c->startLine() == 0 );
		REQUIRE( c->endColumn() == 6 );
		REQUIRE( c->endLine() == 0 );
		REQUIRE( !c->isInlined() );
		REQUIRE( c->text().isEmpty() );
	}
}

/*
```java

*/
TEST_CASE( "054" )
{
	MD::Parser< TRAIT > parser;

	const auto doc = parser.parse( "tests/parser/data/054.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( c->syntax() == u8"java" );
		REQUIRE( !c->isInlined() );
		REQUIRE( c->text().isEmpty() );
		REQUIRE( c->startColumn() == 7 );
		REQUIRE( c->startLine() == 0 );
		REQUIRE( c->endColumn() == 7 );
		REQUIRE( c->endLine() == 0 );
	}
}

/*
  [^footnote]: Paragraph in footnote

    Paragraph in footnote

	Paragraph in footnote

Text

*/
TEST_CASE( "055" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/055.md", false );

	REQUIRE( doc->type() == MD::ItemType::Document );

	REQUIRE( !doc->isEmpty() );
	REQUIRE( doc->items().size() == 2 );
	REQUIRE( doc->items().at( 0 )->type() == MD::ItemType::Anchor );

	REQUIRE( doc->footnotesMap().size() == 1 );

	typename TRAIT::String wd =
#ifdef MD4QT_QT_SUPPORT
		QDir().absolutePath()
#else
		std::filesystem::canonical( std::filesystem::current_path() ).u8string()
#endif
		+ u8"/tests/parser/data/";

#ifndef MD4QT_QT_SUPPORT
	std::string tmp;
	wd.toUTF8String( tmp );
	std::replace( tmp.begin(), tmp.end(), '\\', '/' );
	wd = icu::UnicodeString::fromUTF8( tmp );
#endif

	const typename TRAIT::String label = u8"#^footnote/" + wd + u8"055.md";

	const auto fit = doc->footnotesMap().find( label );
	REQUIRE( fit != doc->footnotesMap().cend() );

	auto f = fit->second;

	REQUIRE( f->type() == MD::ItemType::Footnote );

	REQUIRE( f->items().size() == 3 );
	REQUIRE( f->startColumn() == 0 );
	REQUIRE( f->startLine() == 0 );
	REQUIRE( f->endColumn() == 21 );
	REQUIRE( f->endLine() == 4 );

	static const std::vector< long long int > start = { 14, 4, 1 };

	for( int i = 0; i < 3; ++i )
	{
		REQUIRE( f->items().at( i )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( f->items().at( i ).get() );
		REQUIRE( p->startColumn() == start.at( i ) );
		REQUIRE( p->startLine() == 2 * i );
		REQUIRE( p->endColumn() == start.at( i ) + 20 );
		REQUIRE( p->endLine() == 2 * i );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

		REQUIRE( t->startColumn() == start.at( i ) );
		REQUIRE( t->startLine() == 2 * i );
		REQUIRE( t->endColumn() == start.at( i ) + 20 );
		REQUIRE( t->endLine() == 2 * i );

		REQUIRE( t->text() == u8"Paragraph in footnote" );
	}

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 6 );
	REQUIRE( p->endColumn() == 3 );
	REQUIRE( p->endLine() == 6 );

	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->startColumn() == 0 );
	REQUIRE( t->startLine() == 6 );
	REQUIRE( t->endColumn() == 3 );
	REQUIRE( t->endLine() == 6 );

	REQUIRE( t->text() == u8"Text" );
}

/*
	if( a > b )

      do_something();

    else

      dont_do_anything();

*/
TEST_CASE( "056" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/056.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );

	auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( c->startColumn() == 4 );
	REQUIRE( c->startLine() == 0 );
	REQUIRE( c->endColumn() == 24 );
	REQUIRE( c->endLine() == 6 );

	REQUIRE( c->isInlined() == false );
	REQUIRE( c->text() ==
		u8"if( a > b )\n\n  do_something();\n\nelse\n\n  dont_do_anything();" );
}

/*
```
code

code
```

*/
TEST_CASE( "057" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/057.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );

	auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( c->startColumn() == 0 );
	REQUIRE( c->startLine() == 1 );
	REQUIRE( c->endColumn() == 3 );
	REQUIRE( c->endLine() == 3 );

	REQUIRE( c->isInlined() == false );
	REQUIRE( c->text() == u8"code\n\ncode" );
}

/*
 * List 1

   Paragraph

 *  List 2

    Paragraph

*/
TEST_CASE( "058" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/058.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->startColumn() == 1 );
	REQUIRE( l->startLine() == 0 );
	REQUIRE( l->endColumn() == 12 );
	REQUIRE( l->endLine() == 6 );

	REQUIRE( l->items().size() == 2 );

	REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
	REQUIRE( l->items().at( 1 )->type() == MD::ItemType::ListItem );

	auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 1 ).get() );
	REQUIRE( li->startColumn() == 1 );
	REQUIRE( li->startLine() == 4 );
	REQUIRE( li->endColumn() == 12 );
	REQUIRE( li->endLine() == 6 );

	REQUIRE( li->items().size() == 2 );
	REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
	REQUIRE( li->items().at( 1 )->type() == MD::ItemType::Paragraph );
}

/*
> ## This is a header.
>
> 1.   This is the first list item.
> 2.   This is the second list item.
>
> Here's some example code:
>
>     return shell_exec("echo $input | $markdown_script");

*/
TEST_CASE( "059" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/059.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );

	auto b = static_cast< MD::Blockquote< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( b->startColumn() == 0 );
	REQUIRE( b->startLine() == 0 );
	REQUIRE( b->endColumn() == 57 );
	REQUIRE( b->endLine() == 7 );

	REQUIRE( b->items().size() == 4 );

	REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Heading );
	{
		auto i = static_cast< MD::Heading< TRAIT >* > ( b->items().at( 0 ).get() );
		REQUIRE( i->startColumn() == 2 );
		REQUIRE( i->startLine() == 0 );
		REQUIRE( i->endColumn() == 21 );
		REQUIRE( i->endLine() == 0 );
	}
	REQUIRE( b->items().at( 1 )->type() == MD::ItemType::List );
	{
		auto i = static_cast< MD::List< TRAIT >* > ( b->items().at( 1 ).get() );
		REQUIRE( i->startColumn() == 2 );
		REQUIRE( i->startLine() == 2 );
		REQUIRE( i->endColumn() == 35 );
		REQUIRE( i->endLine() == 3 );
	}
	REQUIRE( b->items().at( 2 )->type() == MD::ItemType::Paragraph );
	{
		auto i = static_cast< MD::Paragraph< TRAIT >* > ( b->items().at( 2 ).get() );
		REQUIRE( i->startColumn() == 2 );
		REQUIRE( i->startLine() == 5 );
		REQUIRE( i->endColumn() == 26 );
		REQUIRE( i->endLine() == 5 );
	}
	REQUIRE( b->items().at( 3 )->type() == MD::ItemType::Code );
	{
		auto i = static_cast< MD::Code< TRAIT >* > ( b->items().at( 3 ).get() );
		REQUIRE( i->startColumn() == 6 );
		REQUIRE( i->startLine() == 7 );
		REQUIRE( i->endColumn() == 57 );
		REQUIRE( i->endLine() == 7 );
	}
}

/*
 * List
   * List

*/
TEST_CASE( "060" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/060.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->startColumn() == 1 );
	REQUIRE( l->startLine() == 0 );
	REQUIRE( l->endColumn() == 8 );
	REQUIRE( l->endLine() == 1 );

	REQUIRE( l->items().size() == 1 );

	REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );

	auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
	REQUIRE( li->startColumn() == 1 );
	REQUIRE( li->startLine() == 0 );
	REQUIRE( li->endColumn() == 8 );
	REQUIRE( li->endLine() == 1 );

	REQUIRE( li->items().size() == 2 );

	REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
	REQUIRE( li->items().at( 1 )->type() == MD::ItemType::List );
}

/*
*   A list item with a code block:

        <code goes here>

*/
TEST_CASE( "061" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/061.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->startColumn() == 0 );
	REQUIRE( l->startLine() == 0 );
	REQUIRE( l->endColumn() == 23 );
	REQUIRE( l->endLine() == 2 );

	REQUIRE( l->items().size() == 1 );

	REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );

	auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
	REQUIRE( li->startColumn() == 0 );
	REQUIRE( li->startLine() == 0 );
	REQUIRE( li->endColumn() == 23 );
	REQUIRE( li->endLine() == 2 );

	REQUIRE( li->items().size() == 2 );

	REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
	REQUIRE( p->startColumn() == 4 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 33 );
	REQUIRE( p->endLine() == 0 );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->startColumn() == 4 );
	REQUIRE( t->startLine() == 0 );
	REQUIRE( t->endColumn() == 33 );
	REQUIRE( t->endLine() == 0 );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"A list item with a code block:" );

	REQUIRE( li->items().at( 1 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code< TRAIT >* > ( li->items().at( 1 ).get() );
	REQUIRE( c->startColumn() == 8 );
	REQUIRE( c->startLine() == 2 );
	REQUIRE( c->endColumn() == 23 );
	REQUIRE( c->endLine() == 2 );
	REQUIRE( c->isInlined() == false );
	REQUIRE( c->syntax().isEmpty() );
	REQUIRE( c->text() == u8"<code goes here>" );
}

/*
**`**`*

**``**``*

**``**`**``*

**`*`**`*

*/
TEST_CASE( "062" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/062.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 5 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( p->startColumn() == 0 );
		REQUIRE( p->startLine() == 0 );
		REQUIRE( p->endColumn() == 6 );
		REQUIRE( p->endLine() == 0 );

		REQUIRE( p->items().size() == 2 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t1 = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t1->startColumn() == 0 );
		REQUIRE( t1->startLine() == 0 );
		REQUIRE( t1->endColumn() == 0 );
		REQUIRE( t1->endLine() == 0 );
		REQUIRE( t1->text() == u8"*" );

		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Code );
		auto c1 = static_cast< MD::Code< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( c1->startColumn() == 3 );
		REQUIRE( c1->startLine() == 0 );
		REQUIRE( c1->endColumn() == 4 );
		REQUIRE( c1->endLine() == 0 );
		REQUIRE( c1->text() == u8"**" );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( p->startColumn() == 0 );
		REQUIRE( p->startLine() == 2 );
		REQUIRE( p->endColumn() == 8 );
		REQUIRE( p->endLine() == 2 );

		REQUIRE( p->items().size() == 2 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t1 = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t1->startColumn() == 0 );
		REQUIRE( t1->startLine() == 2 );
		REQUIRE( t1->endColumn() == 0 );
		REQUIRE( t1->endLine() == 2 );
		REQUIRE( t1->text() == u8"*" );

		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Code );
		auto c1 = static_cast< MD::Code< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( c1->startColumn() == 4 );
		REQUIRE( c1->startLine() == 2 );
		REQUIRE( c1->endColumn() == 5 );
		REQUIRE( c1->endLine() == 2 );
		REQUIRE( c1->text() == u8"**" );
	}

	{
		REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 3 ).get() );
		REQUIRE( p->startColumn() == 0 );
		REQUIRE( p->startLine() == 4 );
		REQUIRE( p->endColumn() == 11 );
		REQUIRE( p->endLine() == 4 );

		REQUIRE( p->items().size() == 2 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t1 = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t1->startColumn() == 0 );
		REQUIRE( t1->startLine() == 4 );
		REQUIRE( t1->endColumn() == 0 );
		REQUIRE( t1->endLine() == 4 );
		REQUIRE( t1->text() == u8"*" );

		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Code );
		auto c1 = static_cast< MD::Code< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( c1->startColumn() == 4 );
		REQUIRE( c1->startLine() == 4 );
		REQUIRE( c1->endColumn() == 8 );
		REQUIRE( c1->endLine() == 4 );
		REQUIRE( c1->text() == u8"**`**" );
	}

	{
		REQUIRE( doc->items().at( 4 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 4 ).get() );
		REQUIRE( p->startColumn() == 0 );
		REQUIRE( p->startLine() == 6 );
		REQUIRE( p->endColumn() == 8 );
		REQUIRE( p->endLine() == 6 );

		REQUIRE( p->items().size() == 2 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Code );
		auto c1 = static_cast< MD::Code< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( c1->startColumn() == 3 );
		REQUIRE( c1->startLine() == 6 );
		REQUIRE( c1->endColumn() == 3 );
		REQUIRE( c1->endLine() == 6 );
		REQUIRE( c1->text() == u8"*" );

		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
		auto t1 = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( t1->startColumn() == 7 );
		REQUIRE( t1->startLine() == 6 );
		REQUIRE( t1->endColumn() == 8 );
		REQUIRE( t1->endLine() == 6 );
		REQUIRE( t1->text() == u8"`*" );
	}
}

/*
_**Lorem.**_ Aenean

*/
TEST_CASE( "063" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/063.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 18 );
	REQUIRE( p->endLine() == 0 );
	REQUIRE( p->items().size() == 2 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->startColumn() == 3 );
		REQUIRE( t->startLine() == 0 );
		REQUIRE( t->endColumn() == 8 );
		REQUIRE( t->endLine() == 0 );
		REQUIRE( t->opts() == ( MD::ItalicText | MD::BoldText ) );
		REQUIRE( t->text() == u8"Lorem." );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( t->isSpaceBefore() );
		REQUIRE( t->startColumn() == 12 );
		REQUIRE( t->startLine() == 0 );
		REQUIRE( t->endColumn() == 18 );
		REQUIRE( t->endLine() == 0 );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Aenean" );
	}
}

/*
\\`

*/
TEST_CASE( "064" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/064.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 2 );
	REQUIRE( p->endLine() == 0 );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->startColumn() == 0 );
	REQUIRE( t->startLine() == 0 );
	REQUIRE( t->endColumn() == 2 );
	REQUIRE( t->endLine() == 0 );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"\\`" );
}

/*
When $a \ne 0$, there are two solutions to $(ax^2 + bx + c = 0)$ and they are
$$ x = {-b \pm \sqrt{b^2-4ac} \over 2a} $$

*/
TEST_CASE( "065" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/065.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 41 );
	REQUIRE( p->endLine() == 1 );

	REQUIRE( p->items().size() == 6 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 0 );
		REQUIRE( t->endColumn() == 4 );
		REQUIRE( t->endLine() == 0 );
		REQUIRE( t->isSpaceAfter() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"When" );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Math );
		auto m = static_cast< MD::Math< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( m->startColumn() == 6 );
		REQUIRE( m->startLine() == 0 );
		REQUIRE( m->endColumn() == 12 );
		REQUIRE( m->endLine() == 0 );
		REQUIRE( m->expr() == u8"a \\ne 0" );
		REQUIRE( m->isInline() );
	}

	{
		REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 2 ).get() );
		REQUIRE( t->startColumn() == 14 );
		REQUIRE( t->startLine() == 0 );
		REQUIRE( t->endColumn() == 42 );
		REQUIRE( t->endLine() == 0 );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8", there are two solutions to" );
	}

	{
		REQUIRE( p->items().at( 3 )->type() == MD::ItemType::Math );
		auto m = static_cast< MD::Math< TRAIT >* > ( p->items().at( 3 ).get() );
		REQUIRE( m->startColumn() == 44 );
		REQUIRE( m->startLine() == 0 );
		REQUIRE( m->endColumn() == 62 );
		REQUIRE( m->endLine() == 0 );
		REQUIRE( m->expr() == u8"(ax^2 + bx + c = 0)" );
		REQUIRE( m->isInline() );
	}

	{
		REQUIRE( p->items().at( 4 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 4 ).get() );
		REQUIRE( t->startColumn() == 64 );
		REQUIRE( t->startLine() == 0 );
		REQUIRE( t->endColumn() == 76 );
		REQUIRE( t->endLine() == 0 );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"and they are" );
	}

	{
		REQUIRE( p->items().at( 5 )->type() == MD::ItemType::Math );
		auto m = static_cast< MD::Math< TRAIT >* > ( p->items().at( 5 ).get() );
		REQUIRE( m->startColumn() == 2 );
		REQUIRE( m->startLine() == 1 );
		REQUIRE( m->endColumn() == 39 );
		REQUIRE( m->endLine() == 1 );
		REQUIRE( m->expr() == u8" x = {-b \\pm \\sqrt{b^2-4ac} \\over 2a} " );
		REQUIRE( !m->isInline() );
	}
}

/*
```math
x = {-b \pm \sqrt{b^2-4ac} \over 2a}
```

*/
TEST_CASE( "066" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/066.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 2 );
	REQUIRE( p->endLine() == 2 );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Math );
	auto m = static_cast< MD::Math< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( m->startColumn() == 0 );
	REQUIRE( m->startLine() == 1 );
	REQUIRE( m->endColumn() == 35 );
	REQUIRE( m->endLine() == 1 );
	REQUIRE( m->expr() == u8"x = {-b \\pm \\sqrt{b^2-4ac} \\over 2a}" );
	REQUIRE( !m->isInline() );
}

/*
Paragraph

 * List
item

   ```
   Code
   ```

*/
TEST_CASE( "067" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/067.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( p->startColumn() == 0 );
		REQUIRE( p->startLine() == 0 );
		REQUIRE( p->endColumn() == 8 );
		REQUIRE( p->endLine() == 0 );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 0 );
		REQUIRE( t->endColumn() == 8 );
		REQUIRE( t->endLine() == 0 );

		REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( t->text() == u8"Paragraph" );
	}

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 2 ).get() );
	REQUIRE( l->startColumn() == 1 );
	REQUIRE( l->startLine() == 2 );
	REQUIRE( l->endColumn() == 5 );
	REQUIRE( l->endLine() == 7 );

	REQUIRE( l->items().size() == 1 );

	REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );

	auto item = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
	REQUIRE( item->startColumn() == 1 );
	REQUIRE( item->startLine() == 2 );
	REQUIRE( item->endColumn() == 5 );
	REQUIRE( item->endLine() == 7 );

	REQUIRE( item->listType() == MD::ListItem< TRAIT >::Unordered );

	REQUIRE( item->items().size() == 2 );

	{
		REQUIRE( item->items().at( 0 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( item->items().at( 0 ).get() );
		REQUIRE( p->startColumn() == 3 );
		REQUIRE( p->startLine() == 2 );
		REQUIRE( p->endColumn() == 3 );
		REQUIRE( p->endLine() == 3 );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->startColumn() == 3 );
		REQUIRE( t->startLine() == 2 );
		REQUIRE( t->endColumn() == 3 );
		REQUIRE( t->endLine() == 3 );

		REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( t->text() == u8"List item" );
	}

	{
		REQUIRE( item->items().at( 1 )->type() == MD::ItemType::Code );

		auto c = static_cast< MD::Code< TRAIT >* > ( item->items().at( 1 ).get() );
		REQUIRE( c->startColumn() == 3 );
		REQUIRE( c->startLine() == 6 );
		REQUIRE( c->endColumn() == 6 );
		REQUIRE( c->endLine() == 6 );

		REQUIRE( c->text() == u8"Code" );
	}
}

/*
<!-- Not
finished
HTML
comment

*/
TEST_CASE( "068" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/068.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 6 );
	REQUIRE( p->endLine() == 3 );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->startColumn() == 0 );
	REQUIRE( t->startLine() == 0 );
	REQUIRE( t->endColumn() == 6 );
	REQUIRE( t->endLine() == 3 );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"<!-- Not finished HTML comment" );
}

/*
<pre>

data

</pre>

*/
TEST_CASE( "069" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/069.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<pre>\n\ndata\n\n</pre>" );
	REQUIRE( h->startColumn() == 0 );
	REQUIRE( h->startLine() == 0 );
	REQUIRE( h->endColumn() == 5 );
	REQUIRE( h->endLine() == 4 );
}

/*
Text <pre>

data

</pre>

*/
TEST_CASE( "070" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/070.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 5 );
	REQUIRE( p->endLine() == 4 );
	REQUIRE( p->items().size() == 2 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"Text" );
	REQUIRE( t->startColumn() == 0 );
	REQUIRE( t->startLine() == 0 );
	REQUIRE( t->endColumn() == 4 );
	REQUIRE( t->endLine() == 0 );
	REQUIRE( t->isSpaceAfter() );

	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<pre>\n\ndata\n\n</pre>" );
	REQUIRE( h->startColumn() == 5 );
	REQUIRE( h->startLine() == 0 );
	REQUIRE( h->endColumn() == 5 );
	REQUIRE( h->endLine() == 4 );
}

/*
<pre>

data

</pre> Text

*/
TEST_CASE( "071" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/071.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<pre>\n\ndata\n\n</pre> Text" );
	REQUIRE( h->startColumn() == 0 );
	REQUIRE( h->startLine() == 0 );
	REQUIRE( h->endColumn() == 10 );
	REQUIRE( h->endLine() == 4 );
}

/*
<pre>

data

</pre>
Text

*/
TEST_CASE( "072" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/072.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<pre>\n\ndata\n\n</pre>" );
	REQUIRE( h->startColumn() == 0 );
	REQUIRE( h->startLine() == 0 );
	REQUIRE( h->endColumn() == 5 );
	REQUIRE( h->endLine() == 4 );

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).get() );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 5 );
	REQUIRE( p->endColumn() == 3 );
	REQUIRE( p->endLine() == 5 );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"Text" );
	REQUIRE( t->startColumn() == 0 );
	REQUIRE( t->startLine() == 5 );
	REQUIRE( t->endColumn() == 3 );
	REQUIRE( t->endLine() == 5 );
}

/*
<pre>abc</pre>
Text

*/
TEST_CASE( "073" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/073.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<pre>abc</pre>" );
	REQUIRE( h->startColumn() == 0 );
	REQUIRE( h->startLine() == 0 );
	REQUIRE( h->endColumn() == 13 );
	REQUIRE( h->endLine() == 0 );

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).get() );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 1 );
	REQUIRE( p->endColumn() == 3 );
	REQUIRE( p->endLine() == 1 );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->startColumn() == 0 );
	REQUIRE( t->startLine() == 1 );
	REQUIRE( t->endColumn() == 3 );
	REQUIRE( t->endLine() == 1 );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"Text" );
}

/*
Text
<pre>abc</pre>

*/
TEST_CASE( "074" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/074.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 3 );
	REQUIRE( p->endLine() == 0 );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"Text" );
	REQUIRE( t->startColumn() == 0 );
	REQUIRE( t->startLine() == 0 );
	REQUIRE( t->endColumn() == 3 );
	REQUIRE( t->endLine() == 0 );

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 2 ).get() );
	REQUIRE( h->text() == u8"<pre>abc</pre>" );
	REQUIRE( h->startColumn() == 0 );
	REQUIRE( h->startLine() == 1 );
	REQUIRE( h->endColumn() == 13 );
	REQUIRE( h->endLine() == 1 );
}

/*
Text <pre>
</pre>
Text

*/
TEST_CASE( "075" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/075.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 3 );
	REQUIRE( p->endLine() == 2 );
	REQUIRE( p->items().size() == 3 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 0 );
		REQUIRE( t->endColumn() == 4 );
		REQUIRE( t->endLine() == 0 );
		REQUIRE( t->isSpaceAfter() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Text" );
	}

	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 1 ).get() );
	REQUIRE( h->startColumn() == 5 );
	REQUIRE( h->startLine() == 0 );
	REQUIRE( h->endColumn() == 5 );
	REQUIRE( h->endLine() == 1 );
	REQUIRE( h->text() == u8"<pre>\n</pre>" );

	{
		REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 2 ).get() );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 2 );
		REQUIRE( t->endColumn() == 3 );
		REQUIRE( t->endLine() == 2 );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Text" );
	}
}

/*
Text
<pre>
</pre>
Text

*/
TEST_CASE( "076" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/076.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 4 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( p->startColumn() == 0 );
		REQUIRE( p->startLine() == 0 );
		REQUIRE( p->endColumn() == 3 );
		REQUIRE( p->endLine() == 0 );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Text" );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 0 );
		REQUIRE( t->endColumn() == 3 );
		REQUIRE( t->endLine() == 0 );
	}

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 2 ).get() );
	REQUIRE( h->text() == u8"<pre>\n</pre>" );
	REQUIRE( h->startColumn() == 0 );
	REQUIRE( h->startLine() == 1 );
	REQUIRE( h->endColumn() == 5 );
	REQUIRE( h->endLine() == 2 );

	{
		REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 3 ).get() );
		REQUIRE( p->startColumn() == 0 );
		REQUIRE( p->startLine() == 3 );
		REQUIRE( p->endColumn() == 3 );
		REQUIRE( p->endLine() == 3 );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Text" );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 3 );
		REQUIRE( t->endColumn() == 3 );
		REQUIRE( t->endLine() == 3 );
	}
}

/*
<!-- Comment -->

*/
TEST_CASE( "077" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/077.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<!-- Comment -->" );
	REQUIRE( h->startColumn() == 0 );
	REQUIRE( h->startLine() == 0 );
	REQUIRE( h->endColumn() == 15 );
	REQUIRE( h->endLine() == 0 );
}

/*
Text <!-- Comment -->

*/
TEST_CASE( "078" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/078.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 20 );
	REQUIRE( p->endLine() == 0 );
	REQUIRE( p->items().size() == 2 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"Text" );
	REQUIRE( t->startColumn() == 0 );
	REQUIRE( t->startLine() == 0 );
	REQUIRE( t->endColumn() == 4 );
	REQUIRE( t->endLine() == 0 );
	REQUIRE( t->isSpaceAfter() );

	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<!-- Comment -->" );
	REQUIRE( h->startColumn() == 5 );
	REQUIRE( h->startLine() == 0 );
	REQUIRE( h->endColumn() == 20 );
	REQUIRE( h->endLine() == 0 );
}

/*
<!-- Comment --> Text

*/
TEST_CASE( "079" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/079.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<!-- Comment --> Text" );
	REQUIRE( h->startColumn() == 0 );
	REQUIRE( h->startLine() == 0 );
	REQUIRE( h->endColumn() == 20 );
	REQUIRE( h->endLine() == 0 );
}

/*
<!-- Comment -->
Text

*/
TEST_CASE( "080" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/080.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<!-- Comment -->" );
	REQUIRE( h->startColumn() == 0 );
	REQUIRE( h->startLine() == 0 );
	REQUIRE( h->endColumn() == 15 );
	REQUIRE( h->endLine() == 0 );

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).get() );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 1 );
	REQUIRE( p->endColumn() == 3 );
	REQUIRE( p->endLine() == 1 );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"Text" );
	REQUIRE( t->startColumn() == 0 );
	REQUIRE( t->startLine() == 1 );
	REQUIRE( t->endColumn() == 3 );
	REQUIRE( t->endLine() == 1 );
}

/*
Text
<!-- Comment -->

*/
TEST_CASE( "081" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/081.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 3 );
	REQUIRE( p->endLine() == 0 );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"Text" );
	REQUIRE( t->startColumn() == 0 );
	REQUIRE( t->startLine() == 0 );
	REQUIRE( t->endColumn() == 3 );
	REQUIRE( t->endLine() == 0 );

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 2 ).get() );
	REQUIRE( h->text() == u8"<!-- Comment -->" );
	REQUIRE( h->startColumn() == 0 );
	REQUIRE( h->startLine() == 1 );
	REQUIRE( h->endColumn() == 15 );
	REQUIRE( h->endLine() == 1 );
}

/*
Text <!-- Comment -->
Text

*/
TEST_CASE( "082" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/082.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 3 );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 3 );
	REQUIRE( p->endLine() == 1 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Text" );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 0 );
		REQUIRE( t->endColumn() == 4 );
		REQUIRE( t->endLine() == 0 );
	}

	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<!-- Comment -->" );
	REQUIRE( h->startColumn() == 5 );
	REQUIRE( h->startLine() == 0 );
	REQUIRE( h->endColumn() == 20 );
	REQUIRE( h->endLine() == 0 );

	{
		REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 2 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Text" );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 1 );
		REQUIRE( t->endColumn() == 3 );
		REQUIRE( t->endLine() == 1 );
	}
}

/*
Text
<!-- Comment -->
Text

*/
TEST_CASE( "083" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/083.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 4 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( p->startColumn() == 0 );
		REQUIRE( p->startLine() == 0 );
		REQUIRE( p->endColumn() == 3 );
		REQUIRE( p->endLine() == 0 );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Text" );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 0 );
		REQUIRE( t->endColumn() == 3 );
		REQUIRE( t->endLine() == 0 );
	}

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 2 ).get() );
	REQUIRE( h->text() == u8"<!-- Comment -->" );
	REQUIRE( h->startColumn() == 0 );
	REQUIRE( h->startLine() == 1 );
	REQUIRE( h->endColumn() == 15 );
	REQUIRE( h->endLine() == 1 );

	{
		REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 3 ).get() );
		REQUIRE( p->startColumn() == 0 );
		REQUIRE( p->startLine() == 2 );
		REQUIRE( p->endColumn() == 3 );
		REQUIRE( p->endLine() == 2 );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Text" );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 2 );
		REQUIRE( t->endColumn() == 3 );
		REQUIRE( t->endLine() == 2 );
	}
}

/*
<?php

echo '>';

?>

*/
TEST_CASE( "084" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/084.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<?php\n\necho '>';\n\n?>" );
	REQUIRE( h->startColumn() == 0 );
	REQUIRE( h->startLine() == 0 );
	REQUIRE( h->endColumn() == 1 );
	REQUIRE( h->endLine() == 4 );
}

/*
Text <?php

echo '>';

?>

*/
TEST_CASE( "085" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/085.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 1 );
	REQUIRE( p->endLine() == 4 );
	REQUIRE( p->items().size() == 2 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"Text" );
	REQUIRE( t->startColumn() == 0 );
	REQUIRE( t->startLine() == 0 );
	REQUIRE( t->endColumn() == 4 );
	REQUIRE( t->endLine() == 0 );

	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<?php\n\necho '>';\n\n?>" );
	REQUIRE( h->startColumn() == 5 );
	REQUIRE( h->startLine() == 0 );
	REQUIRE( h->endColumn() == 1 );
	REQUIRE( h->endLine() == 4 );
}

/*
<?php

echo '>';

?> Text

*/
TEST_CASE( "086" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/086.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<?php\n\necho '>';\n\n?> Text" );
	REQUIRE( h->startColumn() == 0 );
	REQUIRE( h->startLine() == 0 );
	REQUIRE( h->endColumn() == 6 );
	REQUIRE( h->endLine() == 4 );
}

/*
<?php

echo '>';

?>
Text

*/
TEST_CASE( "087" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/087.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<?php\n\necho '>';\n\n?>" );
	REQUIRE( h->startColumn() == 0 );
	REQUIRE( h->startLine() == 0 );
	REQUIRE( h->endColumn() == 1 );
	REQUIRE( h->endLine() == 4 );

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).get() );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 5 );
	REQUIRE( p->endColumn() == 3 );
	REQUIRE( p->endLine() == 5 );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"Text" );
	REQUIRE( t->startColumn() == 0 );
	REQUIRE( t->startLine() == 5 );
	REQUIRE( t->endColumn() == 3 );
	REQUIRE( t->endLine() == 5 );
}

/*
Text
<?php

echo '>';

?>

*/
TEST_CASE( "088" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/088.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 3 );
	REQUIRE( p->endLine() == 0 );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"Text" );
	REQUIRE( t->startColumn() == 0 );
	REQUIRE( t->startLine() == 0 );
	REQUIRE( t->endColumn() == 3 );
	REQUIRE( t->endLine() == 0 );

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 2 ).get() );
	REQUIRE( h->text() == u8"<?php\n\necho '>';\n\n?>" );
	REQUIRE( h->startColumn() == 0 );
	REQUIRE( h->startLine() == 1 );
	REQUIRE( h->endColumn() == 1 );
	REQUIRE( h->endLine() == 5 );
}

/*
Text <?php

echo '>';

?>
Text

*/
TEST_CASE( "089" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/089.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 3 );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 3 );
	REQUIRE( p->endLine() == 5 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Text" );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 0 );
		REQUIRE( t->endColumn() == 4 );
		REQUIRE( t->endLine() == 0 );
	}

	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<?php\n\necho '>';\n\n?>" );
	REQUIRE( h->startColumn() == 5 );
	REQUIRE( h->startLine() == 0 );
	REQUIRE( h->endColumn() == 1 );
	REQUIRE( h->endLine() == 4 );

	{
		REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 2 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Text" );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 5 );
		REQUIRE( t->endColumn() == 3 );
		REQUIRE( t->endLine() == 5 );
	}
}

/*
Text
<?php

echo '>';

?>
Text

*/
TEST_CASE( "090" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/090.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 4 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( p->startColumn() == 0 );
		REQUIRE( p->startLine() == 0 );
		REQUIRE( p->endColumn() == 3 );
		REQUIRE( p->endLine() == 0 );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Text" );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 0 );
		REQUIRE( t->endColumn() == 3 );
		REQUIRE( t->endLine() == 0 );
	}

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 2 ).get() );
	REQUIRE( h->text() == u8"<?php\n\necho '>';\n\n?>" );
	REQUIRE( h->startColumn() == 0 );
	REQUIRE( h->startLine() == 1 );
	REQUIRE( h->endColumn() == 1 );
	REQUIRE( h->endLine() == 5 );

	{
		REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 3 ).get() );
		REQUIRE( p->startColumn() == 0 );
		REQUIRE( p->startLine() == 6 );
		REQUIRE( p->endColumn() == 3 );
		REQUIRE( p->endLine() == 6 );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Text" );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 6 );
		REQUIRE( t->endColumn() == 3 );
		REQUIRE( t->endLine() == 6 );
	}
}

/*
<!DOCTYPE html>

*/
TEST_CASE( "091" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/091.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<!DOCTYPE html>" );
	REQUIRE( h->startColumn() == 0 );
	REQUIRE( h->startLine() == 0 );
	REQUIRE( h->endColumn() == 14 );
	REQUIRE( h->endLine() == 0 );
}

/*
Text <!DOCTYPE html>

*/
TEST_CASE( "092" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/092.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 19 );
	REQUIRE( p->endLine() == 0 );
	REQUIRE( p->items().size() == 2 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"Text" );
	REQUIRE( t->startColumn() == 0 );
	REQUIRE( t->startLine() == 0 );
	REQUIRE( t->endColumn() == 4 );
	REQUIRE( t->endLine() == 0 );

	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<!DOCTYPE html>" );
	REQUIRE( h->startColumn() == 5 );
	REQUIRE( h->startLine() == 0 );
	REQUIRE( h->endColumn() == 19 );
	REQUIRE( h->endLine() == 0 );
}

/*
<!DOCTYPE html> Text

*/
TEST_CASE( "093" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/093.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<!DOCTYPE html> Text" );
	REQUIRE( h->startColumn() == 0 );
	REQUIRE( h->startLine() == 0 );
	REQUIRE( h->endColumn() == 19 );
	REQUIRE( h->endLine() == 0 );
}

/*
<!DOCTYPE html>
Text

*/
TEST_CASE( "094" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/094.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<!DOCTYPE html>" );
	REQUIRE( h->startColumn() == 0 );
	REQUIRE( h->startLine() == 0 );
	REQUIRE( h->endColumn() == 14 );
	REQUIRE( h->endLine() == 0 );

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).get() );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 1 );
	REQUIRE( p->endColumn() == 3 );
	REQUIRE( p->endLine() == 1 );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"Text" );
	REQUIRE( t->startColumn() == 0 );
	REQUIRE( t->startLine() == 1 );
	REQUIRE( t->endColumn() == 3 );
	REQUIRE( t->endLine() == 1 );
}

/*
Text
<!DOCTYPE html>

*/
TEST_CASE( "095" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/095.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 3 );
	REQUIRE( p->endLine() == 0 );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"Text" );
	REQUIRE( t->startColumn() == 0 );
	REQUIRE( t->startLine() == 0 );
	REQUIRE( t->endColumn() == 3 );
	REQUIRE( t->endLine() == 0 );

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 2 ).get() );
	REQUIRE( h->text() == u8"<!DOCTYPE html>" );
	REQUIRE( h->startColumn() == 0 );
	REQUIRE( h->startLine() == 1 );
	REQUIRE( h->endColumn() == 14 );
	REQUIRE( h->endLine() == 1 );
}

/*
Text <!DOCTYPE html>
Text

*/
TEST_CASE( "096" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/096.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 3 );
	REQUIRE( p->endLine() == 1 );
	REQUIRE( p->items().size() == 3 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Text" );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 0 );
		REQUIRE( t->endColumn() == 4 );
		REQUIRE( t->endLine() == 0 );
	}

	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<!DOCTYPE html>" );
	REQUIRE( h->startColumn() == 5 );
	REQUIRE( h->startLine() == 0 );
	REQUIRE( h->endColumn() == 19 );
	REQUIRE( h->endLine() == 0 );

	{
		REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 2 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Text" );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 1 );
		REQUIRE( t->endColumn() == 3 );
		REQUIRE( t->endLine() == 1 );
	}
}

/*
Text
<!DOCTYPE html>
Text

*/
TEST_CASE( "097" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/097.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 4 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( p->startColumn() == 0 );
		REQUIRE( p->startLine() == 0 );
		REQUIRE( p->endColumn() == 3 );
		REQUIRE( p->endLine() == 0 );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Text" );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 0 );
		REQUIRE( t->endColumn() == 3 );
		REQUIRE( t->endLine() == 0 );
	}

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 2 ).get() );
	REQUIRE( h->text() == u8"<!DOCTYPE html>" );
	REQUIRE( h->startColumn() == 0 );
	REQUIRE( h->startLine() == 1 );
	REQUIRE( h->endColumn() == 14 );
	REQUIRE( h->endLine() == 1 );

	{
		REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 3 ).get() );
		REQUIRE( p->startColumn() == 0 );
		REQUIRE( p->startLine() == 2 );
		REQUIRE( p->endColumn() == 3 );
		REQUIRE( p->endLine() == 2 );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Text" );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 2 );
		REQUIRE( t->endColumn() == 3 );
		REQUIRE( t->endLine() == 2 );
	}
}

/*
<![CDATA[
function matchwo(a,b)
{
  if (a < b && a < 0) then {
    return 1;

  } else {

    return 0;
  }
}
]]>

*/
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
	REQUIRE( h->startColumn() == 0 );
	REQUIRE( h->startLine() == 0 );
	REQUIRE( h->endColumn() == 2 );
	REQUIRE( h->endLine() == 11 );
}

/*
Text <![CDATA[
function matchwo(a,b)
{
  if (a < b && a < 0) then {
    return 1;

  } else {

    return 0;
  }
}
]]>

*/
TEST_CASE( "099" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/099.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 2 );
	REQUIRE( p->endLine() == 11 );
	REQUIRE( p->items().size() == 2 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"Text" );
	REQUIRE( t->startColumn() == 0 );
	REQUIRE( t->startLine() == 0 );
	REQUIRE( t->endColumn() == 4 );
	REQUIRE( t->endLine() == 0 );

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
	REQUIRE( h->startColumn() == 5 );
	REQUIRE( h->startLine() == 0 );
	REQUIRE( h->endColumn() == 2 );
	REQUIRE( h->endLine() == 11 );
}

/*
<![CDATA[
function matchwo(a,b)
{
  if (a < b && a < 0) then {
    return 1;

  } else {

    return 0;
  }
}
]]> Text

*/
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
	REQUIRE( h->startColumn() == 0 );
	REQUIRE( h->startLine() == 0 );
	REQUIRE( h->endColumn() == 7 );
	REQUIRE( h->endLine() == 11 );
}

/*
<![CDATA[
function matchwo(a,b)
{
  if (a < b && a < 0) then {
    return 1;

  } else {

    return 0;
  }
}
]]>
Text

*/
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
	REQUIRE( h->startColumn() == 0 );
	REQUIRE( h->startLine() == 0 );
	REQUIRE( h->endColumn() == 2 );
	REQUIRE( h->endLine() == 11 );

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).get() );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 12 );
	REQUIRE( p->endColumn() == 3 );
	REQUIRE( p->endLine() == 12 );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"Text" );
	REQUIRE( t->startColumn() == 0 );
	REQUIRE( t->startLine() == 12 );
	REQUIRE( t->endColumn() == 3 );
	REQUIRE( t->endLine() == 12 );
}

/*
Text
<![CDATA[
function matchwo(a,b)
{
  if (a < b && a < 0) then {
    return 1;

  } else {

    return 0;
  }
}
]]>

*/
TEST_CASE( "102" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/102.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 3 );
	REQUIRE( p->endLine() == 0 );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"Text" );
	REQUIRE( t->startColumn() == 0 );
	REQUIRE( t->startLine() == 0 );
	REQUIRE( t->endColumn() == 3 );
	REQUIRE( t->endLine() == 0 );

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
	REQUIRE( h->startColumn() == 0 );
	REQUIRE( h->startLine() == 1 );
	REQUIRE( h->endColumn() == 2 );
	REQUIRE( h->endLine() == 12 );
}

/*
Text <![CDATA[
function matchwo(a,b)
{
  if (a < b && a < 0) then {
    return 1;

  } else {

    return 0;
  }
}
]]>
Text

*/
TEST_CASE( "103" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/103.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 3 );
	REQUIRE( p->endLine() == 12 );
	REQUIRE( p->items().size() == 3 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Text" );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 0 );
		REQUIRE( t->endColumn() == 4 );
		REQUIRE( t->endLine() == 0 );
		REQUIRE( t->isSpaceAfter() );
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
	REQUIRE( h->startColumn() == 5 );
	REQUIRE( h->startLine() == 0 );
	REQUIRE( h->endColumn() == 2 );
	REQUIRE( h->endLine() == 11 );

	{
		REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 2 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Text" );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 12 );
		REQUIRE( t->endColumn() == 3 );
		REQUIRE( t->endLine() == 12 );
	}
}

/*
Text
<![CDATA[
function matchwo(a,b)
{
  if (a < b && a < 0) then {
    return 1;

  } else {

    return 0;
  }
}
]]>
Text

*/
TEST_CASE( "104" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/104.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 4 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( p->startColumn() == 0 );
		REQUIRE( p->startLine() == 0 );
		REQUIRE( p->endColumn() == 3 );
		REQUIRE( p->endLine() == 0 );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Text" );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 0 );
		REQUIRE( t->endColumn() == 3 );
		REQUIRE( t->endLine() == 0 );
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
	REQUIRE( h->startColumn() == 0 );
	REQUIRE( h->startLine() == 1 );
	REQUIRE( h->endColumn() == 2 );
	REQUIRE( h->endLine() == 12 );

	{
		REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 3 ).get() );
		REQUIRE( p->startColumn() == 0 );
		REQUIRE( p->startLine() == 13 );
		REQUIRE( p->endColumn() == 3 );
		REQUIRE( p->endLine() == 13 );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Text" );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 13 );
		REQUIRE( t->endColumn() == 3 );
		REQUIRE( t->endLine() == 13 );
	}
}

/*
<form>

*/
TEST_CASE( "105" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/105.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<form>" );
	REQUIRE( h->startColumn() == 0 );
	REQUIRE( h->startLine() == 0 );
	REQUIRE( h->endColumn() == 5 );
	REQUIRE( h->endLine() == 0 );
}

/*
Text <form>

*/
TEST_CASE( "106" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/106.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 10 );
	REQUIRE( p->endLine() == 0 );
	REQUIRE( p->items().size() == 2 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"Text" );
	REQUIRE( t->startColumn() == 0 );
	REQUIRE( t->startLine() == 0 );
	REQUIRE( t->endColumn() == 4 );
	REQUIRE( t->endLine() == 0 );
	REQUIRE( t->isSpaceAfter() );

	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<form>" );
	REQUIRE( h->startColumn() == 5 );
	REQUIRE( h->startLine() == 0 );
	REQUIRE( h->endColumn() == 10 );
	REQUIRE( h->endLine() == 0 );
}

/*
<form> Text

*/
TEST_CASE( "107" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/107.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<form> Text" );
	REQUIRE( h->startColumn() == 0 );
	REQUIRE( h->startLine() == 0 );
	REQUIRE( h->endColumn() == 10 );
	REQUIRE( h->endLine() == 0 );
}

/*
<form>
Text

*/
TEST_CASE( "108" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/108.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<form>\nText" );
	REQUIRE( h->startColumn() == 0 );
	REQUIRE( h->startLine() == 0 );
	REQUIRE( h->endColumn() == 3 );
	REQUIRE( h->endLine() == 1 );
}

/*
Text
<form>

*/
TEST_CASE( "109" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/109.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 3 );
	REQUIRE( p->endLine() == 0 );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"Text" );
	REQUIRE( t->startColumn() == 0 );
	REQUIRE( t->startLine() == 0 );
	REQUIRE( t->endColumn() == 3 );
	REQUIRE( t->endLine() == 0 );

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 2 ).get() );
	REQUIRE( h->text() == u8"<form>" );
	REQUIRE( h->startColumn() == 0 );
	REQUIRE( h->startLine() == 1 );
	REQUIRE( h->endColumn() == 5 );
	REQUIRE( h->endLine() == 1 );
}

/*
Text <form>
Text

*/
TEST_CASE( "110" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/110.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 3 );
	REQUIRE( p->endLine() == 1 );
	REQUIRE( p->items().size() == 2 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Text" );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 0 );
		REQUIRE( t->endColumn() == 4 );
		REQUIRE( t->endLine() == 0 );
	}

	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<form>\nText" );
	REQUIRE( h->startColumn() == 5 );
	REQUIRE( h->startLine() == 0 );
	REQUIRE( h->endColumn() == 3 );
	REQUIRE( h->endLine() == 1 );
}

/*
Text
<form>
Text

*/
TEST_CASE( "111" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/111.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( p->startColumn() == 0 );
		REQUIRE( p->startLine() == 0 );
		REQUIRE( p->endColumn() == 3 );
		REQUIRE( p->endLine() == 0 );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Text" );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 0 );
		REQUIRE( t->endColumn() == 3 );
		REQUIRE( t->endLine() == 0 );
	}

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 2 ).get() );
	REQUIRE( h->text() == u8"<form>\nText" );
	REQUIRE( h->startColumn() == 0 );
	REQUIRE( h->startLine() == 1 );
	REQUIRE( h->endColumn() == 3 );
	REQUIRE( h->endLine() == 2 );
}

/*
<a href="www.google.com">
Google
</a>

*/
TEST_CASE( "112" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/112.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<a href=\"www.google.com\">\nGoogle\n</a>" );
	REQUIRE( h->startColumn() == 0 );
	REQUIRE( h->startLine() == 0 );
	REQUIRE( h->endColumn() == 3 );
	REQUIRE( h->endLine() == 2 );
}

/*
Text <a href="www.google.com">Google</a>

*/
TEST_CASE( "113" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/113.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 39 );
	REQUIRE( p->endLine() == 0 );
	REQUIRE( p->items().size() == 4 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Text" );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 0 );
		REQUIRE( t->endColumn() == 4 );
		REQUIRE( t->endLine() == 0 );
		REQUIRE( t->isSpaceAfter() );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::RawHtml );
		auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( h->text() == u8"<a href=\"www.google.com\">" );
		REQUIRE( h->startColumn() == 5 );
		REQUIRE( h->startLine() == 0 );
		REQUIRE( h->endColumn() == 29 );
		REQUIRE( h->endLine() == 0 );
	}

	{
		REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 2 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Google" );
		REQUIRE( t->startColumn() == 30 );
		REQUIRE( t->startLine() == 0 );
		REQUIRE( t->endColumn() == 35 );
		REQUIRE( t->endLine() == 0 );
		REQUIRE( !t->isSpaceBefore() );
		REQUIRE( !t->isSpaceAfter() );
	}

	{
		REQUIRE( p->items().at( 3 )->type() == MD::ItemType::RawHtml );
		auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 3 ).get() );
		REQUIRE( h->text() == u8"</a>" );
		REQUIRE( h->startColumn() == 36 );
		REQUIRE( h->startLine() == 0 );
		REQUIRE( h->endColumn() == 39 );
		REQUIRE( h->endLine() == 0 );
	}
}

/*
<a href="www.google.com">Google</a> Text

*/
TEST_CASE( "114" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/114.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 39 );
	REQUIRE( p->endLine() == 0 );
	REQUIRE( p->items().size() == 4 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::RawHtml );
		auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( h->text() == u8"<a href=\"www.google.com\">" );
		REQUIRE( h->startColumn() == 0 );
		REQUIRE( h->startLine() == 0 );
		REQUIRE( h->endColumn() == 24 );
		REQUIRE( h->endLine() == 0 );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Google" );
		REQUIRE( t->startColumn() == 25 );
		REQUIRE( t->startLine() == 0 );
		REQUIRE( t->endColumn() == 30 );
		REQUIRE( t->endLine() == 0 );
	}

	{
		REQUIRE( p->items().at( 2 )->type() == MD::ItemType::RawHtml );
		auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 2 ).get() );
		REQUIRE( h->text() == u8"</a>" );
		REQUIRE( h->startColumn() == 31 );
		REQUIRE( h->startLine() == 0 );
		REQUIRE( h->endColumn() == 34 );
		REQUIRE( h->endLine() == 0 );
	}

	{
		REQUIRE( p->items().at( 3 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 3 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Text" );
		REQUIRE( t->startColumn() == 35 );
		REQUIRE( t->isSpaceBefore() );
		REQUIRE( t->startLine() == 0 );
		REQUIRE( t->endColumn() == 39 );
		REQUIRE( t->endLine() == 0 );
	}
}

/*
<a href="www.google.com">
Google
</a>
Text

*/
TEST_CASE( "115" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/115.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<a href=\"www.google.com\">\nGoogle\n</a>\nText" );
	REQUIRE( h->startColumn() == 0 );
	REQUIRE( h->startLine() == 0 );
	REQUIRE( h->endColumn() == 3 );
	REQUIRE( h->endLine() == 3 );
}

/*
Text
<a href="www.google.com">
Google
</a>

*/
TEST_CASE( "116" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/116.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 3 );
	REQUIRE( p->endLine() == 3 );
	REQUIRE( p->items().size() == 4 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Text" );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 0 );
		REQUIRE( t->endColumn() == 3 );
		REQUIRE( t->endLine() == 0 );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::RawHtml );
		auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( h->text() == u8"<a href=\"www.google.com\">" );
		REQUIRE( h->startColumn() == 0 );
		REQUIRE( h->startLine() == 1 );
		REQUIRE( h->endColumn() == 24 );
		REQUIRE( h->endLine() == 1 );
	}

	{
		REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 2 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Google" );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 2 );
		REQUIRE( t->endColumn() == 5 );
		REQUIRE( t->endLine() == 2 );
	}

	{
		REQUIRE( p->items().at( 3 )->type() == MD::ItemType::RawHtml );
		auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 3 ).get() );
		REQUIRE( h->text() == u8"</a>" );
		REQUIRE( h->startColumn() == 0 );
		REQUIRE( h->startLine() == 3 );
		REQUIRE( h->endColumn() == 3 );
		REQUIRE( h->endLine() == 3 );
	}
}

/*
Text <a href="www.google.com">Google</a>
Text

*/
TEST_CASE( "117" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/117.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 5 );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 3 );
	REQUIRE( p->endLine() == 1 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Text" );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 0 );
		REQUIRE( t->endColumn() == 4 );
		REQUIRE( t->endLine() == 0 );
		REQUIRE( t->isSpaceAfter() );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::RawHtml );
		auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( h->text() == u8"<a href=\"www.google.com\">" );
		REQUIRE( h->startColumn() == 5 );
		REQUIRE( h->startLine() == 0 );
		REQUIRE( h->endColumn() == 29 );
		REQUIRE( h->endLine() == 0 );
	}

	{
		REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 2 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Google" );
		REQUIRE( t->startColumn() == 30 );
		REQUIRE( t->startLine() == 0 );
		REQUIRE( t->endColumn() == 35 );
		REQUIRE( t->endLine() == 0 );
	}

	{
		REQUIRE( p->items().at( 3 )->type() == MD::ItemType::RawHtml );
		auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 3 ).get() );
		REQUIRE( h->text() == u8"</a>" );
		REQUIRE( h->startColumn() == 36 );
		REQUIRE( h->startLine() == 0 );
		REQUIRE( h->endColumn() == 39 );
		REQUIRE( h->endLine() == 0 );
	}

	{
		REQUIRE( p->items().at( 4 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 4 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Text" );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 1 );
		REQUIRE( t->endColumn() == 3 );
		REQUIRE( t->endLine() == 1 );
	}
}

/*
Text
<a href="www.google.com">
Google
</a>
Text

*/
TEST_CASE( "118" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/118.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 5 );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 3 );
	REQUIRE( p->endLine() == 4 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Text" );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 0 );
		REQUIRE( t->endColumn() == 3 );
		REQUIRE( t->endLine() == 0 );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::RawHtml );
		auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( h->text() == u8"<a href=\"www.google.com\">" );
		REQUIRE( h->startColumn() == 0 );
		REQUIRE( h->startLine() == 1 );
		REQUIRE( h->endColumn() == 24 );
		REQUIRE( h->endLine() == 1 );
	}

	{
		REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 2 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Google" );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 2 );
		REQUIRE( t->endColumn() == 5 );
		REQUIRE( t->endLine() == 2 );
	}

	{
		REQUIRE( p->items().at( 3 )->type() == MD::ItemType::RawHtml );
		auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 3 ).get() );
		REQUIRE( h->text() == u8"</a>" );
		REQUIRE( h->startColumn() == 0 );
		REQUIRE( h->startLine() == 3 );
		REQUIRE( h->endColumn() == 3 );
		REQUIRE( h->endLine() == 3 );
	}

	{
		REQUIRE( p->items().at( 4 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 4 ).get() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"Text" );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 4 );
		REQUIRE( t->endColumn() == 3 );
		REQUIRE( t->endLine() == 4 );
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
	std::ifstream stream( fileName, std::ios::in | std::ios::binary );

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
		REQUIRE( p->items().size() == 5 );
		REQUIRE( p->startColumn() == 0 );
		REQUIRE( p->startLine() == 0 );
		REQUIRE( p->endColumn() == 3 );
		REQUIRE( p->endLine() == 4 );

		{
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"Text" );
			REQUIRE( t->startColumn() == 0 );
			REQUIRE( t->startLine() == 0 );
			REQUIRE( t->endColumn() == 3 );
			REQUIRE( t->endLine() == 0 );
		}

		{
			REQUIRE( p->items().at( 1 )->type() == MD::ItemType::RawHtml );
			auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 1 ).get() );
			REQUIRE( h->text() == u8"<a href=\"www.google.com\">" );
			REQUIRE( h->startColumn() == 0 );
			REQUIRE( h->startLine() == 1 );
			REQUIRE( h->endColumn() == 24 );
			REQUIRE( h->endLine() == 1 );
		}

		{
			REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 2 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"Google" );
			REQUIRE( t->startColumn() == 0 );
			REQUIRE( t->startLine() == 2 );
			REQUIRE( t->endColumn() == 5 );
			REQUIRE( t->endLine() == 2 );
		}

		{
			REQUIRE( p->items().at( 3 )->type() == MD::ItemType::RawHtml );
			auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 3 ).get() );
			REQUIRE( h->text() == u8"</a>" );
			REQUIRE( h->startColumn() == 0 );
			REQUIRE( h->startLine() == 3 );
			REQUIRE( h->endColumn() == 3 );
			REQUIRE( h->endLine() == 3 );
		}

		{
			REQUIRE( p->items().at( 4 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 4 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"Text" );
			REQUIRE( t->startColumn() == 0 );
			REQUIRE( t->startLine() == 4 );
			REQUIRE( t->endColumn() == 3 );
			REQUIRE( t->endLine() == 4 );
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
