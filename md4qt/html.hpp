/*
	SPDX-FileCopyrightText: 2022-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

#ifndef MD4QT_MD_HTML_HPP_INCLUDED
#define MD4QT_MD_HTML_HPP_INCLUDED

// md4qt include.
#include "doc.hpp"
#include "visitor.hpp"

// C++ include.
#include <string>
#include <utility>


namespace MD {

namespace details {

template< class Trait >
typename Trait::String
headingIdToHtml( Heading< Trait > * h )
{
	typename Trait::String html;

	if( h->isLabeled() )
	{
		html.push_back( Trait::latin1ToString( " id=\"" ) );
		auto label = h->label();
		if( label.startsWith( Trait::latin1ToString( "#" ) ) )
			label.remove( 0, 1 );
		html.push_back( label );
		html.push_back( Trait::latin1ToString( "\"" ) );
	}

	return html;
}

template< class Trait >
typename Trait::String
prepareTextForHtml( const typename Trait::String & t )
{
	auto tmp = t;
	tmp.replace( Trait::latin1ToChar( '&' ), Trait::latin1ToString( "&amp;" ) );
	tmp.replace( Trait::latin1ToChar( '<' ), Trait::latin1ToString( "&lt;" ) );
	tmp.replace( Trait::latin1ToChar( '>' ), Trait::latin1ToString( "&gt;" ) );

	return tmp;
}

template< class Trait >
typename Trait::String
tableAlignmentToHtml( typename Table< Trait >::Alignment a )
{
	typename Trait::String html;

	switch( a )
	{
		case Table< Trait >::AlignLeft :
			html.push_back( Trait::latin1ToString( " align=\"left\"" ) );
			break;

		case Table< Trait >::AlignCenter :
			html.push_back( Trait::latin1ToString( " align=\"center\"" ) );
			break;

		case Table< Trait >::AlignRight :
			html.push_back( Trait::latin1ToString( " align=\"right\"" ) );
			break;

		default :
			break;
	}

	return html;
}


//
// HtmlVisitor
//

//! HTML visitor interface to walk through Document.
template< class Trait >
class HtmlVisitor
	:	public Visitor< Trait >
{
public:
	HtmlVisitor() = default;
	~HtmlVisitor() override = default;

	virtual typename Trait::String toHtml( std::shared_ptr< Document< Trait > > doc,
		const typename Trait::String & hrefForRefBackImage,
		bool wrappedInArticle = true )
	{
		isWrappedInArticle = wrappedInArticle;

		html.clear();
		fns.clear();

		this->process( doc );

		onFootnotes( hrefForRefBackImage );

		return html;
	}

protected:
	virtual void openStyle( const typename ItemWithOpts< Trait >::Styles & styles )
	{
		for( const auto & s : styles )
		{
			switch( s.style() )
			{
				case TextOption::BoldText :
					html.push_back( Trait::latin1ToString( "<strong>" ) );
					break;

				case TextOption::ItalicText :
					html.push_back( Trait::latin1ToString( "<em>" ) );
					break;

				case TextOption::StrikethroughText :
					html.push_back( Trait::latin1ToString( "<del>" ) );
					break;

				default :
					break;
			}
		}
	}

	virtual void closeStyle( const typename ItemWithOpts< Trait >::Styles & styles )
	{
		for( const auto & s : styles )
		{
			switch( s.style() )
			{
				case TextOption::BoldText :
					html.push_back( Trait::latin1ToString( "</strong>" ) );
					break;

				case TextOption::ItalicText :
					html.push_back( Trait::latin1ToString( "</em>" ) );
					break;

				case TextOption::StrikethroughText :
					html.push_back( Trait::latin1ToString( "</del>" ) );
					break;

				default :
					break;
			}
		}
	}

	void onAddLineEnding() override
	{
		if( !justCollectFootnoteRefs )
			html.push_back( Trait::latin1ToString( "\n" ) );
	}

	void onText(
		//! Text.
		Text< Trait > * t ) override
	{
		if( !justCollectFootnoteRefs )
		{
			openStyle( t->openStyles() );

			if( t->isSpaceBefore() )
				html.push_back( Trait::latin1ToString( " " ) );

			html.push_back( prepareTextForHtml< Trait >( t->text() ) );

			if( t->isSpaceAfter() )
				html.push_back( Trait::latin1ToString( " " ) );

			closeStyle( t->closeStyles() );
		}
	}

	void onMath(
		//! Math.
		Math< Trait > * m ) override
	{
		if( !justCollectFootnoteRefs )
		{
			openStyle( m->openStyles() );

			html.push_back( m->isInline() ? Trait::latin1ToString( "$ " ) :
				Trait::latin1ToString( "$$ " ) );
			html.push_back( prepareTextForHtml< Trait >( m->expr() ) );
			html.push_back( m->isInline() ? Trait::latin1ToString( " $" ) :
				Trait::latin1ToString( " $$" ) );

			closeStyle( m->closeStyles() );
		}
	}

	void onLineBreak(
		//! Linebreak.
		LineBreak< Trait > * ) override
	{
		if( !justCollectFootnoteRefs )
			html.push_back( Trait::latin1ToString( "<br />" ) );
	}

	void onParagraph(
		//! Paragraph.
		Paragraph< Trait > * p,
		//! Wrap this paragraph with something or no? It's useful to not wrap standalone
		//! paragraph in list item, for example.
		bool wrap ) override
	{
		if( wrap && !justCollectFootnoteRefs )
			html.push_back( Trait::latin1ToString( "<p>" ) );

		Visitor< Trait >::onParagraph( p, wrap );

		if( wrap && !justCollectFootnoteRefs )
			html.push_back( Trait::latin1ToString( "</p>" ) );
	}

	void onHeading(
		//! Heading.
		Heading< Trait > * h ) override
	{
		if( !justCollectFootnoteRefs )
			html.push_back( Trait::latin1ToString( "\n" ) );

		switch( h->level() )
		{
			case 1 :
			case 2 :
			case 3 :
			case 4 :
			case 5 :
			case 6 :
				onHeading( h, Trait::latin1ToString( "h" ) +
					Trait::latin1ToString( std::to_string( h->level() ).c_str() ) );
				break;

			default :
				break;
		}

		if( !justCollectFootnoteRefs )
			html.push_back( Trait::latin1ToString( "\n" ) );
	}

	void onCode(
		//! Code.
		Code< Trait > * c ) override
	{
		if( !justCollectFootnoteRefs )
		{
			html.push_back( Trait::latin1ToString( "\n" ) );
			html.push_back( Trait::latin1ToString( "<pre><code" ) );

			if( !c->syntax().isEmpty() )
			{
				html.push_back( Trait::latin1ToString( " class=\"language-" ) );
				html.push_back( c->syntax() );
				html.push_back( Trait::latin1ToString( "\"" ) );
			}

			html.push_back( Trait::latin1ToString( ">" ) );
			html.push_back( prepareTextForHtml< Trait >( c->text() ) );
			html.push_back( Trait::latin1ToString( "</code></pre>" ) );
			html.push_back( Trait::latin1ToString( "\n" ) );
		}
	}

	void onInlineCode(
		//! Code.
		Code< Trait > * c ) override
	{
		if( !justCollectFootnoteRefs )
		{
			openStyle( c->openStyles() );

			html.push_back( Trait::latin1ToString( "<code>" ) );

			html.push_back( prepareTextForHtml< Trait >( c->text() ) );

			html.push_back( Trait::latin1ToString( "</code>" ) );

			closeStyle( c->closeStyles() );
		}
	}

	void onBlockquote(
		//! Blockquote.
		Blockquote< Trait > * b ) override
	{
		if( !justCollectFootnoteRefs )
			html.push_back( Trait::latin1ToString( "\n<blockquote>" ) );

		Visitor< Trait >::onBlockquote( b );

		if( !justCollectFootnoteRefs )
			html.push_back( Trait::latin1ToString( "</blockquote>\n" ) );
	}

	void onList(
		//! List.
		List< Trait > * l ) override
	{
		if( !justCollectFootnoteRefs )
			html.push_back( Trait::latin1ToString( "\n" ) );

		typename ListItem< Trait >::ListType type = ListItem< Trait >::Ordered;
		bool first = true;

		for( auto it = l->items().cbegin(), last = l->items().cend(); it != last; ++it )
		{
			if( (*it)->type() == ItemType::ListItem )
			{
				auto * item = static_cast< ListItem< Trait >* > ( it->get() );

				if( first )
				{
					type = item->listType();

					if( type == ListItem< Trait >::Ordered )
					{
						if( !justCollectFootnoteRefs )
						{
							html.push_back( Trait::latin1ToString( "<ol" ) );

							if( item->isTaskList() )
								html.push_back(
									Trait::latin1ToString( " class=\"contains-task-list\"" ) );

							html.push_back( Trait::latin1ToString( ">\n" ) );
						}
					}
					else
					{
						if( !justCollectFootnoteRefs )
						{
							html.push_back( Trait::latin1ToString( "<ul" ) );

							if( item->isTaskList() )
								html.push_back(
									Trait::latin1ToString( " class=\"contains-task-list\"" ) );

							html.push_back( Trait::latin1ToString( ">\n" ) );
						}
					}
				}
				else
				{
					if( item->listType() != type )
					{
						if( !justCollectFootnoteRefs )
						{
							if( type == ListItem< Trait >::Ordered )
								html.push_back( Trait::latin1ToString( "</ol><ul>\n" ) );
							else
								html.push_back( Trait::latin1ToString( "</ul><ol>\n" ) );
						}

						type = item->listType();
					}
				}

				onListItem( item, first );

				first = false;
			}
		}

		if( !first )
		{
			if( !justCollectFootnoteRefs )
			{
				if( type == ListItem< Trait >::Ordered )
					html.push_back( Trait::latin1ToString( "</ol>\n" ) );
				else
					html.push_back( Trait::latin1ToString( "</ul>\n" ) );
			}
		}
	}

	void onTable(
		//! Table.
		Table< Trait > * t ) override
	{
		if( !justCollectFootnoteRefs )
			html.push_back( Trait::latin1ToString( "\n" ) );

		if( !t->isEmpty() )
		{
			if( !justCollectFootnoteRefs )
				html.push_back( Trait::latin1ToString( "<table><thead><tr>\n" ) );

			int columns = 0;

			for( auto th = (*t->rows().cbegin())->cells().cbegin(),
				last = (*t->rows().cbegin())->cells().cend(); th != last; ++th )
			{
				if( !justCollectFootnoteRefs )
				{
					html.push_back( Trait::latin1ToString( "<th" ) );
					html.push_back( tableAlignmentToHtml< Trait >( t->columnAlignment( columns ) ) );
					html.push_back( Trait::latin1ToString( ">\n" ) );
				}

				this->onTableCell( th->get() );

				if( !justCollectFootnoteRefs )
					html.push_back( Trait::latin1ToString( "\n</th>\n" ) );

				++columns;
			}

			if( !justCollectFootnoteRefs )
				html.push_back( Trait::latin1ToString( "</tr></thead><tbody>\n" ) );

			for( auto r = std::next( t->rows().cbegin() ), rlast = t->rows().cend(); r != rlast; ++r )
			{
				if( !justCollectFootnoteRefs )
					html.push_back( Trait::latin1ToString( "<tr>\n" ) );

				int i = 0;

				for( auto c = (*r)->cells().cbegin(), clast = (*r)->cells().cend(); c != clast; ++c )
				{
					if( !justCollectFootnoteRefs )
					{
						html.push_back( Trait::latin1ToString( "\n<td" ) );
						html.push_back( tableAlignmentToHtml< Trait >( t->columnAlignment( i ) ) );
						html.push_back( Trait::latin1ToString( ">\n" ) );
					}

					this->onTableCell( c->get() );

					if( !justCollectFootnoteRefs )
						html.push_back( Trait::latin1ToString( "\n</td>\n" ) );

					++i;

					if( i == columns )
						break;
				}

				if( !justCollectFootnoteRefs )
				{
					for( ; i < columns; ++i )
						html.push_back( Trait::latin1ToString( "<td></td>" ) );

					html.push_back( Trait::latin1ToString( "\n</tr>\n" ) );
				}
			}

			if( !justCollectFootnoteRefs )
				html.push_back( Trait::latin1ToString( "</tbody></table>" ) );
		}

		if( !justCollectFootnoteRefs )
			html.push_back( Trait::latin1ToString( "\n" ) );
	}

	void onAnchor(
		//! Anchor.
		Anchor< Trait > * a ) override
	{
		if( !justCollectFootnoteRefs && isWrappedInArticle )
		{
			html.push_back( Trait::latin1ToString( "\n<div id=\"" ) );
			html.push_back( a->label() );
			html.push_back( Trait::latin1ToString( "\"></div>\n" ) );
		}
	}

	void onRawHtml(
		//! Raw HTML.
		RawHtml< Trait > * h ) override
	{
		if( !justCollectFootnoteRefs )
		{
			openStyle( h->openStyles() );

			html.push_back( h->text() );

			closeStyle( h->closeStyles() );
		}
	}

	void onHorizontalLine(
		//! Horizontal line.
		HorizontalLine< Trait > * ) override
	{
		if( !justCollectFootnoteRefs )
			html.push_back( Trait::latin1ToString( "<hr />" ) );
	}

	void onLink(
		//! Link.
		Link< Trait > * l ) override
	{
		typename Trait::String url = l->url();

		const auto lit = this->doc->labeledLinks().find( url );

		if( lit != this->doc->labeledLinks().cend() )
			url = lit->second->url();

		if( std::find( this->anchors.cbegin(), this->anchors.cend(), url ) != this->anchors.cend() )
			url = Trait::latin1ToString( "#" ) + url;
		else if( url.startsWith( Trait::latin1ToString( "#" ) ) &&
			this->doc->labeledHeadings().find( url ) == this->doc->labeledHeadings().cend() )
		{
			auto path = static_cast< Anchor< Trait >* > ( this->doc->items().at( 0 ).get() )->label();
			const auto sp = path.lastIndexOf( Trait::latin1ToString( "/" ) );
			path.remove( sp, path.length() - sp );
			const auto p = url.indexOf( path ) - 1;
			url.remove( p, url.length() - p );
		}

		if( !justCollectFootnoteRefs )
		{
			openStyle( l->openStyles() );

			html.push_back( Trait::latin1ToString( "<a href=\"" ) );
			html.push_back( url );
			html.push_back( Trait::latin1ToString( "\">" ) );
		}

		if( l->p() && !l->p()->isEmpty() )
			onParagraph( l->p().get(), false );
		else if( !l->img()->isEmpty() )
		{
			if( !justCollectFootnoteRefs )
				onImage( l->img().get() );
		}
		else if( !l->text().isEmpty() )
		{
			if( !justCollectFootnoteRefs )
				html.push_back( prepareTextForHtml< Trait >( l->text() ) );
		}
		else
		{
			if( !justCollectFootnoteRefs )
				html.push_back( prepareTextForHtml< Trait >( l->url() ) );
		}

		if( !justCollectFootnoteRefs )
		{
			html.push_back( Trait::latin1ToString( "</a>" ) );

			closeStyle( l->closeStyles() );
		}
	}

	void onImage(
		//! Image.
		Image< Trait > * i ) override
	{
		if( !justCollectFootnoteRefs )
		{
			openStyle( i->openStyles() );

			html.push_back( Trait::latin1ToString( "<img src=\"" ) );
			html.push_back( i->url() );
			html.push_back( Trait::latin1ToString( "\" alt=\"" ) );
			html.push_back( prepareTextForHtml< Trait >( i->text() ) );
			html.push_back( Trait::latin1ToString( "\" style=\"max-width:100%;\" />" ) );

			closeStyle( i->closeStyles() );
		}
	}

	void onFootnoteRef(
		//! Footnote reference.
		FootnoteRef< Trait > * ref ) override
	{
		const auto fit = this->doc->footnotesMap().find( ref->id() );

		if( fit != this->doc->footnotesMap().cend() )
		{
			const auto r = std::find_if( fns.begin(), fns.end(),
				[&ref] ( const auto & stuff ) { return ref->id() == stuff.id; } );

			if( !justCollectFootnoteRefs )
			{
				openStyle( ref->openStyles() );

				html.push_back( Trait::latin1ToString( "<sup>" ) );
				html.push_back( Trait::latin1ToString( "<a href=\"#" ) );
				html.push_back( ref->id() );
				html.push_back( Trait::latin1ToString( "\" id=\"ref-" ) );
				html.push_back( ref->id() );
				html.push_back( Trait::latin1ToString( "-" ) );
			}

			if( r == fns.end() )
			{
				if( !justCollectFootnoteRefs )
					html.push_back( Trait::latin1ToString( "1" ) );
			}
			else
			{
				if( !justCollectFootnoteRefs )
					html.push_back( Trait::latin1ToString(
						std::to_string( ++( r->current ) ).c_str() ) );

				if( !dontIncrementFootnoteCount )
					++( r->count );
			}

			if( !justCollectFootnoteRefs )
				html.push_back( Trait::latin1ToString( "\">" ) );

			if( r == fns.end() )
			{
				if( !justCollectFootnoteRefs )
					html.push_back( Trait::latin1ToString(
						std::to_string( fns.size() + 1 ).c_str() ) );

				fns.push_back( { ref->id(), 1, 0 } );
			}
			else if( !justCollectFootnoteRefs )
				html.push_back( Trait::latin1ToString(
					std::to_string( std::distance( fns.begin(), r ) + 1 ).c_str() ) );

			if( !justCollectFootnoteRefs )
			{
				html.push_back( Trait::latin1ToString( "</a></sup>" ) );

				closeStyle( ref->closeStyles() );
			}
		}
		else
			onText( static_cast< Text< Trait>* > ( ref ) );
	}

	void onListItem(
		//! List item.
		ListItem< Trait > * i,
		//! Is this item first in the list?
		bool first ) override
	{
		if( !justCollectFootnoteRefs )
		{
			html.push_back( Trait::latin1ToString( "<li" ) );

			if( i->isTaskList() )
			{
				html.push_back( Trait::latin1ToString( " class=\"task-list-item\"><input "
					"type=\"checkbox\" id=\"\" disabled=\"\" class=\"task-list-item-checkbox\"" ) );

				if( i->isChecked() )
					html.push_back( Trait::latin1ToString( " checked=\"\"" ) );
			}

			if( i->listType() == ListItem< Trait >::Ordered && first )
			{
				html.push_back( Trait::latin1ToString( " value=\"" ) );
				html.push_back( Trait::latin1ToString(
					std::to_string( i->startNumber() ).c_str() ) );
				html.push_back( Trait::latin1ToString( "\"" ) );
			}

			html.push_back( Trait::latin1ToString( ">\n" ) );
		}

		Visitor< Trait >::onListItem( i, first );

		if( !justCollectFootnoteRefs )
			html.push_back( Trait::latin1ToString( "</li>\n" ) );
	}

	virtual void onHeading(
		//! Heading.
		Heading< Trait > * h,
		//! Heading tag.
		const typename Trait::String & ht )
	{
		if( !justCollectFootnoteRefs )
		{
			html.push_back( Trait::latin1ToString( "<" ) );
			html.push_back( ht );
			html.push_back( headingIdToHtml( h ) );
			html.push_back( Trait::latin1ToString( ">" ) );
		}

		if( h->text().get() )
			onParagraph( h->text().get(), false );

		if( !justCollectFootnoteRefs )
		{
			html.push_back( Trait::latin1ToString( "</" ) );
			html.push_back( ht );
			html.push_back( Trait::latin1ToString( ">" ) );
		}
	}

	virtual void onFootnotes( const typename Trait::String & hrefForRefBackImage )
	{
		if( !fns.empty() )
			html.push_back( Trait::latin1ToString( "<section class=\"footnotes\"><ol>" ) );

		int i = 1;

		const auto tmpFns = fns;
		justCollectFootnoteRefs = true;

		for( const auto & id : tmpFns )
		{
			const auto fit = this->doc->footnotesMap().find( id.id );

			if( fit != this->doc->footnotesMap().cend() )
				this->onFootnote( fit->second.get() );
		}

		justCollectFootnoteRefs = false;
		dontIncrementFootnoteCount = true;

		for( const auto & id : fns )
		{
			html.push_back( Trait::latin1ToString( "<li id=\"" ) );
			html.push_back( id.id );
			html.push_back( Trait::latin1ToString( "\">" ) );
			++i;

			const auto fit = this->doc->footnotesMap().find( id.id );

			if( fit != this->doc->footnotesMap().cend() )
			{
				this->onFootnote( fit->second.get() );

				if( !hrefForRefBackImage.isEmpty() )
				{
					typename Trait::String backRef;
					long long int backRefPos = html.endsWith( Trait::latin1ToString( "</p>" ) ) ? 4 : 0;

					for( long long int i = 1; i <= id.count; ++i )
					{
						backRef.push_back( Trait::latin1ToString( "<a href=\"#ref-" ) );
						backRef.push_back( id.id );
						backRef.push_back( Trait::latin1ToString( "-" ) );
						backRef.push_back( Trait::latin1ToString( std::to_string( i ).c_str() ) );
						backRef.push_back( Trait::latin1ToString( "\"><img src=\"" ) );
						backRef.push_back( hrefForRefBackImage );
						backRef.push_back( Trait::latin1ToString( "\" /></a>" ) );
					}

					html.insert( html.length() - backRefPos, backRef );
				}

				html.push_back( Trait::latin1ToString( "</li>" ) );
			}
		}

		if( !fns.empty() )
			html.push_back( Trait::latin1ToString( "</ol></section>\n" ) );
	}

protected:
	typename Trait::String html;
	//! Just collect footnote references?
	bool justCollectFootnoteRefs = false;
	//! Just process footnote references and don't increment count number.
	bool dontIncrementFootnoteCount = false;
	//! Is this HTML wrapped in artcile tag?
	bool isWrappedInArticle = true;

	struct FootnoteRefStuff {
		typename Trait::String id;
		long long int count = 0;
		long long int current = 0;
	};

	//! Vector of processed footnotes references.
	typename Trait::template Vector< FootnoteRefStuff > fns;
}; // class HtmlVisitor

} /* namespace details */


template< class Trait >
typename Trait::String
toHtml( std::shared_ptr< Document< Trait > > doc, bool wrapInBodyTag = true,
	const typename Trait::String & hrefForRefBackImage = {},
	bool wrapInArticle = true )
{
	typename Trait::String html;

	if( wrapInBodyTag )
		html.push_back( Trait::latin1ToString( "<!DOCTYPE html>\n<html><head></head><body>\n" ) );

	if( wrapInArticle )
		html.push_back( Trait::latin1ToString( "<article class=\"markdown-body\">" ) );

	details::HtmlVisitor< Trait > visitor;

	html.push_back( visitor.toHtml( doc, hrefForRefBackImage, wrapInArticle ) );

	if( wrapInArticle )
		html.push_back( Trait::latin1ToString( "</article>\n" ) );

	if( wrapInBodyTag )
		html.push_back( Trait::latin1ToString( "</body></html>\n" ) );

	return html;
}

} /* namespace MD */

#endif // MD4QT_MD_HTML_HPP_INCLUDED
