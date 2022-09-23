
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2019 Igor Mironchik

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// md-pdf include
#include "md_doc.hpp"


namespace MD {

//
// PageBreak
//

ItemType
PageBreak::type() const
{
	return ItemType::PageBreak;
}


//
// HorizontalLine
//

ItemType
HorizontalLine::type() const
{
	return ItemType::HorizontalLine;
}


//
// Anchor
//

Anchor::Anchor( const QString & l )
	:	m_label( l )
{
}

ItemType
Anchor::type() const
{
	return ItemType::Anchor;
}

const QString &
Anchor::label() const
{
	return m_label;
}


//
// Heading
//

ItemType
Heading::type() const
{
	return ItemType::Heading;
}

QSharedPointer< Paragraph >
Heading::text() const
{
	return m_text;
}

void
Heading::setText( QSharedPointer< Paragraph > t )
{
	m_text = t;
}

int
Heading::level() const
{
	return m_level;
}

void
Heading::setLevel( int l )
{
	m_level = l;
}

bool
Heading::isLabeled() const
{
	return !m_label.isEmpty();
}

const QString &
Heading::label() const
{
	return m_label;
}

void
Heading::setLabel( const QString & l )
{
	m_label = l;
}


//
// RawHtml
//

ItemType
RawHtml::type() const
{
	return ItemType::RawHtml;
}

const QString &
RawHtml::text() const
{
	return m_text;
}

void
RawHtml::setText( const QString & t )
{
	m_text = t;
}

bool
RawHtml::isFreeTag() const
{
	return m_isFreeTag;
}

void
RawHtml::setFreeTag( bool on )
{
	m_isFreeTag = on;
}


//
// Text
//

Text::Text()
	:	m_isSpaceBefore( false )
	,	m_isSpaceAfter( false )
{
}

ItemType
Text::type() const
{
	return ItemType::Text;
}

const QString &
Text::text() const
{
	return m_text;
}

void
Text::setText( const QString & t )
{
	m_text = t;
}

const TextOptions &
Text::opts() const
{
	return m_opts;
}

void
Text::setOpts( const TextOptions & o )
{
	m_opts = o;
}

bool
Text::isSpaceBefore() const
{
	return m_isSpaceBefore;
}

void
Text::setSpaceBefore( bool on )
{
	m_isSpaceBefore = on;
}

bool
Text::isSpaceAfter() const
{
	return m_isSpaceAfter;
}

void
Text::setSpaceAfter( bool on )
{
	m_isSpaceAfter = on;
}


//
// Math
//

ItemType
Math::type() const
{
	return ItemType::Math;
}

const QString &
Math::expr() const
{
	return m_expr;
}

void
Math::setExpr( const QString & e )
{
	m_expr = e;
}

bool
Math::isInline() const
{
	return m_inline;
}

void
Math::setInline( bool on )
{
	m_inline = on;
}


//
// LineBreak
//

ItemType
LineBreak::type() const
{
	return ItemType::LineBreak;
}


//
// Block
//

const Block::Items &
Block::items() const
{
	return m_items;
}

void
Block::appendItem( QSharedPointer< Item > i )
{
	m_items.append( i );
}

bool
Block::isEmpty() const
{
	return m_items.isEmpty();
}


//
// Paragraph
//

ItemType
Paragraph::type() const
{
	return ItemType::Paragraph;
}

bool
Paragraph::isDirty() const
{
	return m_dirty;
}

void
Paragraph::setDirty( bool on )
{
	m_dirty = on;
}


//
// Blockquote
//

ItemType
Blockquote::type() const
{
	return ItemType::Blockquote;
}


//
// ListItem
//

ItemType
ListItem::type() const
{
	return ItemType::ListItem;
}

ListItem::ListType
ListItem::listType() const
{
	return m_listType;
}

void
ListItem::setListType( ListType t )
{
	m_listType = t;
}

ListItem::OrderedListPreState
ListItem::orderedListPreState() const
{
	return m_orderedListState;
}

void
ListItem::setOrderedListPreState( OrderedListPreState s )
{
	m_orderedListState = s;
}

int
ListItem::startNumber() const
{
	return m_startNumber;
}

void
ListItem::setStartNumber( int n )
{
	m_startNumber = n;
}

bool
ListItem::isTaskList() const
{
	return m_isTaskList;
}

void
ListItem::setTaskList( bool on )
{
	m_isTaskList = on;
}

bool
ListItem::isChecked() const
{
	return m_isChecked;
}

void
ListItem::setChecked( bool on )
{
	m_isChecked = on;
}


//
// List
//

ItemType
List::type() const
{
	return ItemType::List;
}


//
// Image
//

ItemType
Image::type() const
{
	return ItemType::Image;
}

const QString &
Image::url() const
{
	return m_url;
}

void
Image::setUrl( const QString & u )
{
	m_url = u;
}

const QString &
Image::text() const
{
	return m_text;
}

void
Image::setText( const QString & t )
{
	m_text = t;
}

bool
Image::isEmpty() const
{
	return ( m_url.isEmpty() );
}

QSharedPointer< Paragraph >
Image::p() const
{
	return m_p;
}

void
Image::setP( QSharedPointer< Paragraph > v )
{
	m_p = v;
}


//
// Link
//

Link::Link()
	:	m_img( new Image() )
{

}

ItemType
Link::type() const
{
	return ItemType::Link;
}

const QString &
Link::url() const
{
	return m_url;
}

void
Link::setUrl( const QString & u )
{
	m_url = u;
}

const QString &
Link::text() const
{
	return m_text;
}

void
Link::setText( const QString & t )
{
	m_text = t;
}

TextOptions
Link::opts() const
{
	return m_opts;
}

void
Link::setOpts( const TextOptions & o )
{
	m_opts = o;
}

QSharedPointer< Image >
Link::img() const
{
	return m_img;
}

void
Link::setImg( QSharedPointer< Image > i )
{
	m_img = i;
}

QSharedPointer< Paragraph >
Link::p() const
{
	return m_p;
}

void
Link::setP( QSharedPointer< Paragraph > v )
{
	m_p = v;
}


//
// Code
//

Code::Code( const QString & t, bool inl )
	:	m_text( t )
	,	m_inlined( inl )
{
}

ItemType
Code::type() const
{
	return ItemType::Code;
}

const QString &
Code::text() const
{
	return m_text;
}

bool
Code::isInlined() const
{
	return m_inlined;
}

const QString &
Code::syntax() const
{
	return m_syntax;
}

void
Code::setSyntax( const QString & s )
{
	m_syntax = s;
}


//
// TableCell
//

ItemType
TableCell::type() const
{
	return ItemType::TableCell;
}


//
// TableRow
//

ItemType
TableRow::type() const
{
	return ItemType::TableRow;
}

const TableRow::Cells &
TableRow::cells() const
{
	return m_cells;
}

void
TableRow::appendCell( QSharedPointer< TableCell > c )
{
	m_cells.append( c );
}

bool
TableRow::isEmpty() const
{
	return m_cells.isEmpty();
}


//
// Table
//

ItemType
Table::type() const
{
	return ItemType::Table;
}

const Table::Rows &
Table::rows() const
{
	return m_rows;
}

void
Table::appendRow( QSharedPointer< TableRow > r )
{
	m_rows.append( r );
}

Table::Alignment
Table::columnAlignment( int idx ) const
{
	return m_aligns.at( idx );
}

void
Table::setColumnAlignment( int idx, Alignment a )
{
	if( idx + 1 > columnsCount() )
		m_aligns.append( a );
	else
		m_aligns[ idx ] = a;
}

int
Table::columnsCount() const
{
	return m_aligns.size();
}

bool
Table::isEmpty() const
{
	return ( m_aligns.isEmpty() || m_rows.isEmpty() );
}


//
// FootnoteRef
//

FootnoteRef::FootnoteRef( const QString & i )
	:	m_id( i )
{
}

ItemType
FootnoteRef::type() const
{
	return ItemType::FootnoteRef;
}

const QString &
FootnoteRef::id() const
{
	return m_id;
}


//
// Footnote
//

ItemType
Footnote::type() const
{
	return ItemType::Footnote;
}


//
// Document
//

ItemType
Document::type() const
{
	return ItemType::Document;
}

const Document::Footnotes &
Document::footnotesMap() const
{
	return m_footnotes;
}

void
Document::insertFootnote( const QString & id, QSharedPointer< Footnote > fn )
{
	m_footnotes.insert( id, fn );
}

const Document::LabeledLinks &
Document::labeledLinks() const
{
	return m_labeledLinks;
}

void
Document::insertLabeledLink( const QString & label, QSharedPointer< Link > lnk )
{
	m_labeledLinks.insert( label, lnk );
}

const Document::LabeledHeadings &
Document::labeledHeadings() const
{
	return m_labeledHeadings;
}

void
Document::insertLabeledHeading( const QString & label, QSharedPointer< Heading > h )
{
	m_labeledHeadings.insert( label, h );
}

} /* namespace MD */
