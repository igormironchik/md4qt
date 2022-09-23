
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2019 Igor Mironchik

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
