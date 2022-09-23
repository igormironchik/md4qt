
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

#include <md_parser.hpp>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
// doctest include.
#include <doctest/doctest.h>

#include <QFile>
#include <QDir>


TEST_CASE( "001" )
{
	MD::Parser p;
	auto doc = p.parse( QStringLiteral( "tests/parser/data/001.md" ) );
	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 1 );
	REQUIRE( doc->items().at( 0 )->type() == MD::ItemType::Anchor );
}

TEST_CASE( "002" )
{
	MD::Parser p;
	auto doc = p.parse( QStringLiteral( "tests/parser/data/002.md" ) );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );
	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto dp = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );

	REQUIRE( dp->items().size() == 1 );
	REQUIRE( dp->items().first()->type() == MD::ItemType::Text );

	auto dt = static_cast< MD::Text* > ( dp->items().first().data() );

	REQUIRE( dt->opts() == MD::TextOption::TextWithoutFormat );
	REQUIRE( dt->text() == QStringLiteral( "This is just a text!" ) );
}

TEST_CASE( "003" )
{
	MD::Parser parser;

	auto doc = parser.parse( QStringLiteral( "tests/parser/data/003.md" ) );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

		auto dp = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );

		REQUIRE( dp->items().size() == 1 );
		REQUIRE( dp->items().first()->type() == MD::ItemType::Text );

		auto dt = static_cast< MD::Text* > ( dp->items().first().data() );

		REQUIRE( dt->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( dt->text() == QStringLiteral( "Paragraph 1." ) );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );

		auto dp = static_cast< MD::Paragraph* > ( doc->items().at( 2 ).data() );

		REQUIRE( dp->items().size() == 1 );
		REQUIRE( dp->items().first()->type() == MD::ItemType::Text );

		auto dt = static_cast< MD::Text* > ( dp->items().first().data() );

		REQUIRE( dt->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( dt->text() == QStringLiteral( "Paragraph 2." ) );
	}
}

TEST_CASE( "004" )
{
	MD::Parser parser;

	auto doc = parser.parse( QStringLiteral( "tests/parser/data/004.md" ) );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto dp = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );

	REQUIRE( dp->items().size() == 1 );

	REQUIRE( dp->items().at( 0 )->type() == MD::ItemType::Text );

	auto dt = static_cast< MD::Text* > ( dp->items().at( 0 ).data() );

	REQUIRE( dt->opts() == MD::TextOption::TextWithoutFormat );
	REQUIRE( dt->text() == QStringLiteral( "Line 1... Line 2... Line 3..." ) );
}

TEST_CASE( "005" )
{
	MD::Parser parser;

	auto doc = parser.parse( QStringLiteral( "tests/parser/data/005.md" ) );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto dp = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );

	REQUIRE( dp->items().size() == 3 );

	{
		REQUIRE( dp->items().at( 0 )->type() == MD::ItemType::Text );

		auto dt = static_cast< MD::Text* > ( dp->items().at( 0 ).data() );

		REQUIRE( dt->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( dt->text() == QStringLiteral( "Line 1..." ) );
	}

	REQUIRE( dp->items().at( 1 )->type() == MD::ItemType::LineBreak );

	{
		REQUIRE( dp->items().at( 2 )->type() == MD::ItemType::Text );

		auto dt = static_cast< MD::Text* > ( dp->items().at( 2 ).data() );

		REQUIRE( dt->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( dt->text() == QStringLiteral( "Line 2... Line 3..." ) );
	}
}

TEST_CASE( "006" )
{
	MD::Parser parser;

	auto doc = parser.parse( QStringLiteral( "tests/parser/data/006.md" ) );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto dp = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );

	REQUIRE( dp->items().size() == 3 );

	{
		REQUIRE( dp->items().at( 0 )->type() == MD::ItemType::Text );

		auto dt = static_cast< MD::Text* > ( dp->items().at( 0 ).data() );

		REQUIRE( dt->opts() == MD::TextOption::ItalicText );
		REQUIRE( dt->text() == QStringLiteral( "Line 1..." ) );
	}

	{
		REQUIRE( dp->items().at( 1 )->type() == MD::ItemType::Text );

		auto dt = static_cast< MD::Text* > ( dp->items().at( 1 ).data() );

		REQUIRE( dt->opts() == MD::TextOption::BoldText );
		REQUIRE( dt->text() == QStringLiteral( "Line 2..." ) );
	}

	{
		REQUIRE( dp->items().at( 2 )->type() == MD::ItemType::Text );

		auto dt = static_cast< MD::Text* > ( dp->items().at( 2 ).data() );

		REQUIRE( dt->opts() == MD::TextOption::StrikethroughText );
		REQUIRE( dt->text() == QStringLiteral( "Line 3..." ) );
	}
}

TEST_CASE( "007" )
{
	MD::Parser parser;

	auto doc = parser.parse( QStringLiteral( "tests/parser/data/007.md" ) );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto dp = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );

	REQUIRE( dp->items().size() == 1 );

	REQUIRE( dp->items().at( 0 )->type() == MD::ItemType::Text );

	auto dt = static_cast< MD::Text* > ( dp->items().at( 0 ).data() );

	REQUIRE( dt->opts() == ( MD::TextOption::ItalicText | MD::TextOption::BoldText ) );
	REQUIRE( dt->text() == QStringLiteral( "Line 1... Line 2... Line 3..." ) );
}

TEST_CASE( "008" )
{
	MD::Parser parser;

	auto doc = parser.parse( QStringLiteral( "tests/parser/data/008.md" ) );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto dp = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );

	REQUIRE( dp->items().size() == 1 );

	REQUIRE( dp->items().at( 0 )->type() == MD::ItemType::Text );

	auto dt = static_cast< MD::Text* > ( dp->items().at( 0 ).data() );

	REQUIRE( dt->opts() == ( MD::TextOption::ItalicText | MD::TextOption::BoldText |
		MD::TextOption::StrikethroughText ) );
	REQUIRE( dt->text() == QStringLiteral( "Line 1... Line 2... Line 3..." ) );
}

TEST_CASE( "009" )
{
	MD::Parser parser;

	auto doc = parser.parse( QStringLiteral( "tests/parser/data/009.md" ) );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto dp = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );

	REQUIRE( dp->items().size() == 2 );

	{
		REQUIRE( dp->items().at( 0 )->type() == MD::ItemType::Text );

		auto dt = static_cast< MD::Text* > ( dp->items().at( 0 ).data() );

		REQUIRE( dt->opts() == MD::TextOption::StrikethroughText );
		REQUIRE( dt->text() == QStringLiteral( "__*Line 1... Line 2..." ) );
	}

	{
		REQUIRE( dp->items().at( 1 )->type() == MD::ItemType::Text );

		auto dt = static_cast< MD::Text* > ( dp->items().at( 1 ).data() );

		REQUIRE( dt->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( dt->text() == QStringLiteral( "Line 3...*__" ) );
	}
}

TEST_CASE( "010" )
{
	MD::Parser parser;

	auto doc = parser.parse( QStringLiteral( "tests/parser/data/010.md" ) );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto dp = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );

	REQUIRE( dp->items().size() == 1 );

	{
		REQUIRE( dp->items().at( 0 )->type() == MD::ItemType::Text );

		auto dt = static_cast< MD::Text* > ( dp->items().at( 0 ).data() );

		REQUIRE( dt->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( dt->text() == QStringLiteral( "~~__*Line 1... Line 2...~~ Line 3...*__" ) );
	}
}

TEST_CASE( "011" )
{
	MD::Parser parser;

	auto doc = parser.parse( QStringLiteral( "tests/parser/data/011.md" ) );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto dp = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );

	REQUIRE( dp->items().size() == 1 );

	REQUIRE( dp->items().at( 0 )->type() == MD::ItemType::Code );

	auto c = static_cast< MD::Code* > ( dp->items().at( 0 ).data() );

	REQUIRE( c->isInlined() == true );
	REQUIRE( c->text() == QStringLiteral( "code" ) );
}

TEST_CASE( "012" )
{
	MD::Parser parser;

	auto doc = parser.parse( QStringLiteral( "tests/parser/data/012.md" ) );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto dp = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );

	REQUIRE( dp->items().size() == 3 );

	REQUIRE( dp->items().at( 0 )->type() == MD::ItemType::Text );

	auto t1 = static_cast< MD::Text* > ( dp->items().at( 0 ).data() );

	REQUIRE( t1->text() == QStringLiteral( "Code in the" ) );

	REQUIRE( dp->items().at( 1 )->type() == MD::ItemType::Code );

	auto c = static_cast< MD::Code* > ( dp->items().at( 1 ).data() );

	REQUIRE( c->isInlined() == true );
	REQUIRE( c->text() == QStringLiteral( "text" ) );

	REQUIRE( dp->items().at( 2 )->type() == MD::ItemType::Text );

	auto t2 = static_cast< MD::Text* > ( dp->items().at( 2 ).data() );

	REQUIRE( t2->text() == QStringLiteral( "." ) );
}

TEST_CASE( "013" )
{
	MD::Parser parser;

	auto doc = parser.parse( QStringLiteral( "tests/parser/data/013.md" ) );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto dp = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );

	REQUIRE( dp->items().size() == 1 );

	REQUIRE( dp->items().at( 0 )->type() == MD::ItemType::Code );

	auto c = static_cast< MD::Code* > ( dp->items().at( 0 ).data() );

	REQUIRE( c->isInlined() == true );
	REQUIRE( c->text() == QStringLiteral( "Use this `code` in the code" ) );
}

TEST_CASE( "014" )
{
	MD::Parser parser;

	QFile f( "tests/parser/data/014.md" );

	if( f.open( QIODevice::WriteOnly | QIODevice::Truncate ) )
	{
		f.write( "Line 1...\rLine 2...\r\nLine 3...\n" );
		f.close();

		auto doc = parser.parse( QStringLiteral( "tests/parser/data/014.md" ) );

		REQUIRE( doc->isEmpty() == false );
		REQUIRE( doc->items().size() == 2 );

		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

		auto dp = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );

		REQUIRE( dp->items().size() == 1 );

		{
			REQUIRE( dp->items().at( 0 )->type() == MD::ItemType::Text );

			auto dt = static_cast< MD::Text* > ( dp->items().at( 0 ).data() );

			REQUIRE( dt->opts() == MD::TextOption::TextWithoutFormat );
			REQUIRE( dt->text() == QStringLiteral( "Line 1... Line 2... Line 3..." ) );
		}
	}
	else
		REQUIRE( true == false );
}

TEST_CASE( "015" )
{
	MD::Parser parser;

	QFile f( "tests/parser/data/015.md" );

	if( f.open( QIODevice::WriteOnly | QIODevice::Truncate ) )
	{
		f.write( "Line 1...\r\rLine 2...\r\rLine 3...\r" );
		f.close();

		auto doc = parser.parse( QStringLiteral( "tests/parser/data/015.md" ) );

		REQUIRE( doc->isEmpty() == false );
		REQUIRE( doc->items().size() == 4 );

		{
			REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

			auto dp = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );

			REQUIRE( dp->items().size() == 1 );

			REQUIRE( dp->items().at( 0 )->type() == MD::ItemType::Text );

			auto t = static_cast< MD::Text* > ( dp->items().at( 0 ).data() );

			REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "Line 1..." ) );
		}

		{
			REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );

			auto dp = static_cast< MD::Paragraph* > ( doc->items().at( 2 ).data() );

			REQUIRE( dp->items().size() == 1 );

			REQUIRE( dp->items().at( 0 )->type() == MD::ItemType::Text );

			auto t = static_cast< MD::Text* > ( dp->items().at( 0 ).data() );

			REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "Line 2..." ) );
		}

		{
			REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Paragraph );

			auto dp = static_cast< MD::Paragraph* > ( doc->items().at( 3 ).data() );

			REQUIRE( dp->items().size() == 1 );

			REQUIRE( dp->items().at( 0 )->type() == MD::ItemType::Text );

			auto t = static_cast< MD::Text* > ( dp->items().at( 0 ).data() );

			REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "Line 3..." ) );
		}
	}
	else
		REQUIRE( true == false );
}

TEST_CASE( "016" )
{
	MD::Parser parser;

	QFile f( "tests/parser/data/016.md" );

	if( f.open( QIODevice::WriteOnly | QIODevice::Truncate ) )
	{
		f.write( "Line 1...\r\nLine 2...\r\nLine 3...\r\n" );
		f.close();

		auto doc = parser.parse( QStringLiteral( "tests/parser/data/016.md" ) );

		REQUIRE( doc->isEmpty() == false );
		REQUIRE( doc->items().size() == 2 );

		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

		auto dp = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );

		REQUIRE( dp->items().size() == 1 );

		{
			REQUIRE( dp->items().at( 0 )->type() == MD::ItemType::Text );

			auto dt = static_cast< MD::Text* > ( dp->items().at( 0 ).data() );

			REQUIRE( dt->opts() == MD::TextOption::TextWithoutFormat );
			REQUIRE( dt->text() == QStringLiteral( "Line 1... Line 2... Line 3..." ) );
		}
	}
	else
		REQUIRE( true == false );
}

TEST_CASE( "017" )
{
	MD::Parser parser;

	auto doc = parser.parse( QStringLiteral( "tests/parser/data/017.md" ) );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );

	auto bq = static_cast< MD::Blockquote* > ( doc->items().at( 1 ).data() );

	REQUIRE( !bq->isEmpty() );
	REQUIRE( bq->items().size() == 3 );

	{
		REQUIRE( bq->items().at( 0 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph* > ( bq->items().at( 0 ).data() );

		REQUIRE( !p->isEmpty() );
		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );

		REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "Quote paragraph 1." ) );
	}

	{
		REQUIRE( bq->items().at( 1 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph* > ( bq->items().at( 1 ).data() );

		REQUIRE( !p->isEmpty() );
		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );

		REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "Quote paragraph 2." ) );
	}

	REQUIRE( bq->items().at( 2 )->type() == MD::ItemType::Blockquote );

	auto nbq = static_cast< MD::Blockquote* > ( bq->items().at( 2 ).data() );

	REQUIRE( !nbq->isEmpty() );
	REQUIRE( nbq->items().size() == 1 );

	REQUIRE( nbq->items().at( 0 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph* > ( nbq->items().at( 0 ).data() );

	REQUIRE( !p->isEmpty() );
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

	auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );

	REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
	REQUIRE( t->text() == QStringLiteral( "Nested quote" ) );
}

TEST_CASE( "018" )
{
	MD::Parser parser;

	auto doc = parser.parse( QStringLiteral( "tests/parser/data/018.md" ) );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );

	auto bq = static_cast< MD::Blockquote* > ( doc->items().at( 1 ).data() );

	REQUIRE( !bq->isEmpty() );
	REQUIRE( bq->items().size() == 3 );

	{
		REQUIRE( bq->items().at( 0 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph* > ( bq->items().at( 0 ).data() );

		REQUIRE( !p->isEmpty() );
		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );

		REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "Quote paragraph 1." ) );
	}

	{
		REQUIRE( bq->items().at( 1 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph* > ( bq->items().at( 1 ).data() );

		REQUIRE( !p->isEmpty() );
		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );

		REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "Quote paragraph 2." ) );
	}

	REQUIRE( bq->items().at( 2 )->type() == MD::ItemType::Blockquote );

	auto nbq = static_cast< MD::Blockquote* > ( bq->items().at( 2 ).data() );

	REQUIRE( !nbq->isEmpty() );
	REQUIRE( nbq->items().size() == 1 );

	REQUIRE( nbq->items().at( 0 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph* > ( nbq->items().at( 0 ).data() );

	REQUIRE( !p->isEmpty() );
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

	auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );

	REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
	REQUIRE( t->text() == QStringLiteral( "Nested quote" ) );
}

TEST_CASE( "019" )
{
	MD::Parser parser;

	auto doc = parser.parse( QStringLiteral( "tests/parser/data/019.md" ) );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	for( int i = 1; i < 3; ++i )
	{
		REQUIRE( doc->items().at( i )->type() == MD::ItemType::Blockquote );

		auto bq = static_cast< MD::Blockquote* > ( doc->items().at( i ).data() );

		REQUIRE( !bq->isEmpty() );
		REQUIRE( bq->items().size() == 3 );

		{
			REQUIRE( bq->items().at( 0 )->type() == MD::ItemType::Paragraph );

			auto p = static_cast< MD::Paragraph* > ( bq->items().at( 0 ).data() );

			REQUIRE( !p->isEmpty() );
			REQUIRE( p->items().size() == 1 );

			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

			auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );

			REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "Quote paragraph 1." ) );
		}

		{
			REQUIRE( bq->items().at( 1 )->type() == MD::ItemType::Paragraph );

			auto p = static_cast< MD::Paragraph* > ( bq->items().at( 1 ).data() );

			REQUIRE( !p->isEmpty() );
			REQUIRE( p->items().size() == 1 );

			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

			auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );

			REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
			REQUIRE( t->text() == QStringLiteral( "Quote paragraph 2." ) );
		}

		REQUIRE( bq->items().at( 2 )->type() == MD::ItemType::Blockquote );

		auto nbq = static_cast< MD::Blockquote* > ( bq->items().at( 2 ).data() );

		REQUIRE( !nbq->isEmpty() );
		REQUIRE( nbq->items().size() == 1 );

		REQUIRE( nbq->items().at( 0 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph* > ( nbq->items().at( 0 ).data() );

		REQUIRE( !p->isEmpty() );
		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );

		REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "Nested quote" ) );
	}
}

TEST_CASE( "020" )
{
	MD::Parser parser;

	auto doc = parser.parse( QStringLiteral( "tests/parser/data/020.md" ) );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );

	auto c = static_cast< MD::Code* > ( doc->items().at( 1 ).data() );

	REQUIRE( c->isInlined() == false );
	REQUIRE( c->text() ==
		QStringLiteral( "if( a > b )\n  do_something();\nelse\n  dont_do_anything();" ) );
	REQUIRE( c->syntax() == QStringLiteral( "cpp" ) );
}

TEST_CASE( "021" )
{
	MD::Parser parser;

	auto doc = parser.parse( QStringLiteral( "tests/parser/data/021.md" ) );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );

	auto c = static_cast< MD::Code* > ( doc->items().at( 1 ).data() );

	REQUIRE( c->isInlined() == false );
	REQUIRE( c->text() ==
		QStringLiteral( "if( a > b )\n  do_something();\nelse\n  dont_do_anything();" ) );
}

TEST_CASE( "022" )
{
	MD::Parser parser;

	auto doc = parser.parse( QStringLiteral( "tests/parser/data/022.md" ) );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );

	auto c = static_cast< MD::Code* > ( doc->items().at( 1 ).data() );

	REQUIRE( c->isInlined() == false );
	REQUIRE( c->text() ==
		QStringLiteral( "if( a > b )\n  do_something();\nelse\n  dont_do_anything();" ) );
}

TEST_CASE( "023" )
{
	MD::Parser parser;

	auto doc = parser.parse( QStringLiteral( "tests/parser/data/023.md" ) );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List* > ( doc->items().at( 1 ).data() );

	REQUIRE( l->items().size() == 3 );

	for( int i = 0; i < 3; ++i )
	{
		REQUIRE( l->items().at( i )->type() == MD::ItemType::ListItem );

		auto item = static_cast< MD::ListItem* > ( l->items().at( i ).data() );

		REQUIRE( item->listType() == MD::ListItem::Unordered );

		REQUIRE( item->items().size() == 1 );

		REQUIRE( item->items().at( 0 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph* > ( item->items().at( 0 ).data() );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );

		REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( t->text() == ( QString::fromLatin1( "Item " ) + QString::number( i + 1 ) ) );
	}
}

TEST_CASE( "024" )
{
	auto checkItem = [] ( MD::ListItem * item, int i )
	{
		REQUIRE( item->items().at( 0 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph* > ( item->items().at( 0 ).data() );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );

		REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( t->text() == ( QString::fromLatin1( "Item " ) + QString::number( i + 1 ) ) );
	};

	MD::Parser parser;

	auto doc = parser.parse( QStringLiteral( "tests/parser/data/024.md" ) );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List* > ( doc->items().at( 1 ).data() );

	REQUIRE( l->items().size() == 3 );

	for( int i = 0; i < 3; ++i )
	{
		REQUIRE( l->items().at( i )->type() == MD::ItemType::ListItem );

		auto item = static_cast< MD::ListItem* > ( l->items().at( i ).data() );

		REQUIRE( item->listType() == MD::ListItem::Unordered );

		REQUIRE( item->items().size() == 2 );

		checkItem( item, i );

		REQUIRE( item->items().at( 1 )->type() == MD::ItemType::List );

		auto nl = static_cast< MD::List* > ( item->items().at( 1 ).data() );

		REQUIRE( nl->items().size() == 2 );

		for( int j = 0; j < 2; ++j )
		{
			REQUIRE( nl->items().at( j )->type() == MD::ItemType::ListItem );

			auto nitem = static_cast< MD::ListItem* > ( nl->items().at( j ).data() );

			checkItem( nitem, j );
		}
	}
}

TEST_CASE( "025" )
{
	MD::Parser parser;

	auto doc = parser.parse( QStringLiteral( "tests/parser/data/025.md" ) );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List* > ( doc->items().at( 1 ).data() );

	REQUIRE( l->items().size() == 3 );

	for( int i = 0; i < 3; ++i )
	{
		REQUIRE( l->items().at( i )->type() == MD::ItemType::ListItem );

		auto item = static_cast< MD::ListItem* > ( l->items().at( i ).data() );

		REQUIRE( item->listType() == MD::ListItem::Unordered );

		REQUIRE( item->items().size() == 2 );

		{
			REQUIRE( item->items().at( 0 )->type() == MD::ItemType::Paragraph );

			auto p = static_cast< MD::Paragraph* > ( item->items().at( 0 ).data() );

			REQUIRE( p->items().size() == 1 );

			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

			auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );

			REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
			REQUIRE( t->text() == ( QString::fromLatin1( "Item " ) + QString::number( i + 1 ) ) );
		}

		{
			REQUIRE( item->items().at( 1 )->type() == MD::ItemType::Paragraph );

			auto p = static_cast< MD::Paragraph* > ( item->items().at( 1 ).data() );

			REQUIRE( p->items().size() == 1 );

			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

			auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );

			REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
			REQUIRE( t->text() == ( QString::fromLatin1( "Paragraph in list" ) ) );
		}
	}
}

TEST_CASE( "026" )
{
	MD::Parser parser;

	auto doc = parser.parse( QStringLiteral( "tests/parser/data/026.md" ) );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List* > ( doc->items().at( 1 ).data() );

	REQUIRE( l->items().size() == 3 );

	for( int i = 0; i < 3; ++i )
	{
		REQUIRE( l->items().at( i )->type() == MD::ItemType::ListItem );

		auto item = static_cast< MD::ListItem* > ( l->items().at( i ).data() );

		REQUIRE( item->listType() == MD::ListItem::Unordered );

		REQUIRE( item->items().size() == 3 );

		{
			REQUIRE( item->items().at( 0 )->type() == MD::ItemType::Paragraph );

			auto p = static_cast< MD::Paragraph* > ( item->items().at( 0 ).data() );

			REQUIRE( p->items().size() == 1 );

			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

			auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );

			REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
			REQUIRE( t->text() == ( QString::fromLatin1( "Item " ) + QString::number( i + 1 ) ) );
		}

		{
			REQUIRE( item->items().at( 1 )->type() == MD::ItemType::Paragraph );

			auto p = static_cast< MD::Paragraph* > ( item->items().at( 1 ).data() );

			REQUIRE( p->items().size() == 1 );

			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

			auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );

			REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
			REQUIRE( t->text() == ( QString::fromLatin1( "Paragraph in list" ) ) );
		}

		{
			REQUIRE( item->items().at( 2 )->type() == MD::ItemType::List );

			auto nl = static_cast< MD::List* > ( item->items().at( 2 ).data() );

			REQUIRE( nl->items().at( 0 )->type() == MD::ItemType::ListItem );

			auto item = static_cast< MD::ListItem* > ( nl->items().at( 0 ).data() );

			REQUIRE( item->listType() == MD::ListItem::Unordered );

			REQUIRE( item->items().size() == 2 );

			{
				REQUIRE( item->items().at( 0 )->type() == MD::ItemType::Paragraph );

				auto p = static_cast< MD::Paragraph* > ( item->items().at( 0 ).data() );

				REQUIRE( p->items().size() == 1 );

				REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

				auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );

				REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
				REQUIRE( t->text() == QStringLiteral( "Nested" ) );
			}

			{
				REQUIRE( item->items().at( 1 )->type() == MD::ItemType::Paragraph );

				auto p = static_cast< MD::Paragraph* > ( item->items().at( 1 ).data() );

				REQUIRE( p->items().size() == 1 );

				REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

				auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );

				REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
				REQUIRE( t->text() == ( QString::fromLatin1( "Paragraph in list" ) ) );
			}
		}
	}
}

TEST_CASE( "027" )
{
	MD::Parser parser;

	auto doc = parser.parse( QStringLiteral( "tests/parser/data/027.md" ) );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List* > ( doc->items().at( 1 ).data() );

	REQUIRE( l->items().size() == 3 );

	for( int i = 0; i < 3; ++i )
	{
		REQUIRE( l->items().at( i )->type() == MD::ItemType::ListItem );

		auto item = static_cast< MD::ListItem* > ( l->items().at( i ).data() );

		REQUIRE( item->listType() == MD::ListItem::Unordered );

		REQUIRE( item->items().size() == 2 );

		{
			REQUIRE( item->items().at( 0 )->type() == MD::ItemType::Paragraph );

			auto p = static_cast< MD::Paragraph* > ( item->items().at( 0 ).data() );

			REQUIRE( p->items().size() == 1 );

			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

			auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );

			REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
			REQUIRE( t->text() == ( QString::fromLatin1( "Item " ) + QString::number( i + 1 ) ) );
		}

		{
			REQUIRE( item->items().at( 1 )->type() == MD::ItemType::Code );

			auto c = static_cast< MD::Code* > ( item->items().at( 1 ).data() );

			REQUIRE( c->isInlined() == false );
			REQUIRE( c->text() == ( QStringLiteral( "code" ) ) );
		}
	}
}

TEST_CASE( "028" )
{
	MD::Parser parser;

	auto doc = parser.parse( QStringLiteral( "tests/parser/data/028.md" ) );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List* > ( doc->items().at( 1 ).data() );

	REQUIRE( l->items().size() == 3 );

	for( int i = 0; i < 3; ++i )
	{
		REQUIRE( l->items().at( i )->type() == MD::ItemType::ListItem );

		auto item = static_cast< MD::ListItem* > ( l->items().at( i ).data() );

		REQUIRE( item->listType() == MD::ListItem::Unordered );

		REQUIRE( item->items().size() == 2 );

		{
			REQUIRE( item->items().at( 0 )->type() == MD::ItemType::Paragraph );

			auto p = static_cast< MD::Paragraph* > ( item->items().at( 0 ).data() );

			REQUIRE( p->items().size() == 1 );

			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

			auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );

			REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
			REQUIRE( t->text() == ( QString::fromLatin1( "Item " ) + QString::number( i + 1 ) ) );
		}

		{
			REQUIRE( item->items().at( 1 )->type() == MD::ItemType::Code );

			auto c = static_cast< MD::Code* > ( item->items().at( 1 ).data() );

			REQUIRE( c->isInlined() == false );
			REQUIRE( c->text() == ( QStringLiteral( "code" ) ) );
		}
	}
}

TEST_CASE( "029" )
{
	MD::Parser parser;

	auto doc = parser.parse( QStringLiteral( "tests/parser/data/029.md" ) );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List* > ( doc->items().at( 1 ).data() );

	REQUIRE( l->items().size() == 3 );

	for( int i = 0; i < 3; ++i )
	{
		REQUIRE( l->items().at( i )->type() == MD::ItemType::ListItem );

		auto item = static_cast< MD::ListItem* > ( l->items().at( i ).data() );

		REQUIRE( item->listType() == MD::ListItem::Unordered );

		REQUIRE( item->items().size() == 3 );

		{
			REQUIRE( item->items().at( 0 )->type() == MD::ItemType::Paragraph );

			auto p = static_cast< MD::Paragraph* > ( item->items().at( 0 ).data() );

			REQUIRE( p->items().size() == 1 );

			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

			auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );

			REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
			REQUIRE( t->text() == ( QString::fromLatin1( "Item " ) + QString::number( i + 1 ) ) );
		}

		{
			REQUIRE( item->items().at( 1 )->type() == MD::ItemType::Paragraph );

			auto p = static_cast< MD::Paragraph* > ( item->items().at( 1 ).data() );

			REQUIRE( p->items().size() == 1 );

			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

			auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );

			REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
			REQUIRE( t->text() == ( QString::fromLatin1( "Paragraph in list" ) ) );
		}

		{
			REQUIRE( item->items().at( 2 )->type() == MD::ItemType::List );

			auto nl = static_cast< MD::List* > ( item->items().at( 2 ).data() );

			REQUIRE( nl->items().at( 0 )->type() == MD::ItemType::ListItem );

			auto item = static_cast< MD::ListItem* > ( nl->items().at( 0 ).data() );

			REQUIRE( item->listType() == MD::ListItem::Unordered );

			REQUIRE( item->items().size() == 2 );

			{
				REQUIRE( item->items().at( 0 )->type() == MD::ItemType::Paragraph );

				auto p = static_cast< MD::Paragraph* > ( item->items().at( 0 ).data() );

				REQUIRE( p->items().size() == 1 );

				REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

				auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );

				REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
				REQUIRE( t->text() == QStringLiteral( "Nested" ) );
			}

			{
				REQUIRE( item->items().at( 1 )->type() == MD::ItemType::Paragraph );

				auto p = static_cast< MD::Paragraph* > ( item->items().at( 1 ).data() );

				REQUIRE( p->items().size() == 1 );

				REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

				auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );

				REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
				REQUIRE( t->text() == ( QString::fromLatin1( "Paragraph in list" ) ) );
			}
		}
	}

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 2 ).data() );

	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

	auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );

	REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
	REQUIRE( t->text() == QStringLiteral( "Standalone paragraph" ) );
}

TEST_CASE( "030" )
{
	MD::Parser parser;

	auto doc = parser.parse( QStringLiteral( "tests/parser/data/030.md" ) );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );

	REQUIRE( p->items().size() == 6 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

	auto t1 = static_cast< MD::Text* > ( p->items().at( 0 ).data() );

	REQUIRE( t1->text() == QStringLiteral( "Text" ) );

	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Image );

	auto i1 = static_cast< MD::Image* > ( p->items().at( 1 ).data() );

	const QString wd = QDir().absolutePath() + QStringLiteral( "/tests/parser/data/" );

	REQUIRE( i1->text() == QStringLiteral( "Image 1" ) );
	REQUIRE( i1->url() == wd + QStringLiteral( "a.jpg" ) );

	REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );

	auto t2 = static_cast< MD::Text* > ( p->items().at( 2 ).data() );

	REQUIRE( t2->text() == QStringLiteral( "continue" ) );

	REQUIRE( p->items().at( 3 )->type() == MD::ItemType::Image );

	auto i2 = static_cast< MD::Image* > ( p->items().at( 3 ).data() );

	REQUIRE( i2->text() == QStringLiteral( "Image 2" ) );
	REQUIRE( i2->url() == wd + QStringLiteral( "b.png" ) );

	REQUIRE( p->items().at( 4 )->type() == MD::ItemType::Text );

	auto t3 = static_cast< MD::Text* > ( p->items().at( 4 ).data() );

	REQUIRE( t3->text() == QStringLiteral( "and" ) );

	REQUIRE( p->items().at( 5 )->type() == MD::ItemType::Image );

	auto i3 = static_cast< MD::Image* > ( p->items().at( 5 ).data() );

	REQUIRE( i3->text() == QStringLiteral( "Image 3" ) );
	REQUIRE( i3->url() == QStringLiteral( "http://www.where.com/c.jpeg" ) );
}

TEST_CASE( "031" )
{
	MD::Parser parser;

	auto doc = parser.parse( QStringLiteral( "tests/parser/data/031.md" ) );

	const QString wd = QDir().absolutePath() + QStringLiteral( "/tests/parser/data" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 9 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );

	REQUIRE( p->items().size() == 5 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

	auto t0 = static_cast< MD::Text* > ( p->items().at( 0 ).data() );

	REQUIRE( t0->text() == QStringLiteral( "[link 0][wrong-label]" ) );
	REQUIRE( t0->opts() == MD::TextWithoutFormat );

	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Link );

	auto l1 = static_cast< MD::Link* > ( p->items().at( 1 ).data() );

	REQUIRE( l1->text() == QStringLiteral( "link 1" ) );
	REQUIRE( l1->url() == ( wd + QStringLiteral( "/a.md" ) ) );

	REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Link );

	auto l2 = static_cast< MD::Link* > ( p->items().at( 2 ).data() );

	REQUIRE( l2->url() == wd + QStringLiteral( "/b.md" ) );
	REQUIRE( l2->opts() == MD::TextOption::TextWithoutFormat );

	REQUIRE( !l2->img().isNull() );
	REQUIRE( l2->img()->text() == QStringLiteral( "image 1" ) );
	REQUIRE( l2->img()->url() == wd + QStringLiteral( "/" ) +
		QStringLiteral( "a.png" ) );

	REQUIRE( p->items().at( 3 )->type() == MD::ItemType::Link );

	auto l3 = static_cast< MD::Link* > ( p->items().at( 3 ).data() );

	REQUIRE( l3->text() == QStringLiteral( "link 3" ) );

	const QString label = QString::fromLatin1( "#LABEL" ) + QStringLiteral( "/" ) +
		wd + QStringLiteral( "/" ) + QStringLiteral( "031.md" );

	REQUIRE( l3->url() == label );

	REQUIRE( p->items().at( 4 )->type() == MD::ItemType::FootnoteRef );

	auto f1 = static_cast< MD::FootnoteRef* > ( p->items().at( 4 ).data() );

	REQUIRE( f1->id() ==
		QString::fromLatin1( "#^REF" ) + QStringLiteral( "/" ) + wd +
		QStringLiteral( "/" ) + QStringLiteral( "031.md" ) );

	REQUIRE( !doc->labeledLinks().isEmpty() );
	REQUIRE( doc->labeledLinks().contains( label ) );
	REQUIRE( doc->labeledLinks()[ label ]->url() == QStringLiteral( "http://www.where.com/a.md" ) );


	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );

		p = static_cast< MD::Paragraph* > ( doc->items().at( 2 ).data() );

		REQUIRE( p->items().size() == 2 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::FootnoteRef );

		f1 = static_cast< MD::FootnoteRef* > ( p->items().at( 0 ).data() );

		REQUIRE( f1->id() ==
			QString::fromLatin1( "#^REF" ) + QStringLiteral( "/" ) + wd +
			QStringLiteral( "/" ) + QStringLiteral( "031.md" ) );

		auto t = static_cast< MD::Text* > ( p->items().at( 1 ).data() );

		REQUIRE( t->text() == QStringLiteral( "text" ) );

		REQUIRE( doc->labeledLinks().size() == 2 );

		REQUIRE( doc->labeledLinks()[ QString::fromLatin1( "#1" ) +
			QStringLiteral( "/" ) + wd + QStringLiteral( "/" ) +
			QStringLiteral( "031.md" ) ]->url() == wd + QStringLiteral( "/a.md" ) );
	}

	{
		REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Paragraph );

		p = static_cast< MD::Paragraph* > ( doc->items().at( 3 ).data() );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );

		auto l = static_cast< MD::Link* > ( p->items().at( 0 ).data() );

		REQUIRE( l->url() == QString::fromLatin1( "#label" ) +
			QStringLiteral( "/" ) + wd + QStringLiteral( "/" ) +
			QStringLiteral( "031.md" ) );
	}

	REQUIRE( doc->items().at( 4 )->type() == MD::ItemType::PageBreak );
}

TEST_CASE( "032" )
{
	MD::Parser parser;

	auto doc = parser.parse( QStringLiteral( "tests/parser/data/032.md" ) );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );

	auto q = static_cast< MD::Blockquote* > ( doc->items().at( 1 ).data() );

	REQUIRE( q->items().size() == 1 );

	REQUIRE( q->items().at( 0 )->type() == MD::ItemType::Code );

	auto c = static_cast< MD::Code* > ( q->items().at( 0 ).data() );

	REQUIRE( c->isInlined() == false );
	REQUIRE( c->text() == QStringLiteral( "if( a < b )\n  do_something();" ) );
}

TEST_CASE( "033" )
{
	MD::Parser parser;

	auto doc = parser.parse( QStringLiteral( "tests/parser/data/033.md" ) );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );

	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );

	auto l = static_cast< MD::Link* > ( p->items().at( 0 ).data() );

	REQUIRE( l->url() == QStringLiteral( "http://www.google.com" ) );
	REQUIRE( l->text().isEmpty() );
}

TEST_CASE( "034" )
{
	MD::Parser parser;

	auto doc = parser.parse( QStringLiteral( "tests/parser/data/034.md" ) );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );

	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );

	auto l = static_cast< MD::Link* > ( p->items().at( 0 ).data() );

	REQUIRE( l->url() == QStringLiteral( "https://www.google.com" ) );
	REQUIRE( l->text() == QStringLiteral( "Google" ) );
	REQUIRE( l->opts() == MD::TextOption::BoldText );
}

TEST_CASE( "035" )
{
	MD::Parser parser;

	auto doc = parser.parse( QStringLiteral( "tests/parser/data/035.md" ) );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List* > ( doc->items().at( 1 ).data() );

	REQUIRE( l->items().size() == 3 );

	{
		REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );

		auto i1 = static_cast< MD::ListItem* > ( l->items().at( 0 ).data() );

		REQUIRE( i1->listType() == MD::ListItem::Ordered );
		REQUIRE( i1->orderedListPreState() == MD::ListItem::Start );
		REQUIRE( i1->items().size() == 1 );
		REQUIRE( i1->items().at( 0 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph* > ( i1->items().at( 0 ).data() );

		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		REQUIRE( static_cast< MD::Text* > ( p->items().at( 0 ).data() )->text() == QStringLiteral( "1" ) );
	}

	REQUIRE( l->items().size() == 3 );

	{
		REQUIRE( l->items().at( 1 )->type() == MD::ItemType::ListItem );

		auto i1 = static_cast< MD::ListItem* > ( l->items().at( 1 ).data() );

		REQUIRE( i1->listType() == MD::ListItem::Ordered );
		REQUIRE( i1->orderedListPreState() == MD::ListItem::Continue );
		REQUIRE( i1->items().size() == 2 );
		REQUIRE( i1->items().at( 0 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph* > ( i1->items().at( 0 ).data() );

		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		REQUIRE( static_cast< MD::Text* > ( p->items().at( 0 ).data() )->text() == QStringLiteral( "2" ) );

		REQUIRE( i1->items().at( 1 )->type() == MD::ItemType::List );

		auto nl = static_cast< MD::List* > ( i1->items().at( 1 ).data() );

		REQUIRE( nl->items().size() == 2 );

		{
			REQUIRE( nl->items().at( 0 )->type() == MD::ItemType::ListItem );

			auto i1 = static_cast< MD::ListItem* > ( nl->items().at( 0 ).data() );

			REQUIRE( i1->listType() == MD::ListItem::Ordered );
			REQUIRE( i1->orderedListPreState() == MD::ListItem::Start );
			REQUIRE( i1->items().size() == 1 );
			REQUIRE( i1->items().at( 0 )->type() == MD::ItemType::Paragraph );

			auto p = static_cast< MD::Paragraph* > ( i1->items().at( 0 ).data() );

			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			REQUIRE( static_cast< MD::Text* > ( p->items().at( 0 ).data() )->text() == QStringLiteral( "1" ) );
		}

		{
			REQUIRE( nl->items().at( 1 )->type() == MD::ItemType::ListItem );

			auto i1 = static_cast< MD::ListItem* > ( nl->items().at( 1 ).data() );

			REQUIRE( i1->listType() == MD::ListItem::Ordered );
			REQUIRE( i1->orderedListPreState() == MD::ListItem::Continue );
			REQUIRE( i1->items().size() == 1 );
			REQUIRE( i1->items().at( 0 )->type() == MD::ItemType::Paragraph );

			auto p = static_cast< MD::Paragraph* > ( i1->items().at( 0 ).data() );

			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			REQUIRE( static_cast< MD::Text* > ( p->items().at( 0 ).data() )->text() == QStringLiteral( "2" ) );
		}
	}

	{
		REQUIRE( l->items().at( 2 )->type() == MD::ItemType::ListItem );

		auto i1 = static_cast< MD::ListItem* > ( l->items().at( 2 ).data() );

		REQUIRE( i1->listType() == MD::ListItem::Ordered );
		REQUIRE( i1->orderedListPreState() == MD::ListItem::Continue );
		REQUIRE( i1->items().size() == 1 );
		REQUIRE( i1->items().at( 0 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph* > ( i1->items().at( 0 ).data() );

		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		REQUIRE( static_cast< MD::Text* > ( p->items().at( 0 ).data() )->text() == QStringLiteral( "3" ) );
	}
}

TEST_CASE( "036" )
{
	MD::Parser parser;

	auto doc = parser.parse( QStringLiteral( "tests/parser/data/036.md" ) );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );

	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );

	auto l = static_cast< MD::Link* > ( p->items().at( 0 ).data() );

	REQUIRE( l->url() == QStringLiteral( "www.google.com" ) );
	REQUIRE( l->text() == QStringLiteral( "Google" ) );
}

TEST_CASE( "037" )
{
	MD::Parser parser;

	auto doc = parser.parse( QStringLiteral( "tests/parser/data/037.md" ) );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 17 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );

		REQUIRE( t->text() == QStringLiteral( "[Google] ( www.google.com Google Shmoogle..." ) );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph* > ( doc->items().at( 2 ).data() );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );

		REQUIRE( t->text() == QStringLiteral( "[Google] (" ) );
	}

	{
		REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph* > ( doc->items().at( 3 ).data() );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );

		REQUIRE( t->text() == QStringLiteral( "[Google" ) );
	}

	{
		REQUIRE( doc->items().at( 4 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph* > ( doc->items().at( 4 ).data() );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );

		REQUIRE( t->text() == QStringLiteral( "[Google]" ) );
	}

	{
		REQUIRE( doc->items().at( 5 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph* > ( doc->items().at( 5 ).data() );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );

		REQUIRE( t->text() == QStringLiteral( "[![Google](" ) );
	}

	{
		REQUIRE( doc->items().at( 6 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph* > ( doc->items().at( 6 ).data() );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );

		REQUIRE( t->text() == QStringLiteral( "![Google](" ) );
	}

	{
		REQUIRE( doc->items().at( 7 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph* > ( doc->items().at( 7 ).data() );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );

		REQUIRE( t->text() == QStringLiteral( "[Google] ( www.google.com \"Google Shmoogle...\"" ) );
	}

	{
		REQUIRE( doc->items().at( 8 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph* > ( doc->items().at( 8 ).data() );

		REQUIRE( p->items().size() == 2 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );

		REQUIRE( t->text() == QStringLiteral( "[" ) );

		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Image );

		auto i = static_cast< MD::Image* > ( p->items().at( 1 ).data() );

		REQUIRE( i->url() == QStringLiteral( "https://www.google.com/logo.png" ) );
		REQUIRE( i->text() == QStringLiteral( "Google" ) );
	}

	{
		REQUIRE( doc->items().at( 9 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph* > ( doc->items().at( 9 ).data() );

		REQUIRE( p->items().size() == 3 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );

		REQUIRE( t->text() == QStringLiteral( "text" ) );

		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::FootnoteRef );

		REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );

		t = static_cast< MD::Text* > ( p->items().at( 2 ).data() );

		REQUIRE( t->text() == QStringLiteral( ":" ) );
	}

	{
		REQUIRE( doc->items().at( 10 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph* > ( doc->items().at( 10 ).data() );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );

		REQUIRE( t->text() == QStringLiteral( "[^ref" ) );
	}

	{
		REQUIRE( doc->items().at( 11 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph* > ( doc->items().at( 11 ).data() );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );

		REQUIRE( t->text() == QStringLiteral( "[" ) );
	}

	{
		REQUIRE( doc->items().at( 12 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph* > ( doc->items().at( 12 ).data() );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );

		REQUIRE( t->text() == QStringLiteral( "[1]:" ) );
	}

	{
		REQUIRE( doc->items().at( 13 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph* > ( doc->items().at( 13 ).data() );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );

		REQUIRE( t->text() == QStringLiteral( "[1]:" ) );
	}

	{
		REQUIRE( doc->items().at( 14 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph* > ( doc->items().at( 14 ).data() );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );

		REQUIRE( t->text() == QStringLiteral( "[text][link" ) );
	}

	{
		REQUIRE( doc->items().at( 15 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph* > ( doc->items().at( 15 ).data() );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );

		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "[text]#" ) );
	}

	{
		REQUIRE( doc->items().at( 16 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph* > ( doc->items().at( 16 ).data() );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );

		REQUIRE( t->text() == QStringLiteral( "<www.google.com" ) );
	}
}

TEST_CASE( "038" )
{
	MD::Parser parser;

	auto doc = parser.parse( QStringLiteral( "tests/parser/data/038.md" ) );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List* > ( doc->items().at( 1 ).data() );

	REQUIRE( l->items().size() == 3 );

	{
		REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );

		auto i1 = static_cast< MD::ListItem* > ( l->items().at( 0 ).data() );

		REQUIRE( i1->listType() == MD::ListItem::Ordered );
		REQUIRE( i1->orderedListPreState() == MD::ListItem::Start );
		REQUIRE( i1->items().size() == 1 );
		REQUIRE( i1->items().at( 0 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph* > ( i1->items().at( 0 ).data() );

		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		REQUIRE( static_cast< MD::Text* > ( p->items().at( 0 ).data() )->text() == QStringLiteral( "1" ) );
	}

	REQUIRE( l->items().size() == 3 );

	{
		REQUIRE( l->items().at( 1 )->type() == MD::ItemType::ListItem );

		auto i1 = static_cast< MD::ListItem* > ( l->items().at( 1 ).data() );

		REQUIRE( i1->listType() == MD::ListItem::Ordered );
		REQUIRE( i1->orderedListPreState() == MD::ListItem::Continue );
		REQUIRE( i1->items().size() == 2 );
		REQUIRE( i1->items().at( 0 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph* > ( i1->items().at( 0 ).data() );

		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		REQUIRE( static_cast< MD::Text* > ( p->items().at( 0 ).data() )->text() == QStringLiteral( "2" ) );

		REQUIRE( i1->items().at( 1 )->type() == MD::ItemType::List );

		auto nl = static_cast< MD::List* > ( i1->items().at( 1 ).data() );

		REQUIRE( nl->items().size() == 2 );

		{
			REQUIRE( nl->items().at( 0 )->type() == MD::ItemType::ListItem );

			auto i1 = static_cast< MD::ListItem* > ( nl->items().at( 0 ).data() );

			REQUIRE( i1->listType() == MD::ListItem::Ordered );
			REQUIRE( i1->orderedListPreState() == MD::ListItem::Start );
			REQUIRE( i1->items().size() == 1 );
			REQUIRE( i1->items().at( 0 )->type() == MD::ItemType::Paragraph );

			auto p = static_cast< MD::Paragraph* > ( i1->items().at( 0 ).data() );

			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			REQUIRE( static_cast< MD::Text* > ( p->items().at( 0 ).data() )->text() == QStringLiteral( "1" ) );
		}

		{
			REQUIRE( nl->items().at( 1 )->type() == MD::ItemType::ListItem );

			auto i1 = static_cast< MD::ListItem* > ( nl->items().at( 1 ).data() );

			REQUIRE( i1->listType() == MD::ListItem::Ordered );
			REQUIRE( i1->orderedListPreState() == MD::ListItem::Continue );
			REQUIRE( i1->items().size() == 1 );
			REQUIRE( i1->items().at( 0 )->type() == MD::ItemType::Paragraph );

			auto p = static_cast< MD::Paragraph* > ( i1->items().at( 0 ).data() );

			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			REQUIRE( static_cast< MD::Text* > ( p->items().at( 0 ).data() )->text() == QStringLiteral( "2" ) );
		}
	}

	{
		REQUIRE( l->items().at( 2 )->type() == MD::ItemType::ListItem );

		auto i1 = static_cast< MD::ListItem* > ( l->items().at( 2 ).data() );

		REQUIRE( i1->listType() == MD::ListItem::Ordered );
		REQUIRE( i1->orderedListPreState() == MD::ListItem::Continue );
		REQUIRE( i1->items().size() == 1 );
		REQUIRE( i1->items().at( 0 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph* > ( i1->items().at( 0 ).data() );

		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		REQUIRE( static_cast< MD::Text* > ( p->items().at( 0 ).data() )->text() == QStringLiteral( "3" ) );
	}
}

TEST_CASE( "039" )
{
	MD::Parser parser;

	auto doc = parser.parse( QStringLiteral( "tests/parser/data/039.md" ) );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );

	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

	auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );

	REQUIRE( t->opts() == MD::TextOption::BoldText );
	REQUIRE( t->text() == QStringLiteral( "text" ) );
}

TEST_CASE( "040" )
{
	MD::Parser parser;

	auto doc = parser.parse( QStringLiteral( "tests/parser/data/040.md" ) );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto dp = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );

	REQUIRE( dp->items().size() == 3 );

	REQUIRE( dp->items().at( 0 )->type() == MD::ItemType::Text );

	auto t1 = static_cast< MD::Text* > ( dp->items().at( 0 ).data() );

	REQUIRE( t1->text() == QStringLiteral( "``Use this" ) );

	REQUIRE( dp->items().at( 1 )->type() == MD::ItemType::Code );

	auto c1 = static_cast< MD::Code* > ( dp->items().at( 1 ).data() );

	REQUIRE( c1->text() == QStringLiteral( "code" ) );

	REQUIRE( dp->items().at( 2 )->type() == MD::ItemType::Text );

	auto t2 = static_cast< MD::Text* > ( dp->items().at( 2 ).data() );

	REQUIRE( t2->text() == QStringLiteral( "in the code" ) );
}

TEST_CASE( "041" )
{
	MD::Parser parser;

	auto doc = parser.parse( QStringLiteral( "tests/parser/data/041.md" ) );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List* > ( doc->items().at( 1 ).data() );

	REQUIRE( l->items().size() == 3 );

	for( int i = 0; i < 3; ++i )
	{
		REQUIRE( l->items().at( i )->type() == MD::ItemType::ListItem );

		auto item = static_cast< MD::ListItem* > ( l->items().at( i ).data() );

		REQUIRE( item->listType() == MD::ListItem::Unordered );

		REQUIRE( item->items().size() == 2 );

		{
			REQUIRE( item->items().at( 0 )->type() == MD::ItemType::Paragraph );

			auto p = static_cast< MD::Paragraph* > ( item->items().at( 0 ).data() );

			REQUIRE( p->items().size() == 1 );

			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

			auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );

			REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
			REQUIRE( t->text() == ( QString::fromLatin1( "Item " ) + QString::number( i + 1 ) ) );
		}

		{
			REQUIRE( item->items().at( 1 )->type() == MD::ItemType::Paragraph );

			auto p = static_cast< MD::Paragraph* > ( item->items().at( 1 ).data() );

			REQUIRE( p->items().size() == 1 );

			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

			auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );

			REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
			REQUIRE( t->text() == ( QString::fromLatin1( "Paragraph in list" ) ) );
		}
	}
}

TEST_CASE( "042" )
{
	MD::Parser parser;

	auto doc = parser.parse( QStringLiteral( "tests/parser/data/042.md" ) );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 5 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List* > ( doc->items().at( 1 ).data() );

	REQUIRE( l->items().size() == 2 );

	const QString wd = QDir().absolutePath() + QStringLiteral( "/tests/parser/data" );

	for( int i = 0; i < 2; ++i )
	{
		REQUIRE( l->items().at( i )->type() == MD::ItemType::ListItem );

		auto li = static_cast< MD::ListItem* > ( l->items().at( i ).data() );

		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph* > ( li->items().at( 0 ).data() );

		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );

		auto lnk = static_cast< MD::Link* > ( p->items().at( 0 ).data() );

		REQUIRE( lnk->text() == QStringLiteral( "Chapter 1" ) );
		REQUIRE( lnk->url() == wd + QStringLiteral( "/042-1.md" ) );
	}

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::PageBreak );

	REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Anchor );

	REQUIRE( static_cast< MD::Anchor* > ( doc->items().at( 3 ).data() )->label() ==
		wd + QStringLiteral( "/042-1.md" ) );

	REQUIRE( doc->items().at( 4 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 4 ).data() );

	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

	auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );

	REQUIRE( t->text() == QStringLiteral( "Paragraph 1" ) );
}

TEST_CASE( "042-1" )
{
	MD::Parser parser;

	auto doc = parser.parse( QStringLiteral( "tests/parser/data/042.md" ), false );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List* > ( doc->items().at( 1 ).data() );

	REQUIRE( l->items().size() == 2 );

	const QString wd = QDir().absolutePath() + QStringLiteral( "/tests/parser/data" );

	for( int i = 0; i < 2; ++i )
	{
		REQUIRE( l->items().at( i )->type() == MD::ItemType::ListItem );

		auto li = static_cast< MD::ListItem* > ( l->items().at( i ).data() );

		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph* > ( li->items().at( 0 ).data() );

		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );

		auto lnk = static_cast< MD::Link* > ( p->items().at( 0 ).data() );

		REQUIRE( lnk->text() == QStringLiteral( "Chapter 1" ) );
		REQUIRE( lnk->url() == wd + QStringLiteral( "/042-1.md" ) );
	}
}

TEST_CASE( "044" )
{
	MD::Parser parser;

	auto doc = parser.parse( QStringLiteral( "tests/parser/data/044.md" ), false );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List* > ( doc->items().at( 1 ).data() );

	REQUIRE( l->items().size() == 1 );

	REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );

	auto li = static_cast< MD::ListItem* > ( l->items().at( 0 ).data() );

	REQUIRE( li->items().size() == 2 );
	REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph* > ( li->items().at( 0 ).data() );

	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

	auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );

	REQUIRE( t->text() == QStringLiteral( "Item" ) );

	REQUIRE( li->items().at( 1 )->type() == MD::ItemType::Blockquote );

	auto bq = static_cast< MD::Blockquote* > ( li->items().at( 1 ).data() );

	REQUIRE( bq->items().size() == 1 );

	REQUIRE( bq->items().at( 0 )->type() == MD::ItemType::Paragraph );

	p = static_cast< MD::Paragraph* > ( bq->items().at( 0 ).data() );

	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

	t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );

	REQUIRE( t->text() == QStringLiteral( "Quote" ) );
}

TEST_CASE( "045" )
{
	MD::Parser parser;

	auto doc = parser.parse( QStringLiteral( "tests/parser/data/045.md" ), false );

	REQUIRE( !doc->isEmpty() );
	REQUIRE( doc->items().size() == 1 );
	REQUIRE( doc->items().at( 0 )->type() == MD::ItemType::Anchor );

	REQUIRE( doc->footnotesMap().size() == 1 );

	const QString wd = QDir().absolutePath() + QStringLiteral( "/tests/parser/data/" );

	const QString label = QString::fromLatin1( "#^footnote" ) + QStringLiteral( "/" ) + wd +
		QStringLiteral( "045.md" );

	REQUIRE( doc->footnotesMap().contains( label ) );

	auto f = doc->footnotesMap()[ label ];

	REQUIRE( f->items().size() == 3 );

	for( int i = 0; i < 3; ++i )
	{
		REQUIRE( f->items().at( i )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph* > ( f->items().at( i ).data() );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );

		REQUIRE( t->text() == QStringLiteral( "Paragraph in footnote" ) );
	}
}

TEST_CASE( "046" )
{
	MD::Parser parser;

	auto doc = parser.parse( QStringLiteral( "tests/parser/data/046.md" ) );

	REQUIRE( !doc->isEmpty() );
	REQUIRE( doc->items().size() == 14 );

	int idx = 1;

	for( int i = 1; i < 4; ++i )
	{
		for( int j = 1; j < 3; ++j )
		{
			REQUIRE( doc->items().at( idx )->type() == MD::ItemType::Heading );

			auto h = static_cast< MD::Heading* > ( doc->items().at( idx ).data() );

			{
				REQUIRE( h->level() == j );
				REQUIRE( !h->text().isNull() );
				auto p = h->text().data();
				REQUIRE( p->items().size() == 1 );
				REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
				auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
				REQUIRE( t->text() == QString::fromLatin1( "Heading " ) + QString::number( j ) );
			}

			++idx;

			REQUIRE( doc->items().at( idx )->type() == MD::ItemType::Paragraph );

			auto p = static_cast< MD::Paragraph* > ( doc->items().at( idx ).data() );

			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			REQUIRE( static_cast< MD::Text* > ( p->items().at( 0 ).data() )->text() ==
				QString::fromLatin1( "Paragraph " ) + QString::number( j ) );

			++idx;
		}
	}

	REQUIRE( doc->items().at( idx )->type() == MD::ItemType::Heading );

	auto h = static_cast< MD::Heading* > ( doc->items().at( idx ).data() );

	REQUIRE( h->level() == 3 );
	REQUIRE( !h->text().isNull() );
	auto p = h->text().data();
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
	REQUIRE( t->text() == QStringLiteral( "Heading 3" ) );
	REQUIRE( h->isLabeled() );

	const QString wd = QDir().absolutePath() + QStringLiteral( "/tests/parser/data/" );
	const QString label = QString::fromLatin1( "#heading-3" ) + QStringLiteral( "/" ) +
		wd + QStringLiteral( "046.md" );

	REQUIRE( h->label() == label );

	REQUIRE( doc->labeledHeadings().size() == 3 );
	REQUIRE( doc->labeledHeadings().contains( label ) );
	REQUIRE( doc->labeledHeadings()[ label ].data() == h );
}

TEST_CASE( "047" )
{
	MD::Parser parser;

	auto doc = parser.parse( QStringLiteral( "tests/parser/data/047.md" ) );

	REQUIRE( !doc->isEmpty() );
	REQUIRE( doc->items().size() == 3 );

	for( int i = 1; i < 3; ++i )
	{
		REQUIRE( doc->items().at( i )->type() == MD::ItemType::Table );

		auto t = static_cast< MD::Table* > ( doc->items().at( i ).data() );

		REQUIRE( t->columnsCount() == 2 );
		REQUIRE( t->rows().size() == 2 );

		auto r0 = t->rows().at( 0 );

		REQUIRE( r0->type() == MD::ItemType::TableRow );

		REQUIRE( r0->cells().size() == 2 );

		{
			REQUIRE( r0->cells().at( 0 )->type() == MD::ItemType::TableCell );
			auto c0 = static_cast< MD::TableCell* > ( r0->cells().at( 0 ).data() );

			REQUIRE( c0->items().size() == 1 );
			REQUIRE( c0->items().at( 0 )->type() == MD::ItemType::Text );

			auto t0 = static_cast< MD::Text* > ( c0->items().at( 0 ).data() );

			REQUIRE( t0->text() == QStringLiteral( "Column 1" ) );
		}

		{
			auto c1 = static_cast< MD::TableCell* > ( r0->cells().at( 1 ).data() );

			REQUIRE( c1->items().size() == 1 );
			REQUIRE( c1->items().at( 0 )->type() == MD::ItemType::Text );

			auto t1 = static_cast< MD::Text* > ( c1->items().at( 0 ).data() );

			REQUIRE( t1->text() == QStringLiteral( "Column 2" ) );
		}

		auto r1 = t->rows().at( 1 );

		REQUIRE( r1->cells().size() == 2 );

		{
			auto c0 = static_cast< MD::TableCell* > ( r1->cells().at( 0 ).data() );

			REQUIRE( c0->items().size() == 1 );
			REQUIRE( c0->items().at( 0 )->type() == MD::ItemType::Text );

			auto t0 = static_cast< MD::Text* > ( c0->items().at( 0 ).data() );

			REQUIRE( t0->text() == QStringLiteral( "Cell 1" ) );
		}

		{
			auto c1 = static_cast< MD::TableCell* > ( r1->cells().at( 1 ).data() );

			REQUIRE( c1->items().size() == 1 );
			REQUIRE( c1->items().at( 0 )->type() == MD::ItemType::Text );

			auto t1 = static_cast< MD::Text* > ( c1->items().at( 0 ).data() );

			REQUIRE( t1->text() == QStringLiteral( "Cell 2" ) );
		}
	}

	auto table = static_cast< MD::Table* > ( doc->items().at( 1 ).data() );

	REQUIRE( table->columnAlignment( 0 ) == MD::Table::AlignLeft );
	REQUIRE( table->columnAlignment( 1 ) == MD::Table::AlignLeft );

	table = static_cast< MD::Table* > ( doc->items().at( 2 ).data() );

	REQUIRE( table->columnAlignment( 0 ) == MD::Table::AlignCenter );
	REQUIRE( table->columnAlignment( 1 ) == MD::Table::AlignRight );
}

TEST_CASE( "048" )
{
	MD::Parser parser;
	auto doc = parser.parse( QStringLiteral( "tests/parser/data/048.md" ) );
	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 4 );
	REQUIRE( doc->items().at( 0 )->type() == MD::ItemType::Anchor );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading* > ( doc->items().at( 1 ).data() );
		REQUIRE( !h->text().isNull() );
		auto p = h->text().data();
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
		REQUIRE( t->text() == QStringLiteral( "Heading" ) );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );
		auto * p = static_cast< MD::Paragraph* > ( doc->items().at( 2 ).data() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		REQUIRE( static_cast< MD::Text* > ( p->items().at( 0 ).data() )->text() ==
			QStringLiteral( "Paragraph" ) );
	}

	REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Heading );
	auto h = static_cast< MD::Heading* > ( doc->items().at( 1 ).data() );
	REQUIRE( !h->text().isNull() );
	auto p = h->text().data();
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
	REQUIRE( t->text() == QStringLiteral( "Heading" ) );
}

TEST_CASE( "049" )
{
	MD::Parser parser;
	auto doc = parser.parse( QStringLiteral( "tests/parser/data/049.md" ) );
	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 0 )->type() == MD::ItemType::Anchor );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml* > ( doc->items().at( 1 ).data() );
	REQUIRE( h->text() == QStringLiteral( "<!-- --> 1<!-- -->" ) );
}

TEST_CASE( "050" )
{
	MD::Parser parser;
	auto doc = parser.parse( QStringLiteral( "tests/parser/data/050.md" ) );
	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );
	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto * p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 4 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
	auto l0 = static_cast< MD::Link* > ( p->items().at( 0 ).data() );
	REQUIRE( l0->text() == QStringLiteral( "a]" ) );

	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Link );
	auto l1 = static_cast< MD::Link* > ( p->items().at( 1 ).data() );
	REQUIRE( l1->text() == QStringLiteral( "b\\" ) );

	REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Link );
	auto l2 = static_cast< MD::Link* > ( p->items().at( 2 ).data() );
	REQUIRE( l2->text() == QStringLiteral( "c-d" ) );

	REQUIRE( p->items().at( 3 )->type() == MD::ItemType::Link );
	auto l3 = static_cast< MD::Link* > ( p->items().at( 3 ).data() );
	REQUIRE( l3->text() == QStringLiteral( "\\" ) );
}

TEST_CASE( "051" )
{
	MD::Parser parser;
	auto doc = parser.parse( QStringLiteral( "tests/parser/data/051.md" ) );

	const QString wd = QDir().absolutePath() + QStringLiteral( "/tests/parser/data" );

	REQUIRE( doc->items().size() == 8 );

	REQUIRE( static_cast< MD::Anchor* > ( doc->items().at( 0 ).data() )->label() ==
		wd + QStringLiteral( "/051.md" ) );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::PageBreak );

	REQUIRE( static_cast< MD::Anchor* > ( doc->items().at( 3 ).data() )->label() ==
		wd + QStringLiteral( "/051-1.md" ) );

	REQUIRE( doc->items().at( 4 )->type() == MD::ItemType::Paragraph );
	REQUIRE( doc->items().at( 5 )->type() == MD::ItemType::PageBreak );

	REQUIRE( static_cast< MD::Anchor* > ( doc->items().at( 6 ).data() )->label() ==
		wd + QStringLiteral( "/051-2.md" ) );

	REQUIRE( doc->items().at( 7 )->type() == MD::ItemType::Paragraph );
}

TEST_CASE( "052" )
{
	MD::Parser parser;

	auto doc = parser.parse( QStringLiteral( "tests/parser/data/052.md" ) );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );

		auto c = static_cast< MD::Code* > ( doc->items().at( 1 ).data() );

		REQUIRE( c->isInlined() == false );
		REQUIRE( c->text() == QStringLiteral( "_```` *bold* _italic" ) );
		REQUIRE( c->syntax() == QStringLiteral( "code" ) );
	}
}

TEST_CASE( "053" )
{
	MD::Parser parser;

	auto doc = parser.parse( QStringLiteral( "tests/parser/data/053.md" ) );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code* > ( doc->items().at( 1 ).data() );
		REQUIRE( !c->isInlined() );
		REQUIRE( c->text().isEmpty() );
	}
}

TEST_CASE( "054" )
{
	MD::Parser parser;

	const auto doc = parser.parse( QStringLiteral( "tests/parser/data/054.md" ) );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code* > ( doc->items().at( 1 ).data() );
		REQUIRE( !c->isInlined() );
		REQUIRE( c->text().isEmpty() );
	}
}

TEST_CASE( "055" )
{
	MD::Parser parser;

	auto doc = parser.parse( QStringLiteral( "tests/parser/data/055.md" ), false );

	REQUIRE( doc->type() == MD::ItemType::Document );

	REQUIRE( !doc->isEmpty() );
	REQUIRE( doc->items().size() == 2 );
	REQUIRE( doc->items().at( 0 )->type() == MD::ItemType::Anchor );

	REQUIRE( doc->footnotesMap().size() == 1 );

	const QString wd = QDir().absolutePath() + QStringLiteral( "/tests/parser/data/" );

	const QString label = QString::fromLatin1( "#^footnote" ) + QStringLiteral( "/" ) + wd +
		QStringLiteral( "055.md" );

	REQUIRE( doc->footnotesMap().contains( label ) );

	auto f = doc->footnotesMap()[ label ];

	REQUIRE( f->type() == MD::ItemType::Footnote );

	REQUIRE( f->items().size() == 3 );

	for( int i = 0; i < 3; ++i )
	{
		REQUIRE( f->items().at( i )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph* > ( f->items().at( i ).data() );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );

		REQUIRE( t->text() == QStringLiteral( "Paragraph in footnote" ) );
	}

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );

	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

	auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );

	REQUIRE( t->text() == QStringLiteral( "Text" ) );
}

TEST_CASE( "056" )
{
	MD::Parser parser;

	auto doc = parser.parse( QStringLiteral( "tests/parser/data/056.md" ) );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );

	auto c = static_cast< MD::Code* > ( doc->items().at( 1 ).data() );

	REQUIRE( c->isInlined() == false );
	REQUIRE( c->text() ==
		QStringLiteral( "if( a > b )\n\n  do_something();\n\nelse\n\n  dont_do_anything();" ) );
}

TEST_CASE( "057" )
{
	MD::Parser parser;

	auto doc = parser.parse( QStringLiteral( "tests/parser/data/057.md" ) );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );

	auto c = static_cast< MD::Code* > ( doc->items().at( 1 ).data() );

	REQUIRE( c->isInlined() == false );
	REQUIRE( c->text() == QStringLiteral( "code\n\ncode" ) );
}

TEST_CASE( "058" )
{
	MD::Parser parser;

	auto doc = parser.parse( QStringLiteral( "tests/parser/data/058.md" ) );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List* > ( doc->items().at( 1 ).data() );

	REQUIRE( l->items().size() == 2 );

	REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
	REQUIRE( l->items().at( 1 )->type() == MD::ItemType::ListItem );

	auto li = static_cast< MD::ListItem* > ( l->items().at( 1 ).data() );

	REQUIRE( li->items().size() == 2 );
	REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
	REQUIRE( li->items().at( 1 )->type() == MD::ItemType::Paragraph );
}

TEST_CASE( "059" )
{
	MD::Parser parser;

	auto doc = parser.parse( QStringLiteral( "tests/parser/data/059.md" ) );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );

	auto b = static_cast< MD::Blockquote* > ( doc->items().at( 1 ).data() );

	REQUIRE( b->items().size() == 4 );

	REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Heading );
	REQUIRE( b->items().at( 1 )->type() == MD::ItemType::List );
	REQUIRE( b->items().at( 2 )->type() == MD::ItemType::Paragraph );
	REQUIRE( b->items().at( 3 )->type() == MD::ItemType::Code );
}

TEST_CASE( "060" )
{
	MD::Parser parser;

	auto doc = parser.parse( QStringLiteral( "tests/parser/data/060.md" ) );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List* > ( doc->items().at( 1 ).data() );

	REQUIRE( l->items().size() == 1 );

	REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );

	auto li = static_cast< MD::ListItem* > ( l->items().at( 0 ).data() );

	REQUIRE( li->items().size() == 2 );

	REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
	REQUIRE( li->items().at( 1 )->type() == MD::ItemType::List );
}

TEST_CASE( "061" )
{
	MD::Parser parser;

	auto doc = parser.parse( QStringLiteral( "tests/parser/data/061.md" ) );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List* > ( doc->items().at( 1 ).data() );

	REQUIRE( l->items().size() == 1 );

	REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );

	auto li = static_cast< MD::ListItem* > ( l->items().at( 0 ).data() );

	REQUIRE( li->items().size() == 2 );

	REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph* > ( li->items().at( 0 ).data() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == QStringLiteral( "A list item with a code block:" ) );

	REQUIRE( li->items().at( 1 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code* > ( li->items().at( 1 ).data() );
	REQUIRE( c->isInlined() == false );
	REQUIRE( c->syntax().isEmpty() );
	REQUIRE( c->text() == QStringLiteral( "<code goes here>" ) );
}

TEST_CASE( "062" )
{
	MD::Parser parser;

	auto doc = parser.parse( QStringLiteral( "tests/parser/data/062.md" ) );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 5 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );

		REQUIRE( p->items().size() == 2 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t1 = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
		REQUIRE( t1->text() == QStringLiteral( "*" ) );

		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Code );
		auto c1 = static_cast< MD::Code* > ( p->items().at( 1 ).data() );
		REQUIRE( c1->text() == QStringLiteral( "**" ) );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph* > ( doc->items().at( 2 ).data() );

		REQUIRE( p->items().size() == 2 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t1 = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
		REQUIRE( t1->text() == QStringLiteral( "*" ) );

		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Code );
		auto c1 = static_cast< MD::Code* > ( p->items().at( 1 ).data() );
		REQUIRE( c1->text() == QStringLiteral( "**" ) );
	}

	{
		REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph* > ( doc->items().at( 3 ).data() );

		REQUIRE( p->items().size() == 2 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t1 = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
		REQUIRE( t1->text() == QStringLiteral( "*" ) );

		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Code );
		auto c1 = static_cast< MD::Code* > ( p->items().at( 1 ).data() );
		REQUIRE( c1->text() == QStringLiteral( "**`**" ) );
	}

	{
		REQUIRE( doc->items().at( 4 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph* > ( doc->items().at( 4 ).data() );

		REQUIRE( p->items().size() == 2 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Code );
		auto c1 = static_cast< MD::Code* > ( p->items().at( 0 ).data() );
		REQUIRE( c1->text() == QStringLiteral( "*" ) );

		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
		auto t1 = static_cast< MD::Text* > ( p->items().at( 1 ).data() );
		REQUIRE( t1->text() == QStringLiteral( "`*" ) );
	}
}

TEST_CASE( "063" )
{
	MD::Parser parser;

	auto doc = parser.parse( QStringLiteral( "tests/parser/data/063.md" ) );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 2 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == ( MD::ItalicText | MD::BoldText ) );
		REQUIRE( t->text() == QStringLiteral( "Lorem." ) );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text* > ( p->items().at( 1 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "Aenean" ) );
	}
}

TEST_CASE( "064" )
{
	MD::Parser parser;

	auto doc = parser.parse( QStringLiteral( "tests/parser/data/064.md" ) );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == QStringLiteral( "\\`" ) );
}

TEST_CASE( "065" )
{
	MD::Parser parser;

	auto doc = parser.parse( QStringLiteral( "tests/parser/data/065.md" ) );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 6 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "When" ) );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Math );
		auto m = static_cast< MD::Math* > ( p->items().at( 1 ).data() );
		REQUIRE( m->expr() == QStringLiteral( "a \\ne 0" ) );
		REQUIRE( m->isInline() );
	}

	{
		REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text* > ( p->items().at( 2 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( ", there are two solutions to" ) );
	}

	{
		REQUIRE( p->items().at( 3 )->type() == MD::ItemType::Math );
		auto m = static_cast< MD::Math* > ( p->items().at( 3 ).data() );
		REQUIRE( m->expr() == QStringLiteral( "(ax^2 + bx + c = 0)" ) );
		REQUIRE( m->isInline() );
	}

	{
		REQUIRE( p->items().at( 4 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text* > ( p->items().at( 4 ).data() );
		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "and they are" ) );
	}

	{
		REQUIRE( p->items().at( 5 )->type() == MD::ItemType::Math );
		auto m = static_cast< MD::Math* > ( p->items().at( 5 ).data() );
		REQUIRE( m->expr() == QStringLiteral( " x = {-b \\pm \\sqrt{b^2-4ac} \\over 2a} " ) );
		REQUIRE( !m->isInline() );
	}
}

TEST_CASE( "066" )
{
	MD::Parser parser;

	auto doc = parser.parse( QStringLiteral( "tests/parser/data/066.md" ) );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Math );
	auto m = static_cast< MD::Math* > ( p->items().at( 0 ).data() );
	REQUIRE( m->expr() == QStringLiteral( "x = {-b \\pm \\sqrt{b^2-4ac} \\over 2a}" ) );
	REQUIRE( !m->isInline() );
}

TEST_CASE( "067" )
{
	MD::Parser parser;

	auto doc = parser.parse( QStringLiteral( "tests/parser/data/067.md" ) );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph* > ( doc->items().at( 1 ).data() );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );

		REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "Paragraph" ) );
	}

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List* > ( doc->items().at( 2 ).data() );

	REQUIRE( l->items().size() == 1 );

	REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );

	auto item = static_cast< MD::ListItem* > ( l->items().at( 0 ).data() );

	REQUIRE( item->listType() == MD::ListItem::Unordered );

	REQUIRE( item->items().size() == 2 );

	{
		REQUIRE( item->items().at( 0 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph* > ( item->items().at( 0 ).data() );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text* > ( p->items().at( 0 ).data() );

		REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( t->text() == QStringLiteral( "List item" ) );
	}

	{
		REQUIRE( item->items().at( 1 )->type() == MD::ItemType::Code );

		auto c = static_cast< MD::Code* > ( item->items().at( 1 ).data() );

		REQUIRE( c->text() == QStringLiteral( "Code" ) );
	}
}
