
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

	s.replace( "b", "bb" );

	REQUIRE( s.asString() == "abbcde" );

	REQUIRE( s.virginPos( 0 ) == 0 );
	REQUIRE( s.virginPos( 1 ) == 1 );
	REQUIRE( s.virginPos( 2 ) == 1 );
	REQUIRE( s.virginPos( 3 ) == 2 );
	REQUIRE( s.virginPos( 4 ) == 3 );
	REQUIRE( s.virginPos( 5 ) == 4 );

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
}
