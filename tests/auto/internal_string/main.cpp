
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

#include <md4qt/traits.hpp>

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

TEST_CASE( "replace_rmove_6" )
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

	s = s.insert( 0, 'b' );

	REQUIRE( s.asString() == "ba" );
	REQUIRE( s.virginPos( 0 ) == 0 );
	REQUIRE( s.virginPos( 1 ) == 0 );
}
