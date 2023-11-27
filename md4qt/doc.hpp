
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2022 Igor Mironchik

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

#ifndef MD4QT_MD_DOC_HPP_INCLUDED
#define MD4QT_MD_DOC_HPP_INCLUDED

// md4qt include.
#include "utils.hpp"

// C++ include.
#include <memory>


namespace MD {


//
// ItemType
//

//! Enumeration of item types.
enum class ItemType {
	//! Heading.
	Heading,
	//! Text.
	Text,
	//! Paragraph.
	Paragraph,
	//! Line break.
	LineBreak,
	//! Blockquote.
	Blockquote,
	//! List item.
	ListItem,
	//! List.
	List,
	//! Link.
	Link,
	//! Image.
	Image,
	//! Code.
	Code,
	//! Table cell.
	TableCell,
	//! Table row.
	TableRow,
	//! Table.
	Table,
	//! Footnote ref.
	FootnoteRef,
	//! Footnote.
	Footnote,
	//! Document.
	Document,
	//! Page break.
	PageBreak,
	//! Anchor.
	Anchor,
	//! Horizontal line.
	HorizontalLine,
	//! Raw HTML.
	RawHtml,
	//! Math expression.
	Math
}; // enum class ItemType


//
// Item
//

//! Base class for item in Markdown document.
template< class Trait >
class Item {
protected:
	Item() = default;

public:
	virtual ~Item() = default;

	virtual ItemType type() const = 0;

	long long int startColumn() const { return m_startColumn; }
	long long int startLine() const { return m_startLine; }
	long long int endColumn() const { return m_endColumn; }
	long long int endLine() const { return m_endLine; }

	void setStartColumn( long long int c ) { m_startColumn = c; }
	void setStartLine( long long int l ) { m_startLine = l; }
	void setEndColumn( long long int c ) { m_endColumn = c; }
	void setEndLine( long long int l ) { m_endLine = l; }

private:
	long long int m_startColumn = -1;
	long long int m_startLine = -1;
	long long int m_endColumn = -1;
	long long int m_endLine = -1;

	DISABLE_COPY( Item )
}; // class Item


//
// PageBreak
//

//! Page break.
template< class Trait >
class PageBreak final
	:	public Item< Trait >
{
public:
	PageBreak() = default;
	~PageBreak() override = default;

	ItemType type() const override
	{
		return ItemType::PageBreak;
	}

private:
	DISABLE_COPY( PageBreak )
}; // class PageBreak


//
// HorizontalLine
//

//! Horizontal line.
template< class Trait >
class HorizontalLine final
	:	public Item< Trait >
{
public:
	HorizontalLine() = default;
	~HorizontalLine() override = default;

	ItemType type() const override
	{
		return ItemType::HorizontalLine;
	}

private:
	DISABLE_COPY( HorizontalLine )
}; // class HorizontalLine


//
// Anchor
//

//! Just an anchor.
template< class Trait >
class Anchor final
	:	public Item< Trait >
{
public:
	explicit Anchor( const typename Trait::String & l )
		:	m_label( l )
	{
	}

	~Anchor() override = default;

	ItemType type() const override
	{
		return ItemType::Anchor;
	}

	const typename Trait::String & label() const
	{
		return m_label;
	}

private:
	DISABLE_COPY( Anchor )

	typename Trait::String m_label;
}; // class Anchor


//! Text option.
enum TextOption {
	//! No format.
	TextWithoutFormat = 0,
	//! Bold text.
	BoldText = 1,
	//! Italic text.
	ItalicText = 2,
	//! Strikethrough.
	StrikethroughText = 4
}; // enum TextOption


//
// RawHtml
//

//! Raw HTML.
template< class Trait >
class RawHtml final
	:	public Item< Trait >
{
public:
	RawHtml() = default;
	~RawHtml() override = default;

	ItemType type() const override
	{
		return ItemType::RawHtml;
	}

	const typename Trait::String & text() const
	{
		return m_text;
	}

	void setText( const typename Trait::String & t )
	{
		m_text = t;
	}

protected:
	template< class T >
	friend class Parser;

	template< class T >
	friend struct UnprotectedDocsMethods;

	bool isFreeTag() const
	{
		return m_isFreeTag;
	}

	void setFreeTag( bool on = true )
	{
		m_isFreeTag = on;
	}

private:
	typename Trait::String m_text;
	bool m_isFreeTag;

	DISABLE_COPY( RawHtml )
}; // class RawHtml


//
// Text
//

//! Text.
template< typename Trait >
class Text
	:	public Item< Trait >
{
public:
	Text()
		:	m_opts( 0 )
		,	m_isSpaceBefore( false )
		,	m_isSpaceAfter( false )
	{
	}

	~Text() override = default;

	ItemType type() const override
	{
		return ItemType::Text;
	}

	const typename Trait::String & text() const
	{
		return m_text;
	}

	void setText( const typename Trait::String & t )
	{
		m_text = t;
	}

	int opts() const
	{
		return m_opts;
	}

	void setOpts( int o )
	{
		m_opts = o;
	}

	bool isSpaceBefore() const
	{
		return m_isSpaceBefore;
	}

	void setSpaceBefore( bool on = true )
	{
		m_isSpaceBefore = on;
	}

	bool isSpaceAfter() const
	{
		return m_isSpaceAfter;
	}

	void setSpaceAfter( bool on = true )
	{
		m_isSpaceAfter = on;
	}

private:
	typename Trait::String m_text;
	int m_opts;
	bool m_isSpaceBefore;
	bool m_isSpaceAfter;

	DISABLE_COPY( Text )
}; // class Text


//
// Math
//

//! Math expression.
template< class Trait >
class Math final
	:	public Item< Trait >
{
public:
	Math() = default;
	~Math() override = default;

	ItemType type() const override
	{
		return ItemType::Math;
	}

	const typename Trait::String & expr() const
	{
		return m_expr;
	}

	void setExpr( const typename Trait::String & e )
	{
		m_expr = e;
	}

	bool isInline() const
	{
		return m_inline;
	}

	void setInline( bool on = true )
	{
		m_inline = on;
	}

private:
	typename Trait::String m_expr;
	bool m_inline = false;

	DISABLE_COPY( Math )
}; // class Math


//
// LineBreak
//

//! Line break.
template< class Trait >
class LineBreak final
	:	public Text< Trait >
{
public:
	LineBreak() = default;
	~LineBreak() override = default;

	ItemType type() const override
	{
		return ItemType::LineBreak;
	}

private:

	DISABLE_COPY( LineBreak )
}; // class LineBreak


//
// Block
//

//! Abstract block.
template< class Trait >
class Block
	:	public Item< Trait >
{
protected:
	Block() = default;

public:
	~Block() override = default;

	using ItemSharedPointer = std::shared_ptr< Item< Trait > >;
	using Items = typename Trait::template Vector< ItemSharedPointer >;

	const Items & items() const
	{
		return m_items;
	}

	void appendItem( ItemSharedPointer i )
	{
		m_items.push_back( i );
	}

	bool isEmpty() const
	{
		return m_items.empty();
	}

private:
	Items m_items;

	DISABLE_COPY( Block )
}; // class Block


//
// Paragraph
//

//! Paragraph.
template< class Trait >
class Paragraph final
	:	public Block< Trait >
{
public:
	Paragraph() = default;
	~Paragraph() override = default;

	ItemType type() const override
	{
		return ItemType::Paragraph;
	}

protected:
	template< class T >
	friend class Parser;

	template< class T >
	friend struct UnprotectedDocsMethods;

	bool isDirty() const
	{
		return m_dirty;
	}

	void setDirty( bool on = true )
	{
		m_dirty = on;
	}

private:
	bool m_dirty = false;

	DISABLE_COPY( Paragraph )
}; // class Paragraph


//
// Heading
//

//! Heading.
template< class Trait >
class Heading final
	:	public Item< Trait >
{
public:
	Heading()
		:	m_text( new Paragraph< Trait> )
		,	m_level( 0 )
	{
	}

	~Heading() override = default;

	ItemType type() const override
	{
		return ItemType::Heading;
	}

	using ParagraphSharedPointer = std::shared_ptr< Paragraph< Trait > >;

	ParagraphSharedPointer text() const
	{
		return m_text;
	}

	void setText( ParagraphSharedPointer t )
	{
		m_text = t;
	}

	int level() const
	{
		return m_level;
	}

	void setLevel( int l )
	{
		m_level = l;
	}

	bool isLabeled() const
	{
		return m_label.size() > 0;
	}

	const typename Trait::String & label() const
	{
		return m_label;
	}

	void setLabel( const typename Trait::String & l )
	{
		m_label = l;
	}

private:
	ParagraphSharedPointer m_text;
	int m_level;
	typename Trait::String m_label;

	DISABLE_COPY( Heading )
}; // class Heading


//
// Blockquote
//

//! Blockquote.
template< class Trait >
class Blockquote final
	:	public Block< Trait >
{
public:
	Blockquote() = default;
	~Blockquote() override = default;

	ItemType type() const override
	{
		return ItemType::Blockquote;
	}

private:
	DISABLE_COPY( Blockquote )
}; // class Blockquote


//
// ListItem
//

//! List item.
template< class Trait >
class ListItem final
	:	public Block< Trait >
{
public:
	ListItem() = default;
	~ListItem() override = default;

	ItemType type() const override
	{
		return ItemType::ListItem;
	}

	//! Type of the list.
	enum ListType {
		Ordered,
		Unordered
	}; // enum ListType

	//! Preliminary state of the ordered list.
	enum OrderedListPreState {
		Start,
		Continue
	}; // enum OrderedListPreState

	ListType listType() const
	{
		return m_listType;
	}

	void setListType( ListType t )
	{
		m_listType = t;
	}

	OrderedListPreState orderedListPreState() const
	{
		return m_orderedListState;
	}

	void setOrderedListPreState( OrderedListPreState s )
	{
		m_orderedListState = s;
	}

	int startNumber() const
	{
		return m_startNumber;
	}

	void setStartNumber( int n )
	{
		m_startNumber = n;
	}

	bool isTaskList() const
	{
		return m_isTaskList;
	}

	void setTaskList( bool on = true )
	{
		m_isTaskList = on;
	}

	bool isChecked() const
	{
		return m_isChecked;
	}

	void setChecked( bool on = true )
	{
		m_isChecked = on;
	}

private:
	ListType m_listType = Unordered;
	OrderedListPreState m_orderedListState = Start;
	int m_startNumber = 1;
	bool m_isTaskList = false;
	bool m_isChecked = false;

	DISABLE_COPY( ListItem )
}; // class ListItem


//
// List
//

//! List.
template< class Trait >
class List final
	:	public Block< Trait >
{
public:
	List() = default;
	~List() override = default;

	ItemType type() const override
	{
		return ItemType::List;
	}

private:
	DISABLE_COPY( List )
}; // class List


//
// Image
//

//! Image.
template< class Trait >
class Image final
	:	public Item< Trait >
{
public:
	Image()
		:	m_p( new Paragraph< Trait > )
	{
	}

	~Image() override = default;

	ItemType type() const override
	{
		return ItemType::Image;
	}

	using ParagraphSharedPointer = std::shared_ptr< Paragraph< Trait > >;

	const typename Trait::String & url() const
	{
		return m_url;
	}

	void setUrl( const typename Trait::String & u )
	{
		m_url = u;
	}

	const typename Trait::String & text() const
	{
		return m_text;
	}

	void setText( const typename Trait::String & t )
	{
		m_text = t;
	}

	bool isEmpty() const
	{
		return m_url.size() <= 0;
	}

	ParagraphSharedPointer p() const
	{
		return m_p;
	}

	void setP( ParagraphSharedPointer v )
	{
		m_p = v;
	}

private:
	typename Trait::String m_url;
	typename Trait::String m_text;
	ParagraphSharedPointer m_p;

	DISABLE_COPY( Image )
}; // class Image


//
// Link
//

//! Link.
template< class Trait >
class Link
	:	public Item< Trait >
{
public:
	Link()
		:	m_opts( 0 )
		,	m_img( new Image< Trait > )
		,	m_p( new Paragraph< Trait > )
	{
	}

	~Link() override = default;

	ItemType type() const override
	{
		return ItemType::Link;
	}

	const typename Trait::String & url() const
	{
		return m_url;
	}

	void setUrl( const typename Trait::String & u )
	{
		m_url = u;
	}

	const typename Trait::String & text() const
	{
		return m_text;
	}

	void setText( const typename Trait::String & t )
	{
		m_text = t;
	}

	int opts() const
	{
		return m_opts;
	}

	void setOpts( int o )
	{
		m_opts = o;
	}

	using ImageSharedPointer = std::shared_ptr< Image< Trait > >;
	using ParagraphSharedPointer = std::shared_ptr< Paragraph< Trait > >;

	ImageSharedPointer img() const
	{
		return m_img;
	}

	void setImg( ImageSharedPointer i )
	{
		m_img = i;
	}

	ParagraphSharedPointer p() const
	{
		return m_p;
	}

	void setP( ParagraphSharedPointer v )
	{
		m_p = v;
	}

private:
	typename Trait::String m_url;
	typename Trait::String m_text;
	int m_opts;
	ImageSharedPointer m_img;
	ParagraphSharedPointer m_p;

	DISABLE_COPY( Link )
}; // class Link


//
// Code
//

//! Code.
template< class Trait >
class Code final
	:	public Item< Trait >
{
public:
	explicit Code( const typename Trait::String & t, bool fensedCode, bool inl )
		:	m_text( t )
		,	m_inlined( inl )
		,	m_fensed( fensedCode )
	{
	}

	~Code() override = default;

	ItemType type() const override
	{
		return ItemType::Code;
	}

	const typename Trait::String & text() const
	{
		return m_text;
	}

	void setText( const typename Trait::String & t )
	{
		m_text = t;
	}

	bool isInlined() const
	{
		return m_inlined;
	}

	const typename Trait::String & syntax() const
	{
		return m_syntax;
	}

	void setSyntax( const typename Trait::String & s )
	{
		m_syntax = s;
	}

protected:
	template< class T >
	friend struct UnprotectedDocsMethods;

	bool isFensedCode() const
	{
		return m_fensed;
	}

private:
	typename Trait::String m_text;
	bool m_inlined;
	bool m_fensed;
	typename Trait::String m_syntax;

	DISABLE_COPY( Code )
}; // class Code


//
// TableCell
//

//! Table cell.
template< class Trait >
class TableCell final
	:	public Block< Trait >
{
public:
	TableCell() = default;
	~TableCell() override = default;

	ItemType type() const override
	{
		return ItemType::TableCell;
	}

private:
	DISABLE_COPY( TableCell )
}; // class TableCell


//
// TableRow
//

//! Table row.
template< class Trait >
class TableRow final
	:	public Item< Trait >
{
public:
	TableRow() = default;
	~TableRow() override = default;

	ItemType type() const override
	{
		return ItemType::TableRow;
	}

	using TableCellSharedPointer = std::shared_ptr< TableCell< Trait > >;
	using Cells = typename  Trait::template Vector< TableCellSharedPointer >;

	const Cells & cells() const
	{
		return m_cells;
	}

	void appendCell( TableCellSharedPointer c )
	{
		m_cells.push_back( c );
	}

	bool isEmpty() const
	{
		return m_cells.empty();
	}

private:
	Cells m_cells;

	DISABLE_COPY( TableRow )
}; // class TableRow


//
// Table
//

//! Table.
template< class Trait >
class Table final
	:	public Item< Trait >
{
public:
	Table() = default;
	~Table() override = default;

	ItemType type() const override
	{
		return ItemType::Table;
	}

	using TableRowSharedPointer = std::shared_ptr< TableRow< Trait > >;
	using Rows = typename Trait::template Vector< TableRowSharedPointer >;

	const Rows & rows() const
	{
		return m_rows;
	}

	void appendRow( TableRowSharedPointer r )
	{
		m_rows.push_back( r );
	}

	//! Alignment.
	enum Alignment {
		AlignLeft,
		AlignRight,
		AlignCenter
	}; // enum Alignmnet.

	using ColumnsAlignments = typename Trait::template Vector< Alignment >;

	Alignment columnAlignment( int idx ) const
	{
		return m_aligns.at( idx );
	}

	void setColumnAlignment( int idx, Alignment a )
	{
		if( idx + 1 > columnsCount() )
			m_aligns.push_back( a );
		else
			m_aligns[ idx ] = a;
	}

	int columnsCount() const
	{
		return m_aligns.size();
	}

	bool isEmpty() const
	{
		return ( m_aligns.empty() || m_rows.empty() );
	}

private:
	Rows m_rows;
	ColumnsAlignments m_aligns;

	DISABLE_COPY( Table )
}; // class Table


//
// FootnoteRef
//

//! Footnote ref.
template< class Trait >
class FootnoteRef final
	:	public Item< Trait >
{
public:
	explicit FootnoteRef( const typename Trait::String & i )
		:	m_id( i )
	{
	}

	~FootnoteRef() override = default;

	ItemType type() const override
	{
		return ItemType::FootnoteRef;
	}

	const typename Trait::String & id() const
	{
		return m_id;
	}

private:
	typename Trait::String m_id;

	DISABLE_COPY( FootnoteRef )
}; // class FootnoteRef


//
// Footnote
//

//! Footnote.
template< class Trait >
class Footnote final
	:	public Block< Trait >
{
public:
	Footnote() = default;
	~Footnote() override = default;

	ItemType type() const override
	{
		return ItemType::Footnote;
	}

private:
	DISABLE_COPY( Footnote )
}; // class Footnote


//
// Document
//

//! Document.
template< class Trait >
class Document final
	:	public Block< Trait >
{
public:
	Document() = default;
	~Document() override = default;

	ItemType type() const override
	{
		return ItemType::Document;
	}

	using FootnoteSharedPointer = std::shared_ptr< Footnote< Trait > >;
	using Footnotes = typename Trait::template Map< typename Trait::String,
		FootnoteSharedPointer >;

	const Footnotes & footnotesMap() const
	{
		return m_footnotes;
	}

	void insertFootnote( const typename Trait::String & id,
		FootnoteSharedPointer fn )
	{
		m_footnotes.insert( { id, fn } );
	}

	using LinkSharedPointer = std::shared_ptr< Link< Trait > >;
	using LabeledLinks = typename Trait::template Map< typename Trait::String,
		LinkSharedPointer >;

	const LabeledLinks & labeledLinks() const
	{
		return m_labeledLinks;
	}

	void insertLabeledLink( const typename Trait::String & label, LinkSharedPointer lnk )
	{
		m_labeledLinks.insert( { label, lnk } );
	}

	using HeadingSharedPointer = std::shared_ptr< Heading< Trait > >;
	using LabeledHeadings = typename Trait::template Map< typename Trait::String,
		HeadingSharedPointer >;

	const LabeledHeadings & labeledHeadings() const
	{
		return m_labeledHeadings;
	}

	void insertLabeledHeading( const typename Trait::String & label, HeadingSharedPointer h )
	{
		m_labeledHeadings.insert( { label, h } );
	}

private:
	Footnotes m_footnotes;
	LabeledLinks m_labeledLinks;
	LabeledHeadings m_labeledHeadings;

	DISABLE_COPY( Document )
}; // class Document;

} /* namespace MD */

#endif // MD4QT_MD_DOC_HPP_INCLUDED
