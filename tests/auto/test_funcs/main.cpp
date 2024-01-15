
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2022-2024 Igor Mironchik

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

// C++ include.
#include <vector>

using data_t = std::vector< std::pair< std::pair< long long int, bool >, int > >;

TEST_CASE( "emphasis_sequence" )
{
	{
		const data_t d = { { { 2, false }, 1 }, { { 1, false }, 1 }, { { -2, false }, 1 } };

		bool closed = false;
		size_t idx = 0;
		std::tie( closed, idx ) = MD::checkEmphasisSequence( d, 0 );

		REQUIRE( !closed );

		std::tie( closed, idx ) = MD::checkEmphasisSequence( d, 1 );

		REQUIRE( closed );
		REQUIRE( idx == 2 );
	}

	{
		const data_t d = { { { 2, false }, 1 }, { { 2, false }, 1 }, { { -4, false }, 1 } };

		bool closed = false;
		size_t idx = 0;
		std::tie( closed, idx ) = MD::checkEmphasisSequence( d, 0 );

		REQUIRE( closed );
		REQUIRE( idx == 2 );

		std::tie( closed, idx ) = MD::checkEmphasisSequence( d, 1 );

		REQUIRE( closed );
		REQUIRE( idx == 2 );
	}

	{
		const data_t d = { { { 2, false }, 1 }, { { 2, false }, 1 }, { { 1, false }, 1 },
			{ { -4, false }, 1 } };

		bool closed = false;
		size_t idx = 0;
		std::tie( closed, idx ) = MD::checkEmphasisSequence( d, 0 );

		REQUIRE( !closed );

		std::tie( closed, idx ) = MD::checkEmphasisSequence( d, 1 );

		REQUIRE( closed );
		REQUIRE( idx == 3 );

		std::tie( closed, idx ) = MD::checkEmphasisSequence( d, 2 );

		REQUIRE( closed );
		REQUIRE( idx == 3 );
	}

	{
		const data_t d = { { { 2, false }, 1 }, { { 2, false }, 1 }, { { 1, false }, 1 },
			{ { -5, false }, 1 } };

		bool closed = false;
		size_t idx = 0;
		std::tie( closed, idx ) = MD::checkEmphasisSequence( d, 0 );

		REQUIRE( closed );
		REQUIRE( idx == 3 );

		std::tie( closed, idx ) = MD::checkEmphasisSequence( d, 1 );

		REQUIRE( closed );
		REQUIRE( idx == 3 );

		std::tie( closed, idx ) = MD::checkEmphasisSequence( d, 2 );

		REQUIRE( closed );
		REQUIRE( idx == 3 );
	}

	{
		const data_t d = { { { 2, false }, 0 }, { { 2, false }, 2 }, { { 1, false }, 1 },
			{ { -1, false }, 1 }, { { 2, false }, 2 }, { { -2, false }, 0 } };

		bool closed = false;
		size_t idx = 0;
		std::tie( closed, idx ) = MD::checkEmphasisSequence( d, 0 );

		REQUIRE( closed );
		REQUIRE( idx == 5 );

		std::tie( closed, idx ) = MD::checkEmphasisSequence( d, 1 );

		REQUIRE( !closed );

		std::tie( closed, idx ) = MD::checkEmphasisSequence( d, 2 );

		REQUIRE( closed );
		REQUIRE( idx == 3 );
	}

	{
		const data_t d = { { { 2, false }, 0 }, { { 2, false }, 2 }, { { 1, false }, 1 },
			{ { 1, false }, 1 }, { { -2, false }, 2 }, { { -2, false }, 0 } };

		bool closed = false;
		size_t idx = 0;
		std::tie( closed, idx ) = MD::checkEmphasisSequence( d, 0 );

		REQUIRE( closed );
		REQUIRE( idx == 5 );

		std::tie( closed, idx ) = MD::checkEmphasisSequence( d, 1 );

		REQUIRE( closed );
		REQUIRE( idx == 4 );

		std::tie( closed, idx ) = MD::checkEmphasisSequence( d, 2 );

		REQUIRE( !closed );
	}

	{
		const data_t d = { { { 2, false }, 0 }, { { 2, false }, 1 }, { { 1, false }, 2 },
			{ { -1, false }, 2 }, { { -2, false }, 1 }, { { -2, false }, 0 } };

		bool closed = false;
		size_t idx = 0;
		std::tie( closed, idx ) = MD::checkEmphasisSequence( d, 0 );

		REQUIRE( closed );
		REQUIRE( idx == 5 );

		std::tie( closed, idx ) = MD::checkEmphasisSequence( d, 1 );

		REQUIRE( closed );
		REQUIRE( idx == 4 );

		std::tie( closed, idx ) = MD::checkEmphasisSequence( d, 2 );

		REQUIRE( closed );
		REQUIRE( idx == 3 );
	}

	{
		const data_t d = { { { 2, false }, 0 }, { { 2, false }, 1 }, { { 1, false }, 2 },
			{ { -2, false }, 0 }, { { -1, false }, 2 }, { { -2, false }, 1 } };

		bool closed = false;
		size_t idx = 0;
		std::tie( closed, idx ) = MD::checkEmphasisSequence( d, 0 );

		REQUIRE( closed );
		REQUIRE( idx == 3 );

		std::tie( closed, idx ) = MD::checkEmphasisSequence( d, 1 );

		REQUIRE( !closed );

		std::tie( closed, idx ) = MD::checkEmphasisSequence( d, 2 );

		REQUIRE( !closed );
	}

	{
		const data_t d = { { { 1, false }, 0 }, { { 2, false }, 0 }, { { -2, false }, 0 },
			{ { -1, false }, 0 } };

		bool closed = false;
		size_t idx = 0;
		std::tie( closed, idx ) = MD::checkEmphasisSequence( d, 0 );

		REQUIRE( closed );
		REQUIRE( idx == 3 );

		std::tie( closed, idx ) = MD::checkEmphasisSequence( d, 1 );

		REQUIRE( closed );
		REQUIRE( idx == 2 );
	}

	{
		const data_t d = { { { 2, false }, 0 }, { { 2, false }, 1 }, { { -2, false }, 1 },
			{ { 1, false }, 2 }, { { -2, false }, 0 }, { { -1, false }, 2 } };

		bool closed = false;
		size_t idx = 0;
		std::tie( closed, idx ) = MD::checkEmphasisSequence( d, 0 );

		REQUIRE( closed );
		REQUIRE( idx == 4 );

		std::tie( closed, idx ) = MD::checkEmphasisSequence( d, 1 );

		REQUIRE( closed );
		REQUIRE( idx == 2 );

		std::tie( closed, idx ) = MD::checkEmphasisSequence( d, 2 );

		REQUIRE( !closed );
	}

	{
		const data_t d = { { { 1, false }, 1 }, { { 1, false }, 1 }, { { -2, false }, 1 } };

		bool closed = false;
		size_t idx = 0;
		std::tie( closed, idx ) = MD::checkEmphasisSequence( d, 0 );

		REQUIRE( closed );
		REQUIRE( idx == 2 );

		std::tie( closed, idx ) = MD::checkEmphasisSequence( d, 1 );

		REQUIRE( closed );
		REQUIRE( idx == 2 );
	}

	{
		const data_t d = { { { 2, false }, 1 }, { { -1, false }, 1 }, { { -1, false }, 1 } };

		bool closed = false;
		size_t idx = 0;
		std::tie( closed, idx ) = MD::checkEmphasisSequence( d, 0 );

		REQUIRE( closed );
		REQUIRE( idx == 2 );
	}

	{
		const data_t d = { { { 2, false }, 1 } };

		bool closed = false;
		size_t idx = 0;
		std::tie( closed, idx ) = MD::checkEmphasisSequence( d, 0 );

		REQUIRE( !closed );
	}

	{
		const data_t d = { { { 2, false }, 1 }, { { 1, false }, 0 }, { { 2, false }, 1 },
			{ { -4, false }, 1 } };

		bool closed = false;
		size_t idx = 0;
		std::tie( closed, idx ) = MD::checkEmphasisSequence( d, 0 );

		REQUIRE( closed );
		REQUIRE( idx == 3 );
	}

	{
		const data_t d = { { { 4, false }, 1 }, { { 1, false }, 0 }, { { -2, false }, 1 },
			{ { -2, false }, 1 } };

		bool closed = false;
		size_t idx = 0;
		std::tie( closed, idx ) = MD::checkEmphasisSequence( d, 0 );

		REQUIRE( closed );
		REQUIRE( idx == 3 );
	}

	{
		const data_t d = { { { 1, false }, 1 }, { { -2, true }, 1 }, { { -1, false }, 1 } };

		bool closed = false;
		size_t idx = 0;
		std::tie( closed, idx ) = MD::checkEmphasisSequence( d, 0 );

		REQUIRE( closed );
		REQUIRE( idx == 2 );
	}
}

TEST_CASE( "is_footnote" )
{
	REQUIRE( !MD::isFootnote< TRAIT >( u8"[^]:" ) );
	REQUIRE( !MD::isFootnote< TRAIT >( u8"[^ a]:" ) );
	REQUIRE( !MD::isFootnote< TRAIT >( u8"[^  a]:" ) );
	REQUIRE( !MD::isFootnote< TRAIT >( u8"[^ a a]:" ) );
	REQUIRE( !MD::isFootnote< TRAIT >( u8"[^a a]:" ) );
}

TEST_CASE( "is_code_fences" )
{
	REQUIRE( !MD::isCodeFences< TRAIT >( u8"    ~~~" ) );
	REQUIRE( !MD::isCodeFences< TRAIT >( u8"aaa" ) );
	REQUIRE( !MD::isCodeFences< TRAIT >( u8"~~" ) );
}

TEST_CASE( "is_start_of_code" )
{
	REQUIRE( !MD::isStartOfCode< TRAIT >( u8"~~" ) );
	REQUIRE( !MD::isStartOfCode< TRAIT >( u8"~~`" ) );
}

TEST_CASE( "is_horizontal_line" )
{
	REQUIRE( MD::isHorizontalLine< TRAIT >( u8"---   " ) );
	REQUIRE( !MD::isHorizontalLine< TRAIT >( u8"---   =" ) );
}

TEST_CASE( "is_column_alignment" )
{
	REQUIRE( !MD::isColumnAlignment< TRAIT >( u8"a" ) );
	REQUIRE( MD::isColumnAlignment< TRAIT >( u8":-" ) );
	REQUIRE( !MD::isColumnAlignment< TRAIT >( u8":---a" ) );
	REQUIRE( !MD::isColumnAlignment< TRAIT >( u8":--- a" ) );
}

TEST_CASE( "is_table_alignmnet" )
{
	REQUIRE( !MD::isTableAlignment< TRAIT >( u8"|aaa|bbb|" ) );
}

TEST_CASE( "is_html_comment" )
{
	REQUIRE( MD::isHtmlComment< TRAIT >( u8"<!-- -->", false ) );
	REQUIRE( MD::isHtmlComment< TRAIT >( u8"<!-- -- -->", true ) );
	REQUIRE( MD::isHtmlComment< TRAIT >( u8"<!--My favorite operators are > and <!-->", false ) );
	REQUIRE( MD::isHtmlComment< TRAIT >( u8"<!--My favorite operators are > and <!-->", true ) );
	REQUIRE( !MD::isHtmlComment< TRAIT >( u8"<!-- -- -->", false ) );
	REQUIRE( !MD::isHtmlComment< TRAIT >( u8"<-- -->", false ) );
	REQUIRE( !MD::isHtmlComment< TRAIT >( u8"<!-->", false ) );
	REQUIRE( !MD::isHtmlComment< TRAIT >( u8"<!--->", false ) );
	REQUIRE( !MD::isHtmlComment< TRAIT >( u8"<!-- --", false ) );
	REQUIRE( !MD::isHtmlComment< TRAIT >( u8"<!-- -", false ) );
}

TEST_CASE( "test_column_alignment" )
{
	MD::Table< TRAIT > t;

	t.setColumnAlignment( 0, MD::Table< TRAIT >::AlignLeft );

	REQUIRE( t.columnAlignment( 0 ) == MD::Table< TRAIT >::AlignLeft );

	t.setColumnAlignment( 0, MD::Table< TRAIT >::AlignRight );

	REQUIRE( t.columnAlignment( 0 ) == MD::Table< TRAIT >::AlignRight );
}

TEST_CASE( "paragraph_to_label" )
{
	{
		MD::Paragraph< TRAIT > p;
		auto c = std::make_shared< MD::Code< TRAIT > > ( u8"ICU", false, true );
		c->setStartColumn( 0 );
		c->setStartLine( 0 );
		c->setEndColumn( 3 );
		c->setEndLine( 0 );
		p.appendItem( c );
		auto t = std::make_shared< MD::Text< TRAIT > > ();
		t->setText( u8"?" );
		t->setStartColumn( 5 );
		t->setStartLine( 0 );
		t->setEndColumn( 5 );
		t->setEndLine( 0 );
		t->setSpaceBefore( false );
		t->setSpaceAfter( true );
		p.appendItem( t );

		REQUIRE( MD::paragraphToLabel( &p ) == u8"icu" );
	}

	{
		MD::Paragraph< TRAIT > p;
		auto c = std::make_shared< MD::Code< TRAIT > > ( u8"ICU", false, true );
		c->setStartColumn( 0 );
		c->setStartLine( 0 );
		c->setEndColumn( 3 );
		c->setEndLine( 0 );
		p.appendItem( c );

		{
			auto t = std::make_shared< MD::Text< TRAIT > > ();
			t->setText( u8"," );
			t->setStartColumn( 5 );
			t->setStartLine( 0 );
			t->setEndColumn( 5 );
			t->setEndLine( 0 );
			t->setSpaceBefore( false );
			t->setSpaceAfter( true );
			p.appendItem( t );
		}

		auto t = std::make_shared< MD::Text< TRAIT > > ();
		t->setText( u8"text" );
		t->setStartColumn( 0 );
		t->setStartLine( 1 );
		t->setEndColumn( 3 );
		t->setEndLine( 1 );
		t->setSpaceBefore( true );
		t->setSpaceAfter( true );
		p.appendItem( t );

		REQUIRE( MD::paragraphToLabel( &p ) == u8"icutext" );
	}

	{
		MD::Paragraph< TRAIT > p;
		auto c = std::make_shared< MD::Code< TRAIT > > ( u8"text", false, true );
		c->setStartColumn( 0 );
		c->setStartLine( 0 );
		c->setEndColumn( 3 );
		c->setEndLine( 0 );
		p.appendItem( c );

		{
			auto t = std::make_shared< MD::Text< TRAIT > > ();
			t->setText( u8"? text?" );
			t->setStartColumn( 5 );
			t->setStartLine( 0 );
			t->setEndColumn( 11 );
			t->setEndLine( 0 );
			t->setSpaceBefore( false );
			t->setSpaceAfter( true );
			p.appendItem( t );
		}

		REQUIRE( MD::paragraphToLabel( &p ) == u8"text-text" );
	}
}

TEST_CASE( "replace_tabs" )
{
	typename TRAIT::InternalString s1( typename TRAIT::String( u8"-\ttext" ) );
	typename TRAIT::InternalString s2( typename TRAIT::String( u8" >\ttext" ) );
	typename TRAIT::InternalString s3( typename TRAIT::String( u8"> >\ttext" ) );
	typename TRAIT::InternalString s4( typename TRAIT::String( u8"\ttext" ) );

	MD::replaceTabs< TRAIT > ( s1 );
	MD::replaceTabs< TRAIT > ( s2 );
	MD::replaceTabs< TRAIT > ( s3 );
	MD::replaceTabs< TRAIT > ( s4 );

	REQUIRE( s1.asString() == u8"-   text" );
	REQUIRE( s2.asString() == u8" >  text" );
	REQUIRE( s3.asString() == u8"> > text" );
	REQUIRE( s4.asString() == u8"    text" );

	typename TRAIT::InternalString s5( typename TRAIT::String( u8"\t-\ttext" ) );
	typename TRAIT::InternalString s6( typename TRAIT::String( u8"\t >\ttext" ) );
	typename TRAIT::InternalString s7( typename TRAIT::String( u8"\t> >\ttext" ) );
	typename TRAIT::InternalString s8( typename TRAIT::String( u8"\t\ttext" ) );

	MD::replaceTabs< TRAIT > ( s5 );
	MD::replaceTabs< TRAIT > ( s6 );
	MD::replaceTabs< TRAIT > ( s7 );
	MD::replaceTabs< TRAIT > ( s8 );

	REQUIRE( s5.asString() == u8"    -   text" );
	REQUIRE( s6.asString() == u8"     >  text" );
	REQUIRE( s7.asString() == u8"    > > text" );
	REQUIRE( s8.asString() == u8"        text" );

}
