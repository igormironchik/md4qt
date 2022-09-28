
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

// C++ Include.
#include <vector>

using data_t = std::vector< std::pair< qsizetype, int > >;

TEST_CASE( "emphasis_sequence" )
{
	{
		const data_t d = { { 2, 0 }, { 2, 2 }, { 1, 1 }, { -1, 1 }, { 2, 2 }, { -2, 0 } };

		bool closed = false;
		size_t idx = 0;
		std::tie( closed, idx ) = MD::checkEmphasisSequence( d, 0 );

		REQUIRE( closed );
		REQUIRE( idx == 5 );

		std::tie( closed, idx ) = MD::checkEmphasisSequence( d, 1 );

		REQUIRE( !closed );

		std::tie( closed, idx ) = MD::checkEmphasisSequence( d, 2 );

		REQUIRE( !closed );
	}

	{
		const data_t d = { { 2, 0 }, { 2, 2 }, { 1, 1 }, { 1, 1 }, { -2, 2 }, { -2, 0 } };

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
		const data_t d = { { 2, 0 }, { 2, 1 }, { 1, 2 }, { -1, 2 }, { -2, 1 }, { -2, 0 } };

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
		const data_t d = { { 2, 0 }, { 2, 1 }, { 1, 2 }, { -2, 0 }, { -1, 2 }, { -2, 1 } };

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
		const data_t d = { { 2, 0 }, { 2, 1 }, { -2, 1 }, { 1, 2 }, { -2, 0 }, { -1, 2 } };

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
		const data_t d = { { 1, 1 }, { 1, 1 }, { -2, 1 } };

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
		const data_t d = { { 2, 1 }, { -1, 1 }, { -1, 1 } };

		bool closed = false;
		size_t idx = 0;
		std::tie( closed, idx ) = MD::checkEmphasisSequence( d, 0 );

		REQUIRE( closed );
		REQUIRE( idx == 2 );
	}

	{
		const data_t d = { { 2, 1 } };

		bool closed = false;
		size_t idx = 0;
		std::tie( closed, idx ) = MD::checkEmphasisSequence( d, 0 );

		REQUIRE( !closed );
	}

	{
		const data_t d = { { 2, 1 }, { 1, 0 }, { 2, 1 }, { -4, 1 } };

		bool closed = false;
		size_t idx = 0;
		std::tie( closed, idx ) = MD::checkEmphasisSequence( d, 0 );

		REQUIRE( closed );
		REQUIRE( idx == 3 );
	}

	{
		const data_t d = { { 4, 1 }, { 1, 0 }, { -2, 1 }, { -2, 1 } };

		bool closed = false;
		size_t idx = 0;
		std::tie( closed, idx ) = MD::checkEmphasisSequence( d, 0 );

		REQUIRE( closed );
		REQUIRE( idx == 3 );
	}
}

TEST_CASE( "is_footnote" )
{
	REQUIRE( !MD::isFootnote( QStringLiteral( "[^]:" ) ) );
	REQUIRE( !MD::isFootnote( QStringLiteral( "[^ a]:" ) ) );
	REQUIRE( !MD::isFootnote( QStringLiteral( "[^  a]:" ) ) );
	REQUIRE( !MD::isFootnote( QStringLiteral( "[^ a a]:" ) ) );
	REQUIRE( !MD::isFootnote( QStringLiteral( "[^a a]:" ) ) );
}

TEST_CASE( "is_code_fences" )
{
	REQUIRE( !MD::isCodeFences( QStringLiteral( "    ~~~" ) ) );
	REQUIRE( !MD::isCodeFences( QStringLiteral( "aaa" ) ) );
	REQUIRE( !MD::isCodeFences( QStringLiteral( "~~" ) ) );
}

TEST_CASE( "is_start_of_code" )
{
	REQUIRE( !MD::isStartOfCode( QStringLiteral( "~~" ) ) );
	REQUIRE( !MD::isStartOfCode( QStringLiteral( "~~`" ) ) );
}

TEST_CASE( "is_horizontal_line" )
{
	REQUIRE( MD::isHorizontalLine( QStringLiteral( "---   " ) ) );
	REQUIRE( !MD::isHorizontalLine( QStringLiteral( "---   =" ) ) );
}

TEST_CASE( "is_column_alignment" )
{
	REQUIRE( !MD::isColumnAlignment( QStringLiteral( "a" ) ) );
	REQUIRE( MD::isColumnAlignment( QStringLiteral( ":-" ) ) );
	REQUIRE( !MD::isColumnAlignment( QStringLiteral( ":---a" ) ) );
	REQUIRE( !MD::isColumnAlignment( QStringLiteral( ":--- a" ) ) );
}

TEST_CASE( "is_table_alignmnet" )
{
	REQUIRE( !MD::isTableAlignment( QStringLiteral( "|aaa|bbb|" ) ) );
}

TEST_CASE( "is_html_comment" )
{
	REQUIRE( MD::isHtmlComment( QStringLiteral( "<!-- -->" ) ) );
	REQUIRE( !MD::isHtmlComment( QStringLiteral( "<-- -->" ) ) );
	REQUIRE( !MD::isHtmlComment( QStringLiteral( "<!-->" ) ) );
	REQUIRE( !MD::isHtmlComment( QStringLiteral( "<!--->" ) ) );
	REQUIRE( !MD::isHtmlComment( QStringLiteral( "<!-- --" ) ) );
	REQUIRE( !MD::isHtmlComment( QStringLiteral( "<!-- -" ) ) );
}
