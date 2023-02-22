
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2023 Igor Mironchik

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
		if( label.startsWith( typename Trait::Char( '#' ) ) )
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
textToHtml( Text< Trait > * t )
{
	typename Trait::String html;

	html.push_back( openTextStyleToHtml< Trait >( t->opts() ) );

	if( t->isSpaceBefore() )
		html.push_back( " " );

	html.push_back( t->text() );

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

	html.push_back( text );

	html.push_back( closeTextStyleToHtml< Trait >( style ) );

	return html;
}

template< class Trait >
typename Trait::String
inlineCodeToHtml( Code< Trait > * c )
{
	typename Trait::String html;

	html.push_back( "<code>" );

	auto code = c->text();
	code.replace( typename Trait::Char( '&' ), "&amp;" );
	code.replace( typename Trait::Char( '<' ), "&lt;" );
	code.replace( typename Trait::Char( '>' ), "&gt;" );

	html.push_back( code );

	html.push_back( "</code>" );

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
	html.push_back( i->text() );
	html.push_back( "\" style=\"max-width:100%;\" />" );

	return html;
}

template< class Trait >
typename Trait::String
linkToHtml( Link< Trait > * l, std::shared_ptr< Document< Trait > > doc )
{
	typename Trait::String html;

	typename Trait::String url = l->url();

	const auto lit = doc->labeledLinks().find( url );

	if( lit != doc->labeledLinks().cend() )
		url = lit->second->url();

	if( typename Trait::Url( url ).isRelative() )
		url = typename Trait::String( "#" ) + url;

	html.push_back( "<a href=\"" );
	html.push_back( url );
	html.push_back( "\">" );

	if( !l->img()->isEmpty() )
		html.push_back( imgToHtml( l->img().get() ) );
	else if( !l->text().isEmpty() )
		html.push_back( linkTextToHtml< Trait >( l->text(), l->opts() ) );


	html.push_back( "</a>" );

	return html;
}

template< class Trait >
typename Trait::String
footnoteRefToHtml( FootnoteRef< Trait > * ref,
	std::shared_ptr< Document< Trait > > doc,
	typename Trait::template Vector< typename Trait::String > & fns )
{
	typename Trait::String html;

	const auto fit = doc->footnotesMap().find( ref->id() );

	if( fit != doc->footnotesMap().cend() )
	{
		html.push_back( "<a href=\"#" );
		html.push_back( ref->id() );
		html.push_back( "\">[" );
		html.push_back( std::to_string( fns.size() + 1 ).c_str() );
		html.push_back( "]</a>" );

		fns.push_back( ref->id() );
	}

	return html;
}

template< class Trait >
typename Trait::String
paragraphToHtml( Paragraph< Trait > * p, bool wrap,
	std::shared_ptr< Document< Trait > > doc,
	typename Trait::template Vector< typename Trait::String > & fns )
{
	typename Trait::String html;

	if( wrap )
		html.push_back( "<p>" );

	for( auto it = p->items().begin(), last = p->items().end(); it != last; ++it )
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
				html.push_back( linkToHtml( static_cast< Link< Trait >* > ( it->get() ), doc ) );
				break;

			case ItemType::Image :
				html.push_back( imgToHtml( static_cast< Image< Trait >* > ( it->get() ) ) );
				break;

			case ItemType::Math :
				break;

			case ItemType::LineBreak :
				html.push_back( "<br />\n" );
				break;

			case ItemType::FootnoteRef :
				html.push_back( footnoteRefToHtml(
					static_cast< FootnoteRef< Trait >* > ( it->get() ), doc, fns ) );
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
	typename Trait::template Vector< typename Trait::String > & fns )
{
	typename Trait::String html;

	html.push_back( "<" );
	html.push_back( ht );
	html.push_back( headingIdToHtml( h ) );
	html.push_back( ">" );

	if( h->text().get() )
		html.push_back( paragraphToHtml( h->text().get(), false, doc, fns ) );

	html.push_back( "</" );
	html.push_back( ht );
	html.push_back( ">" );

	return html;
}

template< class Trait >
typename Trait::String
headingToHtml( Heading< Trait > * h, std::shared_ptr< Document< Trait > > doc,
	typename Trait::template Vector< typename Trait::String > & fns )
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
				typename Trait::String( "h" ) + std::to_string( h->level() ).c_str(), doc, fns ) );
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
	auto code = c->text();
	code.replace( typename Trait::Char( '&' ), "&amp;" );
	code.replace( typename Trait::Char( '<' ), "&lt;" );
	code.replace( typename Trait::Char( '>' ), "&gt;" );
	html.push_back( code );
	html.push_back( "</code></pre>" );
	html.push_back( "\n" );

	return html;
}

template< class Trait >
typename Trait::String
blockquoteToHtml( Blockquote< Trait > * b, std::shared_ptr< Document< Trait > > doc,
	typename Trait::template Vector< typename Trait::String > & fns );

template< class Trait >
typename Trait::String
listToHtml( List< Trait > * l, std::shared_ptr< Document< Trait > > doc,
	typename Trait::template Vector< typename Trait::String > & fns );

template< class Trait >
typename Trait::String
tableToHtml( Table< Trait > * t, std::shared_ptr< Document< Trait > > doc,
	typename Trait::template Vector< typename Trait::String > & fns );

template< class Trait >
typename Trait::String
listItemToHtml( ListItem< Trait > * i, std::shared_ptr< Document< Trait > > doc,
	typename Trait::template Vector< typename Trait::String > & fns )
{
	typename Trait::String html;

	html.push_back( "<li>\n" );

	for( auto it = i->items().cbegin(), last = i->items().cend(); it != last; ++it )
	{
		switch( (*it)->type() )
		{
			case ItemType::Heading :
				html.push_back( headingToHtml(
					static_cast< Heading< Trait >* > ( it->get() ), doc, fns ) );
				break;

			case ItemType::Paragraph :
				html.push_back( paragraphToHtml(
					static_cast< Paragraph< Trait >* > ( it->get() ), false, doc, fns ) );
				break;

			case ItemType::Code :
				html.push_back( codeToHtml( static_cast< Code< Trait >* > ( it->get() ) ) );
				break;

			case ItemType::Blockquote :
				html.push_back( blockquoteToHtml(
					static_cast< Blockquote< Trait >* > ( it->get() ), doc, fns ) );
				break;

			case ItemType::List :
				html.push_back( listToHtml(
					static_cast< List< Trait >* > ( it->get() ), doc, fns ) );
				break;

			case ItemType::Table :
				html.push_back( tableToHtml(
					static_cast< Table< Trait >* > ( it->get() ), doc, fns ) );
				break;

			case ItemType::RawHtml :
				html.push_back( static_cast< RawHtml< Trait>* > ( it->get() )->text() );
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
	typename Trait::template Vector< typename Trait::String > & fns )
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
				first = false;

				if( type == ListItem< Trait >::Ordered )
					html.push_back( "<ol>\n" );
				else
					html.push_back( "<ul>\n" );
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

			html.push_back( listItemToHtml( item, doc, fns ) );
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
	typename Trait::template Vector< typename Trait::String > & fns )
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
					static_cast< Link< Trait >* > ( it->get() ), doc ) );
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
	typename Trait::template Vector< typename Trait::String > & fns )
{
	typename Trait::String html;

	html.push_back( "\n" );

	if( !t->isEmpty() )
	{
		html.push_back( "<table style=\"width:100%;border:1px solid black;border-collapse:collapse;max-width:100%;\">"
						"<thead><tr>\n" );

		int i = 0;

		for( auto th = (*t->rows().cbegin())->cells().cbegin(),
			last = (*t->rows().cbegin())->cells().cend(); th != last; ++th )
		{
			html.push_back( "<th" );
			html.push_back( tableAlignmentToHtml< Trait >( t->columnAlignment( i ) ) );
			html.push_back( " style=\"border:1px solid black;border-collapse:collapse;max-width:100%;\">\n" );
			html.push_back( cellToHtml( th->get(), doc, fns ) );
			html.push_back( "\n</th>\n" );

			++i;
		}

		html.push_back( "</thead><tbody>\n" );

		for( auto r = std::next( t->rows().cbegin() ), rlast = t->rows().cend(); r != rlast; ++r )
		{
			html.push_back( "<tr>\n" );

			i = 0;

			for( auto c = (*r)->cells().cbegin(), clast = (*r)->cells().cend(); c != clast; ++c )
			{
				html.push_back( "\n<td" );
				html.push_back( tableAlignmentToHtml< Trait >( t->columnAlignment( i ) ) );
				html.push_back( " style=\"border:1px solid black;border-collapse:collapse;max-width:100%;\">\n" );
				html.push_back( cellToHtml( c->get(), doc, fns ) );
				html.push_back( "\n</td>\n" );

				++i;
			}

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
	typename Trait::template Vector< typename Trait::String > & fns )
{
	typename Trait::String html;

	html.push_back( "\n<blockquote style=\"padding-left:10px;border-left:8px solid black;\">" );

	for( auto it = b->items().cbegin(), last = b->items().cend(); it != last; ++it )
	{
		switch( (*it)->type() )
		{
			case ItemType::Heading :
				html.push_back( headingToHtml(
					static_cast< Heading< Trait >* > ( it->get() ), doc, fns ) );
				break;

			case ItemType::Paragraph :
				html.push_back( paragraphToHtml(
					static_cast< Paragraph< Trait >* > ( it->get() ), true, doc, fns ) );
				break;

			case ItemType::Code :
				html.push_back( codeToHtml(
					static_cast< Code< Trait >* > ( it->get() ) ) );
				break;

			case ItemType::Blockquote :
				html.push_back( blockquoteToHtml(
					static_cast< Blockquote< Trait >* > ( it->get() ), doc, fns ) );
				break;

			case ItemType::List :
				html.push_back( listToHtml(
					static_cast< List< Trait >* > ( it->get() ), doc, fns ) );
				break;

			case ItemType::Table :
				html.push_back( tableToHtml(
					static_cast< Table< Trait >* > ( it->get() ), doc, fns ) );
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
	typename Trait::template Vector< typename Trait::String > & fns )
{
	typename Trait::String html;

	if( !fns.empty() )
		html.push_back( "<hr /><table><tbody>" );

	int i = 1;

	typename Trait::template Vector< typename Trait::String > tmp;

	for( const auto & id : fns )
	{
		html.push_back( "<tr><td id=\"" );
		html.push_back( id );
		html.push_back( "\">" );
		html.push_back( std::to_string( i ).c_str() );
		html.push_back( "</td><td>" );
		++i;

		const auto fit = doc->footnotesMap().find( id );

		if( fit != doc->footnotesMap().cend() )
		{
			for( auto it = fit->second->items().cbegin(), last = fit->second->items().cend();
				 it != last; ++it )
			{
				switch( (*it)->type() )
				{
					case ItemType::Heading :
						html.push_back( headingToHtml(
							static_cast< Heading< Trait >* > ( it->get() ), doc, tmp ) );
						break;

					case ItemType::Paragraph :
						html.push_back( paragraphToHtml(
							static_cast< Paragraph< Trait >* > ( it->get() ), true, doc, tmp ) );
						break;

					case ItemType::Code :
						html.push_back( codeToHtml(
							static_cast< Code< Trait >* > ( it->get() ) ) );
						break;

					case ItemType::Blockquote :
						html.push_back( blockquoteToHtml(
							static_cast< Blockquote< Trait >* > ( it->get() ), doc, tmp ) );
						break;

					case ItemType::List :
						html.push_back( listToHtml(
							static_cast< List< Trait >* > ( it->get() ), doc, tmp ) );
						break;

					case ItemType::Table :
						html.push_back( tableToHtml(
							static_cast< Table< Trait >* > ( it->get() ), doc, tmp ) );
						break;

					case ItemType::RawHtml :
						html.push_back( static_cast< RawHtml< Trait >* > ( it->get() )->text() );
						break;

					default :
						break;
				}
			}

			html.push_back( "</td></tr>" );
		}
	}

	if( !fns.empty() )
		html.push_back( "</tbody></table>\n" );

	return html;
}

} /* namespace details */

template< class Trait >
typename Trait::String
toHtml( std::shared_ptr< Document< Trait > > doc )
{
	typename Trait::String html;

	typename Trait::template Vector< typename Trait::String > fns;

	html.push_back( "<!DOCTYPE html>\n<html><head></head><body>" );

	for( auto it = doc->items().cbegin(), last = doc->items().cend(); it != last; ++it )
	{
		switch( (*it)->type() )
		{
			case ItemType::Heading :
				html.push_back( details::headingToHtml(
					static_cast< Heading< Trait >* > ( it->get() ), doc, fns ) );
				break;

			case ItemType::Paragraph :
				html.push_back( details::paragraphToHtml(
					static_cast< Paragraph< Trait >* > ( it->get() ), true, doc, fns ) );
				break;

			case ItemType::Code :
				html.push_back( details::codeToHtml(
					static_cast< Code< Trait >* > ( it->get() ) ) );
				break;

			case ItemType::Blockquote :
				html.push_back( details::blockquoteToHtml(
					static_cast< Blockquote< Trait >* > ( it->get() ), doc, fns ) );
				break;

			case ItemType::List :
				html.push_back( details::listToHtml(
					static_cast< List< Trait >* > ( it->get() ), doc, fns ) );
				break;

			case ItemType::Table :
				html.push_back( details::tableToHtml(
					static_cast< Table< Trait >* > ( it->get() ), doc, fns ) );
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

	html.push_back( details::footnotesToHtml( doc, fns ) );

	html.push_back( "</body></html>\n" );

	return html;
}

} /* namespace MD */

#endif // MD4QT_MD_HTML_HPP_INCLUDED
