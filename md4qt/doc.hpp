/*
	SPDX-FileCopyrightText: 2022-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
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
// WithPosition
//

//! Base for any thing with start and end position.
class WithPosition {	
public:
	WithPosition() = default;
	virtual ~WithPosition() = default;
	
	WithPosition( long long int startColumn,
		long long int startLine,
		long long int endColumn,
		long long int endLine )
		:	m_startColumn( startColumn )
		,	m_startLine( startLine )
		,	m_endColumn( endColumn )
		,	m_endLine( endLine )
	{
	}
	
	void applyPositions( const WithPosition & other )
	{
		if( this != &other )
			*this = other;
	}
	
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
}; // class WithPosition

inline bool operator == ( const WithPosition & l, const WithPosition & r )
{
	return ( l.startColumn() == r.startColumn() &&
		l.startLine() == r.startLine() &&
		l.endColumn() == r.endColumn() &&
		l.endLine() == r.endLine() );
}


template< class Trait >
class Document;


//
// Item
//

//! Base class for item in Markdown document.
template< class Trait >
class Item
	:	public WithPosition
{
protected:
	Item() = default;

public:
	~Item() override = default;

	virtual ItemType type() const = 0;
	
	virtual std::shared_ptr< Item< Trait > > clone( Document< Trait > * doc = nullptr ) const = 0;

private:
	DISABLE_COPY( Item )
}; // class Item


//
// TextOption
//

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
// StyleDelim
//

class StyleDelim final
	:	public WithPosition
{
public:
	StyleDelim( int s,
		long long int startColumn,
		long long int startLine,
		long long int endColumn,
		long long int endLine )
		:	WithPosition( startColumn, startLine, endColumn, endLine )
		,	m_style( s )
	{
	}
	
	~StyleDelim() override = default;
	
	int style() const
	{
		return m_style;
	}
	
	void setStyle( int t )
	{
		m_style = t;
	}
	
private:
	int m_style = TextWithoutFormat;
}; // class StyleDelim

inline bool operator == ( const StyleDelim & l, const StyleDelim & r )
{
	return ( static_cast< WithPosition > ( l ) == static_cast< WithPosition > ( r ) &&
		l.style() == r.style() );
}


//
// ItemWithOpts
//

template< class Trait >
class ItemWithOpts
	:	public Item< Trait >
{
protected:
	ItemWithOpts() = default;

public:
	~ItemWithOpts() override = default;
	
	void applyItemWithOpts( const ItemWithOpts< Trait > & other )
	{
		if( this != &other )
		{
			WithPosition::applyPositions( other );
			m_opts = other.m_opts;
			m_openStyles = other.m_openStyles;
			m_closeStyles = other.m_closeStyles;
		}
	}
	
	using Styles = typename Trait::template Vector< StyleDelim >;

	int opts() const
	{
		return m_opts;
	}

	void setOpts( int o )
	{
		m_opts = o;
	}
	
	const Styles & openStyles() const
	{
		return m_openStyles;
	}
	
	Styles & openStyles()
	{
		return m_openStyles;
	}
	
	const Styles & closeStyles() const
	{
		return m_closeStyles;
	}
	
	Styles & closeStyles()
	{
		return m_closeStyles;
	}

private:
	int m_opts = 0;
	Styles m_openStyles;
	Styles m_closeStyles;

	DISABLE_COPY( ItemWithOpts )
}; // class ItemWithOpts


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
	
	std::shared_ptr< Item< Trait > > clone( Document< Trait > * doc = nullptr ) const override
	{
		return std::make_shared< PageBreak< Trait > > ();
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
	
	std::shared_ptr< Item< Trait > > clone( Document< Trait > * doc = nullptr ) const override
	{
		auto h = std::make_shared< HorizontalLine< Trait > > ();
		h->applyPositions( *this );
		
		return h;
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
	
	std::shared_ptr< Item< Trait > > clone( Document< Trait > * doc = nullptr ) const override
	{
		return std::make_shared< Anchor< Trait > > ( m_label );
	}

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


//
// RawHtml
//

//! Raw HTML.
template< class Trait >
class RawHtml final
	:	public ItemWithOpts< Trait >
{
public:
	RawHtml() = default;
	~RawHtml() override = default;
	
	std::shared_ptr< Item< Trait > > clone( Document< Trait > * doc = nullptr ) const override
	{
		auto h = std::make_shared< RawHtml< Trait > > ();
		h->applyItemWithOpts( *this );
		h->setText( m_text );
		h->setFreeTag( m_isFreeTag );
		
		return h;
	}

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
	bool m_isFreeTag = true;

	DISABLE_COPY( RawHtml )
}; // class RawHtml


//
// Text
//

//! Text.
template< typename Trait >
class Text
	:	public ItemWithOpts< Trait >
{
public:
	Text() = default;
	~Text() override = default;
	
	void applyText( const Text< Trait > & t )
	{
		if( this != &t )
		{
			ItemWithOpts< Trait >::applyItemWithOpts( t );
			setText( t.text() );
			setSpaceBefore( t.isSpaceBefore() );
			setSpaceAfter( t.isSpaceAfter() );
		}
	}
	
	std::shared_ptr< Item< Trait > > clone( Document< Trait > * doc = nullptr ) const override
	{
		auto t = std::make_shared< Text< Trait > > ();
		t->applyText( *this );
		
		return t;
	}

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
	bool m_isSpaceBefore = false;
	bool m_isSpaceAfter = false;

	DISABLE_COPY( Text )
}; // class Text


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
	
	std::shared_ptr< Item< Trait > > clone( Document< Trait > * doc = nullptr ) const override
	{
		auto b = std::make_shared< LineBreak< Trait > > ();
		b->applyText( *this );
		
		return b;
	}

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
	
	void applyBlock( const Block< Trait > & other, Document< Trait > * doc = nullptr )
	{
		if( this != &other )
		{
			WithPosition::applyPositions( other );
			
			m_items.clear();
			
			for( const auto & i : other.items() )
				appendItem( i->clone( doc ) );
		}
	}

	const Items & items() const
	{
		return m_items;
	}

	void insertItem( long long int idx, ItemSharedPointer i )
	{
		m_items.insert( m_items.cbegin() + idx, i );
	}

	void appendItem( ItemSharedPointer i )
	{
		m_items.push_back( i );
	}

	void removeItemAt( long long int idx )
	{
		if( idx >= 0 && idx < m_items.size() )
			m_items.erase( m_items.cbegin() + idx );
	}
	
	ItemSharedPointer getItemAt( long long int idx ) const
	{
		return m_items.at( idx );
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
	
	std::shared_ptr< Item< Trait > > clone( Document< Trait > * doc = nullptr ) const override
	{
		auto p = std::make_shared< Paragraph< Trait > > ();
		p->setDirty( m_dirty );
		p->applyBlock( *this, doc );
		
		return p;
	}

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
	{
	}

	~Heading() override = default;
	
	std::shared_ptr< Item< Trait > > clone( Document< Trait > * doc = nullptr ) const override
	{
		auto h = std::make_shared< Heading< Trait > > ();
		h->applyPositions( *this );
		h->setText( std::static_pointer_cast< Paragraph< Trait > >( m_text->clone( doc ) ) );
		h->setLevel( m_level );
		h->setLabel( m_label );
		h->setDelim( m_delim );
		
		if( doc && isLabeled() )
			doc->insertLabeledHeading( m_label, h );
		
		return h;
	}

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
	
	const WithPosition & delim() const
	{
		return m_delim;
	}
	
	void setDelim( const WithPosition & d )
	{
		m_delim = d;
	}

private:
	ParagraphSharedPointer m_text;
	int m_level = 0;
	typename Trait::String m_label;
	WithPosition m_delim;

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
	
	std::shared_ptr< Item< Trait > > clone( Document< Trait > * doc = nullptr ) const override
	{
		auto b = std::make_shared< Blockquote< Trait > > ();
		b->applyBlock( *this, doc );
		b->delims() = m_delims;
		
		return b;
	}

	ItemType type() const override
	{
		return ItemType::Blockquote;
	}
	
	using Delims = typename Trait::template Vector< WithPosition >;	
	
	const Delims & delims() const
	{
		return m_delims;
	}
	
	Delims & delims()
	{
		return m_delims;
	}

private:
	Delims m_delims;
	
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
	
	std::shared_ptr< Item< Trait > > clone( Document< Trait > * doc = nullptr ) const override
	{
		auto l = std::make_shared< ListItem< Trait > > ();
		l->applyBlock( *this, doc );
		l->setListType( m_listType );
		l->setOrderedListPreState( m_orderedListState );
		l->setStartNumber( m_startNumber );
		l->setTaskList( m_isTaskList );
		l->setChecked( m_isChecked );
		l->setDelim( m_delim );
		l->setTaskDelim( m_taskDelim );
		
		return l;
	}

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
	
	const WithPosition & delim() const
	{
		return m_delim;
	}
	
	void setDelim( const WithPosition & d )
	{
		m_delim = d;
	}
	
	const WithPosition & taskDelim() const
	{
		return m_taskDelim;
	}
	
	void setTaskDelim( const WithPosition & d )
	{
		m_taskDelim = d;
	}

private:
	ListType m_listType = Unordered;
	OrderedListPreState m_orderedListState = Start;
	int m_startNumber = 1;
	bool m_isTaskList = false;
	bool m_isChecked = false;
	WithPosition m_delim = {};
	WithPosition m_taskDelim = {};

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
	
	std::shared_ptr< Item< Trait > > clone( Document< Trait > * doc = nullptr ) const override
	{
		auto l = std::make_shared< List< Trait > > ();
		l->applyBlock( *this, doc );
		
		return l;
	}

	ItemType type() const override
	{
		return ItemType::List;
	}

private:
	DISABLE_COPY( List )
}; // class List


//
// LinkBase
//

//! Base class for links.
template< class Trait >
class LinkBase
	:	public ItemWithOpts< Trait >
{
public:
	LinkBase()
		:	m_p( new Paragraph< Trait > )
	{	
	}
	
	~LinkBase() override = default;
	
	void applyLinkBase( const LinkBase< Trait> & other, Document< Trait > * doc = nullptr )
	{
		if( this != &other )
		{
			ItemWithOpts< Trait >::applyItemWithOpts( other );
			setUrl( other.url() );
			setText( other.text() );
			setP( std::static_pointer_cast< Paragraph< Trait > >( other.p()->clone( doc ) ) );
			setTextPos( other.textPos() );
			setUrlPos( other.urlPos() );
		}
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
	
	const WithPosition & textPos() const
	{
		return m_textPos;
	}
	
	void setTextPos( const WithPosition & pos )
	{
		m_textPos = pos;
	}
	
	const WithPosition & urlPos() const
	{
		return m_urlPos;
	}
	
	void setUrlPos( const WithPosition & pos )
	{
		m_urlPos = pos;
	}

private:
	typename Trait::String m_url;
	typename Trait::String m_text;
	ParagraphSharedPointer m_p;
	WithPosition m_textPos = {};
	WithPosition m_urlPos = {};

	DISABLE_COPY( LinkBase )
}; // class LinkBase


//
// Image
//

//! Image.
template< class Trait >
class Image final
	:	public LinkBase< Trait >
{
public:
	Image() = default;
	~Image() override = default;
	
	std::shared_ptr< Item< Trait > > clone( Document< Trait > * doc = nullptr ) const override
	{
		auto i = std::make_shared< Image< Trait > > ();
		i->applyLinkBase( *this, doc );
		
		return i;
	}

	ItemType type() const override
	{
		return ItemType::Image;
	}

private:
	DISABLE_COPY( Image )
}; // class Image


//
// Link
//

//! Link.
template< class Trait >
class Link
	:	public LinkBase< Trait >
{
public:
	Link()
		:	LinkBase< Trait > ()
		,	m_img( new Image< Trait > )
	{
	}
	
	std::shared_ptr< Item< Trait > > clone( Document< Trait > * doc = nullptr ) const override
	{
		auto l = std::make_shared< Link< Trait > > ();
		l->applyLinkBase( *this, doc );
		l->setImg( std::static_pointer_cast< Image< Trait > > ( m_img->clone( doc ) ) );
		
		return l;
	}

	~Link() override = default;

	ItemType type() const override
	{
		return ItemType::Link;
	}

	using ImageSharedPointer = std::shared_ptr< Image< Trait > >;

	ImageSharedPointer img() const
	{
		return m_img;
	}

	void setImg( ImageSharedPointer i )
	{
		m_img = i;
	}

private:
	ImageSharedPointer m_img;

	DISABLE_COPY( Link )
}; // class Link


//
// Code
//

//! Code.
template< class Trait >
class Code
	:	public ItemWithOpts< Trait >
{
public:
	explicit Code( const typename Trait::String & t, bool fensedCode, bool inl )
		:	ItemWithOpts< Trait > ()
		,	m_text( t )
		,	m_inlined( inl )
		,	m_fensed( fensedCode )
	{
	}

	~Code() override = default;
	
	void applyCode( const Code< Trait > & other )
	{
		if( this != &other )
		{
			ItemWithOpts< Trait >::applyItemWithOpts( other );
			setText( other.text() );
			setInline( other.isInline() );
			setSyntax( other.syntax() );
			setSyntaxPos( other.syntaxPos() );
			setStartDelim( other.startDelim() );
			setEndDelim( other.endDelim() );
			setFensedCode( other.isFensedCode() );
		}
	}
	
	std::shared_ptr< Item< Trait > > clone( Document< Trait > * doc = nullptr ) const override
	{
		auto c = std::make_shared< Code< Trait > > ( m_text, m_fensed, m_inlined );
		c->applyCode( *this );
		
		return c;
	}

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

	bool isInline() const
	{
		return m_inlined;
	}
	
	void setInline( bool on = true )
	{
		m_inlined = on;
	}

	const typename Trait::String & syntax() const
	{
		return m_syntax;
	}

	void setSyntax( const typename Trait::String & s )
	{
		m_syntax = s;
	}
	
	const WithPosition & syntaxPos() const
	{
		return m_syntaxPos;
	}
	
	void setSyntaxPos( const WithPosition & p )
	{
		m_syntaxPos = p;
	}
	
	const WithPosition & startDelim() const
	{
		return m_startDelim;
	}
	
	void setStartDelim( const WithPosition & d )
	{
		m_startDelim = d;
	}
	
	const WithPosition & endDelim() const
	{
		return m_endDelim;
	}
	
	void setEndDelim( const WithPosition & d )
	{
		m_endDelim = d;
	}
	
	bool isFensedCode() const
	{
		return m_fensed;
	}
	
	void setFensedCode( bool on = true )
	{
		m_fensed = on;
	}

private:
	typename Trait::String m_text;
	bool m_inlined = true;
	bool m_fensed = false;
	typename Trait::String m_syntax;
	WithPosition m_startDelim = {};
	WithPosition m_endDelim = {};
	WithPosition m_syntaxPos = {};

	DISABLE_COPY( Code )
}; // class Code


//
// Math
//

//! Math expression.
template< class Trait >
class Math final
	:	public Code< Trait >
{
public:
	Math()
		:	Code< Trait >( typename Trait::String(), false, true )
	{
	}
	
	~Math() override = default;
	
	std::shared_ptr< Item< Trait > > clone( Document< Trait > * doc = nullptr ) const override
	{
		auto m = std::make_shared< Math< Trait > > ();
		m->applyCode( *this );
		
		return m;
	}

	ItemType type() const override
	{
		return ItemType::Math;
	}

	const typename Trait::String & expr() const
	{
		return Code< Trait >::text();
	}

	void setExpr( const typename Trait::String & e )
	{
		Code< Trait >::setText( e );
	}

private:
	DISABLE_COPY( Math )
}; // class Math


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
	
	std::shared_ptr< Item< Trait > > clone( Document< Trait > * doc = nullptr ) const override
	{
		auto c = std::make_shared< TableCell< Trait > > ();
		c->applyBlock( *this, doc );
		
		return c;
	}

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
	
	std::shared_ptr< Item< Trait > > clone( Document< Trait > * doc = nullptr ) const override
	{
		auto t = std::make_shared< TableRow< Trait > > ();
		t->applyPositions( *this );
		
		for( const auto c : cells() )
			t->appendCell( std::static_pointer_cast< TableCell< Trait > > ( c->clone( doc ) ) );
		
		return t;
	}

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
	
	std::shared_ptr< Item< Trait > > clone( Document< Trait > * doc = nullptr ) const override
	{
		auto t = std::make_shared< Table< Trait > > ();
		t->applyPositions( *this );
		
		for( const auto & r : rows() )
			t->appendRow( std::static_pointer_cast< TableRow< Trait > >( r->clone( doc ) ) );
		
		for( int i = 0; i < columnsCount(); ++i )
			t->setColumnAlignment( i, columnAlignment( i ) );
		
		return t;
	}

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
	:	public Text< Trait >
{
public:
	explicit FootnoteRef( const typename Trait::String & i )
		:	m_id( i )
	{
	}

	~FootnoteRef() override = default;
	
	std::shared_ptr< Item< Trait > > clone( Document< Trait > * doc = nullptr ) const override
	{
		auto f = std::make_shared< FootnoteRef< Trait > > ( m_id );
		f->applyText( *this );
		f->setIdPos( m_idPos );
		
		return f;
	}

	ItemType type() const override
	{
		return ItemType::FootnoteRef;
	}

	const typename Trait::String & id() const
	{
		return m_id;
	}
	
	const WithPosition & idPos() const
	{
		return m_idPos;
	}
	
	void setIdPos( const WithPosition & pos )
	{
		m_idPos = pos;
	}

private:
	typename Trait::String m_id;
	WithPosition m_idPos;

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
	
	std::shared_ptr< Item< Trait > > clone( Document< Trait > * doc = nullptr ) const override
	{
		auto f = std::make_shared< Footnote< Trait > > ();
		f->applyBlock( *this, doc );
		f->setIdPos( m_idPos );
		
		return f;
	}

	ItemType type() const override
	{
		return ItemType::Footnote;
	}
	
	const WithPosition & idPos() const
	{
		return m_idPos;
	}
	
	void setIdPos( const WithPosition & pos )
	{
		m_idPos = pos;
	}

private:
	WithPosition m_idPos = {};
	
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
	
	std::shared_ptr< Item< Trait > > clone( Document< Trait > * doc = nullptr ) const override
	{
		auto d = std::make_shared< Document< Trait > > ();
		d->applyBlock( *this, d.get() );
		
		for( auto it = m_footnotes.cbegin(), last = m_footnotes.cend(); it != last; ++it )
			d->insertFootnote( it->first, std::static_pointer_cast< Footnote< Trait > > (
				it->second->clone( d.get() ) ) );
		
		for( auto it = m_labeledLinks.cbegin(), last = m_labeledLinks.cend(); it != last; ++it )
			d->insertLabeledLink( it->first, std::static_pointer_cast< Link< Trait > > (
				it->second->clone( d.get() ) ) );
		
		return d;
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
