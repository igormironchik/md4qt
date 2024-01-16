
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
textToHtml( Text< Trait > * t )
{
	typename Trait::String html;

	html.push_back( openTextStyleToHtml< Trait >( t->opts() ) );

	if( t->isSpaceBefore() )
		html.push_back( " " );

	html.push_back( prepareTextForHtml< Trait >( t->text() ) );

	if( t->isSpaceAfter() )
		html.push_back( " " );

	html.push_back( closeTextStyleToHtml< Trait >( t->opts() ) );

	return html;
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
inlineCodeToHtml( Code< Trait > * c )
{
	typename Trait::String html;

	html.push_back( openTextStyleToHtml< Trait >( c->opts() ) );

	html.push_back( "<code>" );

	html.push_back( prepareTextForHtml< Trait >( c->text() ) );

	html.push_back( "</code>" );

	html.push_back( closeTextStyleToHtml< Trait >( c->opts() ) );

	return html;
}

template< class Trait >
typename Trait::String
imgToHtml( Image< Trait > * i )
{
	typename Trait::String html;

	html.push_back( "<img src=\"" );
	html.push_back( i->url() );
	html.push_back( "\" alt=\"" );
	html.push_back( prepareTextForHtml< Trait >( i->text() ) );
	html.push_back( "\" style=\"max-width:100%;\" />" );

	return html;
}

template< class Trait >
typename Trait::String
paragraphToHtml( Paragraph< Trait > * p, bool wrap,
	std::shared_ptr< Document< Trait > > doc,
	typename Trait::template Vector< std::pair< typename Trait::String, long long int > > & fns,
	const typename Trait::template Vector< typename Trait::String > & anchors );

template< class Trait >
typename Trait::String
linkToHtml( Link< Trait > * l, std::shared_ptr< Document< Trait > > doc,
	typename Trait::template Vector< std::pair< typename Trait::String, long long int > > & fns,
	const typename Trait::template Vector< typename Trait::String > & anchors )
{
	typename Trait::String html;

	typename Trait::String url = l->url();

	const auto lit = doc->labeledLinks().find( url );

	if( lit != doc->labeledLinks().cend() )
		url = lit->second->url();

	if( std::find( anchors.cbegin(), anchors.cend(), url ) != anchors.cend() )
		url = typename Trait::String( "#" ) + url;
	else if( url.startsWith( typename Trait::String( "#" ) ) &&
		doc->labeledHeadings().find( url ) == doc->labeledHeadings().cend() )
	{
		auto path = static_cast< Anchor< Trait >* > ( doc->items().at( 0 ).get() )->label();
		const auto sp = path.lastIndexOf( typename Trait::String( "/" ) );
		path.remove( sp, path.length() - sp );
		const auto p = url.indexOf( path ) - 1;
		url.remove( p, url.length() - p );
	}

	html.push_back( openTextStyleToHtml< Trait >( l->opts() ) );

	html.push_back( "<a href=\"" );
	html.push_back( url );
	html.push_back( "\">" );

	if( !l->img()->isEmpty() )
		html.push_back( imgToHtml( l->img().get() ) );
	else if( l->p() && !l->p()->isEmpty() )
		html.push_back( paragraphToHtml( l->p().get(), false, doc, fns, anchors ) );
	else if( !l->text().isEmpty() )
		html.push_back( linkTextToHtml< Trait >( l->text(), l->opts() ) );
	else
		html.push_back( linkTextToHtml< Trait >( l->url(), l->opts() ) );


	html.push_back( "</a>" );

	html.push_back( closeTextStyleToHtml< Trait >( l->opts() ) );

	return html;
}

template< class Trait >
typename Trait::String
footnoteRefToHtml( FootnoteRef< Trait > * ref,
	std::shared_ptr< Document< Trait > > doc,
	typename Trait::template Vector< std::pair< typename Trait::String, long long int > > & fns )
{
	typename Trait::String html;

	const auto fit = doc->footnotesMap().find( ref->id() );

	if( fit != doc->footnotesMap().cend() )
	{
		const auto r = std::find_if( fns.begin(), fns.end(),
			[&ref] ( const auto & p ) { return ref->id() == p.first; } );

		html.push_back( "<sup>" );
		html.push_back( "<a href=\"#" );
		html.push_back( ref->id() );
		html.push_back( "\" id=\"ref-" );
		html.push_back( ref->id() );
		html.push_back( "-" );

		if( r == fns.end() )
			html.push_back( "1" );
		else
			html.push_back( std::to_string( ++( r->second ) ).c_str() );

		html.push_back( "\">" );

		if( r == fns.end() )
		{
			html.push_back( std::to_string( fns.size() + 1 ).c_str() );

			fns.push_back( { ref->id(), 1 } );
		}
		else
			html.push_back( std::to_string( std::distance( fns.begin(), r ) + 1 ).c_str() );

		html.push_back( "</a></sup>" );
	}

	return html;
}

template< class Trait >
typename Trait::String
paragraphToHtml( Paragraph< Trait > * p, bool wrap,
	std::shared_ptr< Document< Trait > > doc,
	typename Trait::template Vector< std::pair< typename Trait::String, long long int > > & fns,
	const typename Trait::template Vector< typename Trait::String > & anchors )
{
	typename Trait::String html;

	if( wrap )
		html.push_back( "<p>" );

	long long int l = ( !p->items().isEmpty() ? p->items().at( 0 )->startLine() : -1 );

	for( auto it = p->items().begin(), last = p->items().end(); it != last; ++it )
	{
		if( (*it)->startLine() != l )
			html.push_back( "\n" );

		l = (*it)->endLine();

		switch( (*it)->type() )
		{
			case ItemType::Text :
				html.push_back( textToHtml( static_cast< Text< Trait >* > ( it->get() ) ) );
				break;

			case ItemType::Code :
				html.push_back( inlineCodeToHtml( static_cast< Code< Trait >* > ( it->get() ) ) );
				break;

			case ItemType::Link :
				html.push_back( linkToHtml( static_cast< Link< Trait >* > ( it->get() ),
					doc, fns, anchors ) );
				break;

			case ItemType::Image :
				html.push_back( imgToHtml( static_cast< Image< Trait >* > ( it->get() ) ) );
				break;

			case ItemType::Math :
            {
				auto math = static_cast< Math< Trait >* > ( it->get() );
				html.push_back( math->isInline() ? "$ " : "$$ " );
				html.push_back( math->expr() );
				html.push_back( math->isInline() ? " $" : " $$" );
            }
				break;

			case ItemType::LineBreak :
				html.push_back( "<br />" );
				break;

			case ItemType::FootnoteRef :
				html.push_back( footnoteRefToHtml(
					static_cast< FootnoteRef< Trait >* > ( it->get() ), doc, fns ) );
				break;

			case ItemType::RawHtml :
			{
				auto * r = static_cast< RawHtml< Trait >* > ( it->get() );

				html.push_back( r->text() );
			}
				break;

			default :
				break;
		}
	}

	if( wrap )
		html.push_back( "</p>" );

	return html;
}

template< class Trait >
typename Trait::String
headingToHtml( Heading< Trait > * h, const typename Trait::String & ht,
	std::shared_ptr< Document< Trait > > doc,
	typename Trait::template Vector< std::pair< typename Trait::String, long long int > > & fns,
	const typename Trait::template Vector< typename Trait::String > & anchors )
{
	typename Trait::String html;

	html.push_back( "<" );
	html.push_back( ht );
	html.push_back( headingIdToHtml( h ) );
	html.push_back( ">" );

	if( h->text().get() )
		html.push_back( paragraphToHtml( h->text().get(), false, doc, fns, anchors ) );

	html.push_back( "</" );
	html.push_back( ht );
	html.push_back( ">" );

	return html;
}

template< class Trait >
typename Trait::String
headingToHtml( Heading< Trait > * h, std::shared_ptr< Document< Trait > > doc,
	typename Trait::template Vector< std::pair< typename Trait::String, long long int > > & fns,
	const typename Trait::template Vector< typename Trait::String > & anchors )
{
	typename Trait::String html;

	html.push_back( "\n" );

	switch( h->level() )
	{
		case 1 :
		case 2 :
		case 3 :
		case 4 :
		case 5 :
		case 6 :
			html.push_back( headingToHtml( h,
				typename Trait::String( "h" ) + std::to_string( h->level() ).c_str(),
				doc, fns, anchors ) );
			break;

		default :
			break;
	}

	html.push_back( "\n" );

	return html;
}

template< class Trait >
typename Trait::String
codeToHtml( Code< Trait > * c )
{
	typename Trait::String html;

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

	return html;
}

template< class Trait >
typename Trait::String
blockquoteToHtml( Blockquote< Trait > * b, std::shared_ptr< Document< Trait > > doc,
	typename Trait::template Vector< std::pair< typename Trait::String, long long int > > & fns,
	const typename Trait::template Vector< typename Trait::String > & anchors );

template< class Trait >
typename Trait::String
listToHtml( List< Trait > * l, std::shared_ptr< Document< Trait > > doc,
	typename Trait::template Vector< std::pair< typename Trait::String, long long int > > & fns,
	const typename Trait::template Vector< typename Trait::String > & anchors );

template< class Trait >
typename Trait::String
tableToHtml( Table< Trait > * t, std::shared_ptr< Document< Trait > > doc,
	typename Trait::template Vector< std::pair< typename Trait::String, long long int > > & fns,
	const typename Trait::template Vector< typename Trait::String > & anchors );

template< class Trait >
typename Trait::String
listItemToHtml( ListItem< Trait > * i, std::shared_ptr< Document< Trait > > doc,
	bool first,
	typename Trait::template Vector< std::pair< typename Trait::String, long long int > > & fns,
	const typename Trait::template Vector< typename Trait::String > & anchors )
{
	typename Trait::String html;

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

	for( auto it = i->items().cbegin(), last = i->items().cend(); it != last; ++it )
	{
		switch( (*it)->type() )
		{
			case ItemType::Heading :
				html.push_back( headingToHtml(
					static_cast< Heading< Trait >* > ( it->get() ), doc, fns, anchors ) );
				break;

			case ItemType::Paragraph :
				html.push_back( paragraphToHtml(
					static_cast< Paragraph< Trait >* > ( it->get() ),
					( i->items().size() > 1 && i->items().at( 1 )->type() != ItemType::List ),
					doc, fns, anchors ) );
				break;

			case ItemType::Code :
				html.push_back( codeToHtml( static_cast< Code< Trait >* > ( it->get() ) ) );
				break;

			case ItemType::Blockquote :
				html.push_back( blockquoteToHtml(
					static_cast< Blockquote< Trait >* > ( it->get() ), doc, fns, anchors ) );
				break;

			case ItemType::List :
				html.push_back( listToHtml(
					static_cast< List< Trait >* > ( it->get() ), doc, fns, anchors ) );
				break;

			case ItemType::Table :
				html.push_back( tableToHtml(
					static_cast< Table< Trait >* > ( it->get() ), doc, fns, anchors ) );
				break;

			case ItemType::RawHtml :
				html.push_back( static_cast< RawHtml< Trait>* > ( it->get() )->text() );
				break;

			case ItemType::HorizontalLine :
				html.push_back( "<hr />" );
				break;

			default :
				break;
		}
	}

	html.push_back( "</li>\n" );

	return html;
}

template< class Trait >
typename Trait::String
listToHtml( List< Trait > * l, std::shared_ptr< Document< Trait > > doc,
	typename Trait::template Vector< std::pair< typename Trait::String, long long int > > & fns,
	const typename Trait::template Vector< typename Trait::String > & anchors )
{
	typename Trait::String html;

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
					html.push_back( "<ol" );

					if( item->isTaskList() )
						html.push_back( " class=\"contains-task-list\"" );

					html.push_back( ">\n" );
				}
				else
				{
					html.push_back( "<ul" );

					if( item->isTaskList() )
						html.push_back( " class=\"contains-task-list\"" );

					html.push_back( ">\n" );
				}
			}
			else
			{
				if( item->listType() != type )
				{
					if( type == ListItem< Trait >::Ordered )
						html.push_back( "</ol><ul>\n" );
					else
						html.push_back( "</ul><ol>\n" );

					type = item->listType();
				}
			}

			html.push_back( listItemToHtml( item, doc, first, fns, anchors ) );

			first = false;
		}
	}

	if( !first )
	{
		if( type == ListItem< Trait >::Ordered )
			html.push_back( "</ol>\n" );
		else
			html.push_back( "</ul>\n" );
	}

	return html;
}

template< class Trait >
typename Trait::String
cellToHtml( TableCell< Trait > * l, std::shared_ptr< Document< Trait > > doc,
	typename Trait::template Vector< std::pair< typename Trait::String, long long int > > & fns,
	const typename Trait::template Vector< typename Trait::String > & anchors )
{
	typename Trait::String html;

	for( auto it = l->items().cbegin(), last = l->items().cend(); it != last; ++it )
	{
		switch( (*it)->type() )
		{
			case ItemType::Text :
				html.push_back( textToHtml( static_cast< Text< Trait >* > ( it->get() ) ) );
				break;

			case ItemType::Code :
				html.push_back( inlineCodeToHtml( static_cast< Code< Trait >* > ( it->get() ) ) );
				break;

			case ItemType::Link :
				html.push_back( linkToHtml(
					static_cast< Link< Trait >* > ( it->get() ), doc, fns, anchors ) );
				break;

			case ItemType::Image :
				html.push_back( imgToHtml( static_cast< Image< Trait >* > ( it->get() ) ) );
				break;

			case ItemType::FootnoteRef :
				html.push_back( footnoteRefToHtml(
					static_cast< FootnoteRef< Trait >* > ( it->get() ), doc, fns ) );
				break;

			case ItemType::RawHtml :
			{
				auto * r = static_cast< RawHtml< Trait >* > ( it->get() );

				html.push_back( r->text() );
			}
				break;

			default :
				break;
		}
	}

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

template< class Trait >
typename Trait::String
tableToHtml( Table< Trait > * t, std::shared_ptr< Document< Trait > > doc,
	typename Trait::template Vector< std::pair< typename Trait::String, long long int > > & fns,
	const typename Trait::template Vector< typename Trait::String > & anchors )
{
	typename Trait::String html;

	html.push_back( "\n" );

	if( !t->isEmpty() )
	{
		html.push_back( "<table><thead><tr>\n" );

		int columns = 0;

		for( auto th = (*t->rows().cbegin())->cells().cbegin(),
			last = (*t->rows().cbegin())->cells().cend(); th != last; ++th )
		{
			html.push_back( "<th" );
			html.push_back( tableAlignmentToHtml< Trait >( t->columnAlignment( columns ) ) );
			html.push_back( ">\n" );
			html.push_back( cellToHtml( th->get(), doc, fns, anchors ) );
			html.push_back( "\n</th>\n" );

			++columns;
		}

		html.push_back( "</thead><tbody>\n" );

		for( auto r = std::next( t->rows().cbegin() ), rlast = t->rows().cend(); r != rlast; ++r )
		{
			html.push_back( "<tr>\n" );

			int i = 0;

			for( auto c = (*r)->cells().cbegin(), clast = (*r)->cells().cend(); c != clast; ++c )
			{
				html.push_back( "\n<td" );
				html.push_back( tableAlignmentToHtml< Trait >( t->columnAlignment( i ) ) );
				html.push_back( ">\n" );
				html.push_back( cellToHtml( c->get(), doc, fns, anchors ) );
				html.push_back( "\n</td>\n" );

				++i;

				if( i == columns )
					break;
			}

			for( ; i < columns; ++i )
				html.push_back( "<td></td>" );

			html.push_back( "\n</tr>\n" );
		}

		html.push_back( "</tbody></table>" );
	}

	html.push_back( "\n" );

	return html;
}

template< class Trait >
typename Trait::String
blockquoteToHtml( Blockquote< Trait > * b, std::shared_ptr< Document< Trait > > doc,
	typename Trait::template Vector< std::pair< typename Trait::String, long long int > > & fns,
	const typename Trait::template Vector< typename Trait::String > & anchors )
{
	typename Trait::String html;

	html.push_back( "\n<blockquote>" );

	for( auto it = b->items().cbegin(), last = b->items().cend(); it != last; ++it )
	{
		switch( (*it)->type() )
		{
			case ItemType::Heading :
				html.push_back( headingToHtml(
					static_cast< Heading< Trait >* > ( it->get() ), doc, fns, anchors ) );
				break;

			case ItemType::Paragraph :
				html.push_back( paragraphToHtml(
					static_cast< Paragraph< Trait >* > ( it->get() ), true, doc, fns, anchors ) );
				break;

			case ItemType::Code :
				html.push_back( codeToHtml(
					static_cast< Code< Trait >* > ( it->get() ) ) );
				break;

			case ItemType::Blockquote :
				html.push_back( blockquoteToHtml(
					static_cast< Blockquote< Trait >* > ( it->get() ), doc, fns, anchors ) );
				break;

			case ItemType::List :
				html.push_back( listToHtml(
					static_cast< List< Trait >* > ( it->get() ), doc, fns, anchors ) );
				break;

			case ItemType::Table :
				html.push_back( tableToHtml(
					static_cast< Table< Trait >* > ( it->get() ), doc, fns, anchors ) );
				break;

			case ItemType::HorizontalLine :
				html.push_back( "<hr />" );
				break;

			case ItemType::RawHtml :
			{
				auto * r = static_cast< RawHtml< Trait >* > ( it->get() );

				html.push_back( r->text() );
			}
				break;

			default :
				break;
		}
	}

	html.push_back( "</blockquote>\n" );

	return html;
}

template< class Trait >
typename Trait::String
anchorToHtml( Anchor< Trait > * a )
{
	typename Trait::String html;

	html.push_back( "\n<div id=\"" );
	html.push_back( a->label() );
	html.push_back( "\"></div>\n" );

	return html;
}

template< class Trait >
typename Trait::String
footnotesToHtml( std::shared_ptr< Document< Trait > > doc,
	typename Trait::template Vector< std::pair< typename Trait::String, long long int > > & fns,
	const typename Trait::template Vector< typename Trait::String > & anchors,
	const typename Trait::String & hrefForRefBackImage = {} )
{
	typename Trait::String html;

	if( !fns.empty() )
		html.push_back( "<section class=\"footnotes\"><ol>" );

	int i = 1;

	typename Trait::template Vector< std::pair< typename Trait::String, long long int > > tmp;

	for( const auto & id : fns )
	{
		html.push_back( "<li id=\"" );
		html.push_back( id.first );
		html.push_back( "\">" );
		++i;

		const auto fit = doc->footnotesMap().find( id.first );

		if( fit != doc->footnotesMap().cend() )
		{
			int backRefPos = 0;

			for( auto it = fit->second->items().cbegin(), last = fit->second->items().cend();
				 it != last; ++it )
			{
				switch( (*it)->type() )
				{
					case ItemType::Heading :
					{
						html.push_back( headingToHtml(
							static_cast< Heading< Trait >* > ( it->get() ), doc, tmp, anchors ) );

						backRefPos = 0;
					}
						break;

					case ItemType::Paragraph :
					{
						html.push_back( paragraphToHtml(
							static_cast< Paragraph< Trait >* > ( it->get() ),
								true, doc, tmp, anchors ) );

						backRefPos = 4;
					}
						break;

					case ItemType::Code :
					{
						html.push_back( codeToHtml(
							static_cast< Code< Trait >* > ( it->get() ) ) );

						backRefPos = 0;
					}
						break;

					case ItemType::Blockquote :
					{
						html.push_back( blockquoteToHtml(
							static_cast< Blockquote< Trait >* > ( it->get() ),
								doc, tmp, anchors ) );

						backRefPos = 0;
					}
						break;

					case ItemType::List :
					{
						html.push_back( listToHtml(
							static_cast< List< Trait >* > ( it->get() ), doc, tmp, anchors ) );

						backRefPos = 0;
					}
						break;

					case ItemType::Table :
					{
						html.push_back( tableToHtml(
							static_cast< Table< Trait >* > ( it->get() ), doc, tmp, anchors ) );

						backRefPos = 0;
					}
						break;

					case ItemType::RawHtml :
					{
						html.push_back( static_cast< RawHtml< Trait >* > ( it->get() )->text() );

						backRefPos = 0;
					}
						break;

					default :
						break;
				}
			}

			if( !hrefForRefBackImage.isEmpty() )
			{
				typename Trait::String backRef;

				for( long long int i = 1; i <= id.second; ++i )
				{
					backRef.push_back( "<a href=\"#ref-" );
					backRef.push_back( id.first );
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

	return html;
}

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

	typename Trait::template Vector< typename Trait::String > anchors;

	for( auto it = doc->items().cbegin(), last = doc->items().cend(); it != last; ++it )
	{
		switch( (*it)->type() )
		{
			case ItemType::Anchor :
				anchors.push_back( static_cast< Anchor< Trait >* > ( it->get() )->label() );
				break;

			default:
				break;
		}
	}

	for( auto it = doc->items().cbegin(), last = doc->items().cend(); it != last; ++it )
	{
		switch( (*it)->type() )
		{
			case ItemType::Heading :
				html.push_back( details::headingToHtml(
					static_cast< Heading< Trait >* > ( it->get() ), doc, fns, anchors ) );
				break;

			case ItemType::Paragraph :
				html.push_back( details::paragraphToHtml(
					static_cast< Paragraph< Trait >* > ( it->get() ), true, doc, fns, anchors ) );
				break;

			case ItemType::Code :
				html.push_back( details::codeToHtml(
					static_cast< Code< Trait >* > ( it->get() ) ) );
				break;

			case ItemType::Blockquote :
				html.push_back( details::blockquoteToHtml(
					static_cast< Blockquote< Trait >* > ( it->get() ), doc, fns, anchors ) );
				break;

			case ItemType::List :
				html.push_back( details::listToHtml(
					static_cast< List< Trait >* > ( it->get() ), doc, fns, anchors ) );
				break;

			case ItemType::Table :
				html.push_back( details::tableToHtml(
					static_cast< Table< Trait >* > ( it->get() ), doc, fns, anchors ) );
				break;

			case ItemType::Anchor :
				html.push_back( details::anchorToHtml(
					static_cast< Anchor< Trait >* > ( it->get() ) ) );
				break;

			case ItemType::RawHtml :
			{
				auto * r = static_cast< RawHtml< Trait >* > ( it->get() );

				html.push_back( "\n" );
				html.push_back( r->text() );
				html.push_back( "\n" );
			}
				break;

			case ItemType::HorizontalLine :
				html.push_back( "<hr />" );
				break;

			default :
				break;
		}
	}

	html.push_back( details::footnotesToHtml( doc, fns, anchors, hrefForRefBackImage ) );

	html.push_back( "</article>\n" );

	if( wrapInBodyTag )
		html.push_back( "</body></html>\n" );

	return html;
}

} /* namespace MD */

#endif // MD4QT_MD_HTML_HPP_INCLUDED
