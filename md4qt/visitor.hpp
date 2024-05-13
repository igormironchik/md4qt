/*
	SPDX-FileCopyrightText: 2022-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

#ifndef MD4QT_MD_VISITOR_HPP_INCLUDED
#define MD4QT_MD_VISITOR_HPP_INCLUDED

// md4qt include.
#include "doc.hpp"

// C++ include.
#include <string>
#include <utility>


namespace MD {

//
// Visitor
//

//! Visitor interface to walk through Document.
template< class Trait >
class Visitor {
public:
	Visitor() = default;
	virtual ~Visitor() = default;

	void process( std::shared_ptr< Document< Trait > > d )
	{
		anchors.clear();
		doc = d;

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
					onHeading( static_cast< Heading< Trait >* > ( it->get() ) );
					break;

				case ItemType::Paragraph :
					onParagraph( static_cast< Paragraph< Trait >* > ( it->get() ), true );
					break;

				case ItemType::Code :
					onCode( static_cast< Code< Trait >* > ( it->get() ) );
					break;

				case ItemType::Blockquote :
					onBlockquote( static_cast< Blockquote< Trait >* > ( it->get() ) );
					break;

				case ItemType::List :
					onList( static_cast< List< Trait >* > ( it->get() ) );
					break;

				case ItemType::Table :
					onTable( static_cast< Table< Trait >* > ( it->get() ) );
					break;

				case ItemType::Anchor :
					onAnchor( static_cast< Anchor< Trait >* > ( it->get() ) );
					break;

				case ItemType::RawHtml :
					onRawHtml( static_cast< RawHtml< Trait >* > ( it->get() ) );
					break;

				case ItemType::HorizontalLine :
					onHorizontalLine( static_cast< HorizontalLine< Trait >* > ( it->get() ) );
					break;

				default :
					break;
			}
		}
	}

protected:
	//! For some generator it's important to keep line endings like they were in Markdown.
	//! So onParagraph() method invokes this method when necessary to add line ending.
	virtual void onAddLineEnding() = 0;

	virtual void onText(
		//! Text.
		Text< Trait > * t ) = 0;

	virtual void onMath(
		//! Math.
		Math< Trait > * m ) = 0;

	virtual void onLineBreak(
		//! Linebreak.
		LineBreak< Trait > * b ) = 0;

	virtual void onParagraph(
		//! Paragraph.
		Paragraph< Trait > * p,
		//! Wrap this paragraph with something or no? It's useful to not wrap standalone
		//! paragraph in list item, for example.
		bool wrap )
	{
		long long int l = ( !p->items().empty() ? p->items().at( 0 )->startLine() : -1 );

		for( auto it = p->items().begin(), last = p->items().end(); it != last; ++it )
		{
			if( (*it)->startLine() != l )
				onAddLineEnding();

			l = (*it)->endLine();

			switch( (*it)->type() )
			{
				case ItemType::Text :
					onText( static_cast< Text< Trait >* > ( it->get() ) );
					break;

				case ItemType::Code :
					onInlineCode( static_cast< Code< Trait >* > ( it->get() ) );
					break;

				case ItemType::Link :
					onLink( static_cast< Link< Trait >* > ( it->get() ) );
					break;

				case ItemType::Image :
					onImage( static_cast< Image< Trait >* > ( it->get() ) );
					break;

				case ItemType::Math :
					onMath( static_cast< Math< Trait >* > ( it->get() ) );
					break;

				case ItemType::LineBreak :
					onLineBreak( static_cast< LineBreak< Trait >* > ( it->get() ) );
					break;

				case ItemType::FootnoteRef :
					onFootnoteRef( static_cast< FootnoteRef< Trait >* > ( it->get() ) );
					break;

				case ItemType::RawHtml :
					onRawHtml( static_cast< RawHtml< Trait >* > ( it->get() ) );
					break;

				default :
					break;
			}
		}
	}

	virtual void onHeading(
		//! Heading.
		Heading< Trait > * h ) = 0;

	virtual void onCode(
		//! Code.
		Code< Trait > * c ) = 0;

	virtual void onInlineCode(
		//! Code.
		Code< Trait > * c ) = 0;

	virtual void onBlockquote(
		//! Blockquote.
		Blockquote< Trait > * b )
	{
		for( auto it = b->items().cbegin(), last = b->items().cend(); it != last; ++it )
		{
			switch( (*it)->type() )
			{
				case ItemType::Heading :
					onHeading( static_cast< Heading< Trait >* > ( it->get() ) );
					break;

				case ItemType::Paragraph :
					onParagraph( static_cast< Paragraph< Trait >* > ( it->get() ), true );
					break;

				case ItemType::Code :
					onCode( static_cast< Code< Trait >* > ( it->get() ) );
					break;

				case ItemType::Blockquote :
					onBlockquote( static_cast< Blockquote< Trait >* > ( it->get() ) );
					break;

				case ItemType::List :
					onList( static_cast< List< Trait >* > ( it->get() ) );
					break;

				case ItemType::Table :
					onTable( static_cast< Table< Trait >* > ( it->get() ) );
					break;

				case ItemType::HorizontalLine :
					onHorizontalLine( static_cast< HorizontalLine< Trait >* > ( it->get() ) );
					break;

				case ItemType::RawHtml :
					onRawHtml( static_cast< RawHtml< Trait >* > ( it->get() ) );
					break;

				default :
					break;
			}
		}
	}

	virtual void onList(
		//! List.
		List< Trait > * l ) = 0;

	virtual void onTable(
		//! Table.
		Table< Trait > * t ) = 0;

	virtual void onAnchor(
		//! Anchor.
		Anchor< Trait > * a ) = 0;

	virtual void onRawHtml(
		//! Raw HTML.
		RawHtml< Trait > * h ) = 0;

	virtual void onHorizontalLine(
		//! Horizontal line.
		HorizontalLine< Trait > * l ) = 0;

	virtual void onLink(
		//! Link.
		Link< Trait > * l ) = 0;

	virtual void onImage(
		//! Image.
		Image< Trait > * i ) = 0;

	virtual void onFootnoteRef(
		//! Footnote reference.
		FootnoteRef< Trait > * ref ) = 0;

	virtual void onListItem(
		//! List item.
		ListItem< Trait > * i,
		//! Is this item first in the list?
		bool first )
	{
		for( auto it = i->items().cbegin(), last = i->items().cend(); it != last; ++it )
		{
			switch( (*it)->type() )
			{
				case ItemType::Heading :
					onHeading( static_cast< Heading< Trait >* > ( it->get() ) );
					break;

				case ItemType::Paragraph :
					onParagraph( static_cast< Paragraph< Trait >* > ( it->get() ),
						( i->items().size() > 1 && i->items().at( 1 )->type() != ItemType::List ) );
					break;

				case ItemType::Code :
					onCode( static_cast< Code< Trait >* > ( it->get() ) );
					break;

				case ItemType::Blockquote :
					onBlockquote( static_cast< Blockquote< Trait >* > ( it->get() ) );
					break;

				case ItemType::List :
					onList( static_cast< List< Trait >* > ( it->get() ) );
					break;

				case ItemType::Table :
					onTable( static_cast< Table< Trait >* > ( it->get() ) );
					break;

				case ItemType::RawHtml :
					onRawHtml( static_cast< RawHtml< Trait >* > ( it->get() ) );
					break;

				case ItemType::HorizontalLine :
					onHorizontalLine( static_cast< HorizontalLine< Trait >* > ( it->get() ) );
					break;

				default :
					break;
			}
		}
	}

	virtual void onTableCell(
		//! Table cell.
		TableCell< Trait > * c )
	{
		for( auto it = c->items().cbegin(), last = c->items().cend(); it != last; ++it )
		{
			switch( (*it)->type() )
			{
				case ItemType::Text :
					onText( static_cast< Text< Trait >* > ( it->get() ) );
					break;

				case ItemType::Code :
					onInlineCode( static_cast< Code< Trait >* > ( it->get() ) );
					break;

				case ItemType::Link :
					onLink( static_cast< Link< Trait >* > ( it->get() ) );
					break;

				case ItemType::Image :
					onImage( static_cast< Image< Trait >* > ( it->get() ) );
					break;

				case ItemType::FootnoteRef :
					onFootnoteRef( static_cast< FootnoteRef< Trait >* > ( it->get() ) );
					break;

				case ItemType::RawHtml :
					onRawHtml( static_cast< RawHtml< Trait >* > ( it->get() ) );
					break;

				case ItemType::Math :
					onMath( static_cast< Math< Trait >* > ( it->get() ) );

				default :
					break;
			}
		}
	}

	virtual void onFootnote(
		//! Footnote.
		Footnote< Trait > * f )
	{
		for( auto it = f->items().cbegin(), last = f->items().cend(); it != last; ++it )
		{
			switch( (*it)->type() )
			{
				case ItemType::Heading :
					onHeading( static_cast< Heading< Trait >* > ( it->get() ) );
					break;

				case ItemType::Paragraph :
					onParagraph( static_cast< Paragraph< Trait >* > ( it->get() ), true );
					break;

				case ItemType::Code :
					onCode( static_cast< Code< Trait >* > ( it->get() ) );
					break;

				case ItemType::Blockquote :
					onBlockquote( static_cast< Blockquote< Trait >* > ( it->get() ) );
					break;

				case ItemType::List :
					onList( static_cast< List< Trait >* > ( it->get() ) );
					break;

				case ItemType::Table :
					onTable( static_cast< Table< Trait >* > ( it->get() ) );
					break;

				case ItemType::RawHtml :
					onRawHtml( static_cast< RawHtml< Trait >* > ( it->get() ) );
					break;

				case ItemType::HorizontalLine :
					onHorizontalLine( static_cast< HorizontalLine< Trait >* > ( it->get() ) );
					break;

				default :
					break;
			}
		}
	}

protected:
	//! All available anchors in the document.
	typename Trait::template Vector< typename Trait::String > anchors;
	//! Document.
	std::shared_ptr< Document< Trait > > doc;
}; // class Visitor

} /* namespace MD */

#endif // MD4QT_MD_VISITOR_HPP_INCLUDED
