
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

#ifndef MD_PDF_MD_DOC_HPP_INCLUDED
#define MD_PDF_MD_DOC_HPP_INCLUDED

// Qt include.
#include <QObject>
#include <QString>
#include <QFlags>
#include <QSharedPointer>
#include <QUrl>
#include <QMap>
#include <QVector>


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
class Item {
protected:
	Item() = default;

public:
	virtual ~Item() = default;

	virtual ItemType type() const = 0;

private:
	Q_DISABLE_COPY( Item )
}; // class Item


//
// PageBreak
//

//! Page break.
class PageBreak final
	:	public Item
{
public:
	PageBreak() = default;
	~PageBreak() override = default;

	ItemType type() const override;

private:
	Q_DISABLE_COPY( PageBreak )
}; // class PageBreak


//
// HorizontalLine
//

//! Horizontal line.
class HorizontalLine final
	:	public Item
{
public:
	HorizontalLine() = default;
	~HorizontalLine() override = default;

	ItemType type() const override;

private:
	Q_DISABLE_COPY( HorizontalLine )
}; // class PageBreak


//
// Anchor
//

//! Just an anchor.
class Anchor final
	:	public Item
{
public:
	explicit Anchor( const QString & l );
	~Anchor() override = default;

	ItemType type() const override;

	const QString & label() const;

private:
	Q_DISABLE_COPY( Anchor )

	QString m_label;
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

Q_DECLARE_FLAGS( TextOptions, TextOption )
Q_DECLARE_OPERATORS_FOR_FLAGS( TextOptions )


//
// RawHtml
//

//! Raw HTML.
class RawHtml final
	:	public Item
{
public:
	RawHtml() = default;
	~RawHtml() override = default;

	ItemType type() const override;

	const QString & text() const;
	void setText( const QString & t );

protected:
	friend class Parser;
	friend struct UnprotectedDocsMethods;

	bool isFreeTag() const;
	void setFreeTag( bool on = true );

private:
	QString m_text;
	bool m_isFreeTag;

	Q_DISABLE_COPY( RawHtml )
}; // class RawHtml


//
// Text
//

//! Text.
class Text final
	:	public Item
{
public:
	Text();
	~Text() override = default;

	ItemType type() const override;

	const QString & text() const;
	void setText( const QString & t );

	const TextOptions & opts() const;
	void setOpts( const TextOptions & o );

	bool isSpaceBefore() const;
	void setSpaceBefore( bool on = true );

	bool isSpaceAfter() const;
	void setSpaceAfter( bool on = true );

private:
	QString m_text;
	TextOptions m_opts;
	bool m_isSpaceBefore;
	bool m_isSpaceAfter;

	Q_DISABLE_COPY( Text )
}; // class Text


//
// Math
//

//! Math expression.
class Math final
	:	public Item
{
public:
	Math() = default;
	~Math() override = default;

	ItemType type() const override;

	const QString & expr() const;
	void setExpr( const QString & e );

	bool isInline() const;
	void setInline( bool on = true );

private:
	QString m_expr;
	bool m_inline = false;

	Q_DISABLE_COPY( Math )
}; // class Math


//
// LineBreak
//

//! Line break.
class LineBreak final
	:	public Item
{
public:
	LineBreak() = default;
	~LineBreak() override = default;

	ItemType type() const override;

private:
	Q_DISABLE_COPY( LineBreak )
}; // class LineBreak


//
// Block
//

//! Abstract block.
class Block
	:	public Item
{
protected:
	Block() = default;

public:
	~Block() override = default;

	typedef QVector< QSharedPointer< Item > > Items;

	const Items & items() const;
	void appendItem( QSharedPointer< Item > i );

	bool isEmpty() const;

private:
	Items m_items;

	Q_DISABLE_COPY( Block )
}; // class Block


//
// Paragraph
//

//! Paragraph.
class Paragraph final
	:	public Block
{
public:
	Paragraph() = default;
	~Paragraph() override = default;

	ItemType type() const override;

protected:
	friend class Parser;
	friend struct UnprotectedDocsMethods;

	bool isDirty() const;
	void setDirty( bool on = true );

private:
	bool m_dirty = false;

	Q_DISABLE_COPY( Paragraph )
}; // class Paragraph


//
// Heading
//

//! Heading.
class Heading final
	:	public Item
{
public:
	Heading() = default;
	~Heading() override = default;

	ItemType type() const override;

	QSharedPointer< Paragraph > text() const;
	void setText( QSharedPointer< Paragraph > t );

	int level() const;
	void setLevel( int l );

	bool isLabeled() const;
	const QString & label() const;
	void setLabel( const QString & l );

private:
	QSharedPointer< Paragraph > m_text;
	int m_level;
	QString m_label;

	Q_DISABLE_COPY( Heading )
}; // class Heading


//
// Blockquote
//

//! Blockquote.
class Blockquote final
	:	public Block
{
public:
	Blockquote() = default;
	~Blockquote() override = default;

	ItemType type() const override;

private:
	Q_DISABLE_COPY( Blockquote )
}; // class Blockquote


//
// ListItem
//

//! List item.
class ListItem final
	:	public Block
{
public:
	ListItem() = default;
	~ListItem() override = default;

	ItemType type() const override;

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

	ListType listType() const;
	void setListType( ListType t );

	OrderedListPreState orderedListPreState() const;
	void setOrderedListPreState( OrderedListPreState s );

	int startNumber() const;
	void setStartNumber( int n );

	bool isTaskList() const;
	void setTaskList( bool on = true );

	bool isChecked() const;
	void setChecked( bool on = true );

private:
	ListType m_listType = Unordered;
	OrderedListPreState m_orderedListState = Start;
	int m_startNumber = 1;
	bool m_isTaskList = false;
	bool m_isChecked = false;

	Q_DISABLE_COPY( ListItem )
}; // class ListItem


//
// List
//

//! List.
class List final
	:	public Block
{
public:
	List() = default;
	~List() override = default;

	ItemType type() const override;

private:
	Q_DISABLE_COPY( List )
}; // class List


//
// Image
//

class Image final
	:	public Item
{
public:
	Image() = default;
	~Image() override = default;

	ItemType type() const override;

	const QString & url() const;
	void setUrl( const QString & u );

	const QString & text() const;
	void setText( const QString & t );

	bool isEmpty() const;

	QSharedPointer< Paragraph > p() const;
	void setP( QSharedPointer< Paragraph > v );

private:
	QString m_url;
	QString m_text;
	QSharedPointer< Paragraph > m_p;

	Q_DISABLE_COPY( Image )
}; // class Image


//
// Link
//

//! Link.
class Link
	:	public Item
{
public:
	Link();
	~Link() override = default;

	ItemType type() const override;

	const QString & url() const;
	void setUrl( const QString & u );

	const QString & text() const;
	void setText( const QString & t );

	TextOptions opts() const;
	void setOpts( const TextOptions & o );

	QSharedPointer< Image > img() const;
	void setImg( QSharedPointer< Image > i );

	QSharedPointer< Paragraph > p() const;
	void setP( QSharedPointer< Paragraph > v );

private:
	QString m_url;
	QString m_text;
	TextOptions m_opts;
	QSharedPointer< Image > m_img;
	QSharedPointer< Paragraph > m_p;

	Q_DISABLE_COPY( Link )
}; // class Link


//
// Code
//

//! Code.
class Code final
	:	public Item
{
public:
	explicit Code( const QString & t, bool inl = false );
	~Code() override = default;

	ItemType type() const override;

	const QString & text() const;

	bool isInlined() const;

	const QString & syntax() const;
	void setSyntax( const QString & s );

private:
	QString m_text;
	bool m_inlined;
	QString m_syntax;

	Q_DISABLE_COPY( Code )
}; // class Code


//
// TableCell
//

//! Table cell.
class TableCell final
	:	public Block
{
public:
	TableCell() = default;
	~TableCell() override = default;

	ItemType type() const override;

private:
	Q_DISABLE_COPY( TableCell )
}; // class TableCell


//
// TableRow
//

//! Table row.
class TableRow final
	:	public Item
{
public:
	TableRow() = default;
	~TableRow() override = default;

	ItemType type() const override;

	typedef QVector< QSharedPointer< TableCell > > Cells;

	const Cells & cells() const;
	void appendCell( QSharedPointer< TableCell > c );

	bool isEmpty() const;

private:
	Cells m_cells;

	Q_DISABLE_COPY( TableRow )
}; // class TableRow


//
// Table
//

//! Table.
class Table final
	:	public Item
{
public:
	Table() = default;
	~Table() override = default;

	ItemType type() const override;

	typedef QVector< QSharedPointer< TableRow > > Rows;

	const Rows & rows() const;
	void appendRow( QSharedPointer< TableRow > r );

	//! Alignment.
	enum Alignment {
		AlignLeft,
		AlignRight,
		AlignCenter
	}; // enum Alignmnet.

	typedef QVector< Alignment > ColumnsAlignments;

	Alignment columnAlignment( int idx ) const;
	void setColumnAlignment( int idx, Alignment a );

	int columnsCount() const;

	bool isEmpty() const;

private:
	Rows m_rows;
	ColumnsAlignments m_aligns;

	Q_DISABLE_COPY( Table )
}; // class Table


//
// FootnoteRef
//

//! Footnote ref.
class FootnoteRef final
	:	public Item
{
public:
	explicit FootnoteRef( const QString & i );
	~FootnoteRef() override = default;

	ItemType type() const override;

	const QString & id() const;

private:
	QString m_id;

	Q_DISABLE_COPY( FootnoteRef )
}; // class FootnoteRef


//
// Footnote
//

//! Footnote.
class Footnote final
	:	public Block
{
public:
	Footnote() = default;
	~Footnote() override = default;

	ItemType type() const override;

private:
	Q_DISABLE_COPY( Footnote )
}; // class Footnote


//
// Document
//

//! Document.
class Document final
	:	public Block
{
public:
	Document() = default;
	~Document() override = default;

	ItemType type() const override;

	typedef QMap< QString, QSharedPointer< Footnote > > Footnotes;

	const Footnotes & footnotesMap() const;
	void insertFootnote( const QString & id, QSharedPointer< Footnote > fn );

	typedef QMap< QString, QSharedPointer< Link > > LabeledLinks;

	const LabeledLinks & labeledLinks() const;
	void insertLabeledLink( const QString & label, QSharedPointer< Link > lnk );

	typedef QMap< QString, QSharedPointer< Heading > > LabeledHeadings;

	const LabeledHeadings & labeledHeadings() const;
	void insertLabeledHeading( const QString & label, QSharedPointer< Heading > h );

private:
	Footnotes m_footnotes;
	LabeledLinks m_labeledLinks;
	LabeledHeadings m_labeledHeadings;

	Q_DISABLE_COPY( Document )
}; // class Document;

} /* namespace MD */

#endif // MD_PDF_MD_DOC_HPP_INCLUDED
