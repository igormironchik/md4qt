
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

// doctest include.
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

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

	REQUIRE( dp->items().size() == 3 );

	{
		REQUIRE( dp->items().at( 0 )->type() == MD::ItemType::Text );
		auto dt = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 0 ).get() );
		REQUIRE( dt->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( dt->text() == u8"Line 1..." );
		REQUIRE( dt->startColumn() == 0 );
		REQUIRE( dt->startLine() == 0 );
		REQUIRE( dt->endColumn() == 8 );
		REQUIRE( dt->endLine() == 0 );
	}

	{
		REQUIRE( dp->items().at( 1 )->type() == MD::ItemType::Text );
		auto dt = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 1 ).get() );
		REQUIRE( dt->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( dt->text() == u8"Line 2..." );
		REQUIRE( dt->startColumn() == 0 );
		REQUIRE( dt->startLine() == 1 );
		REQUIRE( dt->endColumn() == 8 );
		REQUIRE( dt->endLine() == 1 );
	}

	{
		REQUIRE( dp->items().at( 2 )->type() == MD::ItemType::Text );
		auto dt = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 2 ).get() );
		REQUIRE( dt->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( dt->text() == u8"Line 3..." );
		REQUIRE( dt->startColumn() == 0 );
		REQUIRE( dt->startLine() == 2 );
		REQUIRE( dt->endColumn() == 8 );
		REQUIRE( dt->endLine() == 2 );
	}
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

	REQUIRE( dp->items().size() == 4 );

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
		REQUIRE( dt->text() == u8"Line 2..." );
		REQUIRE( dt->startColumn() == 0 );
		REQUIRE( dt->startLine() == 1 );
		REQUIRE( dt->endColumn() == 8 );
		REQUIRE( dt->endLine() == 1 );
	}

	{
		REQUIRE( dp->items().at( 3 )->type() == MD::ItemType::Text );

		auto dt = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 3 ).get() );

		REQUIRE( dt->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( dt->text() == u8"Line 3..." );
		REQUIRE( dt->startColumn() == 0 );
		REQUIRE( dt->startLine() == 2 );
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

	REQUIRE( dp->items().size() == 3 );

	{
		REQUIRE( dp->items().at( 0 )->type() == MD::ItemType::Text );
		auto dt = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 0 ).get() );
		REQUIRE( dt->opts() == ( MD::TextOption::ItalicText | MD::TextOption::BoldText ) );
		REQUIRE( dt->text() == u8"Line 1..." );
		REQUIRE( dt->startColumn() == 3 );
		REQUIRE( dt->startLine() == 0 );
		REQUIRE( dt->endColumn() == 11 );
		REQUIRE( dt->endLine() == 0 );
	}

	{
		REQUIRE( dp->items().at( 1 )->type() == MD::ItemType::Text );
		auto dt = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 1 ).get() );
		REQUIRE( dt->opts() == ( MD::TextOption::ItalicText | MD::TextOption::BoldText ) );
		REQUIRE( dt->text() == u8"Line 2..." );
		REQUIRE( dt->startColumn() == 0 );
		REQUIRE( dt->startLine() == 1 );
		REQUIRE( dt->endColumn() == 8 );
		REQUIRE( dt->endLine() == 1 );
	}

	{
		REQUIRE( dp->items().at( 2 )->type() == MD::ItemType::Text );
		auto dt = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 2 ).get() );
		REQUIRE( dt->opts() == ( MD::TextOption::ItalicText | MD::TextOption::BoldText ) );
		REQUIRE( dt->text() == u8"Line 3..." );
		REQUIRE( dt->startColumn() == 0 );
		REQUIRE( dt->startLine() == 2 );
		REQUIRE( dt->endColumn() == 8 );
		REQUIRE( dt->endLine() == 2 );
	}
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

	REQUIRE( dp->items().size() == 3 );

	{
		REQUIRE( dp->items().at( 0 )->type() == MD::ItemType::Text );

		auto dt = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 0 ).get() );

		REQUIRE( dt->opts() == ( MD::TextOption::ItalicText | MD::TextOption::BoldText |
			MD::TextOption::StrikethroughText ) );
		REQUIRE( dt->text() == u8"Line 1..." );
		REQUIRE( dt->startColumn() == 5 );
		REQUIRE( dt->startLine() == 0 );
		REQUIRE( dt->endColumn() == 13 );
		REQUIRE( dt->endLine() == 0 );
	}

	{
		REQUIRE( dp->items().at( 1 )->type() == MD::ItemType::Text );

		auto dt = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 1 ).get() );

		REQUIRE( dt->opts() == ( MD::TextOption::ItalicText | MD::TextOption::BoldText |
			MD::TextOption::StrikethroughText ) );
		REQUIRE( dt->text() == u8"Line 2..." );
		REQUIRE( dt->startColumn() == 0 );
		REQUIRE( dt->startLine() == 1 );
		REQUIRE( dt->endColumn() == 8 );
		REQUIRE( dt->endLine() == 1 );
	}

	{
		REQUIRE( dp->items().at( 2 )->type() == MD::ItemType::Text );

		auto dt = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 2 ).get() );

		REQUIRE( dt->opts() == ( MD::TextOption::ItalicText | MD::TextOption::BoldText |
			MD::TextOption::StrikethroughText ) );
		REQUIRE( dt->text() == u8"Line 3..." );
		REQUIRE( dt->startColumn() == 0 );
		REQUIRE( dt->startLine() == 2 );
		REQUIRE( dt->endColumn() == 8 );
		REQUIRE( dt->endLine() == 2 );
	}
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

	REQUIRE( dp->items().size() == 3 );

	{
		REQUIRE( dp->items().at( 0 )->type() == MD::ItemType::Text );

		auto dt = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 0 ).get() );

		REQUIRE( dt->opts() == MD::TextOption::StrikethroughText );
		REQUIRE( dt->text() == u8"__*Line 1..." );
		REQUIRE( dt->startColumn() == 2 );
		REQUIRE( dt->startLine() == 0 );
		REQUIRE( dt->endColumn() == 13 );
		REQUIRE( dt->endLine() == 0 );
	}

	{
		REQUIRE( dp->items().at( 1 )->type() == MD::ItemType::Text );

		auto dt = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 1 ).get() );

		REQUIRE( dt->opts() == MD::TextOption::StrikethroughText );
		REQUIRE( dt->text() == u8"Line 2..." );
		REQUIRE( dt->startColumn() == 0 );
		REQUIRE( dt->startLine() == 1 );
		REQUIRE( dt->endColumn() == 8 );
		REQUIRE( dt->endLine() == 1 );
	}

	{
		REQUIRE( dp->items().at( 2 )->type() == MD::ItemType::Text );

		auto dt = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 2 ).get() );

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

	REQUIRE( dp->items().size() == 3 );

	{
		REQUIRE( dp->items().at( 0 )->type() == MD::ItemType::Text );

		auto dt = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 0 ).get() );

		REQUIRE( dt->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( dt->text() == u8"~~__*Line 1..." );
		REQUIRE( dt->startColumn() == 0 );
		REQUIRE( dt->startLine() == 0 );
		REQUIRE( dt->endColumn() == 18 );
		REQUIRE( dt->endLine() == 0 );
	}

	{
		REQUIRE( dp->items().at( 1 )->type() == MD::ItemType::Text );

		auto dt = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 1 ).get() );

		REQUIRE( dt->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( dt->text() == u8"Line 2...~~" );
		REQUIRE( dt->startColumn() == 0 );
		REQUIRE( dt->startLine() == 1 );
		REQUIRE( dt->endColumn() == 12 );
		REQUIRE( dt->endLine() == 1 );
	}

	{
		REQUIRE( dp->items().at( 2 )->type() == MD::ItemType::Text );

		auto dt = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 2 ).get() );

		REQUIRE( dt->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( dt->text() == u8"Line 3...*__" );
		REQUIRE( dt->startColumn() == 0 );
		REQUIRE( dt->startLine() == 2 );
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

		REQUIRE( dp->items().size() == 3 );

		{
			REQUIRE( dp->items().at( 0 )->type() == MD::ItemType::Text );

			auto dt = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 0 ).get() );

			REQUIRE( dt->opts() == MD::TextOption::TextWithoutFormat );
			REQUIRE( dt->text() == u8"Line 1..." );
			REQUIRE( dt->startColumn() == 0 );
			REQUIRE( dt->startLine() == 0 );
			REQUIRE( dt->endColumn() == 8 );
			REQUIRE( dt->endLine() == 0 );
		}

		{
			REQUIRE( dp->items().at( 1 )->type() == MD::ItemType::Text );

			auto dt = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 1 ).get() );

			REQUIRE( dt->opts() == MD::TextOption::TextWithoutFormat );
			REQUIRE( dt->text() == u8"Line 2..." );
			REQUIRE( dt->startColumn() == 0 );
			REQUIRE( dt->startLine() == 1 );
			REQUIRE( dt->endColumn() == 8 );
			REQUIRE( dt->endLine() == 1 );
		}

		{
			REQUIRE( dp->items().at( 2 )->type() == MD::ItemType::Text );

			auto dt = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 2 ).get() );

			REQUIRE( dt->opts() == MD::TextOption::TextWithoutFormat );
			REQUIRE( dt->text() == u8"Line 3..." );
			REQUIRE( dt->startColumn() == 0 );
			REQUIRE( dt->startLine() == 2 );
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

		REQUIRE( dp->items().size() == 3 );

		{
			REQUIRE( dp->items().at( 0 )->type() == MD::ItemType::Text );

			auto dt = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 0 ).get() );

			REQUIRE( dt->opts() == MD::TextOption::TextWithoutFormat );
			REQUIRE( dt->text() == u8"Line 1..." );
			REQUIRE( dt->startColumn() == 0 );
			REQUIRE( dt->startLine() == 0 );
			REQUIRE( dt->endColumn() == 8 );
			REQUIRE( dt->endLine() == 0 );
		}

		{
			REQUIRE( dp->items().at( 1 )->type() == MD::ItemType::Text );

			auto dt = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 1 ).get() );

			REQUIRE( dt->opts() == MD::TextOption::TextWithoutFormat );
			REQUIRE( dt->text() == u8"Line 2..." );
			REQUIRE( dt->startColumn() == 0 );
			REQUIRE( dt->startLine() == 1 );
			REQUIRE( dt->endColumn() == 8 );
			REQUIRE( dt->endLine() == 1 );
		}

		{
			REQUIRE( dp->items().at( 2 )->type() == MD::ItemType::Text );

			auto dt = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 2 ).get() );

			REQUIRE( dt->opts() == MD::TextOption::TextWithoutFormat );
			REQUIRE( dt->text() == u8"Line 3..." );
			REQUIRE( dt->startColumn() == 0 );
			REQUIRE( dt->startLine() == 2 );
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
