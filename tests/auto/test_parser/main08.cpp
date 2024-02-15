
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

// doctest include.
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

/*
This sentence uses $\` and \`$ delimiters to show math inline:  $`\sqrt{3x-1}+(1+x)^2`$

*/
TEST_CASE( "218" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/218.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT > * > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 4 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Math );

	{
		auto m = static_cast< MD::Math< TRAIT > * > ( p->items().at( 1 ).get() );
		REQUIRE( m->expr() == u8"\\` and \\`" );
	}

	REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );
	REQUIRE( p->items().at( 3 )->type() == MD::ItemType::Math );

	{
		auto m = static_cast< MD::Math< TRAIT > * > ( p->items().at( 3 ).get() );
		REQUIRE( m->expr() == u8"\\sqrt{3x-1}+(1+x)^2" );
	}
}

/*
[^]

[^1]

[^1
]

*/
TEST_CASE( "219" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/219.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 4 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::FootnoteRef );
		auto fr = static_cast< MD::FootnoteRef< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( fr->text() == u8"[^1]" );
		REQUIRE( fr->isSpaceBefore() );
		REQUIRE( fr->isSpaceAfter() );
	}

	{
		REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 3 ).get() );
		REQUIRE( p->items().size() == 2 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
	}
}

/*
foo <!--
* not list
-->

*/
TEST_CASE( "220" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/220.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->text() == u8"foo <!--" );

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 2 ).get() );
	REQUIRE( l->items().size() == 1 );
	REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
	auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
	REQUIRE( li->items().size() == 1 );
	REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );

	{
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
		REQUIRE( p->items().size() == 2 );

		{
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
			REQUIRE( t->text() == u8"not list" );
		}

		{
			REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );
			REQUIRE( t->text() == u8"-->" );
		}
	}
}

/*
|||||
|:---------------------|:---------------------|:---------------------|:-----
|$\text{\'{a}}$ `\'{a}`|$\text{\~{a}}$ `\~{a}`|$\text{\.{a}}$ `\.{a}`|$\text{\H{a}}$ `\H{a}`
|$\text{\`{a}}$ <code>\\`{a}</code>|$\text{\={a}}$ `\={a}`|$\text{\"{a}}$ `\"{a}`|$\text{\v{a}}$ `\v{a}`
|$\text{\^{a}}$ `\^{a}`|$\text{\u{a}}$ `\u{a}`|$\text{\r{a}}$ `\r{a}`|

*/
TEST_CASE( "221" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/221.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Table );
	auto t = static_cast< MD::Table< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( t->columnsCount() == 4 );
	REQUIRE( t->rows().size() == 4 );
	auto c = static_cast< MD::TableCell< TRAIT > * > ( t->rows()[ 1 ]->cells()[ 0 ].get() );
	REQUIRE( c->items().size() == 2 );
	REQUIRE( c->items().at( 0 )->type() == MD::ItemType::Math );
	REQUIRE( c->items().at( 1 )->type() == MD::ItemType::Code );
}

/*
||||
|:--------------|:----------------------------------------|:-----
|$x_n$ `x_n` |$\stackrel{!}{=}$ `\stackrel{!}{=}`| $a \atop b$ `a \atop b`
|$e^x$ `e^x` |$\overset{!}{=}$ `\overset{!}{=}`  | $a\raisebox{0.25em}{$b$}c$ `a\raisebox{0.25em}{$b$}c`
|$_u^o $ `_u^o `| $\underset{!}{=}$ `\underset{!}{=}` | $a+\left(\vcenter{\frac{\frac a b}c}\right)$ `a+\left(\vcenter{\hbox{$\frac{\frac a b}c$}}\right)`
||| $$\sum_{\substack{0<i<m\\0<j<n}}$$ `\sum_{\substack{0<i<m\\0<j<n}}`

*/
TEST_CASE( "222" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/222.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Table );
	auto t = static_cast< MD::Table< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( t->columnsCount() == 3 );
	REQUIRE( t->rows().size() == 5 );
	REQUIRE( t->rows()[ 4 ]->cells().size() == 3 );
	auto c = static_cast< MD::TableCell< TRAIT > * > ( t->rows()[ 4 ]->cells()[ 2 ].get() );
	REQUIRE( c->items().size() == 2 );
	REQUIRE( c->items().at( 0 )->type() == MD::ItemType::Math );
	REQUIRE( c->items().at( 1 )->type() == MD::ItemType::Code );
}
