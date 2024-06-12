
/*
	SPDX-FileCopyrightText: 2022-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

// doctest include.
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

// C++ include.
#include <fstream>

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
		REQUIRE( m->startDelim() == MD::WithPosition{ 19, 0, 19, 0 } );
		REQUIRE( m->endDelim() == MD::WithPosition{ 29, 0, 29, 0 } );
	}

	REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );
	REQUIRE( p->items().at( 3 )->type() == MD::ItemType::Math );

	{
		auto m = static_cast< MD::Math< TRAIT > * > ( p->items().at( 3 ).get() );
		REQUIRE( m->expr() == u8"\\sqrt{3x-1}+(1+x)^2" );
		REQUIRE( m->startDelim() == MD::WithPosition{ 64, 0, 64, 0 } );
		REQUIRE( m->endDelim() == MD::WithPosition{ 86, 0, 86, 0 } );
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
		REQUIRE( fr->idPos() == MD::WithPosition{ 1, 2, 2, 2 } );
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
	REQUIRE( li->delim() == MD::WithPosition{ 0, 1, 0, 1 } );
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
	auto m = static_cast< MD::Math< TRAIT >* > ( c->items().at( 0 ).get() );
	REQUIRE( m->startDelim() == MD::WithPosition{ 1, 2, 1, 2 } );
	REQUIRE( m->endDelim() == MD::WithPosition{ 14, 2, 14, 2 } );
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
	auto ic = static_cast< MD::Code< TRAIT >* > ( c->items().at( 1 ).get() );
	REQUIRE( ic->startDelim() == MD::WithPosition{ 39, 5, 39, 5 } );
	REQUIRE( ic->endDelim() == MD::WithPosition{ 70, 5, 70, 5 } );
}

/*
- List
text
=

*/
TEST_CASE( "223" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/223.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->items().size() == 1 );
	auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
	REQUIRE( li->delim() == MD::WithPosition{ 0, 0, 0, 0 } );
	REQUIRE( li->items().size() == 1 );
	REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
	REQUIRE( p->items().size() == 3 );
}

/*
> quote
text
-

*/
TEST_CASE( "224" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/224.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );
	auto b = static_cast< MD::Blockquote< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( b->items().size() == 1 );
	REQUIRE( b->delims() == MD::Blockquote< TRAIT >::Delims{ { 0, 0, 0, 0 } } );
	REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( b->items().at( 0 ).get() );
	REQUIRE( p->items().size() == 2 );
}

/*
> quote
text
=

*/
TEST_CASE( "225" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/225.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );
	auto b = static_cast< MD::Blockquote< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( b->delims() == MD::Blockquote< TRAIT >::Delims{ { 0, 0, 0, 0 } } );
	REQUIRE( b->items().size() == 1 );
	REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( b->items().at( 0 ).get() );
	REQUIRE( p->items().size() == 3 );
}

/*
* List
  -
    Text

*/
TEST_CASE( "226" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/226.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->items().size() == 1 );
	auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
	REQUIRE( li->delim() == MD::WithPosition{ 0, 0, 0, 0 } );
	REQUIRE( li->items().size() == 2 );
	REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Heading );
	auto h = static_cast< MD::Heading< TRAIT >* > ( li->items().at( 0 ).get() );
	REQUIRE( !h->text()->isEmpty() );
	REQUIRE( h->text()->items().size() == 1 );
	REQUIRE( h->text()->items().at( 0 )->type() == MD::ItemType::Text );
	REQUIRE( h->delims().size() == 1 );
	REQUIRE( h->delims().front() == MD::WithPosition{ 2, 1, 2, 1 } );

	REQUIRE( li->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );
}

/*
- - - text

*/
TEST_CASE( "227" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/227.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	{
		auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( l->items().size() == 1 );

		{
			auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
			REQUIRE( li->delim() == MD::WithPosition{ 0, 0, 0, 0 } );
			REQUIRE( li->items().size() == 1 );
			REQUIRE( li->items().at( 0 )->type() == MD::ItemType::List );
			auto l = static_cast< MD::List< TRAIT >* > ( li->items().at( 0 ).get() );
			REQUIRE( l->items().size() == 1 );

			{
				auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
				REQUIRE( li->delim() == MD::WithPosition{ 2, 0, 2, 0 } );
				REQUIRE( li->items().size() == 1 );
				REQUIRE( li->items().at( 0 )->type() == MD::ItemType::List );
				auto l = static_cast< MD::List< TRAIT >* > ( li->items().at( 0 ).get() );
				REQUIRE( l->items().size() == 1 );

				{
					auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
					REQUIRE( li->delim() == MD::WithPosition{ 4, 0, 4, 0 } );
					REQUIRE( li->items().size() == 1 );
					REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );
					auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
					REQUIRE( p->items().size() == 1 );
				}
			}
		}
	}
}

/*
-
  -
    text
    -

*/
TEST_CASE( "228" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/228.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	{
		auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( l->items().size() == 1 );

		{
			auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
			REQUIRE( li->delim() == MD::WithPosition{ 0, 0, 0, 0 } );
			REQUIRE( li->items().size() == 1 );
			REQUIRE( li->items().at( 0 )->type() == MD::ItemType::List );
			auto l = static_cast< MD::List< TRAIT >* > ( li->items().at( 0 ).get() );
			REQUIRE( l->items().size() == 1 );

			{
				auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
				REQUIRE( li->delim() == MD::WithPosition{ 2, 1, 2, 1 } );
				REQUIRE( li->items().size() == 1 );
				REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Heading );
				auto h = static_cast< MD::Heading< TRAIT >* > ( li->items().at( 0 ).get() );
				REQUIRE( h->delims().size() == 1 );
				REQUIRE( h->delims().front() == MD::WithPosition{ 4, 3, 4, 3 } );
				REQUIRE( !h->text()->isEmpty() );
				REQUIRE( h->text()->items().size() == 1 );
				REQUIRE( h->text()->items().at( 0 )->type() == MD::ItemType::Text );
			}
		}
	}
}

/*
-

  text

*/
TEST_CASE( "229" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/229.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );
}

/*
- text
  -

- text
  -

*/
TEST_CASE( "230" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/230.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->items().size() == 2 );

	{
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
		REQUIRE( li->delim() == MD::WithPosition{ 0, 0, 0, 0 } );
		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( li->items().at( 0 ).get() );
		REQUIRE( !h->text()->isEmpty() );
		REQUIRE( h->text()->items().size() == 1 );
		REQUIRE( h->text()->items().at( 0 )->type() == MD::ItemType::Text );
		REQUIRE( h->delims().size() == 1 );
		REQUIRE( h->delims().front() == MD::WithPosition{ 2, 1, 2, 1 } );
	}

	{
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 1 ).get() );
		REQUIRE( li->delim() == MD::WithPosition{ 0, 3, 0, 3 } );
		REQUIRE( li->items().size() == 1 );
		REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( li->items().at( 0 ).get() );
		REQUIRE( !h->text()->isEmpty() );
		REQUIRE( h->text()->items().size() == 1 );
		REQUIRE( h->text()->items().at( 0 )->type() == MD::ItemType::Text );
		REQUIRE( h->delims().size() == 1 );
		REQUIRE( h->delims().front() == MD::WithPosition{ 2, 4, 2, 4 } );
	}
}

/*
www.google.com

*/
TEST_CASE( "231" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/231.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
	auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( l->startColumn() == 0 );
	REQUIRE( l->startLine() == 0 );
	REQUIRE( l->endColumn() == 13 );
	REQUIRE( l->endLine() == 0 );
	REQUIRE( l->url() == u8"http://www.google.com" );
	REQUIRE( l->textPos() == MD::WithPosition{ 0, 0, 13, 0 } );
	REQUIRE( l->urlPos() == MD::WithPosition{ 0, 0, 13, 0 } );
}

/*
www.google.com   www.google.com

*/
TEST_CASE( "232" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/232.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 2 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( l->startColumn() == 0 );
		REQUIRE( l->startLine() == 0 );
		REQUIRE( l->endColumn() == 13 );
		REQUIRE( l->endLine() == 0 );
		REQUIRE( l->url() == u8"http://www.google.com" );
		REQUIRE( l->textPos() == MD::WithPosition{ 0, 0, 13, 0 } );
		REQUIRE( l->urlPos() == MD::WithPosition{ 0, 0, 13, 0 } );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( l->startColumn() == 17 );
		REQUIRE( l->startLine() == 0 );
		REQUIRE( l->endColumn() == 30 );
		REQUIRE( l->endLine() == 0 );
		REQUIRE( l->url() == u8"http://www.google.com" );
		REQUIRE( l->textPos() == MD::WithPosition{ 17, 0, 30, 0 } );
		REQUIRE( l->urlPos() == MD::WithPosition{ 17, 0, 30, 0 } );
	}
}

/*
(www.google.com)

*/
TEST_CASE( "233" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/233.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 3 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 0 );
		REQUIRE( t->endColumn() == 0 );
		REQUIRE( t->endLine() == 0 );
		REQUIRE( t->text() == u8"(" );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( l->startColumn() == 1 );
		REQUIRE( l->startLine() == 0 );
		REQUIRE( l->endColumn() == 14 );
		REQUIRE( l->endLine() == 0 );
		REQUIRE( l->url() == u8"http://www.google.com" );
		REQUIRE( l->textPos() == MD::WithPosition{ 1, 0, 14, 0 } );
		REQUIRE( l->urlPos() == l->textPos() );
	}

	{
		REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 2 ).get() );
		REQUIRE( t->startColumn() == 15 );
		REQUIRE( t->startLine() == 0 );
		REQUIRE( t->endColumn() == 15 );
		REQUIRE( t->endLine() == 0 );
		REQUIRE( t->text() == u8")" );
	}
}

/*
(www.google.com)   (https://www.google.com)

*/
TEST_CASE( "234" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/234.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 5 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 0 );
		REQUIRE( t->endColumn() == 0 );
		REQUIRE( t->endLine() == 0 );
		REQUIRE( t->text() == u8"(" );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( l->startColumn() == 1 );
		REQUIRE( l->startLine() == 0 );
		REQUIRE( l->endColumn() == 14 );
		REQUIRE( l->endLine() == 0 );
		REQUIRE( l->url() == u8"http://www.google.com" );
		REQUIRE( l->textPos() == MD::WithPosition{ 1, 0, 14, 0 } );
		REQUIRE( l->urlPos() == l->textPos() );
	}

	{
		REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 2 ).get() );
		REQUIRE( t->startColumn() == 15 );
		REQUIRE( t->startLine() == 0 );
		REQUIRE( t->endColumn() == 19 );
		REQUIRE( t->endLine() == 0 );
		REQUIRE( t->text() == u8") (" );
	}

	{
		REQUIRE( p->items().at( 3 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 3 ).get() );
		REQUIRE( l->startColumn() == 20 );
		REQUIRE( l->startLine() == 0 );
		REQUIRE( l->endColumn() == 41 );
		REQUIRE( l->endLine() == 0 );
		REQUIRE( l->url() == u8"https://www.google.com" );
		REQUIRE( l->textPos() == MD::WithPosition{ 20, 0, 41, 0 } );
		REQUIRE( l->urlPos() == l->textPos() );
	}

	{
		REQUIRE( p->items().at( 4 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 4 ).get() );
		REQUIRE( t->startColumn() == 42 );
		REQUIRE( t->startLine() == 0 );
		REQUIRE( t->endColumn() == 42 );
		REQUIRE( t->endLine() == 0 );
		REQUIRE( t->text() == u8")" );
	}
}

/*
www.google.com
www.google.com

*/
TEST_CASE( "235" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/235.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 2 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( l->startColumn() == 0 );
		REQUIRE( l->startLine() == 0 );
		REQUIRE( l->endColumn() == 13 );
		REQUIRE( l->endLine() == 0 );
		REQUIRE( l->url() == u8"http://www.google.com" );
		REQUIRE( l->textPos() == MD::WithPosition{ 0, 0, 13, 0 } );
		REQUIRE( l->urlPos() == l->textPos() );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( l->startColumn() == 0 );
		REQUIRE( l->startLine() == 1 );
		REQUIRE( l->endColumn() == 13 );
		REQUIRE( l->endLine() == 1 );
		REQUIRE( l->url() == u8"http://www.google.com" );
		REQUIRE( l->textPos() == MD::WithPosition{ 0, 1, 13, 1 } );
		REQUIRE( l->urlPos() == l->textPos() );
	}
}

/*
text
text www.google.com text
text

*/
TEST_CASE( "236" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/236.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 5 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 0 );
		REQUIRE( t->endColumn() == 3 );
		REQUIRE( t->endLine() == 0 );
		REQUIRE( t->text() == u8"text" );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 1 );
		REQUIRE( t->endColumn() == 4 );
		REQUIRE( t->endLine() == 1 );

		REQUIRE( t->isSpaceBefore() );
		REQUIRE( t->isSpaceAfter() );

		REQUIRE( t->text() == u8"text" );
	}

	{
		REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 2 ).get() );
		REQUIRE( l->startColumn() == 5 );
		REQUIRE( l->startLine() == 1 );
		REQUIRE( l->endColumn() == 18 );
		REQUIRE( l->endLine() == 1 );
		REQUIRE( l->url() == u8"http://www.google.com" );
		REQUIRE( l->textPos() == MD::WithPosition{ 5, 1, 18, 1 } );
		REQUIRE( l->urlPos() == l->textPos() );
	}

	{
		REQUIRE( p->items().at( 3 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 3 ).get() );
		REQUIRE( t->startColumn() == 20 );
		REQUIRE( t->startLine() == 1 );
		REQUIRE( t->endColumn() == 23 );
		REQUIRE( t->endLine() == 1 );
		REQUIRE( t->text() == u8"text" );
	}

	{
		REQUIRE( p->items().at( 4 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 4 ).get() );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 2 );
		REQUIRE( t->endColumn() == 3 );
		REQUIRE( t->endLine() == 2 );
		REQUIRE( t->text() == u8"text" );
	}
}

/*
text
www.google.com text
text

*/
TEST_CASE( "237" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/237.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 4 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 0 );
		REQUIRE( t->endColumn() == 3 );
		REQUIRE( t->endLine() == 0 );
		REQUIRE( t->text() == u8"text" );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( l->startColumn() == 0 );
		REQUIRE( l->startLine() == 1 );
		REQUIRE( l->endColumn() == 13 );
		REQUIRE( l->endLine() == 1 );
		REQUIRE( l->url() == u8"http://www.google.com" );
		REQUIRE( l->textPos() == MD::WithPosition{ 0, 1, 13, 1 } );
		REQUIRE( l->urlPos() == l->textPos() );
	}

	{
		REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 2 ).get() );
		REQUIRE( t->startColumn() == 15 );
		REQUIRE( t->startLine() == 1 );
		REQUIRE( t->endColumn() == 18 );
		REQUIRE( t->endLine() == 1 );
		REQUIRE( t->text() == u8"text" );
	}

	{
		REQUIRE( p->items().at( 3 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 3 ).get() );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 2 );
		REQUIRE( t->endColumn() == 3 );
		REQUIRE( t->endLine() == 2 );
		REQUIRE( t->text() == u8"text" );
	}
}

/*
text
www.google.com
text

*/
TEST_CASE( "238" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/238.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 3 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 0 );
		REQUIRE( t->endColumn() == 3 );
		REQUIRE( t->endLine() == 0 );
		REQUIRE( t->text() == u8"text" );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( l->startColumn() == 0 );
		REQUIRE( l->startLine() == 1 );
		REQUIRE( l->endColumn() == 13 );
		REQUIRE( l->endLine() == 1 );
		REQUIRE( l->url() == u8"http://www.google.com" );
		REQUIRE( l->textPos() == MD::WithPosition{ 0, 1, 13, 1 } );
		REQUIRE( l->urlPos() == l->textPos() );
	}

	{
		REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 2 ).get() );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 2 );
		REQUIRE( t->endColumn() == 3 );
		REQUIRE( t->endLine() == 2 );
		REQUIRE( t->text() == u8"text" );
	}
}

/*
igor@gmail.com
mailto:igor@gmail.com

*/
TEST_CASE( "239" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/239.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 2 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( l->startColumn() == 0 );
		REQUIRE( l->startLine() == 0 );
		REQUIRE( l->endColumn() == 13 );
		REQUIRE( l->endLine() == 0 );
		REQUIRE( l->url() == u8"mailto:igor@gmail.com" );
		REQUIRE( l->textPos() == MD::WithPosition{ 0, 0, 13, 0 } );
		REQUIRE( l->urlPos() == l->textPos() );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( l->startColumn() == 0 );
		REQUIRE( l->startLine() == 1 );
		REQUIRE( l->endColumn() == 20 );
		REQUIRE( l->endLine() == 1 );
		REQUIRE( l->url() == u8"mailto:igor@gmail.com" );
		REQUIRE( l->textPos() == MD::WithPosition{ 0, 1, 20, 1 } );
		REQUIRE( l->urlPos() == l->textPos() );
	}
}

/*
www.google.com `code` www.google.com
<!-- -->
  www.google.com   `code`   www.google.com


*/
TEST_CASE( "240" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/240.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 4 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( p->items().size() == 3 );

		{
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
			auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
			REQUIRE( l->url() == u8"http://www.google.com" );
			REQUIRE( l->textPos() == MD::WithPosition{ 0, 0, 13, 0 } );
			REQUIRE( l->urlPos() == l->textPos() );
		}

		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( c->startDelim() == MD::WithPosition{ 15, 0, 15, 0 } );
		REQUIRE( c->endDelim() == MD::WithPosition{ 20, 0, 20, 0 } );

		{
			REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Link );
			auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 2 ).get() );
			REQUIRE( l->url() == u8"http://www.google.com" );
			REQUIRE( l->textPos() == MD::WithPosition{ 22, 0, 35, 0 } );
			REQUIRE( l->urlPos() == l->textPos() );
		}
	}

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::RawHtml );

	{
		REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 3 ).get() );
		REQUIRE( p->items().size() == 3 );

		{
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
			auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
			REQUIRE( l->url() == u8"http://www.google.com" );
			REQUIRE( l->textPos() == MD::WithPosition{ 2, 2, 15, 2 } );
			REQUIRE( l->urlPos() == l->textPos() );
		}

		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Code );
		auto c = static_cast< MD::Code< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( c->startDelim() == MD::WithPosition{ 19, 2, 19, 2 } );
		REQUIRE( c->endDelim() == MD::WithPosition{ 24, 2, 24, 2 } );

		{
			REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Link );
			auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 2 ).get() );
			REQUIRE( l->url() == u8"http://www.google.com" );
			REQUIRE( l->textPos() == MD::WithPosition{ 28, 2, 41, 2 } );
			REQUIRE( l->urlPos() == l->textPos() );
		}
	}
}

/*
[<img src="https://c5.patreon.com/external/logo/become_a_patron_button.png" align="right">](https://www.patreon.com/onqtam)

*/
TEST_CASE( "241" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/241.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
	auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( l->url() == u8"https://www.patreon.com/onqtam" );
	REQUIRE( l->textPos() == MD::WithPosition{ 1, 0, 89, 0 } );
	REQUIRE( l->urlPos() == MD::WithPosition{ 92, 0, 121, 0 } );

	REQUIRE( !l->p()->isEmpty() );
	auto pp = l->p().get();
	REQUIRE( pp->items().size() == 1 );
	REQUIRE( pp->items().at( 0 )->type() == MD::ItemType::RawHtml );
}

/*
[https://github.com/igormironchik/md-pdf](https://github.com/igormironchik/md-pdf)

*/
TEST_CASE( "242" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/242.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
	auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( l->url() == u8"https://github.com/igormironchik/md-pdf" );
	REQUIRE( l->textPos() == MD::WithPosition{ 1, 0, 39, 0 } );
	REQUIRE( l->urlPos() == MD::WithPosition{ 42, 0, 80, 0 } );
	REQUIRE( l->text() == u8"https://github.com/igormironchik/md-pdf" );
}

/*
# reference

[link](243-1.md)

*/
TEST_CASE( "243" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/243.md", true );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 6 );

	REQUIRE( doc->items().at( 0 )->type() == MD::ItemType::Anchor );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Heading );
	auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( h->delims().size() == 1 );
	REQUIRE( h->delims().front() == MD::WithPosition{ 0, 0, 0, 0 } );

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

	const typename TRAIT::String label = u8"#reference/" + wd + u8"243.md";

	REQUIRE( h->label() == label );

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );
	REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::PageBreak );
	REQUIRE( doc->items().at( 4 )->type() == MD::ItemType::Anchor );
	REQUIRE( doc->items().at( 5 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 5 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
	auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( l->url() == label );
	REQUIRE( l->textPos() == MD::WithPosition{ 1, 0, 4, 0 } );
	REQUIRE( l->urlPos() == MD::WithPosition{ 7, 0, 22, 0 } );
}

TEST_CASE( "244" )
{
	typename TRAIT::String wd =
#ifdef MD4QT_QT_SUPPORT
		QDir().absolutePath()
#else
		std::filesystem::canonical( std::filesystem::current_path() ).u8string()
#endif
		+ u8"/tests/parser/data";

#ifndef MD4QT_QT_SUPPORT
{
	std::string tmp;
	wd.toUTF8String( tmp );
	std::replace( tmp.begin(), tmp.end(), '\\', '/' );
	wd = icu::UnicodeString::fromUTF8( tmp );
}
#endif

	const typename TRAIT::String fn = wd + u8"/244.md";
	const typename TRAIT::String ln = wd + u8"/244-1.md";

#ifdef MD4QT_QT_SUPPORT
	const auto fnData = fn.toLocal8Bit();
	const auto lnData = ln.toLocal8Bit();

	const char * fileName = fnData.data();
	const char * link = lnData.data();
#else
	char tmp;
	const auto len1 = fn.extract( 0, fn.length(), &tmp, 1 );
	std::vector< char > data1( len1 + 1, 0 );
	fn.extract( 0, fn.length(), data1.data(), len1 );
	const char * fileName = data1.data();

	const auto len2 = ln.extract( 0, ln.length(), &tmp, 1 );
	std::vector< char > data2( len2 + 1, 0 );
	ln.extract( 0, ln.length(), data2.data(), len2 );
	const char * link = data2.data();
#endif

	std::ofstream f( fileName, std::ios::out | std::ios::trunc );

	if( f.good() )
	{
		f << "[link](" << link << ")\n";
		f.close();
	}
	else
		REQUIRE( false );

	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/244.md", true );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 5 );

	REQUIRE( doc->items().at( 0 )->type() == MD::ItemType::Anchor );
	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::PageBreak );
	REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Anchor );
	REQUIRE( doc->items().at( 4 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 4 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->text() == u8"secret" );
}

TEST_CASE( "244-ref" )
{
	typename TRAIT::String wd =
#ifdef MD4QT_QT_SUPPORT
		QDir().absolutePath()
#else
		std::filesystem::canonical( std::filesystem::current_path() ).u8string()
#endif
		+ u8"/tests/parser/data";

#ifndef MD4QT_QT_SUPPORT
{
	std::string tmp;
	wd.toUTF8String( tmp );
	std::replace( tmp.begin(), tmp.end(), '\\', '/' );
	wd = icu::UnicodeString::fromUTF8( tmp );
}
#endif

	const typename TRAIT::String fn = wd + u8"/244-ref.md";
	const typename TRAIT::String label = u8"#ref/" + wd + u8"/244-1.md";
	const typename TRAIT::String ln = wd + u8"/244-1.md#ref";

#ifdef MD4QT_QT_SUPPORT
	const auto fnData = fn.toLocal8Bit();
	const auto lnData = ln.toLocal8Bit();

	const char * fileName = fnData.data();
	const char * link = lnData.data();
#else
	char tmp;
	const auto len1 = fn.extract( 0, fn.length(), &tmp, 1 );
	std::vector< char > data1( len1 + 1, 0 );
	fn.extract( 0, fn.length(), data1.data(), len1 );
	const char * fileName = data1.data();

	const auto len2 = ln.extract( 0, ln.length(), &tmp, 1 );
	std::vector< char > data2( len2 + 1, 0 );
	ln.extract( 0, ln.length(), data2.data(), len2 );
	const char * link = data2.data();
#endif

	std::ofstream f( fileName, std::ios::out | std::ios::trunc );

	if( f.good() )
	{
		f << "[link](" << link << ")\n";
		f.close();
	}
	else
		REQUIRE( false );

	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/244-ref.md", true );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 5 );

	REQUIRE( doc->items().at( 0 )->type() == MD::ItemType::Anchor );
	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	{
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( l->url() == label );
	}

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::PageBreak );
	REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Anchor );
	REQUIRE( doc->items().at( 4 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 4 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->text() == u8"secret" );
}

/*
~~text ~text~~

*/
TEST_CASE( "245" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/245.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::StrikethroughText );
	REQUIRE( t->text() == u8"text ~text" );
	REQUIRE( t->openStyles().size() == 1 );
	REQUIRE( t->openStyles().at( 0 ) == MD::StyleDelim{ MD::StrikethroughText, 0, 0, 1, 0 } );
	REQUIRE( t->closeStyles().size() == 1 );
	REQUIRE( t->closeStyles().at( 0 ) == MD::StyleDelim{ MD::StrikethroughText, 12, 0, 13, 0 } );
}

/*
~~text~

*/
TEST_CASE( "246" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/246.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::TextWithoutFormat );
	REQUIRE( t->text() == u8"~~text~" );
	REQUIRE( t->openStyles().empty() );
	REQUIRE( t->closeStyles().empty() );
}

/*
**text **text****

*/
TEST_CASE( "247" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/247.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->opts() == MD::BoldText );
	REQUIRE( t->text() == u8"text text" );
	REQUIRE( t->openStyles().size() == 2 );
	REQUIRE( t->openStyles().at( 0 ) == MD::StyleDelim{ MD::BoldText, 0, 0, 1, 0 } );
	REQUIRE( t->openStyles().at( 1 ) == MD::StyleDelim{ MD::BoldText, 7, 0, 8, 0 } );
	REQUIRE( t->closeStyles().size() == 2 );
	REQUIRE( t->closeStyles().at( 0 ) == MD::StyleDelim{ MD::BoldText, 13, 0, 14, 0 } );
	REQUIRE( t->closeStyles().at( 1 ) == MD::StyleDelim{ MD::BoldText, 15, 0, 16, 0 } );
}

/*
*<a>*

*/
TEST_CASE( "248" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/248.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( h->opts() == MD::ItalicText );
	REQUIRE( h->text() == u8"<a>" );
	REQUIRE( h->openStyles().size() == 1 );
	REQUIRE( h->openStyles().at( 0 ) == MD::StyleDelim{ MD::ItalicText, 0, 0, 0, 0 } );
	REQUIRE( h->closeStyles().size() == 1 );
	REQUIRE( h->closeStyles().at( 0 ) == MD::StyleDelim{ MD::ItalicText, 4, 0, 4, 0 } );
}
