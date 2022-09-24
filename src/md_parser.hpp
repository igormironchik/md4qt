
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

#ifndef MD4QT_MD_PARSER_HPP_INCLUDED
#define MD4QT_MD_PARSER_HPP_INCLUDED

// md-pdf include.
#include "md_doc.hpp"

// Qt include.
#include <QTextStream>

// C++ include.
#include <set>
#include <vector>


namespace MD {

static const QChar c_32 = QLatin1Char( ' ' );


//
// RawHtmlBlock
//

//! Internal structure.
struct RawHtmlBlock {
	QSharedPointer< RawHtml > html = {};
	int htmlBlockType = -1;
	bool continueHtml = false;
	bool onLine = false;
}; // struct RawHtmlBlock


//
// MdLineData
//

//! Internal structure.
struct MdLineData {
	qsizetype lineNumber = -1;
	std::vector< bool > htmlCommentClosed = {};
}; // struct MdLineData


//
// MdBlock
//

//! Internal structure.
struct MdBlock {
	using Data = QVector< QPair< QString, MdLineData > >;

	Data data;
	qsizetype emptyLinesBefore = 0;
	bool emptyLineAfter = true;
}; // struct MdBlock


//
// StringListStream
//

//! Wrapper for QStringList to be behaved like a stream.
class StringListStream final
{
public:
	StringListStream( MdBlock::Data & stream )
		:	m_stream( stream )
		,	m_pos( 0 )
	{
	}

	bool atEnd() const { return ( m_pos >= m_stream.size() ); }
	QString readLine() { return m_stream.at( m_pos++ ).first; }
	qsizetype currentLineNumber() const
		{ return ( m_pos < size() ? m_stream.at( m_pos ).second.lineNumber : size() ); }
	QString lineAt( qsizetype pos ) { return m_stream.at( pos ).first; }
	qsizetype size() const { return m_stream.size(); }

private:
	MdBlock::Data & m_stream;
	int m_pos;
}; // class StringListStream


// \return Is sequence of emphasis closed, and closing index of the sequence?
std::pair< bool, size_t >
checkEmphasisSequence( const std::vector< std::pair< qsizetype, int > > & s, size_t idx );

//! \return Is string a footnote?
bool
isFootnote( const QString & s );

//! \return Is string a code fences?
bool
isCodeFences( const QString & s, bool closing = false );

//! \return Is string a start of code?
bool
isStartOfCode( QStringView str, QString * syntax = nullptr );

//! \return Is string a horizontal line?
bool
isHorizontalLine( QStringView s );

//! \return Is string a column alignment?
bool
isColumnAlignment( const QString & s );

//! \return Number of columns?
int
isTableAlignment( const QString & s );


//
// Parser
//

//! MD parser.
class Parser final
{
public:
	Parser() = default;
	~Parser() = default;

	//! \return Parsed Markdown document.
	QSharedPointer< Document > parse(
		//! File name of the Markdown document.
		const QString & fileName,
		//! Should parsing be recursive? If recursive all links to existing Markdown
		//! files will be parsed and presented in the returned document.
		bool recursive = true,
		//! Allowed extensions for Markdonw document files. If Markdown file doesn't
		//! have given extension it will be ignored.
		const QStringList & ext =
			QStringList() << QStringLiteral( "md" ) << QStringLiteral( "markdown" ) );

	//! \return Parsed Markdown document.
	QSharedPointer< Document > parse(
		//! Stream to parse.
		QTextStream & stream,
		//! This argument needed only for anchor.
		const QString & fileName );

private:
	void parseFile( const QString & fileName, bool recursive, QSharedPointer< Document > doc,
		const QStringList & ext, QStringList * parentLinks = nullptr );
	void parseStream( QTextStream & stream,
		const QString & workingPath, const QString & fileName,
		bool recursive, QSharedPointer< Document > doc,
		const QStringList & ext, QStringList * parentLinks = nullptr );
	void clearCache();

	enum class BlockType {
		Unknown,
		Text,
		List,
		ListWithFirstEmptyLine,
		CodeIndentedBySpaces,
		Code,
		Blockquote,
		Heading
	}; // enum BlockType

	BlockType whatIsTheLine( QString & str, bool inList = false, qsizetype * indent = nullptr,
		bool calcIndent = false, const std::set< qsizetype > * indents = nullptr ) const;
	void parseFragment( MdBlock & fr, QSharedPointer< Block > parent,
		QSharedPointer< Document > doc,
		QStringList & linksToParse, const QString & workingPath,
		const QString & fileName, bool collectRefLinks,
		RawHtmlBlock & html );
	void parseText( MdBlock & fr, QSharedPointer< Block > parent,
		QSharedPointer< Document > doc,
		QStringList & linksToParse, const QString & workingPath,
		const QString & fileName, bool collectRefLinks,
		RawHtmlBlock & html );
	void parseBlockquote( MdBlock & fr, QSharedPointer< Block > parent,
		QSharedPointer< Document > doc,
		QStringList & linksToParse, const QString & workingPath,
		const QString & fileName, bool collectRefLinks,
		RawHtmlBlock & html );
	void parseList( MdBlock & fr, QSharedPointer< Block > parent,
		QSharedPointer< Document > doc,
		QStringList & linksToParse, const QString & workingPath,
		const QString & fileName, bool collectRefLinks,
		RawHtmlBlock & html );
	void parseCode( MdBlock & fr, QSharedPointer< Block > parent,
		bool collectRefLinks, int indent = 0 );
	void parseCodeIndentedBySpaces( MdBlock & fr, QSharedPointer< Block > parent,
		bool collectRefLinks,
		int indent = 4, const QString & syntax = QString() );
	void parseListItem( MdBlock & fr, QSharedPointer< Block > parent,
		QSharedPointer< Document > doc,
		QStringList & linksToParse, const QString & workingPath,
		const QString & fileName, bool collectRefLinks,
		RawHtmlBlock & html );
	void parseHeading( MdBlock & fr, QSharedPointer< Block > parent,
		QSharedPointer< Document > doc,
		QStringList & linksToParse, const QString & workingPath,
		const QString & fileName,
		bool collectRefLinks );
	void parseFootnote( MdBlock & fr, QSharedPointer< Block > parent,
		QSharedPointer< Document > doc,
		QStringList & linksToParse, const QString & workingPath,
		const QString & fileName, bool collectRefLinks );
	void parseTable( MdBlock & fr, QSharedPointer< Block > parent,
		QSharedPointer< Document > doc,
		QStringList & linksToParse, const QString & workingPath,
		const QString & fileName, bool collectRefLinks, int columnsCount );
	void parseParagraph( MdBlock & fr, QSharedPointer< Block > parent,
		QSharedPointer< Document > doc,
		QStringList & linksToParse, const QString & workingPath,
		const QString & fileName, bool collectRefLinks,
		RawHtmlBlock & html );
	void parseFormattedTextLinksImages( MdBlock & fr, QSharedPointer< Block > parent,
		QSharedPointer< Document > doc,
		QStringList & linksToParse, const QString & workingPath,
		const QString & fileName, bool collectRefLinks, bool ignoreLineBreak,
		RawHtmlBlock & html );

	void parse( StringListStream & stream, QSharedPointer< Block > parent,
		QSharedPointer< Document > doc, QStringList & linksToParse,
		const QString & workingPath, const QString & fileName,
		bool collectRefLinks,
		bool top = false );

private:
	QStringList m_parsedFiles;

	Q_DISABLE_COPY( Parser )
}; // class Parser

} /* namespace MD */

#endif // MD4QT_MD_PARSER_HPP_INCLUDED
