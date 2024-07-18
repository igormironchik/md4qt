
/*
	SPDX-FileCopyrightText: 2022-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

#include <md4qt/parser.hpp>
#include <md4qt/traits.hpp>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
// doctest include.
#include <doctest/doctest.h>

// C++ include.
#include <vector>

using data_t = std::vector< std::pair< std::pair< long long int, bool >, int > >;

namespace MD {

struct PrivateAccess {
	static std::pair< bool, size_t > checkEmphasisSequence( const data_t & s, size_t idx )
	{
		return p.checkEmphasisSequence( s, idx );
	}

	static Parser< TRAIT > p;
};

Parser< TRAIT > PrivateAccess::p = Parser< TRAIT > ();

} /* namespace MD */

TEST_CASE( "emphasis_sequence" )
{
	{
		const data_t d = { { { 2, false }, 1 }, { { 1, false }, 1 }, { { -2, false }, 1 } };

		bool closed = false;
		size_t idx = 0;
		std::tie( closed, idx ) = MD::PrivateAccess::checkEmphasisSequence( d, 0 );

		REQUIRE( !closed );

		std::tie( closed, idx ) = MD::PrivateAccess::checkEmphasisSequence( d, 1 );

		REQUIRE( closed );
		REQUIRE( idx == 2 );
	}

	{
		const data_t d = { { { 2, false }, 1 }, { { 2, false }, 1 }, { { -4, false }, 1 } };

		bool closed = false;
		size_t idx = 0;
		std::tie( closed, idx ) = MD::PrivateAccess::checkEmphasisSequence( d, 0 );

		REQUIRE( closed );
		REQUIRE( idx == 2 );

		std::tie( closed, idx ) = MD::PrivateAccess::checkEmphasisSequence( d, 1 );

		REQUIRE( closed );
		REQUIRE( idx == 2 );
	}

	{
		const data_t d = { { { 2, false }, 1 }, { { 2, false }, 1 }, { { 1, false }, 1 },
			{ { -4, false }, 1 } };

		bool closed = false;
		size_t idx = 0;
		std::tie( closed, idx ) = MD::PrivateAccess::checkEmphasisSequence( d, 0 );

		REQUIRE( !closed );

		std::tie( closed, idx ) = MD::PrivateAccess::checkEmphasisSequence( d, 1 );

		REQUIRE( closed );
		REQUIRE( idx == 3 );

		std::tie( closed, idx ) = MD::PrivateAccess::checkEmphasisSequence( d, 2 );

		REQUIRE( closed );
		REQUIRE( idx == 3 );
	}

	{
		const data_t d = { { { 2, false }, 1 }, { { 2, false }, 1 }, { { 1, false }, 1 },
			{ { -5, false }, 1 } };

		bool closed = false;
		size_t idx = 0;
		std::tie( closed, idx ) = MD::PrivateAccess::checkEmphasisSequence( d, 0 );

		REQUIRE( closed );
		REQUIRE( idx == 3 );

		std::tie( closed, idx ) = MD::PrivateAccess::checkEmphasisSequence( d, 1 );

		REQUIRE( closed );
		REQUIRE( idx == 3 );

		std::tie( closed, idx ) = MD::PrivateAccess::checkEmphasisSequence( d, 2 );

		REQUIRE( closed );
		REQUIRE( idx == 3 );
	}

	{
		const data_t d = { { { 2, false }, 0 }, { { 2, false }, 2 }, { { 1, false }, 1 },
			{ { -1, false }, 1 }, { { 2, false }, 2 }, { { -2, false }, 0 } };

		bool closed = false;
		size_t idx = 0;
		std::tie( closed, idx ) = MD::PrivateAccess::checkEmphasisSequence( d, 0 );

		REQUIRE( closed );
		REQUIRE( idx == 5 );

		std::tie( closed, idx ) = MD::PrivateAccess::checkEmphasisSequence( d, 1 );

		REQUIRE( !closed );

		std::tie( closed, idx ) = MD::PrivateAccess::checkEmphasisSequence( d, 2 );

		REQUIRE( closed );
		REQUIRE( idx == 3 );
	}

	{
		const data_t d = { { { 2, false }, 0 }, { { 2, false }, 2 }, { { 1, false }, 1 },
			{ { 1, false }, 1 }, { { -2, false }, 2 }, { { -2, false }, 0 } };

		bool closed = false;
		size_t idx = 0;
		std::tie( closed, idx ) = MD::PrivateAccess::checkEmphasisSequence( d, 0 );

		REQUIRE( closed );
		REQUIRE( idx == 5 );

		std::tie( closed, idx ) = MD::PrivateAccess::checkEmphasisSequence( d, 1 );

		REQUIRE( closed );
		REQUIRE( idx == 4 );

		std::tie( closed, idx ) = MD::PrivateAccess::checkEmphasisSequence( d, 2 );

		REQUIRE( !closed );
	}

	{
		const data_t d = { { { 2, false }, 0 }, { { 2, false }, 1 }, { { 1, false }, 2 },
			{ { -1, false }, 2 }, { { -2, false }, 1 }, { { -2, false }, 0 } };

		bool closed = false;
		size_t idx = 0;
		std::tie( closed, idx ) = MD::PrivateAccess::checkEmphasisSequence( d, 0 );

		REQUIRE( closed );
		REQUIRE( idx == 5 );

		std::tie( closed, idx ) = MD::PrivateAccess::checkEmphasisSequence( d, 1 );

		REQUIRE( closed );
		REQUIRE( idx == 4 );

		std::tie( closed, idx ) = MD::PrivateAccess::checkEmphasisSequence( d, 2 );

		REQUIRE( closed );
		REQUIRE( idx == 3 );
	}

	{
		const data_t d = { { { 2, false }, 0 }, { { 2, false }, 1 }, { { 1, false }, 2 },
			{ { -2, false }, 0 }, { { -1, false }, 2 }, { { -2, false }, 1 } };

		bool closed = false;
		size_t idx = 0;
		std::tie( closed, idx ) = MD::PrivateAccess::checkEmphasisSequence( d, 0 );

		REQUIRE( closed );
		REQUIRE( idx == 3 );

		std::tie( closed, idx ) = MD::PrivateAccess::checkEmphasisSequence( d, 1 );

		REQUIRE( !closed );

		std::tie( closed, idx ) = MD::PrivateAccess::checkEmphasisSequence( d, 2 );

		REQUIRE( !closed );
	}

	{
		const data_t d = { { { 1, false }, 0 }, { { 2, false }, 0 }, { { -2, false }, 0 },
			{ { -1, false }, 0 } };

		bool closed = false;
		size_t idx = 0;
		std::tie( closed, idx ) = MD::PrivateAccess::checkEmphasisSequence( d, 0 );

		REQUIRE( closed );
		REQUIRE( idx == 3 );

		std::tie( closed, idx ) = MD::PrivateAccess::checkEmphasisSequence( d, 1 );

		REQUIRE( closed );
		REQUIRE( idx == 2 );
	}

	{
		const data_t d = { { { 2, false }, 0 }, { { 2, false }, 1 }, { { -2, false }, 1 },
			{ { 1, false }, 2 }, { { -2, false }, 0 }, { { -1, false }, 2 } };

		bool closed = false;
		size_t idx = 0;
		std::tie( closed, idx ) = MD::PrivateAccess::checkEmphasisSequence( d, 0 );

		REQUIRE( closed );
		REQUIRE( idx == 4 );

		std::tie( closed, idx ) = MD::PrivateAccess::checkEmphasisSequence( d, 1 );

		REQUIRE( closed );
		REQUIRE( idx == 2 );

		std::tie( closed, idx ) = MD::PrivateAccess::checkEmphasisSequence( d, 2 );

		REQUIRE( !closed );
	}

	{
		const data_t d = { { { 1, false }, 1 }, { { 1, false }, 1 }, { { -2, false }, 1 } };

		bool closed = false;
		size_t idx = 0;
		std::tie( closed, idx ) = MD::PrivateAccess::checkEmphasisSequence( d, 0 );

		REQUIRE( closed );
		REQUIRE( idx == 2 );

		std::tie( closed, idx ) = MD::PrivateAccess::checkEmphasisSequence( d, 1 );

		REQUIRE( closed );
		REQUIRE( idx == 2 );
	}

	{
		const data_t d = { { { 2, false }, 1 }, { { -1, false }, 1 }, { { -1, false }, 1 } };

		bool closed = false;
		size_t idx = 0;
		std::tie( closed, idx ) = MD::PrivateAccess::checkEmphasisSequence( d, 0 );

		REQUIRE( closed );
		REQUIRE( idx == 2 );
	}

	{
		const data_t d = { { { 2, false }, 1 } };

		bool closed = false;
		size_t idx = 0;
		std::tie( closed, idx ) = MD::PrivateAccess::checkEmphasisSequence( d, 0 );

		REQUIRE( !closed );
	}

	{
		const data_t d = { { { 2, false }, 1 }, { { 1, false }, 0 }, { { 2, false }, 1 },
			{ { -4, false }, 1 } };

		bool closed = false;
		size_t idx = 0;
		std::tie( closed, idx ) = MD::PrivateAccess::checkEmphasisSequence( d, 0 );

		REQUIRE( closed );
		REQUIRE( idx == 3 );
	}

	{
		const data_t d = { { { 4, false }, 1 }, { { 1, false }, 0 }, { { -2, false }, 1 },
			{ { -2, false }, 1 } };

		bool closed = false;
		size_t idx = 0;
		std::tie( closed, idx ) = MD::PrivateAccess::checkEmphasisSequence( d, 0 );

		REQUIRE( closed );
		REQUIRE( idx == 3 );
	}

	{
		const data_t d = { { { 1, false }, 1 }, { { -2, true }, 1 }, { { -1, false }, 1 } };

		bool closed = false;
		size_t idx = 0;
		std::tie( closed, idx ) = MD::PrivateAccess::checkEmphasisSequence( d, 0 );

		REQUIRE( closed );
		REQUIRE( idx == 2 );
	}

	{
		const data_t d = { { { 2, false }, 0 }, { { -2, false }, 0 } };

		bool closed = false;
		size_t idx = 0;
		std::tie( closed, idx ) = MD::PrivateAccess::checkEmphasisSequence( d, 0 );

		REQUIRE( closed );
		REQUIRE( idx == 1 );

		std::tie( closed, idx ) = MD::PrivateAccess::checkEmphasisSequence( d, 1 );

		REQUIRE( !closed );
	}

	{
		const data_t d = { { { 2, false }, 0 }, { { -2, false }, 1 } };

		bool closed = false;
		size_t idx = 0;
		std::tie( closed, idx ) = MD::PrivateAccess::checkEmphasisSequence( d, 0 );

		REQUIRE( !closed );
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
	REQUIRE( MD::isHtmlComment< TRAIT >( u8"<!-- -->" ) );
	REQUIRE( MD::isHtmlComment< TRAIT >( u8"<!-- -- -->" ) );
	REQUIRE( MD::isHtmlComment< TRAIT >( u8"<!--My favorite operators are > and <!-->" ) );
	REQUIRE( MD::isHtmlComment< TRAIT >( u8"<!--My favorite operators are > and <!-->" ) );
	REQUIRE( !MD::isHtmlComment< TRAIT >( u8"<-- -->" ) );
	REQUIRE( !MD::isHtmlComment< TRAIT >( u8"<!-- --" ) );
	REQUIRE( !MD::isHtmlComment< TRAIT >( u8"<!-- -" ) );
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

TEST_CASE( "is_email" )
{
	REQUIRE( MD::isEmail< TRAIT > ( typename TRAIT::String( "igor@gmail.com" ) ) );
	REQUIRE( !MD::isEmail< TRAIT > ( typename TRAIT::String( "igor@gmail-.com" ) ) );
	REQUIRE( !MD::isEmail< TRAIT > ( typename TRAIT::String( "igor@-gmail.com" ) ) );

	static const auto i63 = typename TRAIT::String( 63, typename TRAIT::Char( 'i' ) );
	static const auto i64 = typename TRAIT::String( 64, typename TRAIT::Char( 'i' ) );

	static const auto okEmail = typename TRAIT::String( "igor@" ) + i63 +
		typename TRAIT::String( 1, typename TRAIT::Char( '.' ) ) + i63;
	REQUIRE( MD::isEmail< TRAIT > ( okEmail ) );

	static const auto wrongEmail = typename TRAIT::String( "igor@" ) + i64 +
		typename TRAIT::String( 1, typename TRAIT::Char( '.' ) ) + i63;
	REQUIRE( !MD::isEmail< TRAIT > ( wrongEmail ) );

	REQUIRE( !MD::isEmail< TRAIT > ( typename TRAIT::String( "i[]gor@gmail.com" ) ) );

	REQUIRE( MD::isEmail< TRAIT > ( typename TRAIT::String( "igor@gmail-gmail.com" ) ) );

	REQUIRE( !MD::isEmail< TRAIT > ( typename TRAIT::String( "igor@gmail-gmail." ) ) );
	REQUIRE( !MD::isEmail< TRAIT > ( typename TRAIT::String( "igor@gmail-gmail" ) ) );
	REQUIRE( !MD::isEmail< TRAIT > ( typename TRAIT::String( "igor@." ) ) );
	REQUIRE( MD::isEmail< TRAIT > ( typename TRAIT::String( "a@a.a" ) ) );
	REQUIRE( !MD::isEmail< TRAIT > ( typename TRAIT::String( "@a.a" ) ) );
	REQUIRE( !MD::isEmail< TRAIT > ( typename TRAIT::String( "@.a" ) ) );
	REQUIRE( !MD::isEmail< TRAIT > ( typename TRAIT::String( "@." ) ) );
}

#define INIT_VARS_FOR_OPTIMIZE_PARAGRAPH \
	std::shared_ptr< MD::Block< TRAIT > > parent = std::make_shared< MD::Paragraph< TRAIT > > (); \
	auto doc = std::make_shared< MD::Document< TRAIT > > (); \
	MD::MdBlock< TRAIT > fr; \
	typename TRAIT::StringList links; \
	MD::RawHtmlBlock< TRAIT > html; \
	const MD::TextPluginsMap< TRAIT > textPlugins; \
	 \
	MD::TextParsingOpts< TRAIT > po = { fr, parent, nullptr, doc, links, "", "", \
		false, false, html, textPlugins }; \
	 \
	auto p = std::make_shared< MD::Paragraph< TRAIT > > ();

TEST_CASE( "optimize_paragraph" )
{
	auto makeText = [] ( MD::TextParsingOpts< TRAIT > & po, std::shared_ptr< MD::Paragraph< TRAIT > > p,
		long long int line, int opts )
	{
		auto t = std::make_shared< MD::Text< TRAIT > > ();
		t->setText( "Text" );
		t->setStartColumn( 0 );
		t->setStartLine( line );
		t->setEndColumn( 0 );
		t->setEndLine( line );
		t->setSpaceBefore( false );
		t->setSpaceAfter( false );
		t->setOpts( opts );

		po.rawTextData.push_back( { "Text", 0, line, false, false } );

		p->appendItem( t );
	};

	auto makeCode = [] ( MD::TextParsingOpts< TRAIT > & po, std::shared_ptr< MD::Paragraph< TRAIT > > p,
		long long int line )
	{
		auto c = std::make_shared< MD::Code< TRAIT > > ( "code", false, true );
		c->setStartColumn( 0 );
		c->setStartLine( line );
		c->setEndColumn( 0 );
		c->setEndLine( line );

		p->appendItem( c );
	};

	auto makeHtml = [] ( MD::TextParsingOpts< TRAIT > & po, std::shared_ptr< MD::Paragraph< TRAIT > > p,
		long long int line, bool isFree )
	{
		auto h = std::make_shared< MD::RawHtml< TRAIT > > ();
		h->setStartColumn( 0 );
		h->setStartLine( line );
		h->setEndColumn( 0 );
		h->setEndLine( line );
		MD::UnprotectedDocsMethods< TRAIT >::setFreeTag( h, isFree );

		p->appendItem( h );
	};

	auto checkP = [] ( const std::string & d, std::shared_ptr< MD::Paragraph< TRAIT > > p )
	{
		REQUIRE( d.length() == p->items().size() );

		long long int i = 0;

		for( const auto & c : d )
		{
			switch( c )
			{
				case 'c' :
					REQUIRE( p->items().at( i )->type() == MD::ItemType::Code );
					break;

				case 't' :
					REQUIRE( p->items().at( i )->type() == MD::ItemType::Text );
					break;

				case 'h' :
					REQUIRE( p->items().at( i )->type() == MD::ItemType::RawHtml );
					break;

				default :
				{
					INFO( "Unknown type. This is an error in test..." );
					REQUIRE( false );
				}
					break;
			}

			++i;
		}
	};

	{
		INIT_VARS_FOR_OPTIMIZE_PARAGRAPH

		makeText( po, p, 0, MD::TextWithoutFormat );

		MD::optimizeParagraph( p );

		checkP( "t", p  );
	}

	{
		INIT_VARS_FOR_OPTIMIZE_PARAGRAPH

		makeCode( po, p, 0 );

		MD::optimizeParagraph( p );

		checkP( "c", p );
	}

	{
		INIT_VARS_FOR_OPTIMIZE_PARAGRAPH

		makeText( po, p, 0, MD::TextWithoutFormat );
		makeCode( po, p, 0 );
		makeText( po, p, 0, MD::TextWithoutFormat );

		MD::optimizeParagraph( p );

		checkP( "tct", p );
	}

	{
		INIT_VARS_FOR_OPTIMIZE_PARAGRAPH

		makeText( po, p, 0, MD::TextWithoutFormat );
		makeText( po, p, 0, MD::TextWithoutFormat );
		makeCode( po, p, 0 );
		makeText( po, p, 0, MD::TextWithoutFormat );
		makeText( po, p, 0, MD::TextWithoutFormat );
		makeText( po, p, 1, MD::TextWithoutFormat );

		MD::optimizeParagraph( p );

		checkP( "tctt", p );
	}

	{
		INIT_VARS_FOR_OPTIMIZE_PARAGRAPH

		makeText( po, p, 0, MD::TextWithoutFormat );
		makeText( po, p, 0, MD::ItalicText );
		makeCode( po, p, 0 );
		makeText( po, p, 0, MD::TextWithoutFormat );
		makeText( po, p, 0, MD::ItalicText );
		makeText( po, p, 1, MD::TextWithoutFormat );

		MD::optimizeParagraph( p );

		checkP( "ttcttt", p );
	}

	{
		INIT_VARS_FOR_OPTIMIZE_PARAGRAPH

		makeText( po, p, 0, MD::TextWithoutFormat );
		makeText( po, p, 1, MD::TextWithoutFormat );
		makeCode( po, p, 2 );
		makeText( po, p, 3, MD::TextWithoutFormat );
		makeText( po, p, 4, MD::TextWithoutFormat );
		makeText( po, p, 5, MD::TextWithoutFormat );

		MD::optimizeParagraph( p );

		checkP( "ttcttt", p );
	}

	{
		INIT_VARS_FOR_OPTIMIZE_PARAGRAPH

		makeText( po, p, 0, MD::TextWithoutFormat );
		makeText( po, p, 1, MD::TextWithoutFormat );
		makeHtml( po, p, 2, true );
		makeText( po, p, 3, MD::TextWithoutFormat );
		makeText( po, p, 4, MD::TextWithoutFormat );
		makeText( po, p, 5, MD::TextWithoutFormat );

		MD::optimizeParagraph( p );

		checkP( "tthttt", p );

		p = MD::splitParagraphsAndFreeHtml( parent, p, po, false );

		REQUIRE( parent->items().size() == 2 );
		REQUIRE( parent->items().at( 0 )->type() == MD::ItemType::Paragraph );
		REQUIRE( parent->items().at( 1 )->type() == MD::ItemType::RawHtml );

		checkP( "ttt", p );
	}

	{
		INIT_VARS_FOR_OPTIMIZE_PARAGRAPH

		makeText( po, p, 0, MD::TextWithoutFormat );
		makeText( po, p, 1, MD::TextWithoutFormat );
		makeHtml( po, p, 2, false );
		makeText( po, p, 3, MD::TextWithoutFormat );
		makeText( po, p, 4, MD::TextWithoutFormat );
		makeText( po, p, 5, MD::TextWithoutFormat );

		MD::optimizeParagraph( p );

		checkP( "tthttt", p );

		p = MD::splitParagraphsAndFreeHtml( parent, p, po, false );

		REQUIRE( parent->items().size() == 0 );

		checkP( "tthttt", p );
	}

	{
		INIT_VARS_FOR_OPTIMIZE_PARAGRAPH

		makeText( po, p, 0, MD::TextWithoutFormat );
		makeText( po, p, 0, MD::TextWithoutFormat );
		makeHtml( po, p, 0, true );
		makeText( po, p, 0, MD::TextWithoutFormat );
		makeText( po, p, 0, MD::TextWithoutFormat );
		makeText( po, p, 1, MD::TextWithoutFormat );

		MD::optimizeParagraph( p );

		checkP( "thtt", p );

		p = MD::splitParagraphsAndFreeHtml( parent, p, po, false );

		REQUIRE( parent->items().size() == 2 );
		REQUIRE( parent->items().at( 0 )->type() == MD::ItemType::Paragraph );
		REQUIRE( parent->items().at( 1 )->type() == MD::ItemType::RawHtml );

		checkP( "tt", p );
	}

	{
		INIT_VARS_FOR_OPTIMIZE_PARAGRAPH

		makeText( po, p, 0, MD::TextWithoutFormat );
		makeText( po, p, 0, MD::TextWithoutFormat );
		makeHtml( po, p, 0, false );
		makeText( po, p, 0, MD::TextWithoutFormat );
		makeText( po, p, 0, MD::TextWithoutFormat );
		makeText( po, p, 1, MD::TextWithoutFormat );

		MD::optimizeParagraph( p );

		checkP( "thtt", p );

		p = MD::splitParagraphsAndFreeHtml( parent, p, po, false );

		REQUIRE( parent->items().size() == 0 );

		checkP( "thtt", p );
	}
}

TEST_CASE( "virgin_substr" )
{
	MD::MdBlock< TRAIT > data;
	data.data.push_back( { typename TRAIT::String( "**text**" ), { 1 } } );
	data.data.push_back( { typename TRAIT::String( "__text__" ), { 2 } } );
	data.data.push_back( { typename TRAIT::String( "text" ), { 3 } } );
	data.data.push_back( { typename TRAIT::String( "~~text~~" ), { 4 } } );
	data.data.push_back( { typename TRAIT::String( "~text*" ), { 5 } } );

	REQUIRE( MD::virginSubstr< TRAIT > ( data, { 0, 0, 1, 0 } ).isEmpty() );
	REQUIRE( MD::virginSubstr< TRAIT > ( data, { 0, 1, 1, 1 } ) == u8"**" );
	REQUIRE( MD::virginSubstr< TRAIT > ( data, { 0, 1, 10, 1 } ) == u8"**text**" );
	REQUIRE( MD::virginSubstr< TRAIT > ( data, { 0, 2, 1, 2 } ) == u8"__" );
	REQUIRE( MD::virginSubstr< TRAIT > ( data, { 6, 1, 1, 2 } ) == u8"**\n__" );
	REQUIRE( MD::virginSubstr< TRAIT > ( data, { 0, 3, 0, 10 } ) == u8"text\n~~text~~\n~text*" );
	REQUIRE( MD::virginSubstr< TRAIT > ( data, { 0, 0, 100, 100 } ) == u8"**text**\n__text__\ntext\n~~text~~\n~text*" );

	data.data[ 1 ].first.remove( 0, 2 );

	REQUIRE( MD::virginSubstr< TRAIT > ( data, { 0, 1, 1, 1 } ) == u8"**" );
	REQUIRE( MD::virginSubstr< TRAIT > ( data, { 0, 1, 10, 1 } ) == u8"**text**" );
	REQUIRE( MD::virginSubstr< TRAIT > ( data, { 0, 2, 1, 2 } ).isEmpty() );
	REQUIRE( MD::virginSubstr< TRAIT > ( data, { 6, 1, 1, 2 } ) == u8"**\n" );
	REQUIRE( MD::virginSubstr< TRAIT > ( data, { 6, 1, 0, 2 } ) == u8"**\n" );
	REQUIRE( MD::virginSubstr< TRAIT > ( data, { 6, 1, 2, 2 } ) == u8"**\nt" );
	REQUIRE( MD::virginSubstr< TRAIT > ( data, { 0, 2, 10, 3 } ) == u8"text__\ntext" );
	REQUIRE( MD::virginSubstr< TRAIT > ( data, { 0, 0, 10, 2 } ) == u8"**text**\ntext__" );
	REQUIRE( MD::virginSubstr< TRAIT > ( data, { 0, 10, 0, 20 } ).isEmpty() );
	REQUIRE( MD::virginSubstr< TRAIT > ( data, { 0, 3, 7, 4 } ) == u8"text\n~~text~~" );

	{
		MD::MdBlock< TRAIT > dd;

		REQUIRE( MD::virginSubstr< TRAIT > ( dd, { 0, 3, 7, 4 } ).isEmpty() );
	}
}

TEST_CASE( "local_pos_from_virgin" )
{
	MD::MdBlock< TRAIT > data;
	data.data.push_back( { typename TRAIT::String( "**text**" ), { 1 } } );
	data.data.push_back( { typename TRAIT::String( "**text**" ), { 2 } } );
	data.data.push_back( { typename TRAIT::String( "**text**" ), { 3 } } );

	using pair = std::pair< long long int, long long int >;

	REQUIRE( MD::localPosFromVirgin< TRAIT > ( data, 0, 0 ) == pair{ -1, -1 } );
	REQUIRE( MD::localPosFromVirgin< TRAIT > ( data, 8, 1 ) == pair{ -1, -1 } );
	REQUIRE( MD::localPosFromVirgin< TRAIT > ( data, 0, 2 ) == pair{ 0, 1 } );
	REQUIRE( MD::localPosFromVirgin< TRAIT > ( data, 1, 1 ) == pair{ 1, 0 } );
	REQUIRE( MD::localPosFromVirgin< TRAIT > ( data, 0, 4 ) == pair{ -1, -1 } );

	data.data[ 0 ].first.remove( 0, 2 );

	REQUIRE( MD::localPosFromVirgin< TRAIT > ( data, 0, 1 ) == pair{ -1, -1 } );
	REQUIRE( MD::localPosFromVirgin< TRAIT > ( data, 2, 1 ) == pair{ 0, 0 } );
	REQUIRE( MD::localPosFromVirgin< TRAIT > ( data, 4, 1 ) == pair{ 2, 0 } );

	REQUIRE( MD::localPosFromVirgin< TRAIT > ( data, 1, 2 ) == pair{ 1, 1 } );
	REQUIRE( MD::localPosFromVirgin< TRAIT > ( data, 2, 3 ) == pair{ 2, 2 } );
	REQUIRE( MD::localPosFromVirgin< TRAIT > ( data, 100, 3 ) == pair{ -1, -1 } );

	{
		MD::MdBlock< TRAIT > dd;

		REQUIRE( MD::localPosFromVirgin< TRAIT > ( dd, 0, 0 ) == pair{ -1, -1 } );

		dd.data.push_back( { typename TRAIT::String( "" ), { 1 } } );

		REQUIRE( MD::localPosFromVirgin< TRAIT > ( dd, 0, 1 ) == pair{ -1, -1 } );
	}
}
