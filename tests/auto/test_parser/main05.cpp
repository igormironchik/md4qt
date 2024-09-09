
/*
	SPDX-FileCopyrightText: 2022-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

// doctest include.
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

inline typename TRAIT::String
to_string( int i )
{
#ifdef MD4QT_QT_SUPPORT
	return QString::number( i );
#else
	return std::to_string( i );
#endif
}

/*
> foo
bar
--

*/
TEST_CASE( "121" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/121.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );
		auto b = static_cast< MD::Blockquote< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( b->startColumn() == 0 );
		REQUIRE( b->startLine() == 0 );
		REQUIRE( b->endColumn() == 1 );
		REQUIRE( b->endLine() == 2 );
		REQUIRE( b->delims() == MD::Blockquote< TRAIT >::Delims{ { 0, 0, 0, 0 } } );
		REQUIRE( b->items().size() == 1 );
		REQUIRE( b->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( b->items().at( 0 ).get() );
		REQUIRE( p->items().size() == 3 );
		REQUIRE( p->startColumn() == 2 );
		REQUIRE( p->startLine() == 0 );
		REQUIRE( p->endColumn() == 1 );
		REQUIRE( p->endLine() == 2 );

		{
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"foo" );
			REQUIRE( t->startColumn() == 2 );
			REQUIRE( t->startLine() == 0 );
			REQUIRE( t->endColumn() == 4 );
			REQUIRE( t->endLine() == 0 );
		}

		{
			REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"bar" );
			REQUIRE( t->startColumn() == 0 );
			REQUIRE( t->startLine() == 1 );
			REQUIRE( t->endColumn() == 2 );
			REQUIRE( t->endLine() == 1 );
		}

		{
			REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 2 ).get() );
			REQUIRE( t->opts() == MD::TextWithoutFormat );
			REQUIRE( t->text() == u8"--" );
			REQUIRE( t->startColumn() == 0 );
			REQUIRE( t->startLine() == 2 );
			REQUIRE( t->endColumn() == 1 );
			REQUIRE( t->endLine() == 2 );
		}
	}
}

/*
```math
\[\mathrm{\mathbf{M}}(\alpha) =
   \left(
      \begin{matrix}
         \cos(\alpha)+n_x^2\cdot (1-\cos(\alpha))  &  n_x\cdot n_y\cdot (1-\cos(\alpha))-n_z\cdot \sin(\alpha) &  n_x\cdot n_z\cdot (1-\cos(\alpha))+n_y\cdot \sin(\alpha)\\
         n_x\cdot n_y\cdot (1-\cos(\alpha))+n_z\cdot \sin(\alpha) & \cos(\alpha)+n_y^2\cdot (1-\cos(\alpha))  &   n_y\cdot n_z\cdot (1-\cos(\alpha))-n_x\cdot \sin(\alpha)\\
         n_z\cdot n_x\cdot (1-\cos(\alpha))-n_y\cdot \sin(\alpha) & n_z\cdot n_y\cdot (1-\cos(\alpha))+n_x\cdot \sin(\alpha)  & \cos(\alpha)+n_z^2\cdot (1-\cos(\alpha))
      \end{matrix}
   \right)
\]
```

*/
TEST_CASE( "122" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/122.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 2 );
	REQUIRE( p->endLine() == 10 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Math );
	auto m = static_cast< MD::Math< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( !m->isInline() );
	REQUIRE( m->expr() == u8"\\[\\mathrm{\\mathbf{M}}(\\alpha) =\n"
		"   \\left(\n"
		"      \\begin{matrix}\n"
		"         \\cos(\\alpha)+n_x^2\\cdot (1-\\cos(\\alpha))  &  "
		"n_x\\cdot n_y\\cdot (1-\\cos(\\alpha))-n_z\\cdot \\sin(\\alpha) &  "
		"n_x\\cdot n_z\\cdot (1-\\cos(\\alpha))+n_y\\cdot \\sin(\\alpha)\\\\\n"
		"         n_x\\cdot n_y\\cdot (1-\\cos(\\alpha))+n_z\\cdot \\sin(\\alpha) & "
		"\\cos(\\alpha)+n_y^2\\cdot (1-\\cos(\\alpha))  &   "
		"n_y\\cdot n_z\\cdot (1-\\cos(\\alpha))-n_x\\cdot \\sin(\\alpha)\\\\\n"
		"         n_z\\cdot n_x\\cdot (1-\\cos(\\alpha))-n_y\\cdot \\sin(\\alpha) & "
		"n_z\\cdot n_y\\cdot (1-\\cos(\\alpha))+n_x\\cdot \\sin(\\alpha)  & "
		"\\cos(\\alpha)+n_z^2\\cdot (1-\\cos(\\alpha))\n"
		"      \\end{matrix}\n"
		"   \\right)\n"
		"\\]" );
	REQUIRE( m->startColumn() == 0 );
	REQUIRE( m->startLine() == 1 );
	REQUIRE( m->endColumn() == 1 );
	REQUIRE( m->endLine() == 9 );
	REQUIRE( m->startDelim() == MD::WithPosition{ 0, 0, 2, 0 } );
	REQUIRE( m->endDelim() == MD::WithPosition{ 0, 10, 2, 10 } );
	REQUIRE( m->syntaxPos() == MD::WithPosition{ 3, 0, 6, 0 } );
}

/*
Text

# Heading

 * List

*/
TEST_CASE( "123" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/123.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 4 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 3 );
	REQUIRE( p->endLine() == 0 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->text() == u8"Text" );
	REQUIRE( t->startColumn() == 0 );
	REQUIRE( t->startLine() == 0 );
	REQUIRE( t->endColumn() == 3 );
	REQUIRE( t->endLine() == 0 );

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( h->startColumn() == 0 );
		REQUIRE( h->startLine() == 2 );
		REQUIRE( h->endColumn() == 8 );
		REQUIRE( h->endLine() == 2 );
		REQUIRE( h->delims().size() == 1 );
		REQUIRE( h->delims().front() == MD::WithPosition{ 0, 2, 0, 2 } );

		auto p = h->text().get();
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->startColumn() == 2 );
		REQUIRE( p->startLine() == 2 );
		REQUIRE( p->endColumn() == 8 );
		REQUIRE( p->endLine() == 2 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->text() == u8"Heading" );
		REQUIRE( t->startColumn() == 2 );
		REQUIRE( t->startLine() == 2 );
		REQUIRE( t->endColumn() == 8 );
		REQUIRE( t->endLine() == 2 );
	}

	{
		REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::List );
		auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 3 ).get() );
		REQUIRE( l->startColumn() == 1 );
		REQUIRE( l->startLine() == 4 );
		REQUIRE( l->endColumn() == 6 );
		REQUIRE( l->endLine() == 4 );

		REQUIRE( l->items().size() == 1 );

		REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
		auto item = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
		REQUIRE( item->startColumn() == 1 );
		REQUIRE( item->startLine() == 4 );
		REQUIRE( item->endColumn() == 6 );
		REQUIRE( item->endLine() == 4 );
		REQUIRE( item->delim() == MD::WithPosition{ 1, 4, 1, 4 } );
		REQUIRE( item->listType() == MD::ListItem< TRAIT >::Unordered );
		REQUIRE( item->items().size() == 1 );
		REQUIRE( item->items().at( 0 )->type() == MD::ItemType::Paragraph );
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( item->items().at( 0 ).get() );
		REQUIRE( p->startColumn() == 3 );
		REQUIRE( p->startLine() == 4 );
		REQUIRE( p->endColumn() == 6 );
		REQUIRE( p->endLine() == 4 );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( t->text() == u8"List" );
		REQUIRE( t->startColumn() == 3 );
		REQUIRE( t->startLine() == 4 );
		REQUIRE( t->endColumn() == 6 );
		REQUIRE( t->endLine() == 4 );
	}
}

/*
**Why is implementation of the following method so terrible?**

 *
   ```cpp
   template< class Trait >
   inline void
   Parser< Trait >::parse( StringListStream< Trait > & stream,
       std::shared_ptr< Block< Trait > > parent,
       std::shared_ptr< Document< Trait > > doc,
       typename Trait::StringList & linksToParse,
       const typename Trait::String & workingPath,
       const typename Trait::String & fileName,
       bool collectRefLinks, bool top );
   ```

   Implementation of the above method is so ugly because

*/
TEST_CASE( "124" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/124.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 61 );
	REQUIRE( p->endLine() == 0 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->text() == u8"Why is implementation of the following method so terrible?" );
	REQUIRE( t->startColumn() == 2 );
	REQUIRE( t->startLine() == 0 );
	REQUIRE( t->endColumn() == 59 );
	REQUIRE( t->endLine() == 0 );
	REQUIRE( t->openStyles().size() == 1 );
	REQUIRE( t->openStyles().at( 0 ) == MD::StyleDelim{ MD::BoldText, 0, 0, 1, 0 } );
	REQUIRE( t->closeStyles().size() == 1 );
	REQUIRE( t->closeStyles().at( 0 ) == MD::StyleDelim{ MD::BoldText, 60, 0, 61, 0 } );

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 2 ).get() );
	REQUIRE( l->startColumn() == 1 );
	REQUIRE( l->startLine() == 2 );
	REQUIRE( l->endColumn() == 55 );
	REQUIRE( l->endLine() == 15 );

	REQUIRE( l->items().size() == 1 );

	REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );

	auto item = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
	REQUIRE( item->startColumn() == 1 );
	REQUIRE( item->startLine() == 2 );
	REQUIRE( item->endColumn() == 55 );
	REQUIRE( item->endLine() == 15 );
	REQUIRE( item->delim() == MD::WithPosition{ 1, 2, 1, 2 } );

	REQUIRE( item->listType() == MD::ListItem< TRAIT >::Unordered );

	REQUIRE( item->items().size() == 2 );

	REQUIRE( item->items().at( 0 )->type() == MD::ItemType::Code );

	auto c = static_cast< MD::Code< TRAIT >* > ( item->items().at( 0 ).get() );
	REQUIRE( c->startColumn() == 3 );
	REQUIRE( c->startLine() == 4 );
	REQUIRE( c->endColumn() == 39 );
	REQUIRE( c->endLine() == 12 );
	REQUIRE( c->syntax() == u8"cpp" );
	REQUIRE( !c->isInline() );
	REQUIRE( c->startDelim() == MD::WithPosition{ 3, 3, 5, 3 } );
	REQUIRE( c->endDelim() == MD::WithPosition{ 3, 13, 5, 13 } );
	REQUIRE( c->syntaxPos() == MD::WithPosition{ 6, 3, 8, 3 } );
	REQUIRE( c->text() == u8"template< class Trait >\n"
						  "inline void\n"
						  "Parser< Trait >::parse( StringListStream< Trait > & stream,\n"
						  "    std::shared_ptr< Block< Trait > > parent,\n"
						  "    std::shared_ptr< Document< Trait > > doc,\n"
						  "    typename Trait::StringList & linksToParse,\n"
						  "    const typename Trait::String & workingPath,\n"
						  "    const typename Trait::String & fileName,\n"
						  "    bool collectRefLinks, bool top );" );

	REQUIRE( item->items().at( 1 )->type() == MD::ItemType::Paragraph );
	{
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( item->items().at( 1 ).get() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->startColumn() == 3 );
		REQUIRE( p->startLine() == 15 );
		REQUIRE( p->endColumn() == 55 );
		REQUIRE( p->endLine() == 15 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->text() == u8"Implementation of the above method is so ugly because" );
		REQUIRE( t->startColumn() == 3 );
		REQUIRE( t->startLine() == 15 );
		REQUIRE( t->endColumn() == 55 );
		REQUIRE( t->endLine() == 15 );
	}
}

/*
* [Issues](#issues)<!-- endToc -->

*/
TEST_CASE( "125" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/125.md" );

	typename TRAIT::String wd =
#ifdef MD4QT_QT_SUPPORT
		QDir().absolutePath()
#else
		std::filesystem::canonical( std::filesystem::current_path() ).u8string()
#endif
		+ u8"/tests/parser/data";

#ifndef MD4QT_QT_SUPPORT
	std::string tmp;
	wd.toUTF8String( tmp );
	std::replace( tmp.begin(), tmp.end(), '\\', '/' );
	wd = icu::UnicodeString::fromUTF8( tmp );
#endif

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->startColumn() == 0 );
	REQUIRE( l->startLine() == 0 );
	REQUIRE( l->endColumn() == 33 );
	REQUIRE( l->endLine() == 0 );

	REQUIRE( l->items().size() == 1 );

	REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );

	auto item = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
	REQUIRE( item->startColumn() == 0 );
	REQUIRE( item->startLine() == 0 );
	REQUIRE( item->endColumn() == 33 );
	REQUIRE( item->endLine() == 0 );
	REQUIRE( item->delim() == MD::WithPosition{ 0, 0, 0, 0 } );

	REQUIRE( item->listType() == MD::ListItem< TRAIT >::Unordered );

	REQUIRE( item->items().size() == 1 );

	REQUIRE( item->items().at( 0 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph< TRAIT >* > ( item->items().at( 0 ).get() );
	REQUIRE( p->startColumn() == 2 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 33 );
	REQUIRE( p->endLine() == 0 );

	REQUIRE( p->items().size() == 2 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );

	{
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );

		REQUIRE( l->text() == u8"Issues" );
		REQUIRE( l->url() == u8"#issues/" + wd + u8"/125.md" );
		REQUIRE( l->startColumn() == 2 );
		REQUIRE( l->startLine() == 0 );
		REQUIRE( l->endColumn() == 18 );
		REQUIRE( l->endLine() == 0 );
		REQUIRE( l->textPos() == MD::WithPosition{ 3, 0, 8, 0 } );
		REQUIRE( l->urlPos() == MD::WithPosition{ 11, 0, 17, 0 } );
	}

	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::RawHtml );

	auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 1 ).get() );

	REQUIRE( h->text() == u8"<!-- endToc -->" );
	REQUIRE( h->startColumn() == 19 );
	REQUIRE( h->startLine() == 0 );
	REQUIRE( h->endColumn() == 33 );
	REQUIRE( h->endLine() == 0 );
}

/*
# Radiated 1G-6G
Measured in GTEM using 12 Faces Method.
<foo>
## Results
![Measurement Result](MeasurementResult)

<Peaklist>
<bar>

## Observation
No relevant emissions other than radio transmissions.

## Images

*/
TEST_CASE( "126" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/126.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 9 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Heading );

		auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( h->startColumn() == 0 );
		REQUIRE( h->startLine() == 0 );
		REQUIRE( h->endColumn() == 15 );
		REQUIRE( h->endLine() == 0 );
		REQUIRE( h->delims().size() == 1 );
		REQUIRE( h->delims().front() == MD::WithPosition{ 0, 0, 0, 0 } );

		{
			REQUIRE( h->level() == 1 );
			REQUIRE( h->text().get() );
			auto p = h->text().get();
			REQUIRE( p->startColumn() == 2 );
			REQUIRE( p->startLine() == 0 );
			REQUIRE( p->endColumn() == 15 );
			REQUIRE( p->endLine() == p->startLine() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
			REQUIRE( t->text() == u8"Radiated 1G-6G" );
			REQUIRE( t->startColumn() == 2 );
			REQUIRE( t->startLine() == 0 );
			REQUIRE( t->endColumn() == 15 );
			REQUIRE( t->endLine() == t->startLine() );
		}
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( p->startColumn() == 0 );
		REQUIRE( p->startLine() == 1 );
		REQUIRE( p->endColumn() == 4 );
		REQUIRE( p->endLine() == 2 );

		REQUIRE( p->items().size() == 2 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

		REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( t->text() == u8"Measured in GTEM using 12 Faces Method." );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 1 );
		REQUIRE( t->endColumn() == 38 );
		REQUIRE( t->endLine() == 1 );

		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::RawHtml );

		auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 1 ).get() );

		REQUIRE( h->text() == u8"<foo>" );
		REQUIRE( h->startColumn() == 0 );
		REQUIRE( h->startLine() == 2 );
		REQUIRE( h->endColumn() == 4 );
		REQUIRE( h->endLine() == 2 );
	}

	{
		REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Heading );

		auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 3 ).get() );
		REQUIRE( h->startColumn() == 0 );
		REQUIRE( h->startLine() == 3 );
		REQUIRE( h->endColumn() == 9 );
		REQUIRE( h->endLine() == 3 );
		REQUIRE( h->delims().size() == 1 );
		REQUIRE( h->delims().front() == MD::WithPosition{ 0, 3, 1, 3 } );

		{
			REQUIRE( h->level() == 2 );
			REQUIRE( h->text().get() );
			auto p = h->text().get();
			REQUIRE( p->startColumn() == 3 );
			REQUIRE( p->startLine() == 3 );
			REQUIRE( p->endColumn() == 9 );
			REQUIRE( p->endLine() == p->startLine() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
			REQUIRE( t->text() == u8"Results" );
			REQUIRE( t->startColumn() == 3 );
			REQUIRE( t->startLine() == 3 );
			REQUIRE( t->endColumn() == 9 );
			REQUIRE( t->endLine() == t->startLine() );
		}
	}

	{
		REQUIRE( doc->items().at( 4 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 4 ).get() );
		REQUIRE( p->startColumn() == 0 );
		REQUIRE( p->startLine() == 4 );
		REQUIRE( p->endColumn() == 39 );
		REQUIRE( p->endLine() == 4 );

		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Image );

		auto i = static_cast< MD::Image< TRAIT >* > ( p->items().at( 0 ).get() );

		REQUIRE( i->text() == u8"Measurement Result" );
		REQUIRE( i->url() == u8"MeasurementResult" );
		REQUIRE( i->startColumn() == 0 );
		REQUIRE( i->startLine() == 4 );
		REQUIRE( i->endColumn() == 39 );
		REQUIRE( i->endLine() == 4 );
		REQUIRE( i->textPos() == MD::WithPosition{ 2, 4, 19, 4 } );
		REQUIRE( i->urlPos() == MD::WithPosition{ 22, 4, 38, 4 } );
	}

	{
		REQUIRE( doc->items().at( 5 )->type() == MD::ItemType::RawHtml );

		auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 5 ).get() );

		REQUIRE( h->text() == u8"<Peaklist>\n<bar>" );
		REQUIRE( h->startColumn() == 0 );
		REQUIRE( h->startLine() == 6 );
		REQUIRE( h->endColumn() == 4 );
		REQUIRE( h->endLine() == 7 );
	}

	{
		REQUIRE( doc->items().at( 6 )->type() == MD::ItemType::Heading );

		auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 6 ).get() );
		REQUIRE( h->startColumn() == 0 );
		REQUIRE( h->startLine() == 9 );
		REQUIRE( h->endColumn() == 13 );
		REQUIRE( h->endLine() == 9 );
		REQUIRE( h->delims().size() == 1 );
		REQUIRE( h->delims().front() == MD::WithPosition{ 0, 9, 1, 9 } );

		{
			REQUIRE( h->level() == 2 );
			REQUIRE( h->text().get() );
			auto p = h->text().get();
			REQUIRE( p->startColumn() == 3 );
			REQUIRE( p->startLine() == 9 );
			REQUIRE( p->endColumn() == 13 );
			REQUIRE( p->endLine() == p->startLine() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
			REQUIRE( t->text() == u8"Observation" );
			REQUIRE( t->startColumn() == 3 );
			REQUIRE( t->startLine() == 9 );
			REQUIRE( t->endColumn() == 13 );
			REQUIRE( t->endLine() == t->startLine() );
		}
	}

	{
		REQUIRE( doc->items().at( 7 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 7 ).get() );
		REQUIRE( p->startColumn() == 0 );
		REQUIRE( p->startLine() == 10 );
		REQUIRE( p->endColumn() == 52 );
		REQUIRE( p->endLine() == 10 );

		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

		REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( t->text() == u8"No relevant emissions other than radio transmissions." );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 10 );
		REQUIRE( t->endColumn() == 52 );
		REQUIRE( t->endLine() == 10 );
	}

	{
		REQUIRE( doc->items().at( 8 )->type() == MD::ItemType::Heading );

		auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 8 ).get() );
		REQUIRE( h->startColumn() == 0 );
		REQUIRE( h->startLine() == 12 );
		REQUIRE( h->endColumn() == 8 );
		REQUIRE( h->endLine() == 12 );
		REQUIRE( h->delims().size() == 1 );
		REQUIRE( h->delims().front() == MD::WithPosition{ 0, 12, 1, 12 } );

		{
			REQUIRE( h->level() == 2 );
			REQUIRE( h->text().get() );
			auto p = h->text().get();
			REQUIRE( p->startColumn() == 3 );
			REQUIRE( p->startLine() == 12 );
			REQUIRE( p->endColumn() == 8 );
			REQUIRE( p->endLine() == p->startLine() );
			REQUIRE( p->items().size() == 1 );
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
			REQUIRE( t->text() == u8"Images" );
			REQUIRE( t->startColumn() == 3 );
			REQUIRE( t->startLine() == 12 );
			REQUIRE( t->endColumn() == 8 );
			REQUIRE( t->endLine() == t->startLine() );
		}
	}
}

/*
<?php

echo ''

echo ''

echo ''

*/
TEST_CASE( "127" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/127.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );

	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( h->text() == u8"<?php\n\necho ''\n\necho ''\n\necho ''" );
	REQUIRE( h->startColumn() == 0 );
	REQUIRE( h->startLine() == 0 );
	REQUIRE( h->endColumn() == 6 );
	REQUIRE( h->endLine() == 6 );
}

/*
- full example
```C++
#include <iostream>

int main() { return 0; }
```

# Heading

*/
TEST_CASE( "128" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/128.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 4 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

		auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( l->startColumn() == 0 );
		REQUIRE( l->startLine() == 0 );
		REQUIRE( l->endColumn() == 13 );
		REQUIRE( l->endLine() == 0 );

		REQUIRE( l->items().size() == 1 );

		REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );

		auto item = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
		REQUIRE( item->startColumn() == 0 );
		REQUIRE( item->startLine() == 0 );
		REQUIRE( item->endColumn() == 13 );
		REQUIRE( item->endLine() == 0 );
		REQUIRE( item->delim() == MD::WithPosition{ 0, 0, 0, 0 } );

		REQUIRE( item->listType() == MD::ListItem< TRAIT >::Unordered );

		REQUIRE( item->items().size() == 1 );

		REQUIRE( item->items().at( 0 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( item->items().at( 0 ).get() );
		REQUIRE( p->startColumn() == 2 );
		REQUIRE( p->startLine() == 0 );
		REQUIRE( p->endColumn() == 13 );
		REQUIRE( p->endLine() == 0 );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

		REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( t->text() == u8"full example" );
		REQUIRE( t->startColumn() == 2 );
		REQUIRE( t->startLine() == 0 );
		REQUIRE( t->endColumn() == 13 );
		REQUIRE( t->endLine() == 0 );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Code );

		auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 2 ).get() );

		REQUIRE( !c->isInline() );
		REQUIRE( c->text() == u8"#include <iostream>\n\nint main() { return 0; }" );
		REQUIRE( c->startColumn() == 0 );
		REQUIRE( c->startLine() == 2 );
		REQUIRE( c->endColumn() == 23 );
		REQUIRE( c->endLine() == 4 );
		REQUIRE( c->startDelim() == MD::WithPosition{ 0, 1, 2, 1 } );
		REQUIRE( c->endDelim() == MD::WithPosition{ 0, 5, 2, 5 } );
		REQUIRE( c->syntaxPos() == MD::WithPosition{ 3, 1, 5, 1 } );
	}

	{
		REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Heading );

		auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 3 ).get() );
		REQUIRE( h->startColumn() == 0 );
		REQUIRE( h->startLine() == 7 );
		REQUIRE( h->endColumn() == 8 );
		REQUIRE( h->endLine() == 7 );
		REQUIRE( h->delims().size() == 1 );
		REQUIRE( h->delims().front() == MD::WithPosition{ 0, 7, 0, 7 } );

		REQUIRE( h->level() == 1 );
		REQUIRE( h->text().get() );
		auto p = h->text().get();
		REQUIRE( p->startColumn() == 2 );
		REQUIRE( p->startLine() == 7 );
		REQUIRE( p->endColumn() == 8 );
		REQUIRE( p->endLine() == p->startLine() );
		REQUIRE( p->items().size() == 1 );
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( t->text() == u8"Heading" );
		REQUIRE( t->startColumn() == 2 );
		REQUIRE( t->startLine() == 7 );
		REQUIRE( t->endColumn() == 8 );
		REQUIRE( t->endLine() == t->startLine() );
	}
}

/*
<!--[![Downloads](https://pepy.tech/badge/ludwig)](https://pepy.tech/project/ludwig)-->

*/
TEST_CASE( "129" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/129.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<!--[![Downloads](https://pepy.tech/badge/ludwig)](https://pepy.tech/project/ludwig)-->" );
	REQUIRE( h->startColumn() == 0 );
	REQUIRE( h->startLine() == 0 );
	REQUIRE( h->endColumn() == 86 );
	REQUIRE( h->endLine() == h->startLine() );
}

/*
<!--[![Downloads](https://pepy.tech/badge/ludwig)](https://pepy.tech/project/ludwig)
text

text

*/
TEST_CASE( "130" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/130.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( h->text() == u8"<!--[![Downloads](https://pepy.tech/badge/ludwig)](https://pepy.tech/project/ludwig)\ntext\n\ntext" );
	REQUIRE( h->startColumn() == 0 );
	REQUIRE( h->startLine() == 0 );
	REQUIRE( h->endColumn() == 3 );
	REQUIRE( h->endLine() == 3 );
}

/*
> <!--
> text
>

text

*/
TEST_CASE( "131" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/131.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Blockquote );
	auto b = static_cast< MD::Blockquote< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( b->items().size() == 1 );
	REQUIRE( b->startColumn() == 0 );
	REQUIRE( b->startLine() == 0 );
	REQUIRE( b->endColumn() == 1 );
	REQUIRE( b->endLine() == 2 );
	REQUIRE( b->delims() == MD::Blockquote< TRAIT >::Delims{ { 0, 0, 0, 0 },
		{ 0, 1, 0, 1 }, { 0, 2, 0, 2 } } );
	REQUIRE( b->items().at( 0 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT >* > ( b->items().at( 0 ).get() );
	REQUIRE( h->text() == u8"<!--\ntext" );
	REQUIRE( h->startColumn() == 2 );
	REQUIRE( h->startLine() == 0 );
	REQUIRE( h->endColumn() == 5 );
	REQUIRE( h->endLine() == 1 );

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).get() );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 4 );
	REQUIRE( p->endColumn() == 3 );
	REQUIRE( p->endLine() == 4 );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->text() == u8"text" );
	REQUIRE( t->startColumn() == 0 );
	REQUIRE( t->startLine() == 4 );
	REQUIRE( t->endColumn() == 3 );
	REQUIRE( t->endLine() == 4 );
}

/*
* <!--
* text

text

*/
TEST_CASE( "132" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/132.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->items().size() == 2 );
	REQUIRE( l->startColumn() == 0 );
	REQUIRE( l->startLine() == 0 );
	REQUIRE( l->endColumn() == 5 );
	REQUIRE( l->endLine() == 1 );
	REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
	auto i = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
	REQUIRE( i->items().size() == 1 );
	REQUIRE( i->startColumn() == 0 );
	REQUIRE( i->startLine() == 0 );
	REQUIRE( i->endColumn() == 5 );
	REQUIRE( i->endLine() == 0 );
	REQUIRE( i->delim() == MD::WithPosition{ 0, 0, 0, 0 } );
	REQUIRE( i->items().at( 0 )->type() == MD::ItemType::RawHtml );
	auto h = static_cast< MD::RawHtml< TRAIT>* > ( i->items().at( 0 ).get() );
	REQUIRE( h->text() == u8"<!--" );
	REQUIRE( h->startColumn() == 2 );
	REQUIRE( h->startLine() == 0 );
	REQUIRE( h->endColumn() == 5 );
	REQUIRE( h->endLine() == 0 );
	REQUIRE( l->items().at( 1 )->type() == MD::ItemType::ListItem );

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).get() );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 3 );
	REQUIRE( p->endColumn() == 3 );
	REQUIRE( p->endLine() == 3 );
	REQUIRE( p->items().size() == 1 );
	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );
	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );
	REQUIRE( t->text() == u8"text" );
	REQUIRE( t->startColumn() == 0 );
	REQUIRE( t->startLine() == 3 );
	REQUIRE( t->endColumn() == 3 );
	REQUIRE( t->endLine() == 3 );
}

/*
[Standalone GPU-Ready solution](#standalone-gpu-ready-solution)
[Files in `work/data` too](#files-in-workdata-too)

# Standalone GPU-Ready solution

# Files in `work/data` too

*/
TEST_CASE( "133" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/133.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 4 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );
	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 49 );
	REQUIRE( p->endLine() == 1 );
	REQUIRE( p->items().size() == 2 );

	typename TRAIT::String wd =
#ifdef MD4QT_QT_SUPPORT
		QDir().absolutePath()
#else
		std::filesystem::canonical( std::filesystem::current_path() ).u8string()
#endif
		+ u8"/tests/parser/data";

#ifndef MD4QT_QT_SUPPORT
	std::string tmp;
	wd.toUTF8String( tmp );
	std::replace( tmp.begin(), tmp.end(), '\\', '/' );
	wd = icu::UnicodeString::fromUTF8( tmp );
#endif

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( l->url() == u8"#standalone-gpu-ready-solution/" + wd + u8"/133.md" );
		REQUIRE( l->textPos() == MD::WithPosition{ 1, 0, 29, 0 } );
		REQUIRE( l->urlPos() == MD::WithPosition{ 32, 0, 61, 0 } );
		REQUIRE( doc->labeledHeadings().find( l->url() ) != doc->labeledHeadings().cend() );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( l->url() == u8"#files-in-workdata-too/" + wd + u8"/133.md" );
		REQUIRE( l->textPos() == MD::WithPosition{ 1, 1, 24, 1 } );
		REQUIRE( l->urlPos() == MD::WithPosition{ 27, 1, 48, 1 } );
		REQUIRE( doc->labeledHeadings().find( l->url() ) != doc->labeledHeadings().cend() );
	}

	{
		REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( h->delims().size() == 1 );
		REQUIRE( h->delims().front() == MD::WithPosition{ 0, 3, 0, 3 } );
	}

	{
		REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 3 ).get() );
		REQUIRE( h->delims().size() == 1 );
		REQUIRE( h->delims().front() == MD::WithPosition{ 0, 5, 0, 5 } );
	}
}

/*
Text
| Column 1 |
| -------- |
Text

*/
TEST_CASE( "134" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/134.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto dp = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( dp->startColumn() == 0 );
	REQUIRE( dp->startLine() == 0 );
	REQUIRE( dp->endColumn() == 3 );
	REQUIRE( dp->endLine() == 0 );

	REQUIRE( dp->items().size() == 1 );
	REQUIRE( dp->items().front()->type() == MD::ItemType::Text );

	auto dt = static_cast< MD::Text< TRAIT >* > ( dp->items().front().get() );

	REQUIRE( dt->opts() == MD::TextOption::TextWithoutFormat );
	REQUIRE( dt->text() == u8"Text" );
	REQUIRE( dt->startColumn() == 0 );
	REQUIRE( dt->startLine() == 0 );
	REQUIRE( dt->endColumn() == 3 );
	REQUIRE( dt->endLine() == 0 );

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Table );

	auto t = static_cast< MD::Table< TRAIT >* > ( doc->items().at( 2 ).get() );
	REQUIRE( t->startColumn() == 0 );
	REQUIRE( t->startLine() == 1 );
	REQUIRE( t->endColumn() == 3 );
	REQUIRE( t->endLine() == 3 );

	REQUIRE( t->columnsCount() == 1 );
	REQUIRE( t->rows().size() == 2 );

	auto r0 = t->rows().at( 0 );
	REQUIRE( r0->startColumn() == 0 );
	REQUIRE( r0->startLine() == 1 );
	REQUIRE( r0->endColumn() == 11 ) ;
	REQUIRE( r0->endLine() == r0->startLine() );

	REQUIRE( r0->type() == MD::ItemType::TableRow );

	REQUIRE( r0->cells().size() == 1 );

	REQUIRE( r0->cells().at( 0 )->type() == MD::ItemType::TableCell );
	auto c0 = static_cast< MD::TableCell< TRAIT >* > ( r0->cells().at( 0 ).get() );
	REQUIRE( c0->startColumn() == 0 );
	REQUIRE( c0->startLine() == r0->startLine() );
	REQUIRE( c0->endColumn() == r0->endColumn() ) ;
	REQUIRE( c0->endLine() == c0->startLine() );

	REQUIRE( c0->items().size() == 1 );
	REQUIRE( c0->items().at( 0 )->type() == MD::ItemType::Text );

	auto t0 = static_cast< MD::Text< TRAIT >* > ( c0->items().at( 0 ).get() );
	REQUIRE( t0->startColumn() == 2 );
	REQUIRE( t0->startLine() == c0->startLine() );
	REQUIRE( t0->endColumn() == 9 );
	REQUIRE( t0->endLine() == t0->startLine() );

	REQUIRE( t0->text() == u8"Column 1" );

	auto r1 = t->rows().at( 1 );
	REQUIRE( r1->startColumn() == 0 );
	REQUIRE( r1->startLine() == 3 );
	REQUIRE( r1->endColumn() == 3 ) ;
	REQUIRE( r1->endLine() == r1->startLine() );

	REQUIRE( r1->type() == MD::ItemType::TableRow );

	REQUIRE( r1->cells().size() == 1 );

	REQUIRE( r1->cells().at( 0 )->type() == MD::ItemType::TableCell );
	auto c1 = static_cast< MD::TableCell< TRAIT >* > ( r1->cells().at( 0 ).get() );
	REQUIRE( c1->startColumn() == 0 );
	REQUIRE( c1->startLine() == r1->startLine() );
	REQUIRE( c1->endColumn() == r1->endColumn() ) ;
	REQUIRE( c1->endLine() == c1->startLine() );

	REQUIRE( c1->items().size() == 1 );
	REQUIRE( c1->items().at( 0 )->type() == MD::ItemType::Text );

	auto t1 = static_cast< MD::Text< TRAIT >* > ( c1->items().at( 0 ).get() );
	REQUIRE( t1->startColumn() == 0 );
	REQUIRE( t1->startLine() == c1->startLine() );
	REQUIRE( t1->endColumn() == 3 );
	REQUIRE( t1->endLine() == t1->startLine() );

	REQUIRE( t1->text() == u8"Text" );
}

/*
`Text
| Column 1 |`
| -------- |
Text

*/
TEST_CASE( "135" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/135.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto dp = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( dp->startColumn() == 0 );
	REQUIRE( dp->startLine() == 0 );
	REQUIRE( dp->endColumn() == 3 );
	REQUIRE( dp->endLine() == 3 );

	REQUIRE( dp->items().size() == 3 );
	REQUIRE( dp->items().at( 0 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code< TRAIT >* > ( dp->items().at( 0 ).get() );
	REQUIRE( c->text() == u8"Text | Column 1 |" );
	REQUIRE( c->startColumn() == 1 );
	REQUIRE( c->startLine() == 0 );
	REQUIRE( c->endColumn() == 11 );
	REQUIRE( c->endLine() == 1 );
	REQUIRE( c->startDelim() == MD::WithPosition{ 0, 0, 0, 0 } );
	REQUIRE( c->endDelim() == MD::WithPosition{ 12, 1, 12, 1 } );

	{
		REQUIRE( dp->items().at( 1 )->type() == MD::ItemType::Text );
		auto dt = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 1 ).get() );
		REQUIRE( dt->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( dt->text() == u8"| -------- |" );
		REQUIRE( dt->startColumn() == 0 );
		REQUIRE( dt->startLine() == 2 );
		REQUIRE( dt->endColumn() == 11 );
		REQUIRE( dt->endLine() == 2 );
	}

	{
		REQUIRE( dp->items().at( 2 )->type() == MD::ItemType::Text );
		auto dt = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 2 ).get() );
		REQUIRE( dt->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( dt->text() == u8"Text" );
		REQUIRE( dt->startColumn() == 0 );
		REQUIRE( dt->startLine() == 3 );
		REQUIRE( dt->endColumn() == 3 );
		REQUIRE( dt->endLine() == 3 );
	}
}

/*
Text
| Column 1 |
| -------- |
===

*/
TEST_CASE( "136" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/136.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto dp = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( dp->startColumn() == 0 );
	REQUIRE( dp->startLine() == 0 );
	REQUIRE( dp->endColumn() == 3 );
	REQUIRE( dp->endLine() == 0 );

	REQUIRE( dp->items().size() == 1 );
	REQUIRE( dp->items().front()->type() == MD::ItemType::Text );

	auto dt = static_cast< MD::Text< TRAIT >* > ( dp->items().front().get() );

	REQUIRE( dt->opts() == MD::TextOption::TextWithoutFormat );
	REQUIRE( dt->text() == u8"Text" );
	REQUIRE( dt->startColumn() == 0 );
	REQUIRE( dt->startLine() == 0 );
	REQUIRE( dt->endColumn() == 3 );
	REQUIRE( dt->endLine() == 0 );

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Table );

	auto t = static_cast< MD::Table< TRAIT >* > ( doc->items().at( 2 ).get() );
	REQUIRE( t->startColumn() == 0 );
	REQUIRE( t->startLine() == 1 );
	REQUIRE( t->endColumn() == 2 );
	REQUIRE( t->endLine() == 3 );

	REQUIRE( t->columnsCount() == 1 );
	REQUIRE( t->rows().size() == 2 );

	auto r0 = t->rows().at( 0 );
	REQUIRE( r0->startColumn() == 0 );
	REQUIRE( r0->startLine() == 1 );
	REQUIRE( r0->endColumn() == 11 ) ;
	REQUIRE( r0->endLine() == r0->startLine() );

	REQUIRE( r0->type() == MD::ItemType::TableRow );

	REQUIRE( r0->cells().size() == 1 );

	REQUIRE( r0->cells().at( 0 )->type() == MD::ItemType::TableCell );
	auto c0 = static_cast< MD::TableCell< TRAIT >* > ( r0->cells().at( 0 ).get() );
	REQUIRE( c0->startColumn() == 0 );
	REQUIRE( c0->startLine() == r0->startLine() );
	REQUIRE( c0->endColumn() == r0->endColumn() ) ;
	REQUIRE( c0->endLine() == c0->startLine() );

	REQUIRE( c0->items().size() == 1 );
	REQUIRE( c0->items().at( 0 )->type() == MD::ItemType::Text );

	auto t0 = static_cast< MD::Text< TRAIT >* > ( c0->items().at( 0 ).get() );
	REQUIRE( t0->startColumn() == 2 );
	REQUIRE( t0->startLine() == c0->startLine() );
	REQUIRE( t0->endColumn() == 9 );
	REQUIRE( t0->endLine() == t0->startLine() );

	REQUIRE( t0->text() == u8"Column 1" );

	auto r1 = t->rows().at( 1 );
	REQUIRE( r1->startColumn() == 0 );
	REQUIRE( r1->startLine() == 3 );
	REQUIRE( r1->endColumn() == 2 ) ;
	REQUIRE( r1->endLine() == r1->startLine() );

	REQUIRE( r1->type() == MD::ItemType::TableRow );

	REQUIRE( r1->cells().size() == 1 );

	REQUIRE( r1->cells().at( 0 )->type() == MD::ItemType::TableCell );
	auto c1 = static_cast< MD::TableCell< TRAIT >* > ( r1->cells().at( 0 ).get() );
	REQUIRE( c1->startColumn() == 0 );
	REQUIRE( c1->startLine() == r1->startLine() );
	REQUIRE( c1->endColumn() == r1->endColumn() ) ;
	REQUIRE( c1->endLine() == c1->startLine() );

	REQUIRE( c1->items().size() == 1 );
	REQUIRE( c1->items().at( 0 )->type() == MD::ItemType::Text );

	auto t1 = static_cast< MD::Text< TRAIT >* > ( c1->items().at( 0 ).get() );
	REQUIRE( t1->startColumn() == 0 );
	REQUIRE( t1->startLine() == c1->startLine() );
	REQUIRE( t1->endColumn() == 2 );
	REQUIRE( t1->endLine() == t1->startLine() );

	REQUIRE( t1->text() == u8"===" );
}

/*
`Text
| Column 1 |`
| -------- |
===

*/
TEST_CASE( "137" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/137.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Heading );
	auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( h->startColumn() == 0 );
	REQUIRE( h->startLine() == 0 );
	REQUIRE( h->endColumn() == 2 );
	REQUIRE( h->endLine() == 3 );
	REQUIRE( h->delims().size() == 1 );
	REQUIRE( h->delims().front() == MD::WithPosition{ 0, 3, 2, 3 } );

	auto dp = h->text().get();
	REQUIRE( dp->startColumn() == 0 );
	REQUIRE( dp->startLine() == 0 );
	REQUIRE( dp->endColumn() == 11 );
	REQUIRE( dp->endLine() == 2 );

	REQUIRE( dp->items().size() == 2 );
	REQUIRE( dp->items().at( 0 )->type() == MD::ItemType::Code );
	auto c = static_cast< MD::Code< TRAIT >* > ( dp->items().at( 0 ).get() );
	REQUIRE( c->text() == u8"Text | Column 1 |" );
	REQUIRE( c->startColumn() == 1 );
	REQUIRE( c->startLine() == 0 );
	REQUIRE( c->endColumn() == 11 );
	REQUIRE( c->endLine() == 1 );
	REQUIRE( c->startDelim() == MD::WithPosition{ 0, 0, 0, 0 } );
	REQUIRE( c->endDelim() == MD::WithPosition{ 12, 1, 12, 1 } );

	REQUIRE( dp->items().at( 1 )->type() == MD::ItemType::Text );
	auto dt = static_cast< MD::Text< TRAIT >* > ( dp->items().at( 1 ).get() );
	REQUIRE( dt->opts() == MD::TextOption::TextWithoutFormat );
	REQUIRE( dt->text() == u8"| -------- |" );
	REQUIRE( dt->startColumn() == 0 );
	REQUIRE( dt->startLine() == 2 );
	REQUIRE( dt->endColumn() == 11 );
	REQUIRE( dt->endLine() == 2 );
}

/*
1.
   Monday

   fgh

   ```
   code
   ```
2.
Tuesday
3.
Wednesday

*/
TEST_CASE( "138" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/138.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->startColumn() == 0 );
	REQUIRE( l->startLine() == 0 );
	REQUIRE( l->endColumn() == 1 );
	REQUIRE( l->endLine() == 8 );

	REQUIRE( l->items().size() == 2 );

	REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );

	auto i1 = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
	REQUIRE( i1->startColumn() == 0 );
	REQUIRE( i1->startLine() == 0 );
	REQUIRE( i1->endColumn() == 5 );
	REQUIRE( i1->endLine() == 7 );
	REQUIRE( i1->delim() == MD::WithPosition{ 0, 0, 1, 0 } );

	REQUIRE( i1->listType() == MD::ListItem< TRAIT >::Ordered );

	REQUIRE( i1->items().size() == 3 );

	{
		REQUIRE( i1->items().at( 0 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( i1->items().at( 0 ).get() );
		REQUIRE( p->startColumn() == 3 );
		REQUIRE( p->startLine() == 1 );
		REQUIRE( p->endColumn() == 8 );
		REQUIRE( p->endLine() == 1 );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

		REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( t->text() == u8"Monday" );
		REQUIRE( t->startColumn() == 3 );
		REQUIRE( t->startLine() == 1 );
		REQUIRE( t->endColumn() == 8 );
		REQUIRE( t->endLine() == 1 );
	}

	{
		REQUIRE( i1->items().at( 1 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( i1->items().at( 1 ).get() );
		REQUIRE( p->startColumn() == 3 );
		REQUIRE( p->startLine() == 3 );
		REQUIRE( p->endColumn() == 5 );
		REQUIRE( p->endLine() == 3 );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

		REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( t->text() == u8"fgh" );
		REQUIRE( t->startColumn() == 3 );
		REQUIRE( t->startLine() == 3 );
		REQUIRE( t->endColumn() == 5 );
		REQUIRE( t->endLine() == 3 );
	}

	{
		REQUIRE( i1->items().at( 2 )->type() == MD::ItemType::Code );

		auto c = static_cast< MD::Code< TRAIT >* > ( i1->items().at( 2 ).get() );
		REQUIRE( c->startColumn() == 3 );
		REQUIRE( c->startLine() == 6 );
		REQUIRE( c->endColumn() == 6 );
		REQUIRE( c->endLine() == 6 );
		REQUIRE( c->startDelim() == MD::WithPosition{ 3, 5, 5, 5 } );
		REQUIRE( c->endDelim() == MD::WithPosition{ 3, 7, 5, 7 } );
		REQUIRE( c->syntaxPos() == MD::WithPosition{ -1, -1, -1, -1 } );
		REQUIRE( c->text() == u8"code" );
	}

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 2 ).get() );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 9 );
	REQUIRE( p->endColumn() == 8 );
	REQUIRE( p->endLine() == 11 );

	REQUIRE( p->items().size() == 3 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

		REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( t->text() == u8"Tuesday" );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 9 );
		REQUIRE( t->endColumn() == 6 );
		REQUIRE( t->endLine() == 9 );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );

		REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( t->text() == u8"3." );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 10 );
		REQUIRE( t->endColumn() == 1 );
		REQUIRE( t->endLine() == 10 );
	}

	{
		REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 2 ).get() );

		REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( t->text() == u8"Wednesday" );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 11 );
		REQUIRE( t->endColumn() == 8 );
		REQUIRE( t->endLine() == 11 );
	}

	REQUIRE( l->items().at( 1 )->type() == MD::ItemType::ListItem );

	auto i2 = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 1 ).get() );
	REQUIRE( i2->isEmpty() );
}

/*
* Text
    -

*/
TEST_CASE( "139" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/139.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->startColumn() == 0 );
	REQUIRE( l->startLine() == 0 );
	REQUIRE( l->endColumn() == 4 );
	REQUIRE( l->endLine() == 1 );

	REQUIRE( l->items().size() == 1 );

	REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );

	auto item = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
	REQUIRE( item->startColumn() == 0 );
	REQUIRE( item->startLine() == 0 );
	REQUIRE( item->endColumn() == 4 );
	REQUIRE( item->endLine() == 1 );
	REQUIRE( item->delim() == MD::WithPosition{ 0, 0, 0, 0 } );

	REQUIRE( item->listType() == MD::ListItem< TRAIT >::Unordered );

	REQUIRE( item->items().size() == 1 );

	REQUIRE( item->items().at( 0 )->type() == MD::ItemType::Heading );

	auto h = static_cast< MD::Heading< TRAIT >* > ( item->items().at( 0 ).get() );
	REQUIRE( h->startColumn() == 2 );
	REQUIRE( h->startLine() == 0 );
	REQUIRE( h->endColumn() == 4 );
	REQUIRE( h->endLine() == 1 );

	REQUIRE( h->level() == 2 );
	REQUIRE( !h->text()->isEmpty() );

	auto p = h->text().get();
	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

	REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
	REQUIRE( t->text() == u8"Text" );
	REQUIRE( t->startColumn() == 2 );
	REQUIRE( t->startLine() == 0 );
	REQUIRE( t->endColumn() == 5 );
	REQUIRE( t->endLine() == 0 );
}

/*
Text

*   Text
    Text

	Text
	Text

*/
TEST_CASE( "140" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/140.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	{
		REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
		REQUIRE( p->startColumn() == 0 );
		REQUIRE( p->startLine() == 0 );
		REQUIRE( p->endColumn() == 3 );
		REQUIRE( p->endLine() == 0 );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

		REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( t->text() == u8"Text" );
		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 0 );
		REQUIRE( t->endColumn() == 3 );
		REQUIRE( t->endLine() == 0 );
	}

	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 2 ).get() );
	REQUIRE( l->startColumn() == 0 );
	REQUIRE( l->startLine() == 2 );
	REQUIRE( l->endColumn() == 7 );
	REQUIRE( l->endLine() == 6 );

	REQUIRE( l->items().size() == 1 );

	REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );

	auto item = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
	REQUIRE( item->startColumn() == 0 );
	REQUIRE( item->startLine() == 2 );
	REQUIRE( item->endColumn() == 7 );
	REQUIRE( item->endLine() == 6 );
	REQUIRE( item->delim() == MD::WithPosition{ 0, 2, 0, 2 } );

	REQUIRE( item->listType() == MD::ListItem< TRAIT >::Unordered );

	REQUIRE( item->items().size() == 2 );

	{
		REQUIRE( item->items().at( 0 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( item->items().at( 0 ).get() );
		REQUIRE( p->startColumn() == 4 );
		REQUIRE( p->startLine() == 2 );
		REQUIRE( p->endColumn() == 7 );
		REQUIRE( p->endLine() == 3 );

		REQUIRE( p->items().size() == 2 );

		{
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

			REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
			REQUIRE( t->text() == u8"Text" );
			REQUIRE( t->startColumn() == 4 );
			REQUIRE( t->startLine() == 2 );
			REQUIRE( t->endColumn() == 7 );
			REQUIRE( t->endLine() == 2 );
		}

		{
			REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );

			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );

			REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
			REQUIRE( t->text() == u8"Text" );
			REQUIRE( t->startColumn() == 4 );
			REQUIRE( t->startLine() == 3 );
			REQUIRE( t->endColumn() == 7 );
			REQUIRE( t->endLine() == 3 );
		}
	}

	{
		REQUIRE( item->items().at( 1 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( item->items().at( 1 ).get() );
		REQUIRE( p->startColumn() == 4 );
		REQUIRE( p->startLine() == 5 );
		REQUIRE( p->endColumn() == 7 );
		REQUIRE( p->endLine() == 6 );

		REQUIRE( p->items().size() == 2 );

		{
			REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

			REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
			REQUIRE( t->text() == u8"Text" );
			REQUIRE( t->startColumn() == 4 );
			REQUIRE( t->startLine() == 5 );
			REQUIRE( t->endColumn() == 7 );
			REQUIRE( t->endLine() == 5 );
		}

		{
			REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );

			auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );

			REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
			REQUIRE( t->text() == u8"Text" );
			REQUIRE( t->startColumn() == 4 );
			REQUIRE( t->startLine() == 6 );
			REQUIRE( t->endColumn() == 7 );
			REQUIRE( t->endLine() == 6 );
		}
	}
}

/*
![sample code output](https://raw.githubusercontent.com/wiki/ocornut/imgui/web/v160/code_sample_02_jp.png)
<br>_(settings: Dark style (left), Light style (right) / Font: NotoSansCJKjp-Medium, 20px / Rounding: 5)_

*/
TEST_CASE( "141" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/141.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 104 );
	REQUIRE( p->endLine() == 1 );

	REQUIRE( p->items().size() == 3 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Image );

	auto i = static_cast< MD::Image< TRAIT >* > ( p->items().at( 0 ).get() );

	REQUIRE( i->startColumn() == 0 );
	REQUIRE( i->startLine() == 0 );
	REQUIRE( i->endColumn() == 105 );
	REQUIRE( i->endLine() == 0 );
	REQUIRE( i->textPos() == MD::WithPosition{ 2, 0, 19, 0 } );
	REQUIRE( i->urlPos() == MD::WithPosition{ 22, 0, 104, 0 } );

	REQUIRE( p->items().at( 1 )->type() == MD::ItemType::RawHtml );

	auto h = static_cast< MD::RawHtml< TRAIT >* > ( p->items().at( 1 ).get() );

	REQUIRE( h->startColumn() == 0 );
	REQUIRE( h->startLine() == 1 );
	REQUIRE( h->endColumn() == 3 );
	REQUIRE( h->endLine() == 1 );

	REQUIRE( p->items().at( 2 )->type() == MD::ItemType::Text );

	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 2 ).get() );

	REQUIRE( t->opts() == MD::TextOption::ItalicText );
	REQUIRE( t->text() == u8"(settings: Dark style (left), Light style (right) "
		"/ Font: NotoSansCJKjp-Medium, 20px / Rounding: 5)" );
	REQUIRE( t->startColumn() == 5 );
	REQUIRE( t->startLine() == 1 );
	REQUIRE( t->endColumn() == 103 );
	REQUIRE( t->endLine() == 1 );
}

/*
- Text

      Code
        Code
          Code
        Code
      Code

*/
TEST_CASE( "142" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/142.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );

	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );
	REQUIRE( l->startColumn() == 0 );
	REQUIRE( l->startLine() == 0 );
	REQUIRE( l->endColumn() == 9 );
	REQUIRE( l->endLine() == 6 );

	REQUIRE( l->items().size() == 1 );

	REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );

	auto item = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
	REQUIRE( item->startColumn() == 0 );
	REQUIRE( item->startLine() == 0 );
	REQUIRE( item->endColumn() == 9 );
	REQUIRE( item->endLine() == 6 );
	REQUIRE( item->delim() == MD::WithPosition{ 0, 0, 0, 0 } );

	REQUIRE( item->listType() == MD::ListItem< TRAIT >::Unordered );

	REQUIRE( item->items().size() == 2 );

	{
		REQUIRE( item->items().at( 0 )->type() == MD::ItemType::Paragraph );

		auto p = static_cast< MD::Paragraph< TRAIT >* > ( item->items().at( 0 ).get() );
		REQUIRE( p->startColumn() == 2 );
		REQUIRE( p->startLine() == 0 );
		REQUIRE( p->endColumn() == 5 );
		REQUIRE( p->endLine() == 0 );

		REQUIRE( p->items().size() == 1 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

		REQUIRE( t->opts() == MD::TextOption::TextWithoutFormat );
		REQUIRE( t->text() == u8"Text" );
		REQUIRE( t->startColumn() == 2 );
		REQUIRE( t->startLine() == 0 );
		REQUIRE( t->endColumn() == 5 );
		REQUIRE( t->endLine() == 0 );
	}

	{
		REQUIRE( item->items().at( 1 )->type() == MD::ItemType::Code );

		auto c = static_cast< MD::Code< TRAIT >* > ( item->items().at( 1 ).get() );
		REQUIRE( c->startColumn() == 6 );
		REQUIRE( c->startLine() == 2 );
		REQUIRE( c->endColumn() == 9 );
		REQUIRE( c->endLine() == 6 );

		REQUIRE( c->text() == u8"Code\n  Code\n    Code\n  Code\nCode" );
	}
}

/*
```
/**
 * Code
 *
 * Code
 -/
```

*/

TEST_CASE( "143" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/143.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Code );

	auto c = static_cast< MD::Code< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( c->startColumn() == 0 );
	REQUIRE( c->startLine() == 1 );
	REQUIRE( c->endColumn() == 2 );
	REQUIRE( c->endLine() == 5 );
	REQUIRE( c->startDelim() == MD::WithPosition{ 0, 0, 2, 0 } );
	REQUIRE( c->endDelim() == MD::WithPosition{ 0, 6, 2, 6 } );
	REQUIRE( c->syntaxPos() == MD::WithPosition{ -1, -1, -1, -1 } );

	REQUIRE( c->text() == u8"/**\n * Code\n *\n * Code\n */" );
}

/*
**text (*).**

*/
TEST_CASE( "144" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/144.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 12 );
	REQUIRE( p->endLine() == 0 );

	REQUIRE( p->items().size() == 1 );

	REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

	auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

	REQUIRE( t->startColumn() == 2 );
	REQUIRE( t->startLine() == 0 );
	REQUIRE( t->endColumn() == 10 );
	REQUIRE( t->endLine() == 0 );

	REQUIRE( t->opts() == MD::BoldText );
	REQUIRE( t->text() == u8"text (*)." );
	REQUIRE( t->openStyles().size() == 1 );
	REQUIRE( t->openStyles().at( 0 ) == MD::StyleDelim{ MD::BoldText, 0, 0, 1, 0 } );
	REQUIRE( t->closeStyles().size() == 1 );
	REQUIRE( t->closeStyles().at( 0 ) == MD::StyleDelim{ MD::BoldText, 11, 0, 12, 0 } );
}

/*
text******text***

*/
TEST_CASE( "145" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/145.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 16 );
	REQUIRE( p->endLine() == 0 );

	REQUIRE( p->items().size() == 2 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 0 );
		REQUIRE( t->endColumn() == 6 );
		REQUIRE( t->endLine() == 0 );

		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"text***" );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );

		REQUIRE( t->startColumn() == 10 );
		REQUIRE( t->startLine() == 0 );
		REQUIRE( t->endColumn() == 13 );
		REQUIRE( t->endLine() == 0 );

		REQUIRE( t->opts() == ( MD::BoldText | MD::ItalicText ) );
		REQUIRE( t->text() == u8"text" );
		REQUIRE( t->openStyles().size() == 2 );
		REQUIRE( t->openStyles().at( 0 ) == MD::StyleDelim{ MD::ItalicText, 7, 0, 7, 0 } );
		REQUIRE( t->openStyles().at( 1 ) == MD::StyleDelim{ MD::BoldText, 8, 0, 9, 0 } );
		REQUIRE( t->closeStyles().size() == 2 );
		REQUIRE( t->closeStyles().at( 0 ) == MD::StyleDelim{ MD::BoldText, 14, 0, 15, 0 } );
		REQUIRE( t->closeStyles().at( 1 ) == MD::StyleDelim{ MD::ItalicText, 16, 0, 16, 0 } );
	}
}

/*
******text***

*/
TEST_CASE( "146" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/146.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 12 );
	REQUIRE( p->endLine() == 0 );

	REQUIRE( p->items().size() == 2 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

		REQUIRE( t->startColumn() == 0 );
		REQUIRE( t->startLine() == 0 );
		REQUIRE( t->endColumn() == 2 );
		REQUIRE( t->endLine() == 0 );

		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"***" );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );

		REQUIRE( t->startColumn() == 6 );
		REQUIRE( t->startLine() == 0 );
		REQUIRE( t->endColumn() == 9 );
		REQUIRE( t->endLine() == 0 );

		REQUIRE( t->opts() == ( MD::BoldText | MD::ItalicText ) );
		REQUIRE( t->text() == u8"text" );
		REQUIRE( t->openStyles().size() == 2 );
		REQUIRE( t->openStyles().at( 0 ) == MD::StyleDelim{ MD::ItalicText, 3, 0, 3, 0 } );
		REQUIRE( t->openStyles().at( 1 ) == MD::StyleDelim{ MD::BoldText, 4, 0, 5, 0 } );
		REQUIRE( t->closeStyles().size() == 2 );
		REQUIRE( t->closeStyles().at( 0 ) == MD::StyleDelim{ MD::BoldText, 10, 0, 11, 0 } );
		REQUIRE( t->closeStyles().at( 1 ) == MD::StyleDelim{ MD::ItalicText, 12, 0, 12, 0 } );
	}
}

/*
***text******

*/
TEST_CASE( "147" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/147.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::Paragraph );

	auto p = static_cast< MD::Paragraph< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( p->startColumn() == 0 );
	REQUIRE( p->startLine() == 0 );
	REQUIRE( p->endColumn() == 12 );
	REQUIRE( p->endLine() == 0 );

	REQUIRE( p->items().size() == 2 );

	{
		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 0 ).get() );

		REQUIRE( t->startColumn() == 3 );
		REQUIRE( t->startLine() == 0 );
		REQUIRE( t->endColumn() == 6 );
		REQUIRE( t->endLine() == 0 );

		REQUIRE( t->opts() == ( MD::BoldText | MD::ItalicText ) );
		REQUIRE( t->text() == u8"text" );
		REQUIRE( t->openStyles().size() == 2 );
		REQUIRE( t->openStyles().at( 0 ) == MD::StyleDelim{ MD::ItalicText, 0, 0, 0, 0 } );
		REQUIRE( t->openStyles().at( 1 ) == MD::StyleDelim{ MD::BoldText, 1, 0, 2, 0 } );
		REQUIRE( t->closeStyles().size() == 2 );
		REQUIRE( t->closeStyles().at( 0 ) == MD::StyleDelim{ MD::BoldText, 7, 0, 8, 0 } );
		REQUIRE( t->closeStyles().at( 1 ) == MD::StyleDelim{ MD::ItalicText, 9, 0, 9, 0 } );
	}

	{
		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );

		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );

		REQUIRE( t->startColumn() == 10 );
		REQUIRE( t->startLine() == 0 );
		REQUIRE( t->endColumn() == 12 );
		REQUIRE( t->endLine() == 0 );

		REQUIRE( t->opts() == MD::TextWithoutFormat );
		REQUIRE( t->text() == u8"***" );
	}
}

/*
  - [link](#link) **text**

    - `item 1`
    - `item 2`

    <br />

    ```
    code
    ```

*/
TEST_CASE( "148" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/148.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 1 ).get() );

	REQUIRE( l->startColumn() == 2 );
	REQUIRE( l->startLine() == 0 );
	REQUIRE( l->endColumn() == 6 );
	REQUIRE( l->endLine() == 9 );

	REQUIRE( l->items().size() == 1 );

	REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );

	auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );

	REQUIRE( li->items().size() == 4 );
	REQUIRE( li->delim() == MD::WithPosition{ 2, 0, 2, 0 } );

	REQUIRE( li->items().at( 0 )->type() == MD::ItemType::Paragraph );

	{
		auto p = static_cast< MD::Paragraph< TRAIT >* > ( li->items().at( 0 ).get() );
		REQUIRE( p->startColumn() == 4 );
		REQUIRE( p->startLine() == 0 );
		REQUIRE( p->endColumn() == 25 );
		REQUIRE( p->endLine() == 0 );

		REQUIRE( p->items().size() == 2 );

		REQUIRE( p->items().at( 0 )->type() == MD::ItemType::Link );
		auto l = static_cast< MD::Link< TRAIT >* > ( p->items().at( 0 ).get() );
		REQUIRE( l->startColumn() == 4 );
		REQUIRE( l->startLine() == 0 );
		REQUIRE( l->endColumn() == 16 );
		REQUIRE( l->endLine() == 0 );
		REQUIRE( l->textPos() == MD::WithPosition{ 5, 0, 8, 0 } );
		REQUIRE( l->urlPos() == MD::WithPosition{ 11, 0, 15, 0 } );

		REQUIRE( p->items().at( 1 )->type() == MD::ItemType::Text );
		auto t = static_cast< MD::Text< TRAIT >* > ( p->items().at( 1 ).get() );
		REQUIRE( t->startColumn() == 20 );
		REQUIRE( t->startLine() == 0 );
		REQUIRE( t->endColumn() == 23 );
		REQUIRE( t->endLine() == 0 );
		REQUIRE( t->opts() == MD::BoldText );
		REQUIRE( t->text() == u8"text" );
		REQUIRE( t->isSpaceBefore() );
		REQUIRE( t->isSpaceAfter() );
		REQUIRE( t->openStyles().size() == 1 );
		REQUIRE( t->openStyles().at( 0 ) == MD::StyleDelim{ MD::BoldText, 18, 0, 19, 0 } );
		REQUIRE( t->closeStyles().size() == 1 );
		REQUIRE( t->closeStyles().at( 0 ) == MD::StyleDelim{ MD::BoldText, 24, 0, 25, 0 } );
	}

	REQUIRE( li->items().at( 1 )->type() == MD::ItemType::List );

	{
		auto l = static_cast< MD::List< TRAIT >* > ( li->items().at( 1 ).get() );
		REQUIRE( l->startColumn() == 4 );
		REQUIRE( l->startLine() == 2 );
		REQUIRE( l->endColumn() == 13 );
		REQUIRE( l->endLine() == 3 );
	}

	REQUIRE( li->items().at( 2 )->type() == MD::ItemType::RawHtml );

	REQUIRE( li->items().at( 3 )->type() == MD::ItemType::Code );

	auto c = static_cast< MD::Code< TRAIT >* > ( li->items().at( 3 ).get() );
	REQUIRE( c->startColumn() == 4 );
	REQUIRE( c->startLine() == 8 );
	REQUIRE( c->endColumn() == 7 );
	REQUIRE( c->endLine() == 8 );
	REQUIRE( c->startDelim() == MD::WithPosition{ 4, 7, 6, 7 } );
	REQUIRE( c->endDelim() == MD::WithPosition{ 4, 9, 6, 9 } );
	REQUIRE( c->syntaxPos() == MD::WithPosition{ -1, -1, -1, -1 } );
	REQUIRE( c->text() == u8"code" );
}

/*
* list
> quote

*/
TEST_CASE( "149" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/149.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 3 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Blockquote );
	auto b = static_cast< MD::Blockquote< TRAIT >* > ( doc->items().at( 2 ).get() );
	REQUIRE( b->delims() == MD::Blockquote< TRAIT >::Delims{ { 0, 1, 0, 1 } } );
}

/*
* list
  > quote

*/
TEST_CASE( "150" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/150.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 2 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
}

/*
* list
> quote
* list
# head
* list
  > quote
* list
  # head

*/
TEST_CASE( "151" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/151.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 6 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Blockquote );
	auto b = static_cast< MD::Blockquote< TRAIT >* > ( doc->items().at( 2 ).get() );
	REQUIRE( b->delims() == MD::Blockquote< TRAIT >::Delims{ { 0, 1, 0, 1 } } );
	REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::List );
	REQUIRE( doc->items().at( 4 )->type() == MD::ItemType::Heading );

	{
		auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 4 ).get() );
		REQUIRE( h->delims().size() == 1 );
		REQUIRE( h->delims().front() == MD::WithPosition{ 0, 3, 0, 3 } );
	}

	REQUIRE( doc->items().at( 5 )->type() == MD::ItemType::List );
	auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 5 ).get() );
	REQUIRE( l->items().size() == 2 );

	{
		REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
		REQUIRE( li->items().size() == 2 );
		REQUIRE( li->items().at( 1 )->type() == MD::ItemType::Blockquote );
		auto b = static_cast< MD::Blockquote< TRAIT >* > ( li->items().at( 1 ).get() );
		REQUIRE( b->delims() == MD::Blockquote< TRAIT >::Delims{ { 2, 5, 2, 5 } } );
	}

	{
		REQUIRE( l->items().at( 1 )->type() == MD::ItemType::ListItem );
		auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 1 ).get() );
		REQUIRE( li->items().size() == 2 );
		REQUIRE( li->items().at( 1 )->type() == MD::ItemType::Heading );
		auto h = static_cast< MD::Heading< TRAIT >* > ( li->items().at( 1 ).get() );
		REQUIRE( h->delims().size() == 1 );
		REQUIRE( h->delims().front() == MD::WithPosition{ 2, 7, 2, 7 } );
	}
}

/*
* list

> quote

* list

# head

* list

  > quote

* list

  # head

*/
TEST_CASE( "152" )
{
	MD::Parser< TRAIT > parser;

	auto doc = parser.parse( "tests/parser/data/152.md" );

	REQUIRE( doc->isEmpty() == false );
	REQUIRE( doc->items().size() == 6 );

	REQUIRE( doc->items().at( 1 )->type() == MD::ItemType::List );
	REQUIRE( doc->items().at( 2 )->type() == MD::ItemType::Blockquote );

	{
		auto b = static_cast< MD::Blockquote< TRAIT >* > ( doc->items().at( 2 ).get() );
		REQUIRE( b->delims() == MD::Blockquote< TRAIT >::Delims{ { 0, 2, 0, 2 } } );
	}

	REQUIRE( doc->items().at( 3 )->type() == MD::ItemType::List );

	REQUIRE( doc->items().at( 4 )->type() == MD::ItemType::Heading );
	auto h = static_cast< MD::Heading< TRAIT >* > ( doc->items().at( 4 ).get() );
	REQUIRE( h->delims().size() == 1 );
	REQUIRE( h->delims().front() == MD::WithPosition{ 0, 6, 0, 6 } );
	REQUIRE( doc->items().at( 5 )->type() == MD::ItemType::List );

	{
		auto l = static_cast< MD::List< TRAIT >* > ( doc->items().at( 5 ).get() );
		REQUIRE( l->items().size() == 2 );

		{
			REQUIRE( l->items().at( 0 )->type() == MD::ItemType::ListItem );
			auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 0 ).get() );
			REQUIRE( li->items().size() == 2 );
			REQUIRE( li->items().at( 1 )->type() == MD::ItemType::Blockquote );
			auto b = static_cast< MD::Blockquote< TRAIT >* > ( li->items().at( 1 ).get() );
			REQUIRE( b->delims() == MD::Blockquote< TRAIT >::Delims{ { 2, 10, 2, 10 } } );
		}

		{
			REQUIRE( l->items().at( 1 )->type() == MD::ItemType::ListItem );
			auto li = static_cast< MD::ListItem< TRAIT >* > ( l->items().at( 1 ).get() );
			REQUIRE( li->items().size() == 2 );
			REQUIRE( li->items().at( 1 )->type() == MD::ItemType::Heading );
			auto h = static_cast< MD::Heading< TRAIT >* > ( li->items().at( 1 ).get() );
			REQUIRE( h->delims().size() == 1 );
			REQUIRE( h->delims().front() == MD::WithPosition{ 2, 14, 2, 14 } );
		}
	}
}
