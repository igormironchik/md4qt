
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
		html.push_back( " id=\"" );
		auto label = h->label();
		if( label.startsWith( typename Trait::String( "#" ) ) )
			label.remove( 0, 1 );
		html.push_back( label );
		html.push_back( "\"" );
	}

	return html;
}

template< class Trait >
typename Trait::String
openTextStyleToHtml( int s )
{
	typename Trait::String html;

	if( s & TextOption::BoldText )
		html.push_back( "<strong>" );

	if( s & TextOption::ItalicText )
		html.push_back( "<em>" );

	if( s & TextOption::StrikethroughText )
		html.push_back( "<del>" );

	return html;
}

template< class Trait >
typename Trait::String
closeTextStyleToHtml( int s )
{
	typename Trait::String html;

	if( s & TextOption::StrikethroughText )
		html.push_back( "</del>" );

	if( s & TextOption::ItalicText )
		html.push_back( "</em>" );

	if( s & TextOption::BoldText )
		html.push_back( "</strong>" );

	return html;
}

template< class Trait >
typename Trait::String
prepareTextForHtml( const typename Trait::String & t )
{
	auto tmp = t;
	tmp.replace( typename Trait::Char( '&' ), "&amp;" );
	tmp.replace( typename Trait::Char( '<' ), "&lt;" );
	tmp.replace( typename Trait::Char( '>' ), "&gt;" );

	return tmp;
}

template< class Trait >
typename Trait::String
linkTextToHtml( const typename Trait::String & text, int style )
{
	typename Trait::String html;

	html.push_back( openTextStyleToHtml< Trait >( style ) );

	html.push_back( prepareTextForHtml< Trait >( text ) );

	html.push_back( closeTextStyleToHtml< Trait >( style ) );

	return html;
}

template< class Trait >
typename Trait::String
tableAlignmentToHtml( typename Table< Trait >::Alignment a )
{
	typename Trait::String html;

	switch( a )
	{
		case Table< Trait >::AlignLeft :
			html.push_back( " align=\"left\"" );
			break;

		case Table< Trait >::AlignCenter :
			html.push_back( " align=\"center\"" );
			break;

		case Table< Trait >::AlignRight :
			html.push_back( " align=\"right\"" );
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

	typename Trait::String toHtml( std::shared_ptr< Document< Trait > > doc,
		const typename Trait::String & hrefForRefBackImage )
	{
		html.clear();

		this->process( doc );

		onFootnotes( hrefForRefBackImage );

		return html;
	}

protected:
	void onAddLineEnding() override
	{
		if( !justCollectFootnoteRefs )
			html.push_back( "\n" );
	}

	void onText(
		//! Text.
		Text< Trait > * t ) override
	{
		if( !justCollectFootnoteRefs )
		{
			html.push_back( openTextStyleToHtml< Trait >( t->opts() ) );

			if( t->isSpaceBefore() )
				html.push_back( " " );

			html.push_back( prepareTextForHtml< Trait >( t->text() ) );

			if( t->isSpaceAfter() )
				html.push_back( " " );

			html.push_back( closeTextStyleToHtml< Trait >( t->opts() ) );
		}
	}

	void onMath(
		//! Math.
		Math< Trait > * m ) override
	{
		if( !justCollectFootnoteRefs )
		{
			html.push_back( m->isInline() ? "$ " : "$$ " );
			html.push_back( m->expr() );
			html.push_back( m->isInline() ? " $" : " $$" );
		}
	}

	void onLineBreak(
		//! Linebreak.
		LineBreak< Trait > * ) override
	{
		if( !justCollectFootnoteRefs )
			html.push_back( "<br />" );
	}

	void onParagraph(
		//! Paragraph.
		Paragraph< Trait > * p,
		//! Wrap this paragraph with something or no? It's useful to not wrap standalone
		//! paragraph in list item, for example.
		bool wrap ) override
	{
		if( wrap && !justCollectFootnoteRefs )
			html.push_back( "<p>" );

		Visitor< Trait >::onParagraph( p, wrap );

		if( wrap && !justCollectFootnoteRefs )
			html.push_back( "</p>" );
	}

	void onHeading(
		//! Heading.
		Heading< Trait > * h ) override
	{
		if( !justCollectFootnoteRefs )
			html.push_back( "\n" );

		switch( h->level() )
		{
			case 1 :
			case 2 :
			case 3 :
			case 4 :
			case 5 :
			case 6 :
				onHeading( h, typename Trait::String( "h" ) + std::to_string( h->level() ).c_str() );
				break;

			default :
				break;
		}

		if( !justCollectFootnoteRefs )
			html.push_back( "\n" );
	}

	void onCode(
		//! Code.
		Code< Trait > * c ) override
	{
		if( !justCollectFootnoteRefs )
		{
			html.push_back( "\n" );
			html.push_back( "<pre><code" );

			if( !c->syntax().isEmpty() )
			{
				html.push_back( " class=\"language-" );
				html.push_back( c->syntax() );
				html.push_back( "\"" );
			}

			html.push_back( ">" );
			html.push_back( prepareTextForHtml< Trait >( c->text() ) );
			html.push_back( "</code></pre>" );
			html.push_back( "\n" );
		}
	}

	void onInlineCode(
		//! Code.
		Code< Trait > * c ) override
	{
		if( !justCollectFootnoteRefs )
		{
			html.push_back( openTextStyleToHtml< Trait >( c->opts() ) );

			html.push_back( "<code>" );

			html.push_back( prepareTextForHtml< Trait >( c->text() ) );

			html.push_back( "</code>" );

			html.push_back( closeTextStyleToHtml< Trait >( c->opts() ) );
		}
	}

	void onBlockquote(
		//! Blockquote.
		Blockquote< Trait > * b ) override
	{
		if( !justCollectFootnoteRefs )
			html.push_back( "\n<blockquote>" );

		Visitor< Trait >::onBlockquote( b );

		if( !justCollectFootnoteRefs )
			html.push_back( "</blockquote>\n" );
	}

	void onList(
		//! List.
		List< Trait > * l ) override
	{
		if( !justCollectFootnoteRefs )
			html.push_back( "\n" );

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
							html.push_back( "<ol" );

							if( item->isTaskList() )
								html.push_back( " class=\"contains-task-list\"" );

							html.push_back( ">\n" );
						}
					}
					else
					{
						if( !justCollectFootnoteRefs )
						{
							html.push_back( "<ul" );

							if( item->isTaskList() )
								html.push_back( " class=\"contains-task-list\"" );

							html.push_back( ">\n" );
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
								html.push_back( "</ol><ul>\n" );
							else
								html.push_back( "</ul><ol>\n" );
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
					html.push_back( "</ol>\n" );
				else
					html.push_back( "</ul>\n" );
			}
		}
	}

	void onTable(
		//! Table.
		Table< Trait > * t ) override
	{
		if( !justCollectFootnoteRefs )
			html.push_back( "\n" );

		if( !t->isEmpty() )
		{
			if( !justCollectFootnoteRefs )
				html.push_back( "<table><thead><tr>\n" );

			int columns = 0;

			for( auto th = (*t->rows().cbegin())->cells().cbegin(),
				last = (*t->rows().cbegin())->cells().cend(); th != last; ++th )
			{
				if( !justCollectFootnoteRefs )
				{
					html.push_back( "<th" );
					html.push_back( tableAlignmentToHtml< Trait >( t->columnAlignment( columns ) ) );
					html.push_back( ">\n" );
				}

				this->onTableCell( th->get() );

				if( !justCollectFootnoteRefs )
					html.push_back( "\n</th>\n" );

				++columns;
			}

			if( !justCollectFootnoteRefs )
				html.push_back( "</thead><tbody>\n" );

			for( auto r = std::next( t->rows().cbegin() ), rlast = t->rows().cend(); r != rlast; ++r )
			{
				if( !justCollectFootnoteRefs )
					html.push_back( "<tr>\n" );

				int i = 0;

				for( auto c = (*r)->cells().cbegin(), clast = (*r)->cells().cend(); c != clast; ++c )
				{
					if( !justCollectFootnoteRefs )
					{
						html.push_back( "\n<td" );
						html.push_back( tableAlignmentToHtml< Trait >( t->columnAlignment( i ) ) );
						html.push_back( ">\n" );
					}

					this->onTableCell( c->get() );

					if( !justCollectFootnoteRefs )
						html.push_back( "\n</td>\n" );

					++i;

					if( i == columns )
						break;
				}

				if( !justCollectFootnoteRefs )
				{
					for( ; i < columns; ++i )
						html.push_back( "<td></td>" );

					html.push_back( "\n</tr>\n" );
				}
			}

			if( !justCollectFootnoteRefs )
				html.push_back( "</tbody></table>" );
		}

		if( !justCollectFootnoteRefs )
			html.push_back( "\n" );
	}

	void onAnchor(
		//! Anchor.
		Anchor< Trait > * a ) override
	{
		if( !justCollectFootnoteRefs )
		{
			html.push_back( "\n<div id=\"" );
			html.push_back( a->label() );
			html.push_back( "\"></div>\n" );
		}
	}

	void onRawHtml(
		//! Raw HTML.
		RawHtml< Trait > * h ) override
	{
		if( !justCollectFootnoteRefs )
			html.push_back( h->text() );
	}

	void onHorizontalLine(
		//! Horizontal line.
		HorizontalLine< Trait > * ) override
	{
		if( !justCollectFootnoteRefs )
			html.push_back( "<hr />" );
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
			url = typename Trait::String( "#" ) + url;
		else if( url.startsWith( typename Trait::String( "#" ) ) &&
			this->doc->labeledHeadings().find( url ) == this->doc->labeledHeadings().cend() )
		{
			auto path = static_cast< Anchor< Trait >* > ( this->doc->items().at( 0 ).get() )->label();
			const auto sp = path.lastIndexOf( typename Trait::String( "/" ) );
			path.remove( sp, path.length() - sp );
			const auto p = url.indexOf( path ) - 1;
			url.remove( p, url.length() - p );
		}

		if( !justCollectFootnoteRefs )
		{
			html.push_back( openTextStyleToHtml< Trait >( l->opts() ) );

			html.push_back( "<a href=\"" );
			html.push_back( url );
			html.push_back( "\">" );
		}

		if( !l->img()->isEmpty() )
		{
			if( !justCollectFootnoteRefs )
				onImage( l->img().get() );
		}
		else if( l->p() && !l->p()->isEmpty() )
			onParagraph( l->p().get(), false );
		else if( !l->text().isEmpty() )
		{
			if( !justCollectFootnoteRefs )
				html.push_back( linkTextToHtml< Trait >( l->text(), l->opts() ) );
		}
		else
		{
			if( !justCollectFootnoteRefs )
				html.push_back( linkTextToHtml< Trait >( l->url(), l->opts() ) );
		}

		if( !justCollectFootnoteRefs )
		{
			html.push_back( "</a>" );

			html.push_back( closeTextStyleToHtml< Trait >( l->opts() ) );
		}
	}

	void onImage(
		//! Image.
		Image< Trait > * i ) override
	{
		if( !justCollectFootnoteRefs )
		{
			html.push_back( "<img src=\"" );
			html.push_back( i->url() );
			html.push_back( "\" alt=\"" );
			html.push_back( prepareTextForHtml< Trait >( i->text() ) );
			html.push_back( "\" style=\"max-width:100%;\" />" );
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
				html.push_back( "<sup>" );
				html.push_back( "<a href=\"#" );
				html.push_back( ref->id() );
				html.push_back( "\" id=\"ref-" );
				html.push_back( ref->id() );
				html.push_back( "-" );
			}

			if( r == fns.end() )
			{
				if( !justCollectFootnoteRefs )
					html.push_back( "1" );
			}
			else
			{
				if( !justCollectFootnoteRefs )
					html.push_back( std::to_string( ++( r->current ) ).c_str() );

				++( r->count );
			}

			html.push_back( "\">" );

			if( r == fns.end() )
			{
				if( !justCollectFootnoteRefs )
					html.push_back( std::to_string( fns.size() + 1 ).c_str() );

				fns.push_back( { ref->id(), 1, 0 } );
			}
			else if( !justCollectFootnoteRefs )
				html.push_back( std::to_string( std::distance( fns.begin(), r ) + 1 ).c_str() );

			if( !justCollectFootnoteRefs )
				html.push_back( "</a></sup>" );
		}
	}

	void onListItem(
		//! List item.
		ListItem< Trait > * i,
		//! Is this item first in the list?
		bool first ) override
	{
		if( !justCollectFootnoteRefs )
		{
			html.push_back( "<li" );

			if( i->isTaskList() )
			{
				html.push_back( " class=\"task-list-item\"><input type=\"checkbox\" id=\"\" disabled=\"\" class=\"task-list-item-checkbox\"" );

				if( i->isChecked() )
					html.push_back( " checked=\"\"" );
			}

			if( i->listType() == ListItem< Trait >::Ordered && first )
			{
				html.push_back( " value=\"" );
				html.push_back( std::to_string( i->startNumber() ).c_str() );
				html.push_back( "\"" );
			}

			html.push_back( ">\n" );
		}

		Visitor< Trait >::onListItem( i, first );

		if( !justCollectFootnoteRefs )
			html.push_back( "</li>\n" );
	}

private:
	void onHeading(
		//! Heading.
		Heading< Trait > * h,
		//! Heading tag.
		const typename Trait::String & ht )
	{
		if( !justCollectFootnoteRefs )
		{
			html.push_back( "<" );
			html.push_back( ht );
			html.push_back( headingIdToHtml( h ) );
			html.push_back( ">" );
		}

		if( h->text().get() )
			onParagraph( h->text().get(), false );

		if( !justCollectFootnoteRefs )
		{
			html.push_back( "</" );
			html.push_back( ht );
			html.push_back( ">" );
		}
	}

	void onFootnotes( const typename Trait::String & hrefForRefBackImage )
	{
		if( !fns.empty() )
			html.push_back( "<section class=\"footnotes\"><ol>" );

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

		for( const auto & id : fns )
		{
			html.push_back( "<li id=\"" );
			html.push_back( id.id );
			html.push_back( "\">" );
			++i;

			const auto fit = this->doc->footnotesMap().find( id.id );

			if( fit != this->doc->footnotesMap().cend() )
			{
				this->onFootnote( fit->second.get() );

				if( !hrefForRefBackImage.isEmpty() )
				{
					typename Trait::String backRef;
					long long int backRefPos = html.endsWith( "</p>" ) ? 4 : 0;

					for( long long int i = 1; i <= id.count; ++i )
					{
						backRef.push_back( "<a href=\"#ref-" );
						backRef.push_back( id.id );
						backRef.push_back( "-" );
						backRef.push_back( std::to_string( i ).c_str() );
						backRef.push_back( "\"><img src=\"" );
						backRef.push_back( hrefForRefBackImage );
						backRef.push_back( "\" /></a>" );
					}

					html.insert( html.length() - backRefPos, backRef );
				}

				html.push_back( "</li>" );
			}
		}

		if( !fns.empty() )
			html.push_back( "</ol></section>\n" );
	}

private:
	typename Trait::String html;
	//! Just collect footnote references?
	bool justCollectFootnoteRefs = false;

	struct FootnoteRefStuff {
		typename Trait::String id;
		long long int count = 0;
		long long int current = 0;
	};

	//! Vector of processed footnotes references.
	typename Trait::template Vector< FootnoteRefStuff > fns;
}; // class Visitor

} /* namespace details */


template< class Trait >
typename Trait::String
toHtml( std::shared_ptr< Document< Trait > > doc, bool wrapInBodyTag = true,
	const typename Trait::String & hrefForRefBackImage = {} )
{
	typename Trait::String html;

	typename Trait::template Vector< std::pair< typename Trait::String, long long int > > fns;

	if( wrapInBodyTag )
		html.push_back( "<!DOCTYPE html>\n<html><head></head><body>\n" );

	html.push_back( "<article class=\"markdown-body\">" );

	details::HtmlVisitor< Trait > visitor;

	html.push_back( visitor.toHtml( doc, hrefForRefBackImage ) );

	html.push_back( "</article>\n" );

	if( wrapInBodyTag )
		html.push_back( "</body></html>\n" );

	return html;
}

} /* namespace MD */

#endif // MD4QT_MD_HTML_HPP_INCLUDED
