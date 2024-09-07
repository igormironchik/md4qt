
/*
	SPDX-FileCopyrightText: 2022-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

#include <md4qt/traits.hpp>
#include <md4qt/parser.hpp>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
// doctest include.
#include <doctest/doctest.h>


TEST_CASE( "replace_remove_1" )
{
	TRAIT::InternalString s( "abcde" );

	REQUIRE( s.virginPos( 0 ) == 0 );
	REQUIRE( s.virginPos( 1 ) == 1 );
	REQUIRE( s.virginPos( 2 ) == 2 );
	REQUIRE( s.virginPos( 3 ) == 3 );
	REQUIRE( s.virginPos( 4 ) == 4 );
	REQUIRE( s.virginPos( 5 ) == 5 );

	s.replace( "b", "bb" );

	REQUIRE( s.asString() == "abbcde" );

	REQUIRE( s.virginPos( 0 ) == 0 );
	REQUIRE( s.virginPos( 1 ) == 1 );
	REQUIRE( s.virginPos( 2 ) == 1 );
	REQUIRE( s.virginPos( 3 ) == 2 );
	REQUIRE( s.virginPos( 4 ) == 3 );
	REQUIRE( s.virginPos( 5 ) == 4 );
	REQUIRE( s.virginPos( 6 ) == 5 );

	s.replace( "bb", "b" );

	REQUIRE( s.asString() == "abcde" );

	REQUIRE( s.virginPos( 0 ) == 0 );
	REQUIRE( s.virginPos( 1 ) == 1 );
	REQUIRE( s.virginPos( 2 ) == 2 );
	REQUIRE( s.virginPos( 3 ) == 3 );
	REQUIRE( s.virginPos( 4 ) == 4 );

	s.replace( "b", "" );

	REQUIRE( s.asString() == "acde" );

	REQUIRE( s.virginPos( 0 ) == 0 );
	REQUIRE( s.virginPos( 1 ) == 2 );
	REQUIRE( s.virginPos( 2 ) == 3 );
	REQUIRE( s.virginPos( 3 ) == 4 );

	s.remove( 0, 1 );

	REQUIRE( s.asString() == "cde" );

	REQUIRE( s.virginPos( 0 ) == 2 );
	REQUIRE( s.virginPos( 1 ) == 3 );
	REQUIRE( s.virginPos( 2 ) == 4 );
}

TEST_CASE( "replace_remove_2" )
{
	TRAIT::InternalString s( "xxxxxx" );

	REQUIRE( s.virginPos( 1 ) == 1 );

	s.replace( "xx", "x" );

	REQUIRE( s.asString() == "xxx" );

	REQUIRE( s.virginPos( 0 ) == 0 );
	REQUIRE( s.virginPos( 1 ) == 2 );
	REQUIRE( s.virginPos( 2 ) == 4 );

	s.remove( 1, 1 );

	REQUIRE( s.asString() == "xx" );

	REQUIRE( s.virginPos( 0 ) == 0 );
	REQUIRE( s.virginPos( 1 ) == 4 );

	s.remove( 0, 1 );

	REQUIRE( s.asString() == "x" );

	REQUIRE( s.virginPos( 0 ) == 4 );
}

TEST_CASE( "replace_remove_3" )
{
	TRAIT::InternalString s( "xxxxxx" );

	REQUIRE( s.virginPos( 1 ) == 1 );

	s.remove( 0, 3 );

	REQUIRE( s.asString() == "xxx" );

	REQUIRE( s.virginPos( 0 ) == 3 );
	REQUIRE( s.virginPos( 1 ) == 4 );
	REQUIRE( s.virginPos( 2 ) == 5 );

	s.replace( "xx", "x" );

	REQUIRE( s.asString() == "xx" );

	REQUIRE( s.virginPos( 0 ) == 3 );
	REQUIRE( s.virginPos( 1 ) == 5 );

	s.remove( 0, 1 );

	REQUIRE( s.asString() == "x" );

	REQUIRE( s.virginPos( 0 ) == 5 );
}

TEST_CASE( "replace_remove_4" )
{
	TRAIT::InternalString s( "xxxxxx" );

	REQUIRE( s.virginPos( 1 ) == 1 );

	s.replace( "xxx", "bbbb" );

	REQUIRE( s.asString() == "bbbbbbbb" );

	REQUIRE( s.virginPos( 0 ) == 0 );
	REQUIRE( s.virginPos( 1 ) == 1 );
	REQUIRE( s.virginPos( 2 ) == 2 );
	REQUIRE( s.virginPos( 3 ) == 2 );

	REQUIRE( s.virginPos( 4 ) == 3 );
	REQUIRE( s.virginPos( 5 ) == 4 );
	REQUIRE( s.virginPos( 6 ) == 5 );
	REQUIRE( s.virginPos( 7 ) == 5 );
}

TEST_CASE( "replace_remove_5" )
{
	TRAIT::InternalString s( "xxxxxx" );

	REQUIRE( s.virginPos( 1 ) == 1 );

	s.replace( "xxx", "bbb" );

	REQUIRE( s.asString() == "bbbbbb" );

	REQUIRE( s.virginPos( 0 ) == 0 );
	REQUIRE( s.virginPos( 1 ) == 1 );
	REQUIRE( s.virginPos( 2 ) == 2 );
	REQUIRE( s.virginPos( 3 ) == 3 );
	REQUIRE( s.virginPos( 4 ) == 4 );
	REQUIRE( s.virginPos( 5 ) == 5 );
}

TEST_CASE( "replace_remove_6" )
{
	TRAIT::InternalString s( "\tParagraph\t" );

	REQUIRE( s.virginPos( 1 ) == 1 );

	s.replace( "\t", "    " );

	REQUIRE( s.asString() == "    Paragraph    " );

	REQUIRE( s.virginPos( 0 ) == 0 );
	REQUIRE( s.virginPos( 1 ) == 0 );
	REQUIRE( s.virginPos( 2 ) == 0 );
	REQUIRE( s.virginPos( 3 ) == 0 );
	REQUIRE( s.virginPos( 4 ) == 1 );
	REQUIRE( s.virginPos( 5 ) == 2 );
	REQUIRE( s.virginPos( 13 ) == 10 );
	REQUIRE( s.virginPos( 14 ) == 10 );
	REQUIRE( s.virginPos( 15 ) == 10 );
	REQUIRE( s.virginPos( 16 ) == 10 );
	REQUIRE( s.virginPos( 17 ) == 11 );
	REQUIRE( s.virginPos( 18 ) == 11 );

	s.remove( 0, 4 );

	REQUIRE( s.asString() == "Paragraph    " );

	REQUIRE( s.virginPos( 0 ) == 1 );
	REQUIRE( s.virginPos( 1 ) == 2 );
	REQUIRE( s.virginPos( 2 ) == 3 );
	REQUIRE( s.virginPos( 9 ) == 10 );
	REQUIRE( s.virginPos( 10 ) == 10 );
	REQUIRE( s.virginPos( 11 ) == 10 );
	REQUIRE( s.virginPos( 12 ) == 10 );
	REQUIRE( s.virginPos( 13 ) == 11 );
	REQUIRE( s.virginPos( 14 ) == 11 );
}

TEST_CASE( "simplified" )
{
	TRAIT::InternalString s( "   a   b   c   " );

	s = s.simplified();

	REQUIRE( s.asString() == "a b c" );

	REQUIRE( s.virginPos( 0 ) == 3 );
	REQUIRE( s.virginPos( 1 ) == 4 );
	REQUIRE( s.virginPos( 2 ) == 7 );
	REQUIRE( s.virginPos( 3 ) == 8 );
	REQUIRE( s.virginPos( 4 ) == 11 );

	s = TRAIT::InternalString( "   a b c   " );

	s = s.simplified();

	REQUIRE( s.asString() == "a b c" );

	REQUIRE( s.virginPos( 0 ) == 3 );
	REQUIRE( s.virginPos( 1 ) == 4 );
	REQUIRE( s.virginPos( 2 ) == 5 );
	REQUIRE( s.virginPos( 3 ) == 6 );
	REQUIRE( s.virginPos( 4 ) == 7 );

	s = TRAIT::InternalString( "a b c" );

	s = s.simplified();

	REQUIRE( s.asString() == "a b c" );

	REQUIRE( s.virginPos( 0 ) == 0 );
	REQUIRE( s.virginPos( 1 ) == 1 );
	REQUIRE( s.virginPos( 2 ) == 2 );
	REQUIRE( s.virginPos( 3 ) == 3 );
	REQUIRE( s.virginPos( 4 ) == 4 );

	s = TRAIT::InternalString( "a b  c" );

	s = s.simplified();

	REQUIRE( s.asString() == "a b c" );

	REQUIRE( s.virginPos( 0 ) == 0 );
	REQUIRE( s.virginPos( 1 ) == 1 );
	REQUIRE( s.virginPos( 2 ) == 2 );
	REQUIRE( s.virginPos( 3 ) == 3 );
	REQUIRE( s.virginPos( 4 ) == 5 );

	s = TRAIT::InternalString( "" );

	s = s.simplified();

	REQUIRE( s.asString() == "" );
	REQUIRE( s.isEmpty() );

	s = TRAIT::InternalString( "   " );

	s = s.simplified();

	REQUIRE( s.asString() == "" );
	REQUIRE( s.isEmpty() );
}

TEST_CASE( "split" )
{
	TRAIT::InternalString s( "|a|b|c|" );

	auto r = s.split( TRAIT::InternalString( "|" ) );

	REQUIRE( r.size() == 3 );

	REQUIRE( r.at( 0 ).asString() == "a" );
	REQUIRE( r.at( 0 ).virginPos( 0 ) == 1 );

	REQUIRE( r.at( 1 ).asString() == "b" );
	REQUIRE( r.at( 1 ).virginPos( 0 ) == 3 );

	REQUIRE( r.at( 2 ).asString() == "c" );
	REQUIRE( r.at( 2 ).virginPos( 0 ) == 5 );

	s = TRAIT::InternalString( " | a | b | c | " );

	r = s.split( TRAIT::InternalString( "|" ) );

	REQUIRE( r.size() == 5 );

	REQUIRE( r.at( 0 ).asString() == " " );
	REQUIRE( r.at( 0 ).virginPos( 0 ) == 0 );

	REQUIRE( r.at( 1 ).asString() == " a " );
	REQUIRE( r.at( 1 ).virginPos( 1 ) == 3 );

	REQUIRE( r.at( 2 ).asString() == " b " );
	REQUIRE( r.at( 2 ).virginPos( 1 ) == 7 );

	REQUIRE( r.at( 3 ).asString() == " c " );
	REQUIRE( r.at( 3 ).virginPos( 1 ) == 11 );

	REQUIRE( r.at( 4 ).asString() == " " );
	REQUIRE( r.at( 4 ).virginPos( 0 ) == 14 );

	s = TRAIT::InternalString( "abc" );

	r = s.split( TRAIT::InternalString( "" ) );

	REQUIRE( r.size() == 3 );

	REQUIRE( r.at( 0 ).asString() == "a" );
	REQUIRE( r.at( 0 ).virginPos( 0 ) == 0 );

	REQUIRE( r.at( 1 ).asString() == "b" );
	REQUIRE( r.at( 1 ).virginPos( 0 ) == 1 );

	REQUIRE( r.at( 2 ).asString() == "c" );
	REQUIRE( r.at( 2 ).virginPos( 0 ) == 2 );

	s = TRAIT::InternalString( " | a | b | c | " );
	s = s.simplified();

	r = s.split( TRAIT::InternalString( "|" ) );

	REQUIRE( r.size() == 3 );

	REQUIRE( r.at( 0 ).asString() == " a " );
	REQUIRE( r.at( 0 ).virginPos( 1 ) == 3 );

	REQUIRE( r.at( 1 ).asString() == " b " );
	REQUIRE( r.at( 1 ).virginPos( 1 ) == 7 );

	REQUIRE( r.at( 2 ).asString() == " c " );
	REQUIRE( r.at( 2 ).virginPos( 1 ) == 11 );
}

TEST_CASE( "sliced" )
{
	TRAIT::InternalString s( "aaabbbccc" );

	s = s.sliced( 3, 3 );

	REQUIRE( s.asString() == "bbb" );
	REQUIRE( s.virginPos( 0 ) == 3 );
	REQUIRE( s.virginPos( 1 ) == 4 );
	REQUIRE( s.virginPos( 2 ) == 5 );

	s = TRAIT::InternalString( "aaabbbccc" );

	s = s.sliced( 3 );

	REQUIRE( s.asString() == "bbbccc" );
	REQUIRE( s.virginPos( 0 ) == 3 );
	REQUIRE( s.virginPos( 1 ) == 4 );
	REQUIRE( s.virginPos( 2 ) == 5 );
	REQUIRE( s.virginPos( 3 ) == 6 );
	REQUIRE( s.virginPos( 4 ) == 7 );
	REQUIRE( s.virginPos( 5 ) == 8 );
}

TEST_CASE( "right" )
{
	TRAIT::InternalString s( "aaabbbccc" );

	s = s.right( 3 );

	REQUIRE( s.asString() == "ccc" );
	REQUIRE( s.virginPos( 0 ) == 6 );
	REQUIRE( s.virginPos( 1 ) == 7 );
	REQUIRE( s.virginPos( 2 ) == 8 );
}

TEST_CASE( "insert" )
{
	TRAIT::InternalString s( "a" );

	s = s.insert( 0, TRAIT::Char( 'b' ) );

	REQUIRE( s.asString() == "ba" );
	REQUIRE( s.virginPos( 0 ) == 0 );
	REQUIRE( s.virginPos( 1 ) == 0 );
}

TEST_CASE( "double_remove" )
{
	TRAIT::InternalString s( "a b c d" );

	s.remove( 2, 1 );

	REQUIRE( s.asString() == "a  c d" );

	s = s.simplified();

	REQUIRE( s.asString() == "a c d" );

	s.remove( 2, 1 );

	REQUIRE( s.asString() == "a  d" );

	s = s.simplified();

	REQUIRE( s.asString() == "a d" );

	REQUIRE( s.virginPos( 0 ) == 0 );
	REQUIRE( s.virginPos( 1 ) == 1 );
	REQUIRE( s.virginPos( 2 ) == 6 );
}

TEST_CASE( "replace_tabs" )
{
	{
		TRAIT::InternalString s( "\tcode\t" );

		MD::replaceTabs< TRAIT > ( s );

		REQUIRE( s.virginPos( 0 ) == 0 );
		REQUIRE( s.virginPos( 1 ) == 0 );
		REQUIRE( s.virginPos( 2 ) == 0 );
		REQUIRE( s.virginPos( 3 ) == 0 );
		REQUIRE( s.virginPos( 4 ) == 1 );
		REQUIRE( s.virginPos( 5 ) == 2 );
		REQUIRE( s.virginPos( 6 ) == 3 );
		REQUIRE( s.virginPos( 7 ) == 4 );
		REQUIRE( s.virginPos( 8 ) == 5 );
		REQUIRE( s.virginPos( 9 ) == 5 );
		REQUIRE( s.virginPos( 10 ) == 5 );
		REQUIRE( s.virginPos( 11 ) == 5 );
	}

	{
		TRAIT::InternalString s( "\tcode\t" );

		MD::replaceTabs< TRAIT > ( s );

		s.remove( 0, 2 );

		REQUIRE( s.virginPos( 0 ) == 0 );
		REQUIRE( s.virginPos( 1 ) == 0 );
		REQUIRE( s.virginPos( 2 ) == 1 );
		REQUIRE( s.virginPos( 3 ) == 2 );
		REQUIRE( s.virginPos( 4 ) == 3 );
		REQUIRE( s.virginPos( 5 ) == 4 );
		REQUIRE( s.virginPos( 6 ) == 5 );
		REQUIRE( s.virginPos( 7 ) == 5 );
		REQUIRE( s.virginPos( 8 ) == 5 );
		REQUIRE( s.virginPos( 9 ) == 5 );
	}

	{
		TRAIT::InternalString s( "\tcode\t" );

		MD::replaceTabs< TRAIT > ( s );

		s.remove( 0, 2 );
		s.remove( 8, 2 );

		REQUIRE( s.virginPos( 0 ) == 0 );
		REQUIRE( s.virginPos( 1 ) == 0 );
		REQUIRE( s.virginPos( 2 ) == 1 );
		REQUIRE( s.virginPos( 3 ) == 2 );
		REQUIRE( s.virginPos( 4 ) == 3 );
		REQUIRE( s.virginPos( 5 ) == 4 );
		REQUIRE( s.virginPos( 6 ) == 5 );
		REQUIRE( s.virginPos( 7 ) == 5 );
	}
}

TEST_CASE( "replace_spaces" )
{
	TRAIT::InternalString s( "    code    c" );

	s.remove( 0, 4 );
	s.remove( 4, 4 );

	REQUIRE( s.virginPos( 0 ) == 4 );
	REQUIRE( s.virginPos( 1 ) == 5 );
	REQUIRE( s.virginPos( 2 ) == 6 );
	REQUIRE( s.virginPos( 3 ) == 7 );
	REQUIRE( s.virginPos( 4 ) == 12 );
}

TEST_CASE( "virgin_string" )
{
	{
		TRAIT::InternalString s( "\tcode" );

		MD::replaceTabs< TRAIT > ( s );

		REQUIRE( s.virginString() == u8"\tcode" );
	}

	{
		TRAIT::InternalString s( "\tcode" );

		MD::replaceTabs< TRAIT > ( s );

		s.remove( 0, 2 );

		REQUIRE( s.virginString() == u8"  code" );
	}

	{
		TRAIT::InternalString s( "\t\tcode" );

		MD::replaceTabs< TRAIT > ( s );

		s.remove( 0, 2 );

		REQUIRE( s.virginString() == u8"  \tcode" );
	}

	{
		TRAIT::InternalString s( "\tcode\t" );

		MD::replaceTabs< TRAIT > ( s );

		REQUIRE( s.virginString() == u8"\tcode\t" );
	}

	{
		TRAIT::InternalString s( "\tcode\t" );

		MD::replaceTabs< TRAIT > ( s );

		s.remove( 0, 2 );

		REQUIRE( s.virginString() == u8"  code\t" );
	}

	{
		TRAIT::InternalString s( "\t\tcode\t\t" );

		MD::replaceTabs< TRAIT > ( s );

		s.remove( 0, 2 );
		s.remove( s.length() - 2, 2 );

		REQUIRE( s.virginString() == u8"  \tcode\t  " );
	}

	{
		TRAIT::InternalString s( "\t\tcode\t\t" );

		s.remove( 3, 2 );

		MD::replaceTabs< TRAIT > ( s );

		REQUIRE( s.virginString() == u8"\t\tcode\t\t" );
	}

	{
		TRAIT::InternalString s( "\t\tcode\t\t" );

		MD::replaceTabs< TRAIT > ( s );

		s.remove( 0, s.length() );

		REQUIRE( s.virginString() == u8"\t\tcode\t\t" );
	}

	{
		TRAIT::InternalString s( "\t\tcode\t\t" );

		MD::replaceTabs< TRAIT > ( s );

		s.remove( 0, s.length() );

		REQUIRE( s.virginString( -1, s.length() + 1 ) == u8"\t\tcode\t\t" );
	}

	{
		TRAIT::InternalString s( "text" );

		s.remove( 0, 2 );
		s.remove( s.length() - 1, 1 );

		REQUIRE( s.virginString() == u8"x" );
	}

	{
		TRAIT::InternalString s( "text" );

		REQUIRE( s.virginString( 1, 1 ) == u8"e" );
	}

	{
		TRAIT::InternalString s( "\t\tcode\t\t" );

		MD::replaceTabs< TRAIT > ( s );

		REQUIRE( s.virginString( 0, 12 ) == u8"\t\tcode" );
	}

	{
		TRAIT::InternalString s( "\ta\t\tb" );

		MD::replaceTabs< TRAIT > ( s );

		REQUIRE( s.virginString( 4 ) == u8"a\t\tb" );
	}
}
