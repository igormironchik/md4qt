
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

#ifndef MD_PDF_MD_PARSER_HPP_INCLUDED
#define MD_PDF_MD_PARSER_HPP_INCLUDED

// md-pdf include.
#include "md_doc.hpp"

// Qt include.
#include <QTextStream>

// C++ include.
#include <set>
#include <vector>


namespace MD {

static const QChar c_35 = QLatin1Char( '#' );
static const QChar c_46 = QLatin1Char( '.' );
static const QChar c_41 = QLatin1Char( ')' );
static const QChar c_96 = QLatin1Char( '`' );
static const QChar c_126 = QLatin1Char( '~' );
static const QChar c_9 = QLatin1Char( '\t' );
static const QChar c_32 = QLatin1Char( ' ' );
static const QChar c_62 = QLatin1Char( '>' );
static const QChar c_45 = QLatin1Char( '-' );
static const QChar c_43 = QLatin1Char( '+' );
static const QChar c_42 = QLatin1Char( '*' );
static const QChar c_91 = QLatin1Char( '[' );
static const QChar c_94 = QLatin1Char( '^' );
static const QChar c_93 = QLatin1Char( ']' );
static const QChar c_58 = QLatin1Char( ':' );
static const QChar c_124 = QLatin1Char( '|' );
static const QChar c_92 = QLatin1Char( '\\' );
static const QChar c_125 = QLatin1Char( '}' );
static const QChar c_61 = QLatin1Char( '=' );
static const QChar c_95 = QLatin1Char( '_' );
static const QChar c_34 = QLatin1Char( '"' );
static const QChar c_40 = QLatin1Char( '(' );
static const QChar c_33 = QLatin1Char( '!' );
static const QChar c_60 = QLatin1Char( '<' );
static const QChar c_10 = QLatin1Char( '\n' );
static const QChar c_13 = QLatin1Char( '\r' );
static const QChar c_39 = QLatin1Char( '\'' );
static const QChar c_47 = QLatin1Char( '/' );
static const QChar c_63 = QLatin1Char( '?' );
static const QChar c_38 = QLatin1Char( '&' );
static const QChar c_59 = QLatin1Char( ';' );
static const QChar c_120 = QLatin1Char( 'x' );
static const QChar c_36 = QLatin1Char( '$' );

static const QString c_startComment = QLatin1String( "<!--" );
static const QString c_endComment = QLatin1String( "-->" );


//! Skip spaces in line from pos \a i.
qsizetype
skipSpaces( qsizetype i, QStringView line );

//! \return Is string a footnote?
bool
isFootnote( const QString & s );

//! \return Starting sequence of the same characters.
QString
startSequence( const QString & line );

//! \return Is string a code fences?
bool
isCodeFences( const QString & s, bool closing = false );

//! \return Is file exist?
bool
fileExists( const QString & fileName, const QString & workingPath );

//! \return Is string an ordered list.
bool
isOrderedList( const QString & s, int * num = nullptr, int * len = nullptr,
	QChar * delim = nullptr, bool * isFirstLineEmpty = nullptr );

inline bool
indentInList( const std::set< qsizetype > * indents, qsizetype indent )
{
	if( indents )
		return ( indents->find( indent ) != indents->cend() );
	else
		return false;
};

// \return Is sequence of emphasis closed, and closing index of the sequence?
std::pair< bool, size_t >
checkEmphasisSequence( const std::vector< std::pair< qsizetype, int > > & s, size_t idx );

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

struct RawHtmlBlock {
	QSharedPointer< RawHtml > html = {};
	int htmlBlockType = -1;
	bool continueHtml = false;
	bool onLine = false;
}; // struct RawHtmlBlock

struct MdLineData {
	qsizetype lineNumber = -1;
	std::vector< bool > htmlCommentClosed = {};
}; // struct MdLineData

struct MdBlock {
	using Data = QVector< QPair< QString, MdLineData > >;

	Data data;
	qsizetype emptyLinesBefore = 0;
	bool emptyLineAfter = true;
}; // struct MdBlock

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
	QSharedPointer< Document > parse( const QString & fileName, bool recursive = true );

private:
	void parseFile( const QString & fileName, bool recursive, QSharedPointer< Document > doc,
		QStringList * parentLinks = nullptr );
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

#endif // MD_PDF_MD_PARSER_HPP_INCLUDED
