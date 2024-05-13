
/*
	SPDX-FileCopyrightText: 2022-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

// doctest include.
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

// md4qt include.
#include <md4qt/algo.hpp>
#include <md4qt/parser.hpp>

int g_count = 0;

void onItem( MD::Item< TRAIT > * i )
{
	++g_count;
}

TEST_CASE( "001" )
{
	MD::Parser< TRAIT > p;
	auto doc = p.parse( "tests/parser/data/001.md" );
	
	MD::forEach< TRAIT >( {}, doc, onItem );
	REQUIRE( g_count == 0 );
	g_count = 0;
	
	MD::forEach< TRAIT >( { MD::ItemType::Anchor }, doc, onItem );
	REQUIRE( g_count == 1 );
	g_count = 0;
	
	MD::forEach< TRAIT >( { MD::ItemType::Paragraph }, doc, onItem );
	REQUIRE( g_count == 0 );
	g_count = 0;
}

/*
This is just a text!

*/
TEST_CASE( "002" )
{
	MD::Parser< TRAIT > p;
	auto doc = p.parse( "tests/parser/data/002.md" );

	MD::forEach< TRAIT >( { MD::ItemType::Paragraph }, doc, onItem );
	REQUIRE( g_count == 1 );
	g_count = 0;
	
	MD::forEach< TRAIT >( { MD::ItemType::Text }, doc, onItem );
	REQUIRE( g_count == 1 );
	g_count = 0;
}

/*

Paragraph 1.

Paragraph 2.

*/
TEST_CASE( "003" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/003.md" );

	MD::forEach< TRAIT >( { MD::ItemType::Paragraph }, doc, onItem );
	REQUIRE( g_count == 2 );
	g_count = 0;
	
	MD::forEach< TRAIT >( { MD::ItemType::Text }, doc, onItem );
	REQUIRE( g_count == 2 );
	g_count = 0;
}

/*
`code`

*/
TEST_CASE( "011" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/011.md" );

	MD::forEach< TRAIT >( { MD::ItemType::Paragraph }, doc, onItem );
	REQUIRE( g_count == 1 );
	g_count = 0;
	
	MD::forEach< TRAIT >( { MD::ItemType::Code }, doc, onItem );
	REQUIRE( g_count == 1 );
	g_count = 0;
}

/*
Code in the `text`.

*/
TEST_CASE( "012" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/012.md" );

	MD::forEach< TRAIT >( { MD::ItemType::Paragraph }, doc, onItem );
	REQUIRE( g_count == 1 );
	g_count = 0;
	
	MD::forEach< TRAIT >( { MD::ItemType::Text }, doc, onItem );
	REQUIRE( g_count == 2 );
	g_count = 0;
	
	MD::forEach< TRAIT >( { MD::ItemType::Code }, doc, onItem );
	REQUIRE( g_count == 1 );
	g_count = 0;
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

	MD::forEach< TRAIT >( { MD::ItemType::Blockquote }, doc, onItem );
	REQUIRE( g_count == 2 );
	g_count = 0;
	
	MD::forEach< TRAIT >( { MD::ItemType::Paragraph }, doc, onItem );
	REQUIRE( g_count == 3 );
	g_count = 0;
	
	MD::forEach< TRAIT >( { MD::ItemType::Paragraph }, doc, onItem, 2 );
	REQUIRE( g_count == 2 );
	g_count = 0;
	
	MD::forEach< TRAIT >( { MD::ItemType::Blockquote, MD::ItemType::Paragraph }, doc, onItem );
	REQUIRE( g_count == 5 );
	g_count = 0;
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

	MD::forEach< TRAIT >( { MD::ItemType::List }, doc, onItem );
	REQUIRE( g_count == 4 );
	g_count = 0;
	
	MD::forEach< TRAIT >( { MD::ItemType::List }, doc, onItem, 1 );
	REQUIRE( g_count == 1 );
	g_count = 0;
	
	MD::forEach< TRAIT >( { MD::ItemType::Paragraph }, doc, onItem );
	REQUIRE( g_count == 13 );
	g_count = 0;
	
	MD::forEach< TRAIT >( { MD::ItemType::Paragraph }, doc, onItem , 3 );
	REQUIRE( g_count == 7 );
	g_count = 0;
}
