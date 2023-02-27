
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

// md4qt include.
#include "traits.hpp"
#include "doc.hpp"
#include "entities_map.hpp"
#include "utils.hpp"

#ifdef MD4QT_QT_SUPPORT

// Qt include.
#include <QTextStream>
#include <QFile>
#include <QDir>
#include <QRegularExpression>

#endif // MD4QT_QT_SUPPORT

#ifdef MD4QT_ICU_STL_SUPPORT

// ICU include.
#include <unicode/regex.h>

// C++ include.
#include <exception>

#endif // MD4QT_ICU_STL_SUPPORT

// C++ include.
#include <set>
#include <vector>
#include <memory>
#include <tuple>
#include <algorithm>
#include <stack>
#include <cmath>
#include <fstream>


namespace MD {

static const char * c_startComment = "<!--";

inline bool
indentInList( const std::set< long long int > * indents, long long int indent )
{
	if( indents )
		return ( indents->find( indent ) != indents->cend() );
	else
		return false;
};

// Skip spaces in line from pos \p i.
template< class Trait >
inline long long int
skipSpaces( long long int i, const typename Trait::String & line )
{
	const auto length = line.length();

	while( i < length && line[ i ].isSpace() )
		++i;

	return i;
}; // skipSpaces


//! \return Starting sequence of the same characters.
template< class Trait >
inline typename Trait::String
startSequence( const typename Trait::String & line )
{
	auto pos = skipSpaces< Trait >( 0, line );

	if( pos >= line.length() )
		return {};

	const auto sch = ( pos < line.length() ? line[ pos ] : typename Trait::Char() );
	const auto start = pos;

	++pos;

	while( pos < line.length() && line[ pos ] == sch )
		++pos;

	return line.sliced( start, pos - start );
}

//! \return Is string an ordered list.
template< class Trait >
inline bool
isOrderedList( const typename Trait::String & s, int * num = nullptr, int * len = nullptr,
	typename Trait::Char * delim = nullptr, bool * isFirstLineEmpty = nullptr )
{
	long long int p = skipSpaces< Trait >( 0, s );

	long long int dp = p;

	for( ; p < s.size(); ++p )
	{
		if( !s[ p ].isDigit() )
			break;
	}

	if( dp != p && p < s.size() )
	{
		const auto digits = s.sliced( dp, p - dp );

		if( digits.size() > 9 )
			return false;

		const auto i = digits.toInt();

		if( num )
			*num = i;

		if( len )
			*len = p - dp;

		if( s[ p ] == typename Trait::Char( '.' ) || s[ p ] == typename Trait::Char( ')' ) )
		{
			if( delim )
				*delim = s[ p ];

			++p;

			long long int tmp = skipSpaces< Trait >( p, s );

			if( isFirstLineEmpty )
				*isFirstLineEmpty = ( tmp == s.size() );

			if( ( p < s.size() && s[ p ] == typename Trait::Char( ' ' ) ) || p == s.size() )
				return true;
		}
	}

	return false;
}


//
// RawHtmlBlock
//

//! Internal structure.
template< class Trait >
struct RawHtmlBlock {
	std::shared_ptr< RawHtml< Trait > > html = {};
	int htmlBlockType = -1;
	bool continueHtml = false;
	bool onLine = false;
}; // struct RawHtmlBlock


//
// MdLineData
//

//! Internal structure.
struct MdLineData {
	long long int lineNumber = -1;
	std::vector< bool > htmlCommentClosed = {};
}; // struct MdLineData


//
// MdBlock
//

//! Internal structure.
template< class Trait >
struct MdBlock {
	using Data = typename Trait::template Vector<
		std::pair< typename Trait::InternalString, MdLineData > >;

	Data data;
	long long int emptyLinesBefore = 0;
	bool emptyLineAfter = true;
}; // struct MdBlock


//
// StringListStream
//

//! Wrapper for typename Trait::StringList to be behaved like a stream.
template< class Trait >
class StringListStream final
{
public:
	StringListStream( typename MdBlock< Trait >::Data & stream )
		:	m_stream( stream )
		,	m_pos( 0 )
	{
	}

	bool atEnd() const { return ( m_pos >= (long long int) m_stream.size() ); }
	typename Trait::InternalString readLine() { return m_stream.at( m_pos++ ).first; }
	long long int currentLineNumber() const
		{ return ( m_pos < size() ? m_stream.at( m_pos ).second.lineNumber : size() ); }
	typename Trait::InternalString lineAt( long long int pos ) { return m_stream.at( pos ).first; }
	long long int size() const { return m_stream.size(); }

private:
	typename MdBlock< Trait >::Data & m_stream;
	long long int m_pos;
}; // class StringListStream


inline void
collapseStack( std::stack< std::pair< long long int, int > > & st, long long int v, int type,
	size_t idx )
{
	while( !st.empty() && v > 0 )
	{
		if( st.top().second != type )
			break;

		v -= st.top().first;
		st.pop();

		if( v > 0 )
		{
			while( !st.empty() && st.size() > idx + 1 && st.top().second != type )
				st.pop();
		}
	}
}

// \return Is sequence of emphasis closed, and closing index of the sequence?
inline std::pair< bool, size_t >
checkEmphasisSequence( const std::vector< std::pair< long long int, int > > & s, size_t idx )
{
	std::stack< std::pair< long long int, int > > st;

	for( size_t i = 0; i <= idx; ++i )
		st.push( s[ i ] );

	size_t ret = 0;
	bool retInit = false;
	size_t ii = idx;

	for( size_t i = idx + 1; i < s.size(); ++i )
	{
		if( s[ i ].first > 0 )
			st.push( s[ i ] );
		else
		{
			if( !st.empty() && st.top().second == s[ i ].second )
			{
				auto v = std::abs( s[ i ].first );

				if( st.top().first <= v )
					collapseStack( st, v, s[ i ].second, ii );
				else
					st.top().first += s[ i ].first;
			}
			else
			{
				while( !st.empty() && st.size() > ii + 1 && st.top().second != s[ i ].second )
					st.pop();

				if( !st.empty() && st.top().second == s[ i ].second )
				{
					auto v = std::abs( s[ i ].first );

					if( st.top().first <= v )
						collapseStack( st, v, s[ i ].second, ii );
					else
						st.top().first += s[ i ].first;
				}
			}
		}

		if( st.size() < ii + 1 )
			ii = st.size() - 1;

		if( st.size() <= idx && !retInit )
		{
			ret = i;
			retInit = true;
		}
	}

	return { st.empty(), ret };
}

//! \return Is string a footnote?
template< class Trait >
inline bool
isFootnote( const typename Trait::String & s )
{
	long long int p = 0;

	for( ; p < s.size(); ++p )
	{
		if( !s[ p ].isSpace() )
			break;
	}

	if( s.size() - p < 5 )
		return false;

	if( s[ p++ ] != typename Trait::Char( '[' ) )
		return false;

	if( s[ p++ ] != typename Trait::Char( '^' ) )
		return false;

	if( s[ p ] == typename Trait::Char( ']' ) || s[ p ].isSpace() )
		return false;

	for( ; p < s.size(); ++p )
	{
		if( s[ p ] == typename Trait::Char( ']' ) )
			break;
		else if( s[ p ].isSpace() )
			return false;
	}

	++p;

	if( p < s.size() && s[ p ] == typename Trait::Char( ':' ) )
		return true;
	else
		return false;
}

//! \return Is string a code fences?
template< class Trait >
inline bool
isCodeFences( const typename Trait::String & s, bool closing = false )
{
	auto p = skipSpaces< Trait >( 0, s );

	if( p > 3 )
		return false;

	const auto ch = s[ p ];

	if( ch != typename Trait::Char( '~' ) && ch != typename Trait::Char( '`' ) )
		return false;

	bool space = false;

	long long int c = 1;
	++p;

	for( ; p < s.length(); ++p )
	{
		if( s[ p ].isSpace() )
			space = true;
		else if( s[ p ] == ch )
		{
			if( space && ( closing ? true : ch == typename Trait::Char( '`' ) ) )
				return false;

			if( !space )
				++c;
		}
		else if( closing )
			return false;
		else
			break;
	}

	if( c < 3 )
		return false;

	if( ch == typename Trait::Char( '`' ) )
	{
		for( ; p < s.length(); ++p )
		{
			if( s[ p ] == typename Trait::Char( '`' ) )
				return false;
		}
	}

	return true;
}

template< class Trait >
inline typename Trait::String
readEscapedSequence( long long int i, const typename Trait::String & str )
{
	bool backslash = false;
	const auto start = i;

	if( start >= str.length() )
		return {};

	while( i < str.length() )
	{
		bool now = false;

		if( str[ i ] == typename Trait::Char( '\\' ) && !backslash )
		{
			backslash = true;
			now = true;
		}
		else if( str[ i ].isSpace() && !backslash )
			break;

		if( !now )
			backslash = false;

		++i;
	}

	return str.sliced( start, i - start );
}

template< class Trait >
static const typename Trait::String c_canBeEscaped = "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";

template< class Trait >
inline typename Trait::InternalString
removeBackslashes( const typename Trait::InternalString & s )
{
	typename Trait::InternalString r = s;
	bool backslash = false;
	long long int extra = 0;

	for( long long int i = 0; i < s.length(); ++i )
	{
		bool now = false;

		if( s[ i ] == typename Trait::Char( '\\' ) && !backslash && i != s.length() - 1 )
		{
			backslash = true;
			now = true;
		}
		else if( c_canBeEscaped< Trait >.contains( s[ i ] ) && backslash )
		{
			r.remove( i - extra - 1, 1 );
			++extra;
		}

		if( !now )
			backslash = false;
	}

	return r;
}

//! \return Is string a start of code?
template< class Trait >
inline bool
isStartOfCode( const typename Trait::String & str, typename Trait::String * syntax = nullptr )
{
	if( str.size() < 3 )
		return false;

	const bool c96 = str[ 0 ] == typename Trait::Char( '`' );
	const bool c126 = str[ 0 ] == typename Trait::Char( '~' );

	if( c96 || c126 )
	{
		long long int p = 1;
		long long int c = 1;

		while( p < str.length() )
		{
			if( str[ p ] != ( c96 ? typename Trait::Char( '`' ) : typename Trait::Char( '~' ) ) )
				break;

			++c;
			++p;
		}

		if( c < 3 )
			return false;

		if( syntax )
		{
			p = skipSpaces< Trait >( p, str );

			if( p < str.size() )
				*syntax = removeBackslashes< Trait >(
					readEscapedSequence< Trait >( p, str ) ).asString();
		}

		return true;
	}

	return false;
}

//! \return Is string a horizontal line?
template< class Trait >
inline bool
isHorizontalLine( const typename Trait::String & s )
{

	if( s.size() < 3 )
		return false;

	typename Trait::Char c;

	if( s[ 0 ] == typename Trait::Char( '*' ) )
		c = '*';
	else if( s[ 0 ] == typename Trait::Char( '-' ) )
		c = '-';
	else if( s[ 0 ] == typename Trait::Char( '_' ) )
		c = '_';
	else
		return false;

	long long int p = 1;
	long long int count = 1;

	for( ; p < s.size(); ++p )
	{
		if( s[ p ] != c && !s[ p ].isSpace() )
			break;
		else if( s[ p ] == c )
			++count;
	}

	if( count < 3 )
		return false;

	if( p == s.size() )
		return true;

	return false;
}

//! \return Is string a column alignment?
template< class Trait >
inline bool
isColumnAlignment( const typename Trait::String & s )
{
	long long int p = skipSpaces< Trait >( 0, s );

	static const typename Trait::String c_legitime = ":-";

	if( !c_legitime.contains( s[ p ] ) )
		return false;

	if( s[ p ] == typename Trait::Char( ':' ) )
		++p;

	for( ; p < s.size(); ++p )
	{
		if( s[ p ] != typename Trait::Char( '-' ) )
			break;
	}

	if( p == s.size() )
		return true;

	if( s[ p ] != typename Trait::Char( ':' ) && !s[ p ].isSpace() )
		return false;

	++p;

	for( ; p < s.size(); ++p )
	{
		if( !s[ p ].isSpace() )
			return false;
	}

	return true;
}

template< class Trait >
typename Trait::StringList
splitString( const typename Trait::String & str, const typename Trait::Char & ch );

#ifdef MD4QT_ICU_STL_SUPPORT

template<>
inline typename UnicodeStringTrait::StringList
splitString< UnicodeStringTrait >( const UnicodeString & str, const UnicodeChar & ch )
{
       return str.split( ch );
}

#endif

#ifdef MD4QT_QT_SUPPORT

template<>
inline typename QStringTrait::StringList
splitString< QStringTrait >( const QString & str, const QChar & ch )
{
       return str.split( ch, Qt::SkipEmptyParts );
}

#endif


//! \return Number of columns?
template< class Trait >
inline int
isTableAlignment( const typename Trait::String & s )
{
	const auto columns = splitString< Trait >( s.simplified(), '|' );

	for( const auto & c : columns )
	{
		if( !isColumnAlignment< Trait >( c ) )
			return 0;
	}

	return columns.size();
}

//! \return Is given string a HTML comment.
template< class Trait >
inline bool
isHtmlComment( const typename Trait::String & s )
{
	auto c = s;

	if( s.startsWith( c_startComment ) )
		c.remove( 0, 4 );
	else
		return false;

	if( c.startsWith( '>' ) )
		return false;

	if( c.startsWith( "->" ) )
		return false;

	const auto p = c.indexOf( "--" );

	if( p > -1 )
	{
		if( c.size() > p + 2 )
			return c[ p + 2 ] == typename Trait::Char( '>' );
		else
			return false;
	}
	else
		return false;
}


//
// Parser
//

//! MD parser.
template< class Trait >
class Parser final
{
public:
	Parser() = default;
	~Parser() = default;

	//! \return Parsed Markdown document.
	std::shared_ptr< Document< Trait > > parse(
		//! File name of the Markdown document.
		const typename Trait::String & fileName,
		//! Should parsing be recursive? If recursive all links to existing Markdown
		//! files will be parsed and presented in the returned document.
		bool recursive = true,
		//! Allowed extensions for Markdonw document files. If Markdown file doesn't
		//! have given extension it will be ignored.
		const typename Trait::StringList & ext = { "md", "markdown" } );

	//! \return Parsed Markdown document.
	std::shared_ptr< Document< Trait > > parse(
		//! Stream to parse.
		typename Trait::TextStream & stream,
		//! This argument needed only for anchor.
		const typename Trait::String & fileName );

private:
	void parseFile( const typename Trait::String & fileName, bool recursive,
		std::shared_ptr< Document< Trait > > doc,
		const typename Trait::StringList & ext,
		typename Trait::StringList * parentLinks = nullptr );
	void parseStream( typename Trait::TextStream & stream,
		const typename Trait::String & workingPath, const typename Trait::String & fileName,
		bool recursive, std::shared_ptr< Document< Trait > > doc,
		const typename Trait::StringList & ext,
		typename Trait::StringList * parentLinks = nullptr );
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

	BlockType whatIsTheLine( typename Trait::InternalString & str, bool inList = false,
		long long int * indent = nullptr,
		bool calcIndent = false, const std::set< long long int > * indents = nullptr ) const;
	void parseFragment( MdBlock< Trait > & fr, std::shared_ptr< Block< Trait > > parent,
		std::shared_ptr< Document< Trait > > doc,
		typename Trait::StringList & linksToParse, const typename Trait::String & workingPath,
		const typename Trait::String & fileName, bool collectRefLinks,
		RawHtmlBlock< Trait > & html );
	void parseText( MdBlock< Trait > & fr, std::shared_ptr< Block< Trait > > parent,
		std::shared_ptr< Document< Trait > > doc,
		typename Trait::StringList & linksToParse, const typename Trait::String & workingPath,
		const typename Trait::String & fileName, bool collectRefLinks,
		RawHtmlBlock< Trait > & html );
	void parseBlockquote( MdBlock< Trait > & fr,
		std::shared_ptr< Block< Trait > > parent,
		std::shared_ptr< Document< Trait > > doc,
		typename Trait::StringList & linksToParse, const typename Trait::String & workingPath,
		const typename Trait::String & fileName, bool collectRefLinks,
		RawHtmlBlock< Trait > & html );
	void parseList( MdBlock< Trait > & fr,
		std::shared_ptr< Block< Trait > > parent,
		std::shared_ptr< Document< Trait > > doc,
		typename Trait::StringList & linksToParse, const typename Trait::String & workingPath,
		const typename Trait::String & fileName, bool collectRefLinks,
		RawHtmlBlock< Trait > & html );
	void parseCode( MdBlock< Trait > & fr,
		std::shared_ptr< Block< Trait > > parent,
		bool collectRefLinks, int indent = 0 );
	void parseCodeIndentedBySpaces( MdBlock< Trait > & fr,
		std::shared_ptr< Block< Trait > > parent,
		bool collectRefLinks,
		int indent = 4, const typename Trait::String & syntax = typename Trait::String() );
	void parseListItem( MdBlock< Trait > & fr,
		std::shared_ptr< Block< Trait > > parent,
		std::shared_ptr< Document< Trait > > doc,
		typename Trait::StringList & linksToParse, const typename Trait::String & workingPath,
		const typename Trait::String & fileName, bool collectRefLinks,
		RawHtmlBlock< Trait > & html );
	void parseHeading( MdBlock< Trait > & fr,
		std::shared_ptr< Block< Trait > > parent,
		std::shared_ptr< Document< Trait > > doc,
		typename Trait::StringList & linksToParse, const typename Trait::String & workingPath,
		const typename Trait::String & fileName,
		bool collectRefLinks );
	void parseFootnote( MdBlock< Trait > & fr,
		std::shared_ptr< Block< Trait > > parent,
		std::shared_ptr< Document< Trait > > doc,
		typename Trait::StringList & linksToParse, const typename Trait::String & workingPath,
		const typename Trait::String & fileName, bool collectRefLinks );
	void parseTable( MdBlock< Trait > & fr,
		std::shared_ptr< Block< Trait > > parent,
		std::shared_ptr< Document< Trait > > doc,
		typename Trait::StringList & linksToParse, const typename Trait::String & workingPath,
		const typename Trait::String & fileName, bool collectRefLinks, int columnsCount );
	void parseParagraph( MdBlock< Trait > & fr,
		std::shared_ptr< Block< Trait > > parent,
		std::shared_ptr< Document< Trait > > doc,
		typename Trait::StringList & linksToParse, const typename Trait::String & workingPath,
		const typename Trait::String & fileName, bool collectRefLinks,
		RawHtmlBlock< Trait > & html );
	void parseFormattedTextLinksImages( MdBlock< Trait > & fr,
		std::shared_ptr< Block< Trait > > parent,
		std::shared_ptr< Document< Trait > > doc,
		typename Trait::StringList & linksToParse, const typename Trait::String & workingPath,
		const typename Trait::String & fileName, bool collectRefLinks, bool ignoreLineBreak,
		RawHtmlBlock< Trait > & html );

	void parse( StringListStream< Trait> & stream,
		std::shared_ptr< Block< Trait > > parent,
		std::shared_ptr< Document< Trait > > doc,
		typename Trait::StringList & linksToParse,
		const typename Trait::String & workingPath,
		const typename Trait::String & fileName,
		bool collectRefLinks,
		bool top = false );

private:
	typename Trait::StringList m_parsedFiles;

	DISABLE_COPY( Parser )
}; // class Parser


//
// Parser
//

template< class Trait >
inline std::shared_ptr< Document< Trait > >
Parser< Trait >::parse( const typename Trait::String & fileName, bool recursive,
	const typename Trait::StringList & ext )
{
	std::shared_ptr< Document< Trait > > doc( new Document< Trait > );

	parseFile( fileName, recursive, doc, ext );

	clearCache();

	return doc;
}

template< class Trait >
inline std::shared_ptr< Document< Trait > >
Parser< Trait >::parse( typename Trait::TextStream & stream,
	const typename Trait::String & fileName )
{
	std::shared_ptr< Document< Trait > > doc( new Document< Trait > );

	parseStream( stream, typename Trait::String( "." ), fileName, false, doc,
		typename Trait::StringList() );

	clearCache();

	return doc;
}

template< class Trait >
class TextStream;

#ifdef MD4QT_QT_SUPPORT

//! Wrapper for QTextStream.
template<>
class TextStream< QStringTrait >
{
public:
	TextStream( QTextStream & stream )
		:	m_stream( stream )
		,	m_lastBuf( false )
		,	m_pos( 0 )
	{
	}

	bool atEnd() const { return ( m_lastBuf && m_pos == m_buf.size() ); }

	QString readLine()
	{
		QString line;
		bool rFound = false;

		while( !atEnd() )
		{
			const auto c = getChar();

			if( rFound && c != '\n' )
			{
				--m_pos;

				return line;
			}

			if( c == '\r' )
			{
				rFound = true;

				continue;
			}
			else if( c == '\n' )
				return line;

			if( !c.isNull() )
				line.push_back( c );
		}

		return line;
	}

private:
	void fillBuf()
	{
		m_buf = m_stream.read( 512 );

		if( m_stream.atEnd() )
			m_lastBuf = true;

		m_pos = 0;
	}

	QChar getChar()
	{
		if( m_pos < m_buf.size() )
			return m_buf.at( m_pos++ );
		else if( !atEnd() )
		{
			fillBuf();

			return getChar();
		}
		else
			return QChar();
	}

private:
	QTextStream & m_stream;
	QString m_buf;
	bool m_lastBuf;
	long long int m_pos;
}; // class TextStream

#endif

#ifdef MD4QT_ICU_STL_SUPPORT

//! Wrapper for std::istream.
template<>
class TextStream< UnicodeStringTrait >
{
public:
	TextStream( std::istream & stream )
		:	m_pos( 0 )
	{
		std::vector< unsigned char > content;

		stream.seekg( 0, std::ios::end );
		const auto ssize = stream.tellg();
		content.resize( (size_t) ssize + 1 );
		stream.seekg( 0, std::ios::beg );
		stream.read( (char*) &content[ 0 ], ssize );
		content[ (size_t) ssize ] = 0;

		const auto z = std::count( content.cbegin(), content.cend(), 0 );

		if( z > 1 )
		{
			std::vector< unsigned char > tmp;
			tmp.resize( content.size() + ( z - 1 ) * 2 );

			for( size_t i = 0, j = 0; i < content.size() - 1; ++i, ++j )
			{
				if( content[ i ] == 0 )
				{
					// 0xFFFD - replacement character in UTF-8.
					tmp[ j++ ] = 0xEF;
					tmp[ j++ ] = 0xBF;
					tmp[ j ] = 0xBD;
				}
				else
					tmp[ j ] = content[ i ];
			}

			tmp[ tmp.size() - 1 ] = 0;

			std::swap( content, tmp );
		}

		m_str = UnicodeString::fromUTF8( (char*) &content[ 0 ] );
	}

	bool atEnd() const { return m_pos == m_str.size(); }

	UnicodeString readLine()
	{
		UnicodeString line;

		bool rFound = false;

		while( !atEnd() )
		{
			const auto c = getChar();

			if( rFound && c != UnicodeChar( '\n' ) )
			{
				--m_pos;

				return line;
			}

			if( c == UnicodeChar( '\r' ) )
			{
				rFound = true;

				continue;
			}
			else if( c == UnicodeChar( '\n' ) )
				return line;

			if( !c.isNull() )
				line.push_back( c );
		}

		return line;
	}

private:
	UnicodeChar getChar()
	{
		if( !atEnd() )
			return m_str[ m_pos++ ];
		else
			return UnicodeChar();
	}

private:
	UnicodeString m_str;
	long long int m_pos;
};

#endif

template< class Trait >
inline bool
checkForEndHtmlComments( const typename Trait::String & line, long long int pos,
	std::vector< bool > & res )
{
	long long int e = line.indexOf( "--", pos );

	if( e != -1 )
	{
		if( e + 2 < line.size() && line[ e + 2 ] == typename Trait::Char( '>' ) )
			res.push_back( isHtmlComment< Trait >( line.sliced( 0, e + 3 ) ) );
		else
			res.push_back( false );

		return true;
	}

	return false;
}

template< class Trait >
inline void
checkForHtmlComments( const typename Trait::String & line, StringListStream< Trait > & stream,
	std::vector< bool > & res )
{
	long long int p = 0, l = stream.currentLineNumber();

	bool addFalse = false;

	while( ( p = line.indexOf( c_startComment, p ) ) != -1 )
	{
		auto c = line.sliced( p );

		if( !checkForEndHtmlComments< Trait >( c, 4, res ) )
		{
			for( ; l < stream.size(); ++l )
			{
				c.push_back( typename Trait::Char( ' ' ) );
				c.push_back( stream.lineAt( l ).asString() );

				if( checkForEndHtmlComments< Trait >( c, 4, res ) )
				{
					addFalse = false;

					break;
				}
				else
					addFalse = true;
			}
		}

		if( addFalse )
			res.push_back( false );

		++p;
	}
}

template< class Trait >
inline void
Parser< Trait >::parse( StringListStream< Trait > & stream,
	std::shared_ptr< Block< Trait > > parent,
	std::shared_ptr< Document< Trait > > doc,
	typename Trait::StringList & linksToParse,
	const typename Trait::String & workingPath,
	const typename Trait::String & fileName,
	bool collectRefLinks, bool top )
{
	typename Trait::template Vector< MdBlock< Trait > > splitted;

	typename MdBlock< Trait >::Data fragment;

	BlockType type = BlockType::Unknown;
	bool emptyLineInList = false;
	long long int emptyLinesCount = 0;
	long long int lineCounter = 0;
	std::set< long long int > indents;
	long long int indent = 0;
	RawHtmlBlock< Trait > html;
	long long int emptyLinesBefore = 0;
	std::vector< bool > htmlCommentClosed;

	// Parse fragment and clear internal cache.
	auto pf = [&]()
		{
			if( !fragment.empty() )
			{
				MdBlock< Trait > block = { fragment, emptyLinesBefore, emptyLinesCount > 0 };

				emptyLinesBefore = emptyLinesCount;

				splitted.push_back( block );

				parseFragment( block, parent, doc, linksToParse,
					workingPath, fileName, collectRefLinks, html );

				fragment.clear();
			}

			type = BlockType::Unknown;
			emptyLineInList = false;
			emptyLinesCount = 0;
			lineCounter = 0;
			indents.clear();
			indent = 0;
		};

	// Eat footnote.
	auto eatFootnote = [&]()
		{
			while( !stream.atEnd() )
			{
				const auto currentLineNumber = stream.currentLineNumber();

				auto line = stream.readLine();

				if( line.isEmpty() || line.asString().startsWith( "    " ) ||
					line.asString().startsWith( '\t' ) )
				{
					fragment.push_back( { line, { currentLineNumber, htmlCommentClosed } } );
				}
				else
				{
					pf();

					type = whatIsTheLine( line );
					fragment.push_back( { line, { currentLineNumber, htmlCommentClosed } } );

					break;
				}
			}

			if( stream.atEnd() && !fragment.empty() )
				pf();
		};

	typename Trait::String startOfCode;

	while( !stream.atEnd() )
	{
		htmlCommentClosed.clear();

		const auto currentLineNumber = stream.currentLineNumber();

		auto line = stream.readLine();

		static const char16_t c_zeroReplaceWith[ 2 ] = { 0xFFFD, 0 };

		line.replace( typename Trait::Char( 0 ), Trait::utf16ToString( &c_zeroReplaceWith[ 0 ] ) );

		checkForHtmlComments( line.asString(), stream, htmlCommentClosed );

		const long long int prevIndent = indent;

		BlockType lineType = whatIsTheLine( line, emptyLineInList, &indent,
			true, &indents );

		if( prevIndent != indent )
			indents.insert( indent );

		const long long int currentIndent = indent;

		const auto ns = skipSpaces< Trait >( 0, line.asString() );

		if( type == BlockType::CodeIndentedBySpaces && ns > 3 )
			lineType = BlockType::CodeIndentedBySpaces;

		if( type == BlockType::ListWithFirstEmptyLine && lineCounter == 2 &&
			lineType != BlockType::ListWithFirstEmptyLine && lineType != BlockType::List )
		{
			if( emptyLinesCount > 0 )
			{
				pf();

				fragment.push_back( { line, { currentLineNumber, htmlCommentClosed } } );
				type = lineType;

				continue;
			}
			else
			{
				emptyLineInList = false;
				emptyLinesCount = 0;
			}
		}

		if( type == BlockType::ListWithFirstEmptyLine && lineCounter == 2 )
			type = BlockType::List;

		if( lineType == BlockType::ListWithFirstEmptyLine )
		{
			type = lineType;
			lineCounter = 1;
			fragment.push_back( { line, { currentLineNumber, htmlCommentClosed } } );

			continue;
		}

		// First line of the fragment.
		if( ns != line.length() && type == BlockType::Unknown )
		{
			type = lineType;

			++lineCounter;

			if( type == BlockType::Code )
				startOfCode = startSequence< Trait >( line.asString() );

			fragment.push_back( { line, { currentLineNumber, htmlCommentClosed } } );

			if( type == BlockType::Heading )
				pf();

			continue;
		}
		else if( ns == line.length() && type == BlockType::Unknown )
			continue;

		++lineCounter;

		// Got new empty line.
		if( ns == line.length() )
		{
			++emptyLinesCount;

			switch( type )
			{
				case BlockType::Text :
				{
					if( isFootnote< Trait >( fragment.front().first.asString() ) )
					{
						fragment.push_back( { typename Trait::String(),
							{ currentLineNumber, htmlCommentClosed } } );

						eatFootnote();
					}

					continue;
				}

				case BlockType::Blockquote :
				{
					pf();

					continue;
				}

				case BlockType::CodeIndentedBySpaces :
					continue;
					break;

				case BlockType::Code :
				{
					fragment.push_back( { line, { currentLineNumber, htmlCommentClosed } } );
					emptyLinesCount = 0;

					continue;
				}

				case BlockType::List :
				case BlockType::ListWithFirstEmptyLine :
				{
					emptyLineInList = true;

					continue;
				}

				default :
					break;
			}
		}
		//! Empty new line in list.
		else if( emptyLineInList )
		{
			if( indentInList( &indents, ns ) || lineType == BlockType::List ||
				lineType == BlockType::CodeIndentedBySpaces )
			{
				for( long long int i = 0; i < emptyLinesCount; ++i )
					fragment.push_back( { typename Trait::String(),
						{ currentLineNumber - emptyLinesCount + i, {} } } );

				fragment.push_back( { line, { currentLineNumber, htmlCommentClosed } } );

				emptyLineInList = false;
				emptyLinesCount = 0;

				continue;
			}
			else
			{
				pf();

				type = lineType;
				fragment.push_back( { line, { currentLineNumber, htmlCommentClosed } } );
				emptyLinesCount = 0;

				continue;
			}
		}
		else if( emptyLinesCount > 0 )
		{
			if( type == BlockType::CodeIndentedBySpaces &&
				lineType == BlockType::CodeIndentedBySpaces )
			{
				const auto indent = skipSpaces< Trait >( 0, fragment.front().first.asString() );

				for( long long int i = 0; i < emptyLinesCount; ++i )
					fragment.push_back( { typename Trait::String( indent, ' ' ),
						{ currentLineNumber - emptyLinesCount + i, {} } } );
			}
			else
			{
				const auto empty = emptyLinesCount;

				pf();

				if( html.htmlBlockType >= 6 )
					html.continueHtml = ( empty <= 0 );

				type = lineType;

				if( type == BlockType::List || type == BlockType::ListWithFirstEmptyLine )
					indents.insert( currentIndent );
				else if( type == BlockType::Code )
					startOfCode = startSequence< Trait >( line.asString() );
			}

			fragment.push_back( { line, { currentLineNumber, htmlCommentClosed } } );
			emptyLinesCount = 0;

			continue;
		}

		// Something new and this is not a code block or a list, blockquote.
		if( type != lineType && type != BlockType::Code && type != BlockType::List &&
			type != BlockType::Blockquote && type != BlockType::ListWithFirstEmptyLine )
		{
			if( type == BlockType::Text && lineType == BlockType::CodeIndentedBySpaces )
				fragment.push_back( { line, { currentLineNumber, htmlCommentClosed } } );
			else
			{
				if( type == BlockType::Text &&
					( lineType == BlockType::ListWithFirstEmptyLine ||
						lineType == BlockType::List ) )
				{
					int num = 0;

					if( isOrderedList< Trait >( line.asString(), &num ) )
					{
						if( num > 1 )
						{
							fragment.push_back( { line, { currentLineNumber, htmlCommentClosed } } );

							continue;
						}
					}
				}

				const auto empty = emptyLinesCount;

				pf();

				if( html.htmlBlockType >= 6 )
					html.continueHtml = ( empty <= 0 );

				type = lineType;

				if( !line.isEmpty() && ns < line.length() )
				{
					if( type == BlockType::List || type == BlockType::ListWithFirstEmptyLine )
						indents.insert( currentIndent );
					else if( type == BlockType::Code )
						startOfCode = startSequence< Trait >( line.asString() );

					fragment.push_back( { line, { currentLineNumber, htmlCommentClosed } } );
				}
			}
		}
		// End of code block.
		else if( type == BlockType::Code && type == lineType &&
			startSequence< Trait >( line.asString() ).contains( startOfCode ) &&
			isCodeFences< Trait >( line.asString(), true ) )
		{
			fragment.push_back( { line, { currentLineNumber, htmlCommentClosed } } );

			pf();
		}
		else
			fragment.push_back( { line, { currentLineNumber, htmlCommentClosed } } );

		emptyLinesCount = 0;
	}

	if( !fragment.empty() )
	{
		if( type == BlockType::Code )
			fragment.push_back( { startOfCode, { -1, {} } } );

		pf();
	}

	auto finishHtml = [&] ()
	{
		if( html.html->isFreeTag() )
			parent->appendItem( html.html );
		else
		{
			if( parent->items().back()->type() == ItemType::Paragraph )
			{
				auto p = static_cast< Paragraph< Trait >* > ( parent->items().back().get() );

				if( p->isDirty() )
					p->appendItem( html.html );
				else
				{
					std::shared_ptr< Paragraph< Trait > > p(
						new Paragraph< Trait > );
					p->appendItem( html.html );
					doc->appendItem( p );
				}
			}
			else
			{
				std::shared_ptr< Paragraph< Trait > > p(
					new Paragraph< Trait > );
				p->appendItem( html.html );
				doc->appendItem( p );
			}
		}

		html.html.reset();
		html.htmlBlockType = -1;
		html.continueHtml = false;
	};

	if( top )
	{
		html.html.reset();
		html.htmlBlockType = -1;
		html.continueHtml = false;

		for( long long int i = 0; i < (long long int) splitted.size(); ++i )
		{
			parseFragment( splitted[ i ], parent, doc, linksToParse,
				workingPath, fileName, false, html );

			if( html.htmlBlockType >= 6 )
				html.continueHtml = ( !splitted[ i ].emptyLineAfter );

			if( html.html.get() && !html.continueHtml )
				finishHtml();
		}
	}

	if( html.html.get() )
		finishHtml();
}

#ifdef MD4QT_QT_SUPPORT

template<>
inline void
Parser< QStringTrait >::parseFile( const QString & fileName,
	bool recursive, std::shared_ptr< Document< QStringTrait > > doc,
	const QStringList & ext, QStringList * parentLinks )
{
	QFileInfo fi( fileName );

	if( fi.exists() && ext.contains( fi.suffix().toLower() ) )
	{
		QFile f( fileName );

		if( f.open( QIODevice::ReadOnly ) )
		{
			QTextStream s( f.readAll() );
			f.close();

			parseStream( s, fi.absolutePath(), fi.fileName(), recursive, doc, ext, parentLinks );
		}
	}
}

#endif

#ifdef MD4QT_ICU_STL_SUPPORT

template<>
inline void
Parser< UnicodeStringTrait >::parseFile( const UnicodeString & fileName,
	bool recursive, std::shared_ptr< Document< UnicodeStringTrait > > doc,
	const std::vector< UnicodeString > & ext, std::vector< UnicodeString > * parentLinks )
{
	if( UnicodeStringTrait::fileExists( fileName, "" ) )
	{
		std::string fn;
		fileName.toUTF8String( fn );

		try {
			auto e = UnicodeString::fromUTF8( std::filesystem::u8path( fn ).extension().u8string() );

			if( !e.isEmpty() )
				e.remove( 0, 1 );

			if( std::find( ext.cbegin(), ext.cend(), e.toLower() ) != ext.cend() )
			{
				auto path = std::filesystem::canonical( std::filesystem::u8path( fn ) );
				std::ifstream file( path.c_str(), std::ios::in | std::ios::binary );

				if( file.good() )
				{
					const auto fileNameS = path.filename().u8string();
					auto workingDirectory = path.remove_filename().u8string();

					if( !workingDirectory.empty() )
						workingDirectory.erase( workingDirectory.size() - 1, 1 );

					std::replace( workingDirectory.begin(), workingDirectory.end(), '\\', '/' );

					parseStream( file, UnicodeString::fromUTF8( workingDirectory ),
						UnicodeString::fromUTF8( fileNameS ),
						recursive, doc, ext, parentLinks );

					file.close();
				}
			}
		}
		catch( const std::exception & )
		{
		}
	}
}

#endif

template< class Trait >
void resolveLinks( typename Trait::StringList & linksToParse,
	std::shared_ptr< Document< Trait > > doc )
{
	for( auto it = linksToParse.begin(), last = linksToParse.end(); it != last; ++it )
	{
		auto nextFileName = *it;

		if( nextFileName.startsWith( '#' ) )
		{
			const auto lit = doc->labeledLinks().find( nextFileName );

			if( lit != doc->labeledLinks().cend() )
				nextFileName = lit->second->url();
			else
				continue;
		}

		if( Trait::fileExists( nextFileName, "" ) )
			*it = Trait::absoluteFilePath( nextFileName );
	}
}


template< class Trait >
inline void
Parser< Trait >::parseStream( typename Trait::TextStream & s,
	const typename Trait::String & workingPath,
	const typename Trait::String & fileName,
	bool recursive,
	std::shared_ptr< Document< Trait > > doc,
	const typename Trait::StringList & ext,
	typename Trait::StringList * parentLinks )
{
	typename Trait::StringList linksToParse;

	doc->appendItem( std::shared_ptr< Anchor< Trait > > (
		new Anchor< Trait >( workingPath + "/" + fileName ) ) );

	typename MdBlock< Trait >::Data data;

	{
		TextStream< Trait > stream( s );

		long long int i = 0;

		while( !stream.atEnd() )
		{
			data.push_back( std::pair< typename Trait::String, MdLineData >(
				stream.readLine(), { i } ) );
			++i;
		}
	}

	StringListStream< Trait > stream( data );

	parse( stream, doc, doc, linksToParse,
		workingPath + "/", fileName, true, true );

	m_parsedFiles.push_back( workingPath + "/" + fileName );

	resolveLinks< Trait >( linksToParse, doc );

	// Parse all links if parsing is recursive.
	if( recursive && !linksToParse.empty() )
	{
		const auto tmpLinks = linksToParse;

		while( !linksToParse.empty() )
		{
			auto nextFileName = linksToParse.front();
			linksToParse.erase( linksToParse.cbegin() );

			if( parentLinks )
			{
				const auto pit = std::find( parentLinks->cbegin(), parentLinks->cend(),
					nextFileName );

				if( pit != parentLinks->cend() )
					continue;
			}

			if( nextFileName.startsWith( '#' ) )
				continue;

			const auto pit = std::find( m_parsedFiles.cbegin(), m_parsedFiles.cend(),
				nextFileName );

			if( pit == m_parsedFiles.cend() )
			{
				if( !doc->isEmpty() && doc->items().back()->type() != ItemType::PageBreak )
					doc->appendItem( std::shared_ptr< PageBreak< Trait > > (
						new PageBreak< Trait > ) );

				parseFile( nextFileName, recursive, doc, ext, &linksToParse );
			}
		}

		if( parentLinks )
			std::copy( tmpLinks.cbegin(), tmpLinks.cend(), std::back_inserter( *parentLinks ) );
	}
}

template< class Trait >
inline long long int
posOfListItem( const typename Trait::String & s, bool ordered )
{
	long long int p = 0;

	for( ; p < s.size(); ++p )
	{
		if( !s[ p ].isSpace() )
			break;
	}

	if( ordered )
	{
		for( ; p < s.size(); ++p )
		{
			if( !s[ p ].isDigit() )
				break;
		}
	}

	++p;

	long long int sc = 0;

	for( ; p < s.size(); ++p )
	{
		if( !s[ p ].isSpace() )
			break;
		else
			++sc;
	}

	if( sc > 4 )
		p = p - sc + 1;

	return p;
}

template< class Trait >
inline typename Parser< Trait >::BlockType
Parser< Trait >::whatIsTheLine( typename Trait::InternalString & str,
	bool inList, long long int * indent, bool calcIndent,
	const std::set< long long int > * indents ) const
{
	str.replace( typename Trait::Char( '\t' ), typename Trait::String( 4, ' ' ) );

	const auto first = skipSpaces< Trait >( 0, str.asString() );

	if( first < str.length() )
	{
		auto s = str.sliced( first );

		if( s.asString().startsWith( '>' ) && first < 4 )
			return BlockType::Blockquote;
		else if( s.asString().startsWith( '#' ) && first < 4 )
		{
			long long int c = 0;

			while( c < s.length() && s[ c ] == typename Trait::Char( '#' ) )
				++c;

			if( c <= 6 && ( ( c < s.length() && s[ c ].isSpace() ) || c == s.length() ) )
				return BlockType::Heading;
			else
				return BlockType::Text;
		}

		if( first < 4 && isHorizontalLine< Trait >( s.asString() ) )
			return BlockType::Text;

		if( inList )
		{
			bool isFirstLineEmpty = false;
			const auto orderedList = isOrderedList< Trait >( str.asString(),
				nullptr, nullptr, nullptr, &isFirstLineEmpty );

			if( ( ( ( s.asString().startsWith( '-' ) ||
					s.asString().startsWith( '+' ) || s.asString().startsWith( '*' ) ) &&
				( ( s.length() > 1 && s[ 1 ] == typename Trait::Char( ' ' ) ) || s.length() == 1 ) ) ||
				orderedList ) && ( first < 4  || indentInList( indents, first ) ) )
			{
				if( calcIndent && indent )
					*indent = posOfListItem< Trait >( str.asString(), orderedList );

				if( s.length() == 1 || isFirstLineEmpty )
					return BlockType::ListWithFirstEmptyLine;
				else
					return BlockType::List;
			}
			else if( str.asString().startsWith( typename Trait::String( ( indent ? *indent : 4 ), ' ' ) ) )
			{
				if( str.asString().startsWith( "    " ) )
					return BlockType::CodeIndentedBySpaces;
			}
		}
		else
		{
			bool isFirstLineEmpty = false;

			const auto orderedList = isOrderedList< Trait >( str.asString(),
				nullptr, nullptr, nullptr, &isFirstLineEmpty );

			if( ( ( ( s.asString().startsWith( '-' ) ||
					s.asString().startsWith( '+' ) || s.asString().startsWith( '*' ) ) &&
				( ( s.length() > 1 && s[ 1 ] == typename Trait::Char( ' ' ) ) || s.length() == 1 ) ) ||
				orderedList ) && ( first < 4  || indentInList( indents, first ) ) )
			{
				if( calcIndent && indent )
					*indent = posOfListItem< Trait >( str.asString(), orderedList );

				if( s.length() == 1 || isFirstLineEmpty )
					return BlockType::ListWithFirstEmptyLine;
				else
					return BlockType::List;
			}
			else if( str.asString().startsWith( "    " ) || str.asString().startsWith( '\t' ) )
				return BlockType::CodeIndentedBySpaces;
		}

		if( s.asString().startsWith( "```" ) || s.asString().startsWith( "~~~" ) )
		{
			if( isCodeFences< Trait >( str.asString() ) )
				return BlockType::Code;
			else
				return BlockType::Text;
		}
	}
	else
		return BlockType::Unknown;

	return BlockType::Text;
}

template< class Trait >
inline void
Parser< Trait >::parseFragment( MdBlock< Trait > & fr,
	std::shared_ptr< Block< Trait > > parent,
	std::shared_ptr< Document< Trait > > doc,
	typename Trait::StringList & linksToParse,
	const typename Trait::String & workingPath,
	const typename Trait::String & fileName, bool collectRefLinks,
	RawHtmlBlock< Trait > & html )
{
	if( html.continueHtml )
		parseText( fr, parent, doc, linksToParse,
			workingPath, fileName, collectRefLinks, html );
	else
	{
		switch( whatIsTheLine( fr.data.front().first ) )
		{
			case BlockType::Text :
				parseText( fr, parent, doc, linksToParse,
					workingPath, fileName, collectRefLinks, html );
				break;

			case BlockType::Blockquote :
				parseBlockquote( fr, parent, doc, linksToParse, workingPath, fileName,
					collectRefLinks, html );
				break;

			case BlockType::Code :
				parseCode( fr, parent, collectRefLinks );
				break;

			case BlockType::CodeIndentedBySpaces :
			{
				int indent = 1;

				if( fr.data.front().first.asString().startsWith( "    " ) )
					indent = 4;

				parseCodeIndentedBySpaces( fr, parent, collectRefLinks, indent );
			}
				break;

			case BlockType::Heading :
				parseHeading( fr, parent, doc, linksToParse, workingPath, fileName,
					collectRefLinks );
				break;

			case BlockType::List :
			case BlockType::ListWithFirstEmptyLine :
				parseList( fr, parent, doc, linksToParse, workingPath, fileName,
					collectRefLinks, html );
				break;

			default :
				break;
		}
	}
}

template< class Trait >
inline void
Parser< Trait >::clearCache()
{
	m_parsedFiles.clear();
}

template< class Trait >
inline int
isTableHeader( const typename Trait::String & s )
{
	if( s.contains( typename Trait::Char( '|' ) ) )
	{
		int c = 0;

		const auto tmp = s.simplified();
		const auto p = tmp.startsWith( '|' ) ? 1 : 0;
		const auto n = tmp.size() - p - ( tmp.endsWith( '|' ) ? 1 : 0 );
		const auto v = tmp.sliced( p, n );

		bool backslash = false;

		for( long long int i = 0; i < v.size(); ++i )
		{
			bool now = false;

			if( v[ i ] == typename Trait::Char( '\\' ) && !backslash )
			{
				backslash = true;
				now = true;
			}
			else if( v[ i ] == typename Trait::Char( '|' ) && !backslash )
				++c;

			if( !now )
				backslash = false;
		}

		++c;

		return c;
	}
	else
		return 0;
}

template< class Trait >
inline void
Parser< Trait >::parseText( MdBlock< Trait > & fr,
	std::shared_ptr< Block< Trait > > parent,
	std::shared_ptr< Document< Trait > > doc,
	typename Trait::StringList & linksToParse,
	const typename Trait::String & workingPath,
	const typename Trait::String & fileName,
	bool collectRefLinks, RawHtmlBlock< Trait > & html )
{
	const auto h = isTableHeader< Trait >( fr.data.front().first.asString() );
	const auto c = fr.data.size() > 1 ?
		isTableAlignment< Trait >( fr.data[ 1 ].first.asString() ) : 0;

	if( isFootnote< Trait >( fr.data.front().first.asString() ) )
		parseFootnote( fr, parent, doc, linksToParse, workingPath, fileName,
			collectRefLinks );
	else if( c && h && c == h )
		parseTable( fr, parent, doc, linksToParse, workingPath, fileName, collectRefLinks, c );
	else
		parseParagraph( fr, parent, doc, linksToParse,
			workingPath, fileName, collectRefLinks, html );
}

template< class Trait >
inline typename Trait::String
findAndRemoveHeaderLabel( typename Trait::String & s )
{
	const auto start = s.indexOf( "{#" );

	if( start >= 0 )
	{
		long long int p = start + 2;

		for( ; p < s.size(); ++p )
		{
			if( s[ p ] == typename Trait::Char( '}' ) )
				break;
		}

		if( p < s.size() && s[ p ] == typename Trait::Char( '}' ) )
		{
			const auto label = s.sliced( start, p - start + 1 );
			s.remove( start, p - start + 1 );
			return label;
		}
	}

	return typename Trait::String();
}

template< class Trait >
inline typename Trait::String
paragraphToLabel( Paragraph< Trait > * p )
{
	typename Trait::String l;

	if( !p )
		return l;

	for( auto it = p->items().cbegin(), last = p->items().cend(); it != last; ++it )
	{
		if( (*it)->type() == ItemType::Text )
		{
			if( !l.isEmpty() )
				l.push_back( "-" );

			auto t = static_cast< Text< Trait >* > ( it->get() );

			const auto tmp = t->text().simplified();

			for( long long int i = 0; i < tmp.length(); ++i )
			{
				const auto c = tmp[ i ];

				if( c.isLetter() || c.isDigit() )
					l.push_back( c.toLower() );
				else if( c.isSpace() )
					l.push_back( "-" );
			}
		}
	}

	return l;
}

template< class Trait >
inline void
findAndRemoveClosingSequence( typename Trait::InternalString & s )
{
	long long int end = -1;
	long long int start = -1;

	for( long long int i = s.length() - 1; i >= 0 ; --i )
	{
		if( !s[ i ].isSpace() && s[ i ] != typename Trait::Char( '#' ) && end == -1 )
			return;

		if( s[ i ] == typename Trait::Char( '#' ) )
		{
			if( end == -1 )
				end = i;

			if( i - 1 >= 0 )
			{
				if( s[ i - 1 ].isSpace() )
				{
					start = i;
					break;
				}
				else if( s[ i - 1 ] != typename Trait::Char( '#' ) )
					return;
			}
			else
				start = 0;
		}
	}

	if( start != -1 && end != -1 )
		s.remove( start, end - start + 1 );
}

template< class Trait >
inline void
Parser< Trait >::parseHeading( MdBlock< Trait > & fr,
	std::shared_ptr< Block< Trait > > parent,
	std::shared_ptr< Document< Trait > > doc,
	typename Trait::StringList & linksToParse,
	const typename Trait::String & workingPath,
	const typename Trait::String & fileName,
	bool collectRefLinks )
{
	if( !fr.data.empty() && !collectRefLinks )
	{
		auto line = fr.data.front().first;
		long long int pos = 0;
		pos = skipSpaces< Trait >( pos, line.asString() );

		if( pos > 0  )
			line = line.sliced( pos );

		pos = 0;
		int lvl = 0;

		while( pos < line.length() && line[ pos ] == typename Trait::Char( '#' ) )
		{
			++lvl;
			++pos;
		}

		pos = skipSpaces< Trait >( pos, line.asString() );

		if( pos > 0 )
			fr.data.front().first = line.sliced( pos );

		const auto label = findAndRemoveHeaderLabel< Trait >( fr.data.front().first.asString() );

		findAndRemoveClosingSequence< Trait >( fr.data.front().first );

		std::shared_ptr< Heading< Trait > > h( new Heading< Trait > );
		h->setLevel( lvl );

		if( !label.isEmpty() )
			h->setLabel( label.sliced( 1, label.length() - 2 ) + "/" +
				workingPath + fileName );

		std::shared_ptr< Paragraph< Trait > > p( new Paragraph< Trait > );

		typename MdBlock< Trait >::Data tmp;
		tmp.push_back( fr.data.front() );
		tmp.front().first = tmp.front().first.simplified();
		MdBlock< Trait > block = { tmp, 0 };

		RawHtmlBlock< Trait > html;

		parseFormattedTextLinksImages( block, p, doc, linksToParse, workingPath, fileName,
			false, false, html );

		fr.data.erase( fr.data.cbegin() );

		if( p->items().size() && p->items().at( 0 )->type() == ItemType::Paragraph )
			h->setText( std::static_pointer_cast< Paragraph< Trait > >( p->items().at( 0 ) ) );
		else
			h->setText( p );

		if( h->isLabeled() )
			doc->insertLabeledHeading( h->label(), h );
		else
		{
			typename Trait::String label = "#" + paragraphToLabel( h->text().get() );

			label += "/" + workingPath + fileName;

			h->setLabel( label );

			doc->insertLabeledHeading( label, h );
		}

		parent->appendItem( h );
	}
}

template< class Trait >
inline typename Trait::InternalString
prepareTableData( typename Trait::InternalString s )
{
	s.replace( "\\|", "|" );

	return s;
}

template< class Trait >
inline typename Trait::InternalStringList
splitTableRow( const typename Trait::InternalString & s )
{
	typename Trait::InternalStringList res;

	bool backslash = false;
	long long int start = 0;

	for( long long int i = 0; i < s.length(); ++i )
	{
		bool now = false;

		if( s[ i ] == typename Trait::Char( '\\' ) && !backslash )
		{
			backslash = true;
			now = true;
		}
		else if( s[ i ] == typename Trait::Char( '|' ) && !backslash )
		{
			res.push_back( prepareTableData< Trait >( s.sliced( start, i - start ).simplified() ) );
			start = i + 1;
		}

		if( !now )
			backslash = false;
	}

	if( start < s.length() )
		res.push_back( prepareTableData< Trait >(
			s.sliced( start, s.length() - start ).simplified() ) );

	return res;
}

template< class Trait >
inline void
Parser< Trait >::parseTable( MdBlock< Trait > & fr,
	std::shared_ptr< Block< Trait > > parent,
	std::shared_ptr< Document< Trait > > doc,
	typename Trait::StringList & linksToParse,
	const typename Trait::String & workingPath,
	const typename Trait::String & fileName,
	bool collectRefLinks, int columnsCount )
{
	static const char sep = '|';

	if( fr.data.size() >= 2 )
	{
		std::shared_ptr< Table< Trait > > table( new Table< Trait > );

		auto parseTableRow = [&] ( const typename Trait::InternalString & row,
			long long int lineNumber )
		{
			auto line = row.simplified();

			if( line.asString().startsWith( sep ) )
				line.remove( 0, 1 );

			if( line.asString().endsWith( sep ) )
				line.remove( line.length() - 1, 1 );

			auto columns = splitTableRow< Trait >( line );

			std::shared_ptr< TableRow< Trait > > tr( new TableRow< Trait > );

			int c = 0;

			for( auto it = columns.begin(), last = columns.end(); it != last; ++it, ++c )
			{
				if( c == columnsCount )
					break;

				std::shared_ptr< TableCell< Trait > > c(
					new TableCell< Trait > );

				if( !it->isEmpty() )
				{
					it->replace( "&#124;", typename Trait::Char( sep ) );

					typename MdBlock< Trait >::Data fragment;
					fragment.push_back( { *it, { lineNumber } } );
					MdBlock< Trait > block = { fragment, 0 };

					std::shared_ptr< Paragraph< Trait > > p( new Paragraph< Trait > );

					RawHtmlBlock< Trait > html;

					parseFormattedTextLinksImages( block, p, doc,
						linksToParse, workingPath, fileName, collectRefLinks, false, html );

					if( !p->isEmpty() && p->items().at( 0 )->type() == ItemType::Paragraph )
					{
						const auto pp = std::static_pointer_cast< Paragraph< Trait > >(
							p->items().at( 0 ) );

						for( auto it = pp->items().cbegin(), last = pp->items().cend();
							it != last; ++it )
						{
							c->appendItem( (*it) );
						}
					}
				}

				tr->appendCell( c );
			}

			if( !tr->isEmpty() )
				table->appendRow( tr );
		};

		{
			auto fmt = fr.data.at( 1 ).first;

			auto columns = fmt.split( typename Trait::String( sep ) );

			for( auto it = columns.begin(), last = columns.end(); it != last; ++it )
			{
				*it = it->simplified();

				if( !it->isEmpty() )
				{
					typename Table< Trait >::Alignment a = Table< Trait >::AlignLeft;

					if( it->asString().endsWith( ':' ) && it->asString().startsWith( ':' ) )
						a = Table< Trait >::AlignCenter;
					else if( it->asString().endsWith( ':' ) )
						a = Table< Trait >::AlignRight;

					table->setColumnAlignment( table->columnsCount(), a );
				}
			}
		}

		fr.data.erase( fr.data.cbegin() + 1 );

		for( const auto & line : std::as_const( fr.data ) )
			parseTableRow( line.first, line.second.lineNumber );

		if( !table->isEmpty() && !collectRefLinks )
			parent->appendItem( table );
	}
}

template< class Trait >
inline bool
isH( const typename Trait::String & s, const typename Trait::Char & c )
{
	long long int p = skipSpaces< Trait >( 0, s );

	if( p > 3 )
		return false;

	const auto start = p;

	for( ; p < s.size(); ++p )
	{
		if( s[ p ] != c )
			break;
	}

	if( p - start < 1 )
		return false;

	for( ; p < s.size(); ++p )
	{
		if( !s[ p ].isSpace() )
			return false;
	}

	return true;
}

template< class Trait >
inline bool
isH1( const typename Trait::String & s )
{
	return isH< Trait >( s, '=' );
}

template< class Trait >
inline bool
isH2( const typename Trait::String & s )
{
	return isH< Trait >( s, '-' );
}

template < class Trait >
inline void
Parser< Trait >::parseParagraph( MdBlock< Trait > & fr,
	std::shared_ptr< Block< Trait > > parent,
	std::shared_ptr< Document< Trait > > doc,
	typename Trait::StringList & linksToParse,
	const typename Trait::String & workingPath,
	const typename Trait::String & fileName,
	bool collectRefLinks, RawHtmlBlock< Trait > & html )
{
	bool heading = false;

	// Check for alternative syntax of H1 and H2 headings.
	if( fr.data.size() >= 2 )
	{
		long long int i = 1;
		int lvl = 0;
		long long int horLines = 0;

		for( ; i < (long long int) fr.data.size(); ++i )
		{
			const auto first = skipSpaces< Trait >( 0, fr.data.at( i - 1 ).first.asString() );

			auto s = fr.data.at( i - 1 ).first.asString().sliced( first );

			const bool prevHorLine = ( first < 4 && isHorizontalLine< Trait >( s ) );

			if( prevHorLine )
				++horLines;

			if( isH1< Trait >( fr.data.at( i ).first.asString() ) && !prevHorLine &&
				!fr.data.at( i - 1 ).first.asString().simplified().isEmpty() )
			{
				lvl = 1;
				heading = true;
				break;
			}
			else if( isH2< Trait >( fr.data.at( i ).first.asString() ) && !prevHorLine &&
				!fr.data.at( i - 1 ).first.asString().simplified().isEmpty() )
			{
				lvl = 2;
				heading = true;
				break;
			}
		}

		if( heading )
		{
			if( !collectRefLinks )
				for( long long int j = 0; j < horLines; ++j )
					parent->appendItem( std::shared_ptr< Item< Trait > > ( new HorizontalLine< Trait > ) );

			fr.data.erase( fr.data.cbegin(), fr.data.cbegin() + horLines );

			std::shared_ptr< Heading< Trait > > h( new Heading< Trait > );
			std::shared_ptr< Paragraph< Trait > > p( new Paragraph< Trait > );

			h->setLevel( lvl );

			typename MdBlock< Trait >::Data tmp;
			std::copy( fr.data.cbegin(), fr.data.cbegin() + ( i - horLines ),
				std::back_inserter( tmp ) );

			const auto ns1 = skipSpaces< Trait >( 0, tmp.front().first.asString() );

			if( ns1 > 0 && ns1 < tmp.front().first.length() )
				tmp.front().first = tmp.front().first.sliced( ns1 );

			long long int ns2 = tmp.back().first.length();

			for( long long int i = tmp.back().first.length() - 1; i >= 0; --i )
			{
				if( tmp.back().first[ i ].isSpace() )
					ns2 = i;
				else
					break;
			}

			if( ns2 < tmp.back().first.length() )
				tmp.back().first = tmp.back().first.sliced( 0, ns2 );

			MdBlock< Trait > block = { tmp, 0 };

			parseFormattedTextLinksImages( block, p, doc, linksToParse,
				workingPath, fileName, collectRefLinks, true, html );

			const bool keepHeadingLine = p->isEmpty();

			fr.data.erase( fr.data.cbegin(), fr.data.cbegin() +
				( i - horLines + ( keepHeadingLine ? 0 : 1 ) ) );

			if( !collectRefLinks && !keepHeadingLine )
			{
				if( p->items().at( 0 )->type() == ItemType::Paragraph )
					h->setText( std::static_pointer_cast< Paragraph< Trait > >(
						p->items().at( 0 ) ) );

				typename Trait::String label = "#" + paragraphToLabel( h->text().get() );

				label += "/" + workingPath + fileName;

				h->setLabel( label );

				doc->insertLabeledHeading( label, h );

				parent->appendItem( h );
			}
		}
	}

	if( !fr.data.empty() )
	{
		if( heading )
		{
			StringListStream< Trait > stream( fr.data );
			parse( stream, parent, doc, linksToParse, workingPath, fileName,
				collectRefLinks );
		}
		else
		{
			std::shared_ptr< Paragraph< Trait > > p( new Paragraph< Trait > );

			parseFormattedTextLinksImages( fr, p, doc, linksToParse, workingPath, fileName,
				collectRefLinks, false, html );

			if( !p->isEmpty() && !collectRefLinks )
			{
				for( auto it = p->items().cbegin(), last = p->items().cend(); it != last; ++it )
				{
					if( (*it)->type() == MD::ItemType::Paragraph )
					{
						auto p = static_cast< Paragraph< Trait >* > ( (*it).get() );

						std::shared_ptr< Paragraph< Trait > > pp( new Paragraph< Trait > );
						pp->setDirty( p->isDirty() );

						for( auto it = p->items().cbegin(), last = p->items().cend(); it != last; ++it )
						{
							if( (*it)->type() == MD::ItemType::RawHtml &&
								std::static_pointer_cast< RawHtml< Trait > >( *it )->isFreeTag() )
							{
								if( !pp->isEmpty() )
								{
									parent->appendItem( pp );
									pp.reset( new Paragraph< Trait > );
									pp->setDirty( p->isDirty() );
								}

								parent->appendItem( (*it) );
							}
							else
								pp->appendItem( (*it) );
						}

						if( !pp->isEmpty() )
						{
							Paragraph< Trait > * prevP = nullptr;

							if( pp->items().at( 0 )->type() == MD::ItemType::RawHtml &&
								parent->items().size() &&
								parent->items().back()->type() == MD::ItemType::Paragraph )
									prevP = static_cast< Paragraph< Trait >* >
										( parent->items().back().get() );

							if( prevP && prevP->isDirty() )
							{
								for( auto it = pp->items().cbegin(), last = pp->items().cend();
									it != last; ++it )
										prevP->appendItem( (*it) );

								prevP->setDirty( false );
							}
							else
								parent->appendItem( pp );
						}
					}
					else
						parent->appendItem( (*it) );
				}
			}
		}
	}
}

template< class Trait >
struct UnprotectedDocsMethods {
	static void setFreeTag( std::shared_ptr< RawHtml< Trait > > html,
		bool on )
	{
		html->setFreeTag( on );
	}

	static void setDirty( std::shared_ptr< Paragraph< Trait > > p )
	{
		p->setDirty( true );
	}
};

struct Delimiter {
	enum DelimiterType {
		// (
		ParenthesesOpen,
		// )
		ParenthesesClose,
		// [
		SquareBracketsOpen,
		// ]
		SquareBracketsClose,
		// ![
		ImageOpen,
		// ~~
		Strikethrough,
		// *
		Emphasis1,
		// _
		Emphasis2,
		// `
		InlineCode,
		// <
		Less,
		// >
		Greater,
		// $
		Math,
		HorizontalLine,
		Unknown
	}; // enum DelimiterType

	DelimiterType m_type = Unknown;
	long long int m_line = -1;
	long long int m_pos = -1;
	long long int m_len = 0;
	bool m_spaceBefore = false;
	bool m_spaceAfter = false;
	bool m_isWordBefore = false;
	bool m_backslashed = false;
	bool m_leftFlanking = false;
	bool m_rightFlanking = false;
}; // struct Delimiter

enum class Style {
	Italic1,
	Italic2,
	Bold1,
	Bold2,
	Strikethrough,
	Unknown
};

template< class Trait >
using Delims = typename Trait::template Vector< Delimiter >;

template< class Trait >
inline Delims< Trait >
collectDelimiters( const typename MdBlock< Trait >::Data & fr )
{
	Delims< Trait > d;

	for( long long int line = 0; line < (long long int) fr.size(); ++line )
	{
		const typename Trait::String & str = fr.at( line ).first.asString();
		const auto p = skipSpaces< Trait >( 0, str );
		const auto withoutSpaces = str.sliced( p );

		if( isHorizontalLine< Trait >( withoutSpaces ) && p < 4 )
			d.push_back( { Delimiter::HorizontalLine, line, 0, str.length(), false, false, false } );
		else
		{
			bool backslash = false;
			bool space = true;
			bool word = false;

			for( long long int i = 0; i < str.size(); ++i )
			{
				bool now = false;

				if( str[ i ] == typename Trait::Char( '\\' ) && !backslash )
				{
					backslash = true;
					now = true;
				}
				else if( str[ i ].isSpace() && !backslash )
				{
					space = true;
					now = true;
				}
				else
				{
					// * or _
					if( ( str[ i ] == typename Trait::Char( '_' ) ||
						str[ i ] == typename Trait::Char( '*' ) ) && !backslash )
					{
						typename Trait::String style;

						const bool punctBefore = ( i > 0 ? str[ i - 1 ].isPunct() ||
							str[ i - 1 ] == typename Trait::Char( '~' ) : false );
						const bool alNumBefore =
							( i > 0 ? str[ i - 1 ].isLetterOrNumber() : false );

						const auto ch = str[ i ];

						while( i < str.length() && str[ i ] == ch )
						{
							style.push_back( str[ i ] );
							++i;
						}

						Delimiter::DelimiterType dt = Delimiter::Unknown;

						if( ch == typename Trait::Char( '*' ) )
							dt = Delimiter::Emphasis1;
						else
							dt = Delimiter::Emphasis2;

						const bool spaceAfter =
							( i < str.length() ? str[ i ].isSpace() : true );
						const bool punctAfter =
							( i < str.length() ? str[ i ].isPunct() ||
								str[ i ] == typename Trait::Char( '~' ) : false );
						const bool leftFlanking =
							( ( space || punctBefore ) && punctAfter ) ||
							( !spaceAfter && !punctAfter );
						const bool rightFlanking =
							( punctBefore && ( spaceAfter || punctAfter ) ) ||
							( !space && !punctBefore );
						const bool disabledEmphasis =
							( ( i < str.length() ? str[ i ].isLetterOrNumber() : false ) &&
							alNumBefore && ch == typename Trait::Char( '_' ) );

						if( ( leftFlanking || rightFlanking ) && !disabledEmphasis )
						{
							for( auto j = 0; j < style.length(); ++j )
							{
								d.push_back( { dt, line, i - style.length() + j,
									1, space, spaceAfter, word, false,
									leftFlanking, rightFlanking } );
							}

							word = false;
						}
						else
							word = true;

						--i;
					}
					// ~
					else if( str[ i ] == typename Trait::Char( '~' ) && !backslash )
					{
						typename Trait::String style;

						const bool punctBefore = ( i > 0 ? str[ i - 1 ].isPunct() ||
							str[ i - 1 ] == typename Trait::Char( '~' ) : false );

						while( i < str.length() && str[ i ] == typename Trait::Char( '~' ) )
						{
							style.push_back( str[ i ] );
							++i;
						}

						if( style.length() == 2 )
						{
							const bool spaceAfter =
								( i < str.length() ? str[ i ].isSpace() : true );
							const bool punctAfter =
								( i < str.length() ? str[ i ].isPunct() ||
									str[ i ] == typename Trait::Char( '~' ) : false );
							const bool leftFlanking =
								( ( space || punctBefore ) && punctAfter ) ||
								( !spaceAfter && !punctAfter );
							const bool rightFlanking =
								( punctBefore && ( spaceAfter || punctAfter ) ) ||
								( !space && !punctBefore );

							if( leftFlanking || rightFlanking )
							{
								d.push_back( { Delimiter::Strikethrough, line, i - style.length(),
									style.length(), space, spaceAfter, word, false,
									leftFlanking, rightFlanking } );

								word = false;
							}
							else
								word = true;
						}
						else
							word = true;

						--i;
					}
					// [
					else if( str[ i ] == typename Trait::Char( '[' ) && !backslash )
					{
						const bool spaceAfter =
							( i < str.length() ? str[ i ].isSpace() : true );

						d.push_back( { Delimiter::SquareBracketsOpen, line, i, 1,
							space, spaceAfter, word, false } );

						word = false;
					}
					// !
					else if( str[ i ] == typename Trait::Char( '!' ) && !backslash )
					{
						if( i + 1 < str.length() )
						{
							if( str[ i + 1 ] == typename Trait::Char( '[' ) )
							{
								const bool spaceAfter =
									( i < str.length() ? str[ i ].isSpace() : true );

								d.push_back( { Delimiter::ImageOpen, line, i, 2,
									space, spaceAfter, word, false } );

								++i;

								word = false;
							}
							else
								word = true;
						}
						else
							word = true;
					}
					// (
					else if( str[ i ] == typename Trait::Char( '(' ) && !backslash )
					{
						const bool spaceAfter =
							( i < str.length() ? str[ i ].isSpace() : true );

						d.push_back( { Delimiter::ParenthesesOpen, line, i, 1,
							space, spaceAfter, word, false } );

						word = false;
					}
					// ]
					else if( str[ i ] == typename Trait::Char( ']' ) && !backslash )
					{
						const bool spaceAfter =
							( i < str.length() ? str[ i ].isSpace() : true );

						d.push_back( { Delimiter::SquareBracketsClose, line, i, 1,
							space, spaceAfter, word, false } );

						word = false;
					}
					// )
					else if( str[ i ] == typename Trait::Char( ')' ) && !backslash )
					{
						const bool spaceAfter =
							( i < str.length() ? str[ i ].isSpace() : true );

						d.push_back( { Delimiter::ParenthesesClose, line, i, 1,
							space, spaceAfter, word, false } );

						word = false;
					}
					// <
					else if( str[ i ] == typename Trait::Char( '<' ) && !backslash )
					{
						const bool spaceAfter =
							( i < str.length() ? str[ i ].isSpace() : true );

						d.push_back( { Delimiter::Less, line, i, 1,
							space, spaceAfter, word, false } );

						word = false;
					}
					// >
					else if( str[ i ] == typename Trait::Char( '>' ) && !backslash )
					{
						const bool spaceAfter =
							( i < str.length() ? str[ i ].isSpace() : true );

						d.push_back( { Delimiter::Greater, line, i, 1,
							space, spaceAfter, word, false } );

						word = false;
					}
					// `
					else if( str[ i ] == typename Trait::Char( '`' ) )
					{
						typename Trait::String code;

						while( i < str.length() && str[ i ] == typename Trait::Char( '`' ) )
						{
							code.push_back( str[ i ] );
							++i;
						}

						if( backslash )
						{
							if( i - code.length() - 2 >= 0 )
							{
								if( str[ i - code.length() - 2 ].isSpace() )
									space = true;
							}
							else
								space = true;
						}

						const bool spaceAfter =
							( i < str.length() ? str[ i ].isSpace() : true );

						d.push_back( { Delimiter::InlineCode,
							line, i - code.length() - ( backslash ? 1 : 0 ),
							code.length() + ( backslash ? 1 : 0 ),
							space, spaceAfter, word, backslash } );

						word = false;

						--i;
					}
					// $
					else if( str[ i ] == typename Trait::Char( '$' ) )
					{
						typename Trait::String m;

						while( i < str.length() && str[ i ] == typename Trait::Char( '$' ) )
						{
							m.push_back( str[ i ] );
							++i;
						}

						if( m.length() <= 2 && !backslash )
						{
							d.push_back( { Delimiter::Math,
								line, i - m.length(), m.length(),
								false, false, false, false } );
						}

						word = false;

						--i;
					}
					else
						word = true;
				}

				if( !now )
				{
					backslash = false;
					space = false;
				}
			}
		}
	}

	return d;
}

template< class Trait >
struct TextParsingOpts {
	MdBlock< Trait > & fr;
	std::shared_ptr< Block< Trait > > parent;
	std::shared_ptr< Document< Trait > > doc;
	typename Trait::StringList & linksToParse;
	typename Trait::String workingPath;
	typename Trait::String fileName;
	bool collectRefLinks;
	bool ignoreLineBreak;
	RawHtmlBlock< Trait > & html;

	bool wasRefLink = false;
	long long int line = 0;
	long long int pos = 0;
	int opts = TextWithoutFormat;
	std::vector< std::pair< Style, long long int > > styles = {};
}; // struct TextParsingOpts

template< class Trait >
inline void
parseFormattedText( MdBlock< Trait > & fr,
	std::shared_ptr< Block< Trait > > parent,
	std::shared_ptr< Document< Trait > > doc,
	typename Trait::StringList & linksToParse,
	const typename Trait::String & workingPath,
	const typename Trait::String & fileName, bool collectRefLinks, bool ignoreLineBreak,
	RawHtmlBlock< Trait > & html );

template< class Trait >
inline bool
isLineBreak( const typename Trait::String & s )
{
	return ( s.endsWith( "  " ) || s.endsWith( '\\' ) );
}

template< class Trait >
inline typename Trait::String
removeLineBreak( const typename Trait::String & s )
{
	if( s.endsWith( '\\' ) )
		return s.sliced( 0, s.size() - 1 );
	else
		return s;
}

template< class Trait >
inline typename Trait::String
replaceEntity( const typename Trait::String & s )
{
	long long int p1 = 0;

	typename Trait::String res;
	long long int i = 0;

	while( ( p1 = s.indexOf( '&', p1 ) ) != -1 )
	{
		if( p1 > 0 && s[ p1 - 1 ] == typename Trait::Char( '\\' ) )
		{
			++p1;

			continue;
		}

		const auto p2 = s.indexOf( ';', p1 );

		if( p2 != -1 )
		{
			const auto en = s.sliced( p1, p2 - p1 + 1 );

			if( en.size() > 2 && en[ 1 ] == typename Trait::Char( '#' ) )
			{
				if( en.size() > 3 && en[ 2 ].toLower() == typename Trait::Char( 'x' ) )
				{
					const auto hex = en.sliced( 3, en.size() - 4 );

					if( hex.size() <= 6 && hex.size() > 0  )
					{
						bool ok = false;

						const auto c = hex.toInt( &ok, 16 );

						if( ok )
						{
							res.push_back( s.sliced( i, p1 - i ) );
							i = p2 + 1;

							if( c )
								res.push_back( typename Trait::Char( c ) );
							else
								res.push_back( typename Trait::Char( 0xFFFD ) );
						}
					}
				}
				else
				{
					const auto dec = en.sliced( 2, en.size() - 3 );

					if( dec.size() <= 7 && dec.size() > 0 )
					{
						bool ok = false;

						const auto c = dec.toInt( &ok, 10 );

						if( ok )
						{
							res.push_back( s.sliced( i, p1 - i ) );
							i = p2 + 1;

							if( c )
								res.push_back( typename Trait::Char( c ) );
							else
								res.push_back( typename Trait::Char( 0xFFFD ) );
						}
					}
				}
			}
			else
			{
				const auto it = c_entityMap< Trait >.find( en );

				if( it != c_entityMap< Trait >.cend() )
				{
					res.push_back( s.sliced( i, p1 - i ) );
					i = p2 + 1;
					res.push_back( Trait::utf16ToString( it->second ) );
				}
			}
		}
		else
			break;

		p1 = p2 + 1;
	}

	res.push_back( s.sliced( i, s.size() - i ) );

	return res;
}

template< class Trait >
inline typename MdBlock< Trait >::Data
removeBackslashes( const typename MdBlock< Trait >::Data & d )
{
	auto tmp = d;

	for( auto & line : tmp )
		line.first = removeBackslashes< Trait>( line.first );

	return tmp;
}

template< class Trait >
inline void
makeTextObject( const typename Trait::String & text, bool spaceBefore, bool spaceAfter,
	TextParsingOpts< Trait > & po, bool doNotEscape )
{
	auto s = replaceEntity< Trait >( text );

	if( !doNotEscape )
		s = removeBackslashes< Trait >( s ).asString();

	if( !s.isEmpty() )
	{
		spaceBefore = spaceBefore || s[ 0 ].isSpace();
		spaceAfter = spaceAfter || s[ s.size() - 1 ].isSpace();
	}

	s = s.simplified();

	if( !s.isEmpty() )
	{
		std::shared_ptr< Text< Trait > > t( new Text< Trait > );
		t->setText( s );
		t->setOpts( po.opts );
		t->setSpaceBefore( spaceBefore );
		t->setSpaceAfter( spaceAfter );

		po.wasRefLink = false;
		po.parent->appendItem( t );
	}
}

template< class Trait >
inline void
makeTextObjectWithLineBreak( const typename Trait::String & text, bool spaceBefore, bool spaceAfter,
	TextParsingOpts< Trait > & po, bool doNotEscape )
{
	makeTextObject( text, spaceBefore, true, po, doNotEscape );

	std::shared_ptr< Item< Trait > > hr( new LineBreak< Trait > );
	po.wasRefLink = false;
	po.parent->appendItem( hr );
}

template< class Trait >
inline void
makeText(
	// Inclusive.
	long long int lastLine,
	// Not inclusive
	long long int lastPos,
	TextParsingOpts< Trait > & po,
	bool doNotEscape = false )
{
	if( po.line > lastLine )
		return;
	else if( po.line == lastLine && po.pos >= lastPos )
		return;

	typename Trait::String text;

	bool spaceBefore = ( po.pos > 0 && po.pos < po.fr.data.at( po.line ).first.length() ?
		po.fr.data.at( po.line ).first[ po.pos - 1 ].isSpace() ||
			po.fr.data.at( po.line ).first[ po.pos ].isSpace() :
		true );

	bool lineBreak = ( !po.ignoreLineBreak && po.line != (long long int) ( po.fr.data.size() - 1 ) &&
		( po.line == lastLine ?
			( lastPos == po.fr.data.at( po.line ).first.length() &&
			isLineBreak< Trait >( po.fr.data.at( po.line ).first.asString() ) ) :
			isLineBreak< Trait >( po.fr.data.at( po.line ).first.asString() ) ) );

	// makeTOWLB
	auto makeTOWLB = [&] () {
		if( po.line != (long long int) ( po.fr.data.size() - 1 ) )
		{
			makeTextObjectWithLineBreak( text, spaceBefore, true, po, doNotEscape );

			text.clear();

			spaceBefore = true;
		}
	}; // makeTOWLB

	if( lineBreak )
	{
		text.push_back( removeLineBreak< Trait >( po.fr.data.at( po.line ).first.asString() )
			.sliced( po.pos ) );

		makeTOWLB();
	}
	else
	{
		const auto s = po.fr.data.at( po.line ).first.asString().sliced( po.pos,
			( po.line == lastLine ? lastPos - po.pos :
				po.fr.data.at( po.line ).first.length() - po.pos ) );
		text.push_back( s );
	}

	if( po.line != lastLine )
	{
		text.push_back( typename Trait::Char( ' ' ) );
		++po.line;

		for( ; po.line < lastLine; ++po.line )
		{
			lineBreak = ( !po.ignoreLineBreak && po.line != (long long int) ( po.fr.data.size() - 1 ) &&
				isLineBreak< Trait >( po.fr.data.at( po.line ).first.asString() ) );

			const auto s = ( lineBreak ?
				removeLineBreak< Trait >( po.fr.data.at( po.line ).first.asString() ) :
				po.fr.data.at( po.line ).first.asString() );
			text.push_back( s );

			text.push_back( typename Trait::Char( ' ' ) );

			if( lineBreak )
				makeTOWLB();
		}

		lineBreak = ( !po.ignoreLineBreak && po.line != (long long int) ( po.fr.data.size() - 1 ) &&
			lastPos == po.fr.data.at( po.line ).first.length() &&
			isLineBreak< Trait >( po.fr.data.at( po.line ).first.asString() ) );

		auto s = po.fr.data.at( po.line ).first.asString().sliced( 0, lastPos );

		if( !lineBreak )
			text.push_back( s );
		else
		{
			s = removeLineBreak< Trait >( s );
			text.push_back( s );

			makeTOWLB();
		}
	}

	po.pos = lastPos;

	makeTextObject( text, spaceBefore,
		( po.pos > 0 ? po.fr.data.at( po.line ).first[ po.pos - 1 ].isSpace() : true ), po,
		doNotEscape );
}

template< class Trait >
inline void
skipSpacesInHtml( long long int & l, long long int & p, const typename MdBlock< Trait >::Data & fr )
{
	while( l < (long long int) fr.size() )
	{
		while( p < fr[ l ].first.length() )
		{
			if( !fr[ l ].first[ p ].isSpace() )
				return;

			++p;
		}

		p = 0;
		++l;
	}
}

template< class Trait >
inline std::pair< bool, bool >
readUnquotedHtmlAttrValue( long long int & l, long long int & p, const typename MdBlock< Trait >::Data & fr )
{
	static const typename Trait::String notAllowed = "\"`=<'";

	const auto start = p;

	for( ; p < fr[ l ].first.length(); ++p )
	{
		if( fr[ l ].first[ p ].isSpace() )
			break;
		else if( notAllowed.contains( fr[ l ].first[ p ] ) )
			return { false, false };
		else if( fr[ l ].first[ p ] == typename Trait::Char( '>' ) )
			return { p - start > 0, p - start > 0 };
	}

	return { p - start > 0, p - start > 0 };
}

template< class Trait >
inline std::pair< bool, bool >
readHtmlAttrValue( long long int & l, long long int & p, const typename MdBlock< Trait >::Data & fr )
{
	if( p < fr[ l ].first.length() && fr[ l ].first[ p ] != typename Trait::Char( '"' ) &&
		fr[ l ].first[ p ] != typename Trait::Char( '\'' ) )
			return readUnquotedHtmlAttrValue< Trait >( l, p, fr );

	const auto s = fr[ l ].first[ p ];

	++p;

	if( p >= fr[ l ].first.length() )
		return { false, false };

	for( ; l < (long long int) fr.size(); ++l )
	{
		bool doBreak = false;

		for( ; p < fr[ l ].first.length(); ++p )
		{
			const auto ch = fr[ l ].first[ p ];

			if( ch == s )
			{
				doBreak = true;

				break;
			}
		}

		if( doBreak )
			break;

		p = 0;
	}

	if( l >= (long long int) fr.size() )
		return { false, false };

	if( p >= fr[ l ].first.length() )
		return { false, false };

	if( fr[ l ].first[ p ] != s )
		return { false, false };

	++p;

	return { true, true };
}

template< class Trait >
inline std::pair< bool, bool >
readHtmlAttr( long long int & l, long long int & p, const typename MdBlock< Trait >::Data & fr,
	bool checkForSpace )
{
	long long int tl = l, tp = p;

	skipSpacesInHtml< Trait >( l, p, fr );

	if( l >= (long long int) fr.size() )
		return { false, false };

	// /
	if( p < fr[ l ].first.length() && fr[ l ].first[ p ] == typename Trait::Char( '/' ) )
		return { false, true };

	// >
	if( p < fr[ l ].first.length() && fr[ l ].first[ p ] == typename Trait::Char( '>' ) )
		return { false, true };

	if( checkForSpace )
	{
		if( tl == l && tp == p )
			return { false, false };
	}

	const auto start = p;

	for( ; p < fr[ l ].first.length(); ++p )
	{
		const auto ch = fr[ l ].first[ p ];

		if( ch.isSpace() || ch == typename Trait::Char( '>' ) || ch == typename Trait::Char( '=' ) )
			break;
	}

	const typename Trait::String name = fr[ l ].first.asString()
		.sliced( start, p - start ).toLower();

	if( !name.startsWith( '_' ) && !name.startsWith( ':' ) &&
		!name.isEmpty() &&
		!( name[ 0 ].unicode() >= 97 && name[ 0 ].unicode() <= 122 ) )
	{
		return { false, false };
	}

	static const typename Trait::String allowedInName =
		"abcdefghijklmnopqrstuvwxyz0123456789_.:-";

	for( long long int i = 1; i < name.length(); ++i )
	{
		if( !allowedInName.contains( name[ i ] ) )
			return { false, false };
	}

	// >
	if( p < fr[ l ].first.length() && fr[ l ].first[ p ] == typename Trait::Char( '>' ) )
		return { false, true };

	tl = l;
	tp = p;

	skipSpacesInHtml< Trait >( l, p, fr );

	if( l >= (long long int) fr.size() )
		return { false, false };

	// =
	if( p < fr[ l ].first.length() )
	{
		if( fr[ l ].first[ p ] != typename Trait::Char( '=' ) )
		{
			l = tl;
			p = tp;

			return { true, true };
		}
		else
			++p;
	}
	else
		return { true, false };

	skipSpacesInHtml< Trait >( l, p, fr );

	if( l >= (long long int) fr.size() )
		return { false, false };

	return readHtmlAttrValue< Trait >( l, p, fr );
}

template< class Trait >
inline std::tuple< bool, long long int, long long int, bool, typename Trait::String >
isHtmlTag( long long int line, long long int pos, TextParsingOpts< Trait > & po, int rule );

template< class Trait >
inline bool
isOnlyHtmlTagsAfterOrClosedRule1( long long int line, long long int pos,
	TextParsingOpts< Trait > & po, int rule )
{
	static const std::set< typename Trait::String > rule1Finish = {
		"/pre",
		"/script",
		"/style",
		"/textarea"
	};

	auto p = skipSpaces< Trait >( pos, po.fr.data[ line ].first.asString() );

	while( p < po.fr.data[ line ].first.length() )
	{
		bool ok = false;

		long long int l;
		typename Trait::String tag;

		std::tie( ok, l, p, std::ignore, tag ) =
			isHtmlTag( line, p, po, rule );

		++p;

		if( rule != 1 )
		{
			if( !ok )
				return false;

			if( l > line )
				return true;
		}
		else
		{
			if( rule1Finish.find( tag.toLower() ) != rule1Finish.cend() && l == line )
				return true;

			if( l > line )
				return false;
		}

		p = skipSpaces< Trait >( p, po.fr.data[ line ].first.asString() );
	}

	if( p >= po.fr.data[ line ].first.length() )
		return true;

	return false;
}

template< class Trait >
inline std::tuple< bool, long long int, long long int, bool, typename Trait::String >
isHtmlTag( long long int line, long long int pos, TextParsingOpts< Trait > & po, int rule )
{
	if( po.fr.data[ line ].first[ pos ] != typename Trait::Char( '<' ) )
		return { false, line, pos, false, {} };

	typename Trait::String tag;

	long long int l = line;
	long long int p = pos + 1;
	bool first = false;

	{
		const auto tmp = skipSpaces< Trait >( 0, po.fr.data[ l ].first.asString() );
		first = ( tmp == pos );
	}

	if( p >= po.fr.data[ l ].first.length() )
		return { false, line, pos, false, tag };

	bool closing = false;

	if( po.fr.data[ l ].first[ p ] == typename Trait::Char( '/' ) )
	{
		closing = true;

		tag.push_back( typename Trait::Char( '/' ) );

		++p;
	}

	const auto start = p;

	// tag
	for( ; p < po.fr.data[ l ].first.length(); ++p )
	{
		const auto ch = po.fr.data[ l ].first[ p ];

		if( ch.isSpace() || ch == typename Trait::Char( '>' ) || ch == typename Trait::Char( '/' ) )
			break;
	}

	tag.push_back( po.fr.data[ l ].first.asString().sliced( start, p - start ) );

	if( p < po.fr.data[ l ].first.length() && po.fr.data[ l ].first[ p ] == typename Trait::Char( '/' ) )
	{
		if( p + 1 < po.fr.data[ l ].first.length() && po.fr.data[ l ].first[ p + 1 ] ==
			typename Trait::Char( '>' ) )
		{
			long long int tmp = 0;

			if( rule == 7 )
				tmp = skipSpaces< Trait >( p + 2, po.fr.data[ l ].first.asString() );

			bool onLine = ( first && ( rule == 7 ? tmp == po.fr.data[ l ].first.length() :
				isOnlyHtmlTagsAfterOrClosedRule1( l, p + 2, po, rule == 1 ) ) );

			return { true, l, p + 1, onLine, tag };
		}
		else
			return { false, line, pos, false, tag };
	}

	if( p < po.fr.data[ l ].first.length() && po.fr.data[ l ].first[ p ] ==
		typename Trait::Char( '>' ) )
	{
		long long int tmp = 0;

		if( rule == 7 )
			tmp = skipSpaces< Trait >( p + 1, po.fr.data[ l ].first.asString() );

		bool onLine = ( first && ( rule == 7 ? tmp == po.fr.data[ l ].first.length() :
			isOnlyHtmlTagsAfterOrClosedRule1( l, p + 1, po, rule == 1 ) ) );

		return { true, l, p, onLine, tag };
	}

	skipSpacesInHtml< Trait >( l, p, po.fr.data );

	if( l >= (long long int) po.fr.data.size() )
		return { false, line, pos, false, tag };

	if( po.fr.data[ l ].first[ p ] == typename Trait::Char( '>' ) )
	{
		long long int tmp = 0;

		if( rule == 7 )
			tmp = skipSpaces< Trait >( p + 1, po.fr.data[ l ].first.asString() );

		bool onLine = ( first && ( rule == 7 ? tmp == po.fr.data[ l ].first.length() :
			isOnlyHtmlTagsAfterOrClosedRule1( l, p + 1, po, rule == 1 ) ) );

		return { true, l, p, onLine, tag };
	}

	bool attr = true;
	bool firstAttr = true;

	while( attr )
	{
		bool ok = false;

		std::tie( attr, ok ) = readHtmlAttr< Trait >( l, p, po.fr.data, !firstAttr );

		firstAttr = false;

		if( closing && attr )
			return { false, line, pos, false, tag };

		if( !ok )
			return { false, line, pos, false, tag };
	}

	if( po.fr.data[ l ].first[ p ] == typename Trait::Char( '/' ) )
		++p;
	else
	{
		skipSpacesInHtml< Trait >( l, p, po.fr.data );

		if( l >= (long long int) po.fr.data.size() )
			return { false, line, pos, false, tag };
	}

	if( po.fr.data[ l ].first[ p ] == typename Trait::Char( '>' ) )
	{
		long long int tmp = 0;

		if( rule == 7 )
			tmp = skipSpaces< Trait >( p + 1, po.fr.data[ l ].first.asString() );

		bool onLine = ( first && ( rule == 7 ? tmp == po.fr.data[ l ].first.length() :
			isOnlyHtmlTagsAfterOrClosedRule1( l, p + 1, po, rule == 1 ) ) );

		return { true, l, p, onLine, tag };
	}

	return { false, line, pos, false, {} };
}

template< class Trait >
inline std::pair< typename Trait::String, bool >
readHtmlTag( typename Delims< Trait >::const_iterator it, TextParsingOpts< Trait > & po )
{
	long long int i = it->m_pos + 1;
	const auto start = i;

	if( start >= po.fr.data[ it->m_line ].first.length() )
		return { {}, false };

	for( ; i < po.fr.data[ it->m_line ].first.length(); ++i )
	{
		const auto ch = po.fr.data[ it->m_line ].first[ i ];

		if( ch.isSpace() || ch == typename Trait::Char( '>' ) )
			break;
	}

	return { po.fr.data[ it->m_line ].first.asString().sliced( start, i - start ),
		i < po.fr.data[ it->m_line ].first.length() ?
			po.fr.data[ it->m_line ].first[ i ] == typename Trait::Char( '>' ) : false };
}

template< class Trait >
inline typename Delims< Trait >::const_iterator
findIt( typename Delims< Trait >::const_iterator it, typename Delims< Trait >::const_iterator last,
	TextParsingOpts< Trait > & po )
{
	auto ret = it;

	for( ; it != last; ++it )
	{
		if( ( it->m_line == po.line && it->m_pos < po.pos ) || it->m_line < po.line )
			ret = it;
	}

	return ret;
}

template< class Trait >
inline void
eatRawHtml( long long int line, long long int pos, long long int toLine, long long int toPos,
	TextParsingOpts< Trait > & po, bool finish, int htmlRule, bool onLine )
{
	if( line <= toLine )
	{
		typename Trait::String h = po.html.html->text();

		if( !h.isEmpty() )
		{
			for( long long int i = 0; i < po.fr.emptyLinesBefore; ++i )
				h.push_back( typename Trait::Char( '\n' ) );
		}

		const auto first = po.fr.data[ line ].first.asString().sliced( pos,
			( line == toLine ? ( toPos >= 0 ? toPos - pos : po.fr.data[ line ].first.length() - pos ) :
				po.fr.data[ line ].first.length() - pos ) );

		if( !h.isEmpty() && !first.isEmpty() )
			h.push_back( typename Trait::Char( '\n' ) );

		if( !first.isEmpty() )
			h.push_back( first );

		++line;

		for( ; line < toLine; ++line )
		{
			h.push_back( typename Trait::Char( '\n' ) );
			h.push_back( po.fr.data[ line ].first.asString() );
		}

		if( line == toLine && toPos != 0 )
		{
			h.push_back( typename Trait::Char( '\n' ) );
			h.push_back( po.fr.data[ line ].first.asString().sliced( 0,
				toPos > 0 ? toPos : po.fr.data[ line ].first.length() ) );
		}

		po.line = ( toPos >= 0 ? toLine : toLine + 1 );
		po.pos = ( toPos >= 0 ? toPos : 0 );

		if( po.line < (long long int) po.fr.data.size() &&
			po.pos >= po.fr.data.at( po.line ).first.length() )
		{
			++po.line;
			po.pos = 0;
		}

		po.html.html->setText( h );
	}

	UnprotectedDocsMethods< Trait >::setFreeTag( po.html.html, onLine );

	if( finish )
	{
		if( po.html.onLine || htmlRule == 7 || po.line < (long long int) po.fr.data.size() )
		{
			if( !po.collectRefLinks )
				po.parent->appendItem( po.html.html );

			po.html.html.reset();
			po.html.htmlBlockType = -1;
			po.html.continueHtml = false;
		}
	}
	else
		po.html.continueHtml = true;
}

template< class Trait >
inline void
finishRule1HtmlTag( typename Delims< Trait >::const_iterator it,
	typename Delims< Trait >::const_iterator last,
	TextParsingOpts< Trait > & po, bool skipFirst )
{
	static const std::set< typename Trait::String > finish = {
		"/pre",
		"/script",
		"/style",
		"/textarea"
	};

	if( po.html.html->text().isEmpty() && it->m_type == Delimiter::Less && skipFirst )
		std::tie( std::ignore, std::ignore, std::ignore, po.html.onLine, std::ignore ) =
			isHtmlTag( it->m_line, it->m_pos, po, 1 );

	for( it = ( skipFirst && it != last ? std::next( it ) : it ); it != last; ++it )
	{
		if( it->m_type == Delimiter::Less )
		{
			typename Trait::String tag;
			bool closed = false;

			std::tie( tag, closed ) = readHtmlTag( it, po );

			if( closed )
			{
				if( finish.find( tag.toLower() ) != finish.cend() )
				{
					eatRawHtml( po.line, po.pos, it->m_line, -1, po, true, 1, po.html.onLine );

					return;
				}
			}
		}
	}

	eatRawHtml( po.line, po.pos, po.fr.data.size() - 1, -1, po, false, 1, po.html.onLine );
}

template< class Trait >
inline void
finishRule2HtmlTag( typename Delims< Trait >::const_iterator it,
	typename Delims< Trait >::const_iterator last,
	TextParsingOpts< Trait > & po )
{
	bool finish = true;
	bool onLine = po.html.onLine;

	if( po.html.html->text().isEmpty() && it->m_type == Delimiter::Less )
	{
		size_t i = 0;

		const auto & s = po.fr.data[ it->m_line ].first.asString();

		long long int p = 0;

		while( ( p = s.indexOf( c_startComment, p ) ) != it->m_pos )
		{
			++i;
			++p;
		}

		finish = po.fr.data[ it->m_line ].second.htmlCommentClosed[ i ];

		onLine = ( it->m_pos == skipSpaces< Trait >( 0, po.fr.data[ it->m_line ].first.asString() ) );
		po.html.onLine = onLine;
	}

	if( finish )
	{
		for( ; it != last; ++it )
		{
			if( it->m_type == Delimiter::Greater )
			{
				if( it->m_pos > 1 && po.fr.data[ it->m_line ].first[ it->m_pos - 1 ] ==
						typename Trait::Char( '-' ) &&
					po.fr.data[ it->m_line ].first[ it->m_pos - 2 ] == typename Trait::Char( '-' ) )
				{
					eatRawHtml( po.line, po.pos, it->m_line, po.fr.data[ it->m_line ].first.length(),
						po, true, 2, onLine );

					return;
				}
			}
		}

		eatRawHtml( po.line, po.pos, po.fr.data.size() - 1, -1, po, false, 2, onLine );
	}
	else
	{
		po.html.html.reset();
		po.html.htmlBlockType = -1;
		po.html.continueHtml = false;
		po.html.onLine = false;
	}
}

template< class Trait >
inline void
finishRule3HtmlTag( typename Delims< Trait >::const_iterator it,
	typename Delims< Trait >::const_iterator last,
	TextParsingOpts< Trait > & po )
{
	bool onLine = po.html.onLine;

	if( po.html.html->text().isEmpty() && it->m_type == Delimiter::Less )
	{
		onLine = ( it->m_pos == skipSpaces< Trait >( 0, po.fr.data[ it->m_line ].first.asString() ) );
		po.html.onLine = onLine;
	}

	for( ; it != last; ++it )
	{
		if( it->m_type == Delimiter::Greater )
		{
			if( it->m_pos > 0 && po.fr.data[ it->m_line ].first[ it->m_pos - 1 ] ==
				typename Trait::Char( '?' ) )
			{
				long long int i = it->m_pos + 1;

				for( ; i < po.fr.data[ it->m_line ].first.length(); ++i )
				{
					if( po.fr.data[ it->m_line ].first[ i ] == typename Trait::Char( '<' ) )
						break;
				}

				eatRawHtml( po.line, po.pos, it->m_line, i , po, true, 3, onLine );

				return;
			}
		}
	}

	eatRawHtml( po.line, po.pos, po.fr.data.size() - 1, -1, po, false, 3, onLine );
}

template< class Trait >
inline void
finishRule4HtmlTag( typename Delims< Trait >::const_iterator it,
	typename Delims< Trait >::const_iterator last,
	TextParsingOpts< Trait > & po )
{
	bool onLine = po.html.onLine;

	if( po.html.html->text().isEmpty() && it->m_type == Delimiter::Less )
	{
		onLine = ( it->m_pos == skipSpaces< Trait >( 0, po.fr.data[ it->m_line ].first.asString() ) );
		po.html.onLine = onLine;
	}

	for( ; it != last; ++it )
	{
		if( it->m_type == Delimiter::Greater )
		{
			long long int i = it->m_pos + 1;

			for( ; i < po.fr.data[ it->m_line ].first.length(); ++i )
			{
				if( po.fr.data[ it->m_line ].first[ i ] == typename Trait::Char( '<' ) )
					break;
			}

			eatRawHtml( po.line, po.pos, it->m_line, i , po, true, 4, onLine );

			return;
		}
	}

	eatRawHtml( po.line, po.pos, po.fr.data.size() - 1, -1, po, false, 4, true );
}

template< class Trait >
inline void
finishRule5HtmlTag( typename Delims< Trait >::const_iterator it,
	typename Delims< Trait >::const_iterator last,
	TextParsingOpts< Trait > & po )
{
	bool onLine = po.html.onLine;

	if( po.html.html->text().isEmpty() && it->m_type == Delimiter::Less )
	{
		onLine = ( it->m_pos == skipSpaces< Trait >( 0, po.fr.data[ it->m_line ].first.asString() ) );
		po.html.onLine = onLine;
	}

	for( ; it != last; ++it )
	{
		if( it->m_type == Delimiter::Greater )
		{
			if( it->m_pos > 1 && po.fr.data[ it->m_line ].first[ it->m_pos - 1 ] ==
					typename Trait::Char( ']' ) &&
				po.fr.data[ it->m_line ].first[ it->m_pos - 2 ] == typename Trait::Char( ']' ) )
			{
				long long int i = it->m_pos + 1;

				for( ; i < po.fr.data[ it->m_line ].first.length(); ++i )
				{
					if( po.fr.data[ it->m_line ].first[ i ] == typename Trait::Char( '<' ) )
						break;
				}

				eatRawHtml( po.line, po.pos, it->m_line, i , po, true, 5, onLine );

				return;
			}
		}
	}

	eatRawHtml( po.line, po.pos, po.fr.data.size() - 1, -1, po, false, 5, true );
}

template< class Trait >
inline void
finishRule6HtmlTag( typename Delims< Trait >::const_iterator it,
	typename Delims< Trait >::const_iterator last,
	TextParsingOpts< Trait > & po )
{
	po.html.onLine = ( it != last ? it->m_pos == skipSpaces< Trait >(
		0, po.fr.data[ it->m_line ].first.asString() ) : true );

	eatRawHtml( po.line, po.pos, po.fr.data.size() - 1, -1, po, false, 6, po.html.onLine );
}

template< class Trait >
inline typename Delims< Trait >::const_iterator
finishRule7HtmlTag( typename Delims< Trait >::const_iterator it,
	typename Delims< Trait >::const_iterator last,
	TextParsingOpts< Trait > & po );

template< class Trait >
inline typename Delims< Trait >::const_iterator
finishRawHtmlTag( typename Delims< Trait >::const_iterator it,
	typename Delims< Trait >::const_iterator last,
	TextParsingOpts< Trait > & po, bool skipFirst )
{
	switch( po.html.htmlBlockType )
	{
		case 1 :
			finishRule1HtmlTag( it, last, po, skipFirst );
			break;

		case 2 :
			finishRule2HtmlTag( it, last, po );
			break;

		case 3 :
			finishRule3HtmlTag( it, last, po );
			break;

		case 4 :
			finishRule4HtmlTag( it, last, po );
			break;

		case 5 :
			finishRule5HtmlTag( it, last, po );
			break;

		case 6 :
			finishRule6HtmlTag( it, last, po );
			break;

		case 7 :
			return finishRule7HtmlTag( it, last, po );

		default :
			break;
	}

	return findIt( it, last, po );
}

template< class Trait >
inline int
htmlTagRule( typename Delims< Trait >::const_iterator it,
	typename Delims< Trait >::const_iterator last,
	TextParsingOpts< Trait > & po )
{
	typename Trait::String tag;

	std::tie( tag, std::ignore ) = readHtmlTag( it, po );

	if( tag == "![CDATA[" )
		return 5;

	tag = tag.toLower();

	if( tag.isEmpty() )
		return -1;

	static const typename Trait::String c_validHtmlTagLetters =
		"abcdefghijklmnopqrstuvwxyz0123456789-";

	bool closing = false;

	if( tag.startsWith( '/' ) )
	{
		tag.remove( 0, 1 );
		closing = true;
	}

	if( tag.endsWith( '/' ) )
		tag.remove( tag.size() - 1, 1 );

	if( !tag.startsWith( '!' ) && !tag.startsWith( '?' ) &&
		!( tag[ 0 ].unicode() >= 97 && tag[ 0 ].unicode() <= 122 ) )
			return -1;

	for( long long int i = 1; i < tag.size(); ++i )
	{
		if( !c_validHtmlTagLetters.contains( tag[ i ] ) )
			return -1;
	}

	static const std::set< typename Trait::String > rule1 = {
		"pre", "script",
		"style", "textarea"
	};

	if( !closing && rule1.find( tag ) != rule1.cend() )
		return 1;
	else if( tag == "!--" )
		return 2;
	else if( tag.startsWith( "?" ) )
		return 3;
	else if( tag.startsWith( '!' ) && tag.size() > 1 &&
		( ( tag[ 1 ].unicode() >= 65 && tag[ 1 ].unicode() <= 90 ) ||
			( tag[ 1 ].unicode() >= 97 && tag[ 1 ].unicode() <= 122 ) ) )
	{
		return 4;
	}
	else
	{
		static const std::set< typename Trait::String > rule6 = {
			"address", "article", "aside",
			"base", "basefont", "blockquote",
			"body", "caption", "center",
			"col", "colgroup", "dd",
			"details", "dialog", "dir",
			"div", "dl", "dt",
			"fieldset", "figcaption", "figure",
			"footer", "form", "frame",
			"frameset", "h1", "h2",
			"h3", "h4", "h5",
			"h6", "head", "header",
			"hr", "html", "iframe",
			"legend", "li", "link",
			"main", "menu", "menuitem",
			"nav", "noframes", "ol",
			"optgroup", "option", "p",
			"param", "section", "source",
			"summary", "table", "tbody",
			"td", "tfoot", "th",
			"thead", "title", "tr",
			"track", "ul"
		};

		if( rule6.find( tag ) != rule6.cend() )
			return 6;
		else
		{
			bool tag = false;

			std::tie( tag, std::ignore, std::ignore, std::ignore, std::ignore ) =
				isHtmlTag( it->m_line, it->m_pos, po, 7 );

			if( tag )
				return 7;
		}
	}

	return -1;
}

template< class Trait >
inline typename Delims< Trait >::const_iterator
checkForRawHtml( typename Delims< Trait >::const_iterator it,
	typename Delims< Trait >::const_iterator last,
	TextParsingOpts< Trait > & po )
{
	const auto rule = htmlTagRule( it, last, po );

	po.wasRefLink = false;

	if( rule == -1 )
	{
		po.html.htmlBlockType = -1;
		po.html.html.reset();

		return it;
	}

	po.html.htmlBlockType = rule;
	po.html.html.reset( new RawHtml< Trait > );

	return finishRawHtmlTag( it, last, po, true );
}

template< class Trait >
inline typename Delims< Trait >::const_iterator
finishRule7HtmlTag( typename Delims< Trait >::const_iterator it,
	typename Delims< Trait >::const_iterator last,
	TextParsingOpts< Trait > & po )
{
	const auto start = it;
	long long int l = -1, p = -1;
	bool onLine = false;
	bool ok = false;

	std::tie( ok, l, p, onLine, std::ignore ) = isHtmlTag( it->m_line, it->m_pos,
		po, 7 );

	onLine = onLine && it->m_line == 0 && l == start->m_line;

	if( ok )
	{
		eatRawHtml( po.line, po.pos, l, ++p, po, !onLine, 7, onLine );

		po.html.onLine = onLine;

		it = findIt( it, last, po );

		if( onLine )
		{
			for( ; it != last; ++it )
			{
				if( it->m_type == Delimiter::Less )
				{
					const auto rule = htmlTagRule( it, last, po );

					if( rule != -1 && rule != 7 )
					{
						eatRawHtml( po.line, po.pos, it->m_line, it->m_pos, po, true, 7, onLine );

						return std::prev( it );
					}
				}
			}

			eatRawHtml( po.line, po.pos, po.fr.data.size() - 1, -1, po, false, 7, onLine );

			return std::prev( last );
		}
		else
			return findIt( it, last, po );
	}
	else
		return it;
}

template< class Trait >
inline typename Delims< Trait >::const_iterator
checkForMath( typename Delims< Trait >::const_iterator it,
	typename Delims< Trait >::const_iterator last,
	TextParsingOpts< Trait > & po )
{
	const auto end = std::find_if( std::next( it ), last,
		[&] ( const auto & d )
		{
			return ( d.m_type == Delimiter::Math &&
					 d.m_len == it->m_len );
		} );

	if( end != last )
	{
		typename Trait::String math;

		if( it->m_line == end->m_line )
			math = po.fr.data[ it->m_line ].first.asString().sliced( it->m_pos + it->m_len,
				end->m_pos - ( it->m_pos + it->m_len ) );
		else
		{
			math = po.fr.data[ it->m_line ].first.asString().sliced( it->m_pos + it->m_len );

			for( long long int i = it->m_line + 1; i < end->m_line; ++i )
			{
				math.push_back( typename Trait::Char( '\n' ) );
				math.push_back( po.fr.data[ i ].first.asString() );
			}

			math.push_back( typename Trait::Char( '\n' ) );
			math.push_back( po.fr.data[ end->m_line ].first.asString().sliced( 0, end->m_pos ) );
		}

		if( !po.collectRefLinks )
		{
			std::shared_ptr< Math< Trait > > m( new Math< Trait > );
			m->setInline( it->m_len == 1 );
			m->setExpr( math );

			po.parent->appendItem( m );

			po.pos = end->m_pos + end->m_len;
			po.line = end->m_line;
		}

		return end;
	}

	return it;
}

template< class Trait >
inline bool isEmail( const typename Trait::String & url );

#ifdef MD4QT_QT_SUPPORT

template<>
inline bool isEmail< QStringTrait >( const QString & url )
{
	static const QRegularExpression er(
		"^[a-zA-Z0-9.!#$%&'*+/=?^_`{|}~-]+@[a-zA-Z0-9](?:[a-zA-Z0-9-]{0,61}[a-zA-Z0-9])?"
		"(?:\\.[a-zA-Z0-9](?:[a-zA-Z0-9-]{0,61}[a-zA-Z0-9])?)*$" );

	QRegularExpressionMatch erm;

	if( url.startsWith( "mailto:", Qt::CaseInsensitive ) )
		erm = er.match( url.right( url.length() - 7 ) );
	else
		erm = er.match( url );

	return erm.hasMatch();
}

#endif

#ifdef MD4QT_ICU_STL_SUPPORT

template<>
inline bool isEmail< UnicodeStringTrait >( const UnicodeString & url )
{
	UParseError er;
	er.line = 0;
	er.offset = 0;
	er.preContext[ 0 ] = 0;
	er.postContext[ 0 ] = 0;

	UErrorCode status = U_ZERO_ERROR;

	static const icu::UnicodeString pattern = icu::UnicodeString::fromUTF8(
		"^[a-zA-Z0-9.!#$%&'*+/=?^_`{|}~-]+@[a-zA-Z0-9](?:[a-zA-Z0-9-]{0,61}[a-zA-Z0-9])?"
		"(?:\\.[a-zA-Z0-9](?:[a-zA-Z0-9-]{0,61}[a-zA-Z0-9])?)*$" );

	if( url.toLower().startsWith( "mailto:" ) )
		return (bool) icu::RegexPattern::matches( pattern, url.right( url.length() - 7 ), er, status );
	else
		return (bool) icu::RegexPattern::matches( pattern, url, er, status );
}

#endif

template< class Trait >
inline typename Delims< Trait >::const_iterator
checkForAutolinkHtml( typename Delims< Trait >::const_iterator it,
	typename Delims< Trait >::const_iterator last,
	TextParsingOpts< Trait > & po, bool updatePos )
{
	const auto nit = std::find_if( std::next( it ), last,
		[] ( const auto & d ) { return ( d.m_type == Delimiter::Greater ); } );

	if( nit != last )
	{
		if( nit->m_line == it->m_line )
		{
			const auto url = po.fr.data.at( it->m_line ).first.asString().sliced( it->m_pos + 1,
				nit->m_pos - it->m_pos - 1 );

			bool isUrl = true;

			for( long long int i = 0; i < url.size(); ++i )
			{
				if( url[ i ].isSpace() )
				{
					isUrl = false;

					break;
				}
			}

			if( isUrl )
			{
				const typename Trait::Url u( url );

				if( ( !u.isValid() || u.isRelative() ) && !isEmail< Trait >( url ) )
					isUrl = false;
			}

			if( isUrl )
			{
				if( !po.collectRefLinks )
				{
					std::shared_ptr< Link< Trait > > lnk( new Link< Trait > );
					lnk->setUrl( url.simplified() );
					lnk->setOpts( po.opts );
					po.parent->appendItem( lnk );
				}

				po.wasRefLink = false;

				if( updatePos )
				{
					po.pos = nit->m_pos + nit->m_len;
					po.line = nit->m_line;
				}

				return nit;
			}
			else
				return checkForRawHtml( it, last, po );
		}
		else
			return checkForRawHtml( it, last, po );
	}
	else
		return checkForRawHtml( it, last, po );
}

template< class Trait >
inline void
makeInlineCode( long long int lastLine, long long int lastPos,
	TextParsingOpts< Trait > & po )
{
	typename Trait::String c;

	for( ; po.line <= lastLine; ++po.line )
	{
		c.push_back( po.fr.data.at( po.line ).first.asString().sliced( po.pos,
			( po.line == lastLine ? lastPos - po.pos :
				po.fr.data.at( po.line ).first.length() - po.pos ) ) );

		if( po.line < lastLine )
			c.push_back( typename Trait::Char( ' ' ) );

		po.pos = 0;
	}

	po.line = lastLine;

	if( c[ 0 ] == typename Trait::Char( ' ' ) && c[ c.size() - 1 ] == typename Trait::Char( ' ' ) &&
		skipSpaces< Trait >( 0, c ) < c.size() )
	{
		c.remove( 0, 1 );
		c.remove( c.size() - 1, 1 );
	}

	if( !c.isEmpty() )
		po.parent->appendItem( std::shared_ptr< Code< Trait > >( new Code< Trait >( c, true ) ) );

	po.wasRefLink = false;
}

template< class Trait >
inline typename Delims< Trait >::const_iterator
checkForInlineCode( typename Delims< Trait >::const_iterator it,
	typename Delims< Trait >::const_iterator last,
	TextParsingOpts< Trait > & po )
{
	const auto len = it->m_len;
	const auto start = it;

	++it;

	for( ; it != last; ++it )
	{
		if( it->m_type == Delimiter::InlineCode &&
			( it->m_len - ( it->m_backslashed ? 1 : 0 ) ) == len )
		{
			if( !po.collectRefLinks )
			{
				makeText( start->m_line, start->m_pos, po );

				po.pos = start->m_pos + start->m_len;

				makeInlineCode( it->m_line, it->m_pos + ( it->m_backslashed ? 1 : 0 ), po );

				po.line = it->m_line;
				po.pos = it->m_pos + it->m_len;
			}

			po.wasRefLink = false;

			return it;
		}
	}

	if( !po.collectRefLinks )
		makeText( start->m_line, start->m_pos + start->m_len, po );

	po.wasRefLink = false;

	return start;
}

template< class Trait >
inline std::pair< typename MdBlock< Trait >::Data, typename Delims< Trait >::const_iterator >
readTextBetweenSquareBrackets( typename Delims< Trait >::const_iterator start,
	typename Delims< Trait >::const_iterator it, typename Delims< Trait >::const_iterator last,
	TextParsingOpts< Trait > & po,
	bool doNotCreateTextOnFail )
{
	if( it != last )
	{
		if( start->m_line == it->m_line )
		{
			const auto p = start->m_pos + start->m_len;
			const auto n = it->m_pos - p;

			return { { { po.fr.data.at( start->m_line ).first.sliced( p, n ).simplified(),
				{ po.fr.data.at( start->m_line ).second.lineNumber } } }, it };
		}
		else
		{
			if( it->m_line - start->m_line < 3 )
			{
				typename MdBlock< Trait >::Data res;
				res.push_back(
					{ po.fr.data.at( start->m_line ).first
						.sliced( start->m_pos + start->m_len ).simplified(),
							{ po.fr.data.at( start->m_line ).second.lineNumber } } );

				long long int i = start->m_line + 1;

				for( ; i <= it->m_line; ++i )
				{
					if( i == it->m_line )
						res.push_back( { po.fr.data.at( i ).first.sliced( 0, it->m_pos ).simplified(),
							{ po.fr.data.at( i ).second.lineNumber } } );
					else
						res.push_back( { po.fr.data.at( i ).first.simplified(),
							{ po.fr.data.at( i ).second.lineNumber } } );
				}

				return { res, it };
			}
			else
			{
				if( !po.collectRefLinks && !doNotCreateTextOnFail )
					makeText( start->m_line, start->m_pos + start->m_len, po );

				return { {}, start };
			}
		}
	}
	else
	{
		if( !po.collectRefLinks && !doNotCreateTextOnFail )
			makeText( start->m_line, start->m_pos + start->m_len, po );

		return { {}, start };
	}
}

template< class Trait >
inline std::pair< typename MdBlock< Trait >::Data, typename Delims< Trait >::const_iterator >
checkForLinkText( typename Delims< Trait >::const_iterator it,
	typename Delims< Trait >::const_iterator last,
	TextParsingOpts< Trait > & po )
{
	const auto start = it;

	long long int brackets = 0;

	const bool collectRefLinks = po.collectRefLinks;
	po.collectRefLinks = true;
	long long int l = po.line, p = po.pos;

	for( it = std::next( it ); it != last; ++it )
	{
		bool quit = false;

		switch( it->m_type )
		{
			case Delimiter::SquareBracketsClose :
			{
				if( !brackets )
					quit = true;
				else
					--brackets;
			}
				break;

			case Delimiter::SquareBracketsOpen :
			case Delimiter::ImageOpen :
				++brackets;
				break;

			case Delimiter::InlineCode :
				it = checkForInlineCode( it, last, po );
				break;

			case Delimiter::Less :
				it = checkForAutolinkHtml( it, last, po, false );
				break;

			default :
				break;
		}

		if( quit )
			break;
	}

	const auto r =  readTextBetweenSquareBrackets( start, it, last, po, false );

	po.collectRefLinks = collectRefLinks;
	po.html.html.reset();
	po.html.htmlBlockType = -1;
	po.html.continueHtml = false;
	po.html.onLine = false;
	po.line = l;
	po.pos = p;

	return r;
}

template< class Trait >
inline std::pair< typename MdBlock< Trait >::Data, typename Delims< Trait >::const_iterator >
checkForLinkLabel( typename Delims< Trait >::const_iterator it,
	typename Delims< Trait >::const_iterator last,
	TextParsingOpts< Trait > & po )
{
	const auto start = it;

	for( it = std::next( it ); it != last; ++it )
	{
		bool quit = false;

		switch( it->m_type )
		{
			case Delimiter::SquareBracketsClose :
			{
				quit = true;
			}
				break;

			case Delimiter::SquareBracketsOpen :
			case Delimiter::ImageOpen :
			{
				it = last;
				quit = true;
			}
				break;

			default :
				break;
		}

		if( quit )
			break;
	}

	return readTextBetweenSquareBrackets( start, it, last, po, true );
}

template< class Trait >
inline typename Trait::String
toSingleLine( const typename MdBlock< Trait >::Data & d )
{
	typename Trait::String res;
	bool first = true;

	for( const auto & s : d )
	{
		if( !first ) res.push_back( typename Trait::Char( ' ' ) );
		res.push_back( s.first.asString() );
		first = false;
	}

	return res;
}

template< class Trait >
inline std::shared_ptr< Link< Trait > >
makeLink( const typename Trait::String & url, const typename MdBlock< Trait >::Data & text,
	TextParsingOpts< Trait > & po,
	bool doNotCreateTextOnFail,
	long long int lastLine, long long int lastPos )
{
	typename Trait::String u = ( url.startsWith( '#' ) ? url : removeBackslashes< Trait >(
		replaceEntity< Trait >( url ) ).asString() );

	if( !u.isEmpty() )
	{
		if( !u.startsWith( '#' ) )
		{
			if( Trait::fileExists( u, po.workingPath ) )
			{
				u = Trait::absoluteFilePath( po.workingPath + u );

				po.linksToParse.push_back( u );
			}
		}
		else
			u = u + "/" + po.workingPath + po.fileName;
	}

	std::shared_ptr< Link< Trait > > link( new Link< Trait > );
	link->setUrl( u );
	link->setOpts( po.opts );

	MdBlock< Trait > block = { text, 0 };

	std::shared_ptr< Paragraph< Trait > > p( new Paragraph< Trait > );

	RawHtmlBlock< Trait > html;

	parseFormattedText( block, std::static_pointer_cast< Block< Trait > >( p ), po.doc,
		po.linksToParse, po.workingPath,
		po.fileName, po.collectRefLinks, true, html );

	if( !p->isEmpty() )
	{
		std::shared_ptr< Image< Trait > > img;

		if( p->items().size() == 1 && p->items().at( 0 )->type() == ItemType::Paragraph )
		{
			const auto ip = std::static_pointer_cast< Paragraph< Trait > >( p->items().at( 0 ) );

			for( auto it = ip->items().cbegin(), last = ip->items().cend(); it != last; ++it )
			{
				switch( (*it)->type() )
				{
					case ItemType::Link :
						return {};

					case ItemType::Image :
					{
						img = std::static_pointer_cast< Image< Trait > >( *it );
					}
						break;

					default :
						break;
				}
			}

			if( img.get() )
				link->setImg( img );

			link->setP( ip );
		}
	}

	link->setText( toSingleLine< Trait >( text ).simplified() );

	return link;
}

template< class Trait >
inline bool
createShortcutLink( const typename MdBlock< Trait >::Data & text,
	TextParsingOpts< Trait > & po,
	long long int lastLine, long long int lastPos,
	typename Delims< Trait >::const_iterator lastIt,
	const typename MdBlock< Trait >::Data & linkText,
	bool doNotCreateTextOnFail )
{
	const auto u = "#" + toSingleLine< Trait >( text ).simplified().toCaseFolded().toUpper();
	const auto url = u + "/" + po.workingPath + po.fileName;

	po.wasRefLink = false;

	if( po.doc->labeledLinks().find( url ) != po.doc->labeledLinks().cend() )
	{
		if( !po.collectRefLinks )
		{
			const auto link = makeLink( u,
				removeBackslashes< Trait >( toSingleLine< Trait > (
					linkText ).simplified().isEmpty() ? text : linkText ),
				po, doNotCreateTextOnFail, lastLine, lastPos );

			if( link.get() )
			{	
				po.linksToParse.push_back( url );
				po.parent->appendItem( link );

				po.line = lastIt->m_line;
				po.pos = lastIt->m_pos + lastIt->m_len;
			}
			else
			{
				if( !po.collectRefLinks && !doNotCreateTextOnFail )
					makeText( lastLine, lastPos, po );

				return false;
			}
		}

		return true;
	}
	else if( !po.collectRefLinks && !doNotCreateTextOnFail )
		makeText( lastLine, lastPos, po );

	return false;
}

template< class Trait >
inline std::shared_ptr< Image< Trait > >
makeImage( const typename Trait::String & url, const typename MdBlock< Trait >::Data & text,
	TextParsingOpts< Trait > & po,
	bool doNotCreateTextOnFail,
	long long int lastLine, long long int lastPos )
{
	std::shared_ptr< Image< Trait > > img( new Image< Trait > );

	typename Trait::String u = ( url.startsWith( '#' ) ? url :
		removeBackslashes< Trait >( replaceEntity< Trait >( url ) ).asString() );

	if( Trait::fileExists( u, po.workingPath ) )
		img->setUrl( po.workingPath + u );
	else
		img->setUrl( u );

	MdBlock< Trait > block = { text, 0 };

	std::shared_ptr< Paragraph< Trait > > p( new Paragraph< Trait > );

	RawHtmlBlock< Trait > html;

	parseFormattedText( block, std::static_pointer_cast< Block< Trait > >( p ), po.doc,
		po.linksToParse, po.workingPath,
		po.fileName, po.collectRefLinks, true, html );

	if( !p->isEmpty() )
	{
		if( p->items().size() == 1 && p->items().at( 0 )->type() == ItemType::Paragraph )
			img->setP( std::static_pointer_cast< Paragraph< Trait > >( p->items().at( 0 ) ) );
	}

	img->setText( toSingleLine< Trait >( removeBackslashes< Trait >( text ) ).simplified() );

	return img;
}

template< class Trait >
inline bool
createShortcutImage( const typename MdBlock< Trait >::Data & text,
	TextParsingOpts< Trait > & po,
	long long int lastLine, long long int lastPos,
	typename Delims< Trait >::const_iterator lastIt,
	const typename MdBlock< Trait >::Data & linkText,
	bool doNotCreateTextOnFail )
{
	const auto url = "#" + toSingleLine< Trait >( text ).simplified().toCaseFolded().toUpper() +
		"/" + po.workingPath + po.fileName;

	po.wasRefLink = false;

	const auto iit = po.doc->labeledLinks().find( url );

	if( iit != po.doc->labeledLinks().cend() )
	{
		if( !po.collectRefLinks )
		{
			const auto img = makeImage( iit->second->url(),
				( toSingleLine< Trait >( linkText ).simplified().isEmpty() ? text : linkText ), po,
				doNotCreateTextOnFail, lastLine, lastPos );

			po.parent->appendItem( img );

			po.line = lastIt->m_line;
			po.pos = lastIt->m_pos + lastIt->m_len;
		}

		return true;
	}
	else if( !po.collectRefLinks && !doNotCreateTextOnFail )
		makeText( lastLine, lastPos, po );

	return false;
}

template< class Trait >
inline void
skipSpacesUpTo1Line( long long int & line, long long int & pos, const typename MdBlock< Trait >::Data & fr )
{
	pos = skipSpaces< Trait >( pos, fr.at( line ).first.asString() );

	if( pos == fr.at( line ).first.length() && line + 1 < (long long int) fr.size() )
	{
		++line;
		pos = skipSpaces< Trait >( 0, fr.at( line ).first.asString() );
	}
}

template< class Trait >
inline std::tuple< long long int, long long int, bool, typename Trait::String, long long int >
readLinkDestination( long long int line, long long int pos, const typename MdBlock< Trait >::Data & fr )
{
	skipSpacesUpTo1Line< Trait >( line, pos, fr );

	const auto destLine = line;
	const auto & s = fr.at( line ).first.asString();
	bool backslash = false;

	if( pos < s.length() )
	{
		if( s[ pos ] == typename Trait::Char( '<' ) )
		{
			++pos;

			const auto start = pos;

			while( pos < s.size() )
			{
				bool now = false;

				if( s[ pos ] == typename Trait::Char( '\\' ) && !backslash )
				{
					backslash = true;
					now = true;
				}
				else if( !backslash && s[ pos ] == typename Trait::Char( '<' ) )
					return { line, pos, false, {}, destLine };
				else if( !backslash && s[ pos ] == typename Trait::Char( '>' ) )
					break;

				if( !now )
					backslash = false;

				++pos;
			}

			if( pos < s.size() && s[ pos ] == typename Trait::Char( '>' ) )
				return { line, ++pos, true, s.sliced( start, pos - start - 1 ), destLine };
			else
				return { line, pos, false, {}, destLine };
		}
		else
		{
			long long int pc = 0;

			const auto start = pos;

			while( pos < s.size() )
			{
				bool now = false;

				if( s[ pos ] == typename Trait::Char( '\\' ) && !backslash )
				{
					backslash = true;
					now = true;
				}
				else if( !backslash && ( s[ pos ] == typename Trait::Char( ' ' ) ||
					s[ pos ] == typename Trait::Char( '\t' ) ) )
				{
					if( !pc )
						return { line, pos, true, s.sliced( start, pos - start ), destLine };
					else
						return { line, pos, false, {}, destLine };
				}
				else if( !backslash && s[ pos ] == typename Trait::Char( '(' ) )
					++pc;
				else if( !backslash && s[ pos ] == typename Trait::Char( ')' ) )
				{
					if( !pc )
						return { line, pos, true, s.sliced( start, pos - start ), destLine };
					else
						--pc;
				}

				if( !now )
					backslash = false;

				++pos;
			}

			return { line, pos, true, s.sliced( start, pos - start ), destLine };
		}
	}
	else
		return { line, pos, false, {}, destLine };
}

template< class Trait >
inline std::tuple< long long int, long long int, bool, typename Trait::String, long long int >
readLinkTitle( long long int line, long long int pos, const typename MdBlock< Trait >::Data & fr )
{
	const auto space = ( pos < fr.at( line ).first.length() ?
		fr.at( line ).first[ pos ].isSpace() : true );

	const auto firstLine = line;

	skipSpacesUpTo1Line< Trait >( line, pos, fr );

	if( pos >= fr.at( line ).first.length() )
		return { line, pos, true, {}, firstLine };

	const auto sc = fr.at( line ).first[ pos ];

	if( sc != typename Trait::Char( '"' ) && sc != typename Trait::Char( '\'' ) &&
		sc != typename Trait::Char( '(' ) && sc != typename Trait::Char( ')' ) )
			return { line, pos, ( firstLine != line ), {}, firstLine };
	else if( !space && sc != typename Trait::Char( ')' ) )
		return { line, pos, false, {}, firstLine };

	if( sc == typename Trait::Char( ')' ) )
		return { line, pos, true, {}, firstLine };

	const auto startLine = line;

	bool backslash = false;

	++pos;

	skipSpacesUpTo1Line< Trait >( line, pos, fr );

	typename Trait::String title;

	while( line < (long long int) fr.size() && pos < fr.at( line ).first.length() )
	{
		bool now = false;

		if( fr.at( line ).first[ pos ] == typename Trait::Char( '\\' ) && !backslash )
		{
			backslash = true;
			now = true;
		}
		else if( sc == typename Trait::Char( '(' ) &&
			fr.at( line ).first[ pos ] == typename Trait::Char( ')' ) && !backslash )
				return { line, ++pos, true, title, startLine };
		else if( sc == typename Trait::Char( '(' ) &&
			fr.at( line ).first[ pos ] == typename Trait::Char( '(' ) && !backslash )
				return { line, pos, false, {}, startLine };
		else if( sc != typename Trait::Char( '(' ) && fr.at( line ).first[ pos ] == sc && !backslash )
			return { line, ++pos, true, title, startLine };
		else
			title.push_back( fr.at( line ).first[ pos ] );

		if( !now )
			backslash = false;

		++pos;

		if( pos == fr.at( line ).first.length() )
			skipSpacesUpTo1Line< Trait >( line, pos, fr );
	}

	return { line, pos, false, {}, startLine };
}

template< class Trait >
inline std::tuple< typename Trait::String, typename Trait::String,
	typename Delims< Trait >::const_iterator, bool >
checkForInlineLink( typename Delims< Trait >::const_iterator it,
	typename Delims< Trait >::const_iterator last,
	TextParsingOpts< Trait > & po )
{
	long long int p = it->m_pos + it->m_len;
	long long int l = it->m_line;
	bool ok = false;
	typename Trait::String dest, title;
	long long int destStartLine = 0;

	std::tie( l, p, ok, dest, destStartLine ) = readLinkDestination< Trait >( l, p, po.fr.data );

	if( !ok )
		return { {}, {}, it, false };

	long long int s = 0;

	std::tie( l, p, ok, title, s ) = readLinkTitle< Trait >( l, p, po.fr.data );

	skipSpacesUpTo1Line< Trait >( l, p, po.fr.data );

	if( !ok || ( l >= (long long int) po.fr.data.size() || p >= po.fr.data.at( l ).first.length() ||
		po.fr.data.at( l ).first[ p ] != typename Trait::Char( ')' ) ) )
			return { {}, {}, it, false };

	for( ; it != last; ++it )
	{
		if( it->m_line == l && it->m_pos == p )
			return { dest, title, it, true };
	}

	return { {}, {}, it, false };
}

template< class Trait >
inline std::tuple< typename Trait::String, typename Trait::String,
	typename Delims< Trait >::const_iterator, bool >
checkForRefLink( typename Delims< Trait >::const_iterator it,
	typename Delims< Trait >::const_iterator last,
	TextParsingOpts< Trait > & po )
{
	long long int p = it->m_pos + it->m_len + 1;
	long long int l = it->m_line;
	bool ok = false;
	typename Trait::String dest, title;
	long long int destStartLine = 0;

	std::tie( l, p, ok, dest, destStartLine ) = readLinkDestination< Trait >( l, p, po.fr.data );

	if( !ok )
		return { {}, {}, it, false };

	long long int titleStartLine = 0;

	std::tie( l, p, ok, title, titleStartLine ) = readLinkTitle< Trait >( l, p, po.fr.data );

	if( !ok )
		return { {}, {}, it, false };

	if( !title.isEmpty() )
	{
		p = skipSpaces< Trait >( p, po.fr.data.at( l ).first.asString() );

		if( titleStartLine == destStartLine && p < po.fr.data.at( l ).first.length() )
			return { {}, {}, it, false };
		else if( titleStartLine != destStartLine && p < po.fr.data.at( l ).first.length() )
		{
			l = destStartLine;
			p = po.fr.data.at( l ).first.length();
			title.clear();
		}
	}

	for( ; it != last; ++it )
	{
		if( it->m_line > l || ( it->m_line == l && it->m_pos >= p ) )
			break;
	}

	po.line = l;
	po.pos = p;

	return { dest, title, std::prev( it ), true };
}

template< class Trait >
inline typename Delims< Trait >::const_iterator
checkForImage( typename Delims< Trait >::const_iterator it,
	typename Delims< Trait >::const_iterator last,
	TextParsingOpts< Trait > & po )
{
	const auto start = it;

	typename MdBlock< Trait >::Data text;

	po.wasRefLink = false;

	std::tie( text, it ) = checkForLinkText( it, last, po );

	if( it != start )
	{
		if( it->m_pos + it->m_len < po.fr.data.at( it->m_line ).first.length() )
		{
			// Inline -> (
			if( po.fr.data.at( it->m_line ).first[ it->m_pos + it->m_len ] ==
				typename Trait::Char( '(' ) )
			{
				typename Trait::String url, title;
				typename Delims< Trait >::const_iterator iit;
				bool ok;

				std::tie( url, title, iit, ok ) = checkForInlineLink( std::next( it ), last, po );

				if( ok )
				{
					if( !po.collectRefLinks )
						po.parent->appendItem( makeImage( url, text, po, false,
							start->m_line, start->m_pos + start->m_len ) );

					po.line = iit->m_line;
					po.pos = iit->m_pos + iit->m_len;

					return iit;
				}
				else if( createShortcutImage( text,
							po, start->m_line, start->m_pos + start->m_len,
							it, {}, false ) )
				{
					return it;
				}
			}
			// Reference -> [
			else if( po.fr.data.at( it->m_line ).first[ it->m_pos + it->m_len ] ==
				typename Trait::Char( '[' ) )
			{
				typename MdBlock< Trait >::Data label;
				typename Delims< Trait >::const_iterator lit;

				std::tie( label, lit ) = checkForLinkLabel( std::next( it ), last, po );

				if( lit != std::next( it ) )
				{
					const auto isLabelEmpty = toSingleLine< Trait > ( label ).simplified().isEmpty();

					if( !isLabelEmpty &&
						createShortcutImage( label,
							po, start->m_line, start->m_pos + start->m_len,
							lit, text, true ) )
					{
						return lit;
					}
					else if( isLabelEmpty &&
						createShortcutImage( text,
							po, start->m_line, start->m_pos + start->m_len,
							lit, {}, false ) )
					{
						return lit;
					}
				}
				else if( createShortcutImage( text,
					po, start->m_line, start->m_pos + start->m_len,
					it, {}, false ) )
				{
					return it;
				}
			}
			else
			{
				std::tie( text, it ) = checkForLinkLabel( start, last, po );

				if( it != start && !toSingleLine< Trait >( text ).simplified().isEmpty() )
				{
					if( createShortcutImage( text,
						po, start->m_line, start->m_pos + start->m_len,
						it, {}, false ) )
					{
						return it;
					}
				}
			}
		}
		// Shortcut
		else
		{
			std::tie( text, it ) = checkForLinkLabel( start, last, po );

			if( it != start && !toSingleLine< Trait >( text ).simplified().isEmpty() )
			{
				if( createShortcutImage( text,
					po, start->m_line, start->m_pos + start->m_len,
					it, {}, false ) )
				{
					return it;
				}
			}
		}
	}
	else if( !po.collectRefLinks )
		makeText( start->m_line, start->m_pos + start->m_len, po );

	return start;
}

template< class Trait >
inline typename Delims< Trait >::const_iterator
checkForLink( typename Delims< Trait >::const_iterator it,
	typename Delims< Trait >::const_iterator last,
	TextParsingOpts< Trait > & po )
{
	const auto start = it;

	typename MdBlock< Trait >::Data text;

	const auto wasRefLink = po.wasRefLink;
	po.wasRefLink = false;

	const auto ns = skipSpaces< Trait >( 0, po.fr.data.at( po.line ).first.asString() );

	std::tie( text, it ) = checkForLinkText( it, last, po );

	if( it != start )
	{
		// Footnote reference.
		if( text.front().first.asString().startsWith( '^' ) )
		{
			if( !po.collectRefLinks )
			{
				std::shared_ptr< FootnoteRef< Trait > > fnr(
					new FootnoteRef< Trait >( "#" +
						toSingleLine< Trait >( text ).simplified().toCaseFolded().toUpper() +
						"/" + po.workingPath + po.fileName ) );

				po.parent->appendItem( fnr );
			}

			po.line = it->m_line;
			po.pos = it->m_pos + it->m_len;

			return it;
		}
		else if( it->m_pos + it->m_len < po.fr.data.at( it->m_line ).first.length() )
		{
			// Reference definition -> :
			if( po.fr.data.at( it->m_line ).first[ it->m_pos + it->m_len ] ==
				typename Trait::Char( ':' ) )
			{
				// Reference definitions allowed only at start of paragraph.
				if( ( po.line == 0 || wasRefLink ) && ns < 4 && start->m_pos == ns )
				{
					typename Trait::String url, title;
					typename Delims< Trait >::const_iterator iit;
					bool ok;

					std::tie( text, it ) = checkForLinkLabel( start, last, po );

					if( it != start && !toSingleLine< Trait >( text ).simplified().isEmpty() )
					{
						std::tie( url, title, iit, ok ) = checkForRefLink( it, last, po );

						if( ok )
						{
							const auto label = "#" +
								toSingleLine< Trait >( text ).simplified().toCaseFolded().toUpper() +
								"/" + po.workingPath + po.fileName;

							std::shared_ptr< Link< Trait > > link( new Link< Trait > );

							url = removeBackslashes< Trait >(
								replaceEntity< Trait >( url ) ).asString();

							if( !url.isEmpty() )
							{
								if( Trait::fileExists( url, po.workingPath ) )
									url = Trait::absoluteFilePath( po.workingPath + url );
							}

							link->setUrl( url );

							po.wasRefLink = true;

							if( po.doc->labeledLinks().find( label ) == po.doc->labeledLinks().cend() )
								po.doc->insertLabeledLink( label, link );

							return iit;
						}
						else
						{
							if( !po.collectRefLinks )
								makeText( start->m_line, start->m_pos + start->m_len, po );

							return start;
						}
					}
					else
					{
						if( !po.collectRefLinks )
							makeText( start->m_line, start->m_pos + start->m_len, po );

						return start;
					}
				}
				else if( !po.collectRefLinks )
				{
					makeText( start->m_line, start->m_pos + start->m_len, po );

					return start;
				}
			}
			// Inline -> (
			else if( po.fr.data.at( it->m_line ).first[ it->m_pos + it->m_len ] ==
				typename Trait::Char( '(' ) )
			{
				typename Trait::String url, title;
				typename Delims< Trait >::const_iterator iit;
				bool ok;

				std::tie( url, title, iit, ok ) = checkForInlineLink( std::next( it ), last, po );

				if( ok )
				{
					const auto link = makeLink( url, removeBackslashes< Trait >( text ), po, false,
						start->m_line, start->m_pos + start->m_len );

					if( link.get() )
					{
						if( !po.collectRefLinks )
							po.parent->appendItem( link );

						po.line = iit->m_line;
						po.pos = iit->m_pos + iit->m_len;

						return iit;
					}
					else
						return start;
				}
				else if( createShortcutLink( text,
							po, start->m_line, start->m_pos + start->m_len,
							it, {}, false ) )
				{
					return it;
				}
			}
			// Reference -> [
			else if( po.fr.data.at( it->m_line ).first[ it->m_pos + it->m_len ] ==
				typename Trait::Char( '[' ) )
			{
				typename MdBlock< Trait >::Data label;
				typename Delims< Trait >::const_iterator lit;

				std::tie( label, lit ) = checkForLinkLabel( std::next( it ), last, po );

				const auto isLabelEmpty = toSingleLine< Trait >( label ).simplified().isEmpty();

				if( lit != std::next( it ) )
				{
					if( !isLabelEmpty && createShortcutLink( label,
							po, start->m_line, start->m_pos + start->m_len,
							lit, text, true ) )
					{
						return lit;
					}
					else if( isLabelEmpty && createShortcutLink( text,
							po, start->m_line, start->m_pos + start->m_len,
							it, {}, false ) )
					{
						po.line = lit->m_line;
						po.pos = lit->m_pos + lit->m_len;

						return lit;
					}
				}
				else if( createShortcutLink( text,
							po, start->m_line, start->m_pos + start->m_len,
							it, {}, false ) )
				{
					return it;
				}
			}
			// Shortcut
			else
			{
				std::tie( text, it ) = checkForLinkLabel( start, last, po );

				if( it != start && !toSingleLine< Trait >( text ).simplified().isEmpty() )
				{
					if( createShortcutLink( text,
							po, start->m_line, start->m_pos + start->m_len,
							it, {}, false ) )
					{
						return it;
					}
				}
			}
		}
		// Shortcut
		else
		{
			std::tie( text, it ) = checkForLinkLabel( start, last, po );

			if( it != start && !toSingleLine< Trait >( text ).simplified().isEmpty() )
			{
				if( createShortcutLink( text,
						po, start->m_line, start->m_pos + start->m_len,
						it, {}, false ) )
				{
					return it;
				}
			}
		}
	}
	else if( !po.collectRefLinks )
		makeText( start->m_line, start->m_pos + start->m_len, po );

	return start;
}

inline bool
isClosingStyle( const std::vector< std::pair< Style, long long int > > & styles, Style s )
{
	const auto it = std::find_if( styles.cbegin(), styles.cend(),
		[&] ( const auto & p ) { return ( p.first == s ); } );

	return it != styles.cend();
}

inline void
closeStyle( std::vector< std::pair< Style, long long int > > & styles, Style s )
{
	const auto it = std::find_if( styles.crbegin(), styles.crend(),
		[&] ( const auto & p ) { return ( p.first == s ); } );

	if( it != styles.crend() )
		styles.erase( it.base() - 1 );
}

inline void
setStyle( int & opts, Style s, bool on )
{
	switch( s )
	{
		case Style::Strikethrough :
			( on ? opts |= StrikethroughText : opts &= ~StrikethroughText );
			break;

		case Style::Italic1 :
		case Style::Italic2 :
			( on ? opts |= ItalicText : opts &= ~ItalicText );
			break;

		case Style::Bold1 :
		case Style::Bold2 :
			( on ? opts |= BoldText : opts &= ~BoldText );
			break;

		default :
			break;
	}
}

inline void
appendPossibleDelimiter( std::vector< std::vector< std::pair< long long int, int > > > & vars,
	long long int len, int type )
{
	for( auto & v : vars )
		v.push_back( { len, type } );
}

inline std::vector< std::pair< long long int, int > >
longestSequenceWithMoreOpeningsAtStart(
	const std::vector< std::vector< std::pair< long long int, int > > > & vars )
{
	size_t max = 0;

	for( const auto & s : vars )
	{
		if( s.size() > max )
			max = s.size();
	}

	std::vector< std::pair< long long int, int > > ret;

	size_t maxOp = 0;

	for( const auto & s : vars )
	{
		if( s.size() == max )
		{
			size_t op = 0;

			for( const auto & v : s )
			{
				if( v.first > 0 )
					++op;
				else
					break;
			}

			if( op > maxOp )
			{
				maxOp = op;
				ret = s;
			}
		}
	}

	return ret;
}

inline std::vector< std::vector< std::pair< long long int, int > > >
closedSequences( const std::vector< std::vector< std::pair< long long int, int > > > & vars,
	size_t idx )
{
	std::vector< std::vector< std::pair< long long int, int > > > tmp;

	const auto longest = longestSequenceWithMoreOpeningsAtStart( vars );

	for( const auto & v : vars )
	{
		if( longest.size() == v.size() )
		{
			bool closed = false;
			std::tie( closed, std::ignore ) = checkEmphasisSequence( v, idx );

			if( closed )
				tmp.push_back( v );
		}
	}

	return tmp;
}

inline void
collectDelimiterVariants( std::vector< std::vector< std::pair< long long int, int > > > & vars,
	long long int itLength, int type, bool leftFlanking, bool rightFlanking )
{
	{
		auto vars1 = vars;
		auto vars2 = vars;

		vars.clear();

		if( leftFlanking )
		{
			appendPossibleDelimiter( vars1, itLength, type );
			std::copy( vars1.cbegin(), vars1.cend(), std::back_inserter( vars ) );
		}

		if( rightFlanking )
		{
			appendPossibleDelimiter( vars2, -itLength, type );
			std::copy( vars2.cbegin(), vars2.cend(), std::back_inserter( vars ) );
		}
	}
}

inline void
createStyles( std::vector< std::pair< Style, long long int > > & s, long long int l,
	Delimiter::DelimiterType t, long long int & count )
{
	if( t != Delimiter::Strikethrough )
	{
		if( l % 2 == 1 )
		{
			s.push_back( { t == Delimiter::Emphasis1 ?
				Style::Italic1 : Style::Italic2, 1 } );
			++count;
		}

		if( l >= 2 )
		{
			s.push_back( { t == Delimiter::Emphasis1 ?
				Style::Bold1 : Style::Bold2, l / 2 } );
			count += ( l / 2 ) * 2;
		}
	}
	else
	{
		s.push_back( { Style::Strikethrough, l / 2 } );
		count += l / 2;
	}
}

inline std::vector< std::pair< Style, long long int > >
createStyles( const std::vector< std::pair< long long int, int > > & s, size_t i,
	Delimiter::DelimiterType t, long long int & count )
{
	std::vector< std::pair< Style, long long int > > styles;

	const size_t idx = i;
	long long int len = s.at( i ).first;

	size_t closeIdx = 0;
	std::tie( std::ignore, closeIdx ) = checkEmphasisSequence( s, i );

	for( i = closeIdx; ; --i )
	{
		if( s[ i ].second == s[ idx ].second && s[ i ].first < 0 )
		{
			auto l = std::abs( s[ i ].first );

			createStyles( styles, std::min( l, len ), t, count );

			len -= std::min( l, len );

			if( !len )
				break;
		}

		if( i == 0 )
			break;
	}

	return styles;
}

template< class Trait >
inline bool
isSequence( typename Delims< Trait >::const_iterator it, long long int itLine, long long int itPos,
	Delimiter::DelimiterType t )
{
	return ( itLine == it->m_line &&
		itPos + ( it->m_type != Delimiter::Strikethrough ? 1 : 2 ) == it->m_pos &&
		it->m_type == t );
}

template< class Trait >
inline typename Delims< Trait >::const_iterator
readSequence( typename Delims< Trait >::const_iterator it,
	typename Delims< Trait >::const_iterator last,
	long long int & line, long long int & pos, long long int & len,
	typename Delims< Trait >::const_iterator & current )
{
	line = it->m_line;
	pos = it->m_pos;
	len = ( it->m_type != Delimiter::Strikethrough ? 1 : 2 );
	current = it;
	const auto t = it->m_type;

	it = std::next( it );

	while( it != last && isSequence< Trait >( it, line, pos, t ) )
	{
		current = it;

		if( it->m_type != Delimiter::Strikethrough )
		{
			++pos;
			++len;
		}
		else
		{
			pos += 2;
			len += 2;
		}

		++it;
	}

	return std::prev( it );
}

inline int
emphasisToInt( Delimiter::DelimiterType t )
{
	switch( t )
	{
		case Delimiter::Strikethrough :
			return 0;

		case Delimiter::Emphasis1 :
			return 1;

		case Delimiter::Emphasis2 :
			return 2;

		default :
			return -1;
	}
}

template< class Trait >
inline std::tuple< bool, std::vector< std::pair< Style, long long int > >, long long int, long long int >
isStyleClosed( typename Delims< Trait >::const_iterator it,
	typename Delims< Trait >::const_iterator last,
	TextParsingOpts< Trait > & po )
{
	const auto open = it;
	auto current =  it;

	std::vector< std::vector< std::pair< long long int, int > > > vars, closed;
	vars.push_back( {} );

	long long int itLine = open->m_line, itPos = open->m_pos,
		itLength = ( open->m_type != Delimiter::Strikethrough ? 1 : 2 );

	const long long int line = po.line, pos = po.pos;
	const bool collectRefLinks = po.collectRefLinks;

	po.collectRefLinks = true;

	bool first = true;

	{
		const auto c = std::count_if( po.styles.cbegin(), po.styles.cend(),
			[] ( const auto & p ) { return ( p.first == Style::Strikethrough ); } );

		if( c )
			vars.front().push_back( { c * 2, 0 } );
	}

	{
		{
			const auto c1 = std::count_if( po.styles.cbegin(), po.styles.cend(),
				[&] ( const auto & p ) { return ( p.first == Style::Italic1 ); } );

			if( c1 )
				vars.front().push_back( { c1, 1 } );

			const auto c2 = std::count_if( po.styles.cbegin(), po.styles.cend(),
				[&] ( const auto & p ) { return ( p.first == Style::Bold1 ); } ) * 2;

			if( c2 )
				vars.front().push_back( { c2, 1 } );
		}

		{
			const auto c1 = std::count_if( po.styles.cbegin(), po.styles.cend(),
				[&] ( const auto & p ) { return ( p.first == Style::Italic2 ); } );

			if( c1 )
				vars.front().push_back( { c1, 2 } );

			const auto c2 = std::count_if( po.styles.cbegin(), po.styles.cend(),
				[&] ( const auto & p ) { return ( p.first == Style::Bold2 ); } ) * 2;

			if( c2 )
				vars.front().push_back( { c2, 2 } );
		}
	}

	const auto idx = vars.front().size();

	for( ; it != last; ++it )
	{
		switch( it->m_type )
		{
			case Delimiter::SquareBracketsOpen :
				it = checkForLink( it, last, po );
				break;

			case Delimiter::ImageOpen :
				it = checkForImage( it, last, po );
				break;

			case Delimiter::Less :
				it = checkForAutolinkHtml( it, last, po, false );
				break;

			case Delimiter::Strikethrough :
			case Delimiter::Emphasis1 :
			case Delimiter::Emphasis2 :
			{
				it = readSequence< Trait >( it, last, itLine, itPos, itLength, current );

				if( first )
				{
					vars.front().push_back( { itLength, emphasisToInt( open->m_type ) } );
					first = false;
				}
				else
				{
					collectDelimiterVariants( vars, itLength, emphasisToInt( it->m_type ),
						it->m_leftFlanking, it->m_rightFlanking );
				}
			}
				break;

			case Delimiter::InlineCode :
				it = checkForInlineCode( it, last, po );
				break;

			default :
				break;
		}
	}

	po.line = line;
	po.pos = pos;
	po.collectRefLinks = collectRefLinks;

	closed = closedSequences( vars, idx );

	if( !closed.empty() )
	{
		long long int itCount = 0;

		return { true, createStyles(
			longestSequenceWithMoreOpeningsAtStart( closed ), idx, open->m_type, itCount ),
			vars.front().at( idx ).first, itCount };
	}
	else
		return { false, { { Style::Unknown, 0 } },
			( open->m_type != Delimiter::Strikethrough ? 1 : 2 ), 1 };
}

template< class Trait >
inline typename Delims< Trait >::const_iterator
checkForStyle( typename Delims< Trait >::const_iterator first,
	typename Delims< Trait >::const_iterator it,
	typename Delims< Trait >::const_iterator last, TextParsingOpts< Trait > & po )
{
	long long int count = 1;

	po.wasRefLink = false;

	if( it->m_rightFlanking )
	{
		long long int line = it->m_line, pos = it->m_pos + it->m_len, ppos = it->m_pos;
		const auto t = it->m_type;
		long long int len = it->m_len;

		for( auto j = std::next( it ); j != last; ++j )
		{
			if( j->m_line == line && pos == j->m_pos && j->m_type == t )
			{
				len += j->m_len;
				pos = j->m_pos + j->m_len;
				++count;
			}
			else
				break;
		}

		if( it != first )
		{
			for( auto j = std::prev( it ); ; --j )
			{
				if( j->m_line == line && ppos - j->m_len == j->m_pos && j->m_type == t )
				{
					len += j->m_len;
					ppos = j->m_pos;
					++count;
				}
				else
					break;

				if( j == first )
					break;
			}
		}

		long long int opened = 0;

		for( auto it = po.styles.crbegin(), last = po.styles.crend(); it != last; ++it )
		{
			bool doBreak = false;

			switch( t )
			{
				case Delimiter::Emphasis1 :
				{
					if( it->first == Style::Italic1 || it->first == Style::Bold1 )
					{
						opened = it->second;
						doBreak = true;
					}
				}
					break;

				case Delimiter::Emphasis2 :
				{
					if( it->first == Style::Italic2 || it->first == Style::Bold2 )
					{
						opened = it->second;
						doBreak = true;
					}
				}
					break;

				case Delimiter::Strikethrough :
				{
					if( it->first == Style::Strikethrough )
					{
						opened = it->second;
						doBreak = true;
					}
				}
					break;

				default :
					break;
			}

			if( doBreak )
				break;
		}

		const bool sumMult3 = ( it->m_leftFlanking ? ( ( opened + len ) % 3 == 0 ) : false );

		if( count && opened && ( !sumMult3 || ( count % 3 == 0 && opened % 3 == 0 ) ) )
		{
			if( count > opened )
				count = opened;

			if( it->m_type == Delimiter::Strikethrough )
			{
				for( auto i = 0; i < count; ++i )
					closeStyle( po.styles, Style::Strikethrough );

				if( std::find_if( po.styles.cbegin(), po.styles.cend(),
					[] ( const auto & p ) { return ( p.first == Style::Strikethrough ); } ) ==
						po.styles.cend() )
				{
					setStyle( po.opts, Style::Strikethrough, false );
				}
			}
			else
			{
				if( count % 2 == 1 )
				{
					const auto st = ( it->m_type == Delimiter::Emphasis1 ?
						Style::Italic1 : Style::Italic2 );

					closeStyle( po.styles, st );

					if( std::find_if( po.styles.cbegin(), po.styles.cend(),
						[&] ( const auto & p ) { return ( p.first == st ); } ) == po.styles.cend() )
							setStyle( po.opts, st, false );
				}

				if( count >= 2 )
				{
					const auto st = ( it->m_type == Delimiter::Emphasis1 ?
						Style::Bold1 : Style::Bold2 );

					for( auto i = 0; i < count / 2; ++i )
						closeStyle( po.styles, st );

					if( std::find_if( po.styles.cbegin(), po.styles.cend(),
						[&] ( const auto & p ) { return ( p.first == st ); } ) == po.styles.cend() )
							setStyle( po.opts, st, false );
				}
			}

			const auto j = it + ( count - 1 );

			po.pos = j->m_pos + j->m_len;
			po.line = j->m_line;

			return it + ( count - 1 );
		}
	}

	count = 1;

	if( it->m_leftFlanking )
	{
		switch( it->m_type )
		{
			case Delimiter::Strikethrough :
			case Delimiter::Emphasis1 :
			case Delimiter::Emphasis2 :
			{
				bool closed = false;
				std::vector< std::pair< Style, long long int > > styles;
				long long int len = 0;

				std::tie( closed, styles, len, count ) = isStyleClosed( it, last, po );

				if( closed )
				{
					for( const auto & p : styles )
					{
						setStyle( po.opts, p.first, true );

						for( long long int i = 0; i < p.second; ++i )
							po.styles.push_back( { p.first, len } );
					}

					po.pos = it->m_pos + len;
					po.line = it->m_line;
				}
				else if( !po.collectRefLinks )
					makeText( it->m_line, it->m_pos + len, po );
			}
				break;

			default :
			{
				if( !po.collectRefLinks )
					makeText( it->m_line, it->m_pos + it->m_len, po );
			}
				break;
		}
	}

	if( !count )
		count = 1;

	po.html.html.reset();
	po.html.htmlBlockType = -1;
	po.html.continueHtml = false;
	po.html.onLine = false;

	return it + ( count - 1 );
}

template< class Trait >
inline std::shared_ptr< Text< Trait > >
concatenateText( typename Block< Trait >::Items::const_iterator it,
	typename Block< Trait >::Items::const_iterator last )
{
	std::shared_ptr< Text< Trait > > t( new Text< Trait > );
	t->setOpts( std::static_pointer_cast< Text< Trait > >( *it )->opts() );
	t->setSpaceBefore( std::static_pointer_cast< Text< Trait > >( *it )->isSpaceBefore() );

	typename Trait::String data;

	for( ; it != last; ++it )
	{
		const auto tt = std::static_pointer_cast< Text< Trait > >( *it );

		if( tt->isSpaceBefore() )
			data.push_back( typename Trait::Char( ' ' ) );

		data.push_back( tt->text() );

		if( tt->isSpaceAfter() )
			data.push_back( typename Trait::Char( ' ' ) );
	}

	t->setText( data.simplified() );

	t->setSpaceAfter( std::static_pointer_cast< Text< Trait > >( *std::prev( it ) )->isSpaceAfter() );

	return t;
}

template< class Trait >
inline void
optimizeParagraph( std::shared_ptr< Paragraph< Trait > > & p )
{
	std::shared_ptr< Paragraph< Trait > > np( new Paragraph< Trait > );

	int opts = TextWithoutFormat;

	auto start = p->items().cend();

	for( auto it = p->items().cbegin(), last = p->items().cend(); it != last; ++it )
	{
		if( (*it)->type() == ItemType::Text )
		{
			const auto t = std::static_pointer_cast< Text< Trait > >( *it );

			if( start == last )
			{
				start = it;
				opts = t->opts();
			}
			else if( opts != t->opts() )
			{
				np->appendItem( concatenateText< Trait >( start, it ) );
				start = it;
				opts = t->opts();
			}
		}
		else
		{
			if( start != last )
			{
				np->appendItem( concatenateText< Trait >( start, it ) );
				start = last;
				opts = TextWithoutFormat;
			}

			np->appendItem( (*it) );
		}
	}

	if( start != p->items().cend() )
		np->appendItem( concatenateText< Trait >( start, p->items().cend() ) );

	p = np;
}

template< class Trait >
inline void
parseFormattedText( MdBlock< Trait > & fr,
	std::shared_ptr< Block< Trait > > parent,
	std::shared_ptr< Document< Trait > > doc,
	typename Trait::StringList & linksToParse,
	const typename Trait::String & workingPath,
	const typename Trait::String & fileName, bool collectRefLinks, bool ignoreLineBreak,
	RawHtmlBlock< Trait > & html )

{
	if( fr.data.empty() )
		return;

	std::shared_ptr< Paragraph< Trait > > p( new Paragraph< Trait > );

	const auto delims = collectDelimiters< Trait >( fr.data );

	TextParsingOpts< Trait > po = { fr, p, doc, linksToParse, workingPath,
		fileName, collectRefLinks, ignoreLineBreak, html };

	if( !delims.empty() )
	{
		for( auto it = delims.cbegin(), last = delims.cend(); it != last; ++it )
		{
			if( html.html.get() && html.continueHtml )
				it = finishRawHtmlTag( it, last, po, false );
			else
			{
				if( html.html.get() )
				{
					if( !collectRefLinks )
						p->appendItem( html.html );

					html.html.reset();
					html.htmlBlockType = -1;
					html.continueHtml = false;
					html.onLine = false;
				}

				if( it->m_line > po.line || it->m_pos > po.pos )
				{
					if( !collectRefLinks )
						makeText( it->m_line, it->m_pos, po );
					else
					{
						po.line = it->m_line;
						po.pos = it->m_pos;
					}
				}

				switch( it->m_type )
				{
					case Delimiter::SquareBracketsOpen :
						it = checkForLink( it, last, po );
						break;

					case Delimiter::ImageOpen :
						it = checkForImage( it, last, po );
						break;

					case Delimiter::Less :
						it = checkForAutolinkHtml( it, last, po, true );
						break;

					case Delimiter::Strikethrough :
					case Delimiter::Emphasis1 :
					case Delimiter::Emphasis2 :
							it = checkForStyle( delims.cbegin(), it, last, po );
						break;

					case Delimiter::Math :
						it = checkForMath( it, last, po );

					case Delimiter::InlineCode :
					{
						if( !it->m_backslashed )
							it = checkForInlineCode( it, last, po );
					}
						break;

					case Delimiter::HorizontalLine :
					{
						if( !collectRefLinks )
						{
							if( !p->isEmpty() )
							{
								optimizeParagraph< Trait >( p );
								parent->appendItem( p );
							}

							std::shared_ptr< Item< Trait > > hr( new HorizontalLine< Trait > );
							parent->appendItem( hr );

							p.reset( new Paragraph< Trait > );

							po.parent = p;
							po.line = it->m_line;
							po.pos = it->m_pos + it->m_len;
						}
					}
						break;

					default :
					{
						if( !collectRefLinks )
							makeText( it->m_line, it->m_pos + it->m_len, po );
						else
						{
							po.line = it->m_line;
							po.pos = it->m_pos + it->m_len;
						}
					}
						break;
				}
			}
		}
	}
	else
	{
		if( html.html.get() && html.continueHtml )
			finishRawHtmlTag( delims.cend(), delims.cend(), po, false );
	}

	if( html.html.get() && !html.continueHtml && html.onLine )
	{
		if( !collectRefLinks )
			p->appendItem( html.html );

		html.html.reset();
		html.htmlBlockType = -1;
		html.continueHtml = false;
		html.onLine = false;
	}

	if( !collectRefLinks )
		makeText( fr.data.size() - 1, fr.data.back().first.length(), po );

	if( !p->isEmpty() )
	{
		optimizeParagraph< Trait >( p );

		if( html.html.get() && !html.onLine )
			UnprotectedDocsMethods< Trait >::setDirty( p );

		parent->appendItem( p );
	}
}

template< class Trait >
inline void
Parser< Trait >::parseFormattedTextLinksImages( MdBlock< Trait > & fr,
	std::shared_ptr< Block< Trait > > parent,
	std::shared_ptr< Document< Trait > > doc,
	typename Trait::StringList & linksToParse,
	const typename Trait::String & workingPath,
	const typename Trait::String & fileName, bool collectRefLinks, bool ignoreLineBreak,
	RawHtmlBlock< Trait > & html )

{
	parseFormattedText( fr, parent, doc, linksToParse, workingPath, fileName,
		collectRefLinks, ignoreLineBreak, html );
}

template< class Trait >
inline void
Parser< Trait >::parseFootnote( MdBlock< Trait > & fr,
	std::shared_ptr< Block< Trait > >,
	std::shared_ptr< Document< Trait > > doc,
	typename Trait::StringList & linksToParse,
	const typename Trait::String & workingPath,
	const typename Trait::String & fileName,
	bool collectRefLinks )
{
	if( !fr.data.empty() )
	{
		std::shared_ptr< Footnote< Trait > > f( new Footnote< Trait > );

		const auto delims = collectDelimiters< Trait >( fr.data );

		RawHtmlBlock< Trait > html;

		TextParsingOpts< Trait > po = { fr, f, doc, linksToParse, workingPath,
			fileName, collectRefLinks, false, html };

		if( !delims.empty() && delims.cbegin()->m_type == Delimiter::SquareBracketsOpen &&
			!delims.cbegin()->m_isWordBefore )
		{
			typename MdBlock< Trait >::Data id;
			typename Delims< Trait >::const_iterator it = delims.cend();

			po.line = delims.cbegin()->m_line;
			po.pos = delims.cbegin()->m_pos;

			std::tie( id, it ) = checkForLinkText( delims.cbegin(), delims.cend(), po );

			if( !toSingleLine< Trait > ( id ).simplified().isEmpty() &&
				id.front().first.asString().startsWith( '^' ) &&
				it != delims.cend() &&
				fr.data.at( it->m_line ).first.length() > it->m_pos + 1 &&
				fr.data.at( it->m_line ).first[ it->m_pos + 1 ] == typename Trait::Char( ':' ) )
			{
				{
					typename MdBlock< Trait >::Data tmp;
					std::copy( fr.data.cbegin() + it->m_line, fr.data.cend(),
						std::back_inserter( tmp ) );
					fr.data = tmp;
				}

				fr.data.front().first = fr.data.front().first.sliced( it->m_pos + 2 );

				for( auto it = fr.data.begin(), last = fr.data.end(); it != last; ++it )
				{
					if( it->first.asString().startsWith( "    " ) )
						it->first = it->first.sliced( 4 );
					else if( it->first.asString().startsWith( '\t' ) )
						it->first = it->first.sliced( 1 );
				}

				StringListStream< Trait > stream( fr.data );

				parse( stream, f, doc, linksToParse, workingPath, fileName, collectRefLinks );

				if( !f->isEmpty() )
					doc->insertFootnote( "#" + toSingleLine< Trait > ( id ).simplified() +
						"/" + workingPath + fileName, f );
			}
		}
	}
}

template< class Trait >
inline void
Parser< Trait >::parseBlockquote( MdBlock< Trait > & fr,
	std::shared_ptr< Block< Trait > > parent,
	std::shared_ptr< Document< Trait > > doc,
	typename Trait::StringList & linksToParse,
	const typename Trait::String & workingPath,
	const typename Trait::String & fileName,
	bool collectRefLinks, RawHtmlBlock< Trait > & html )
{
	const int pos = fr.data.front().first.asString().indexOf( '>' );

	if( pos > -1 )
	{
		long long int i = 0, j = 0;

		BlockType bt = BlockType::Unknown;

		for( auto it = fr.data.begin(), last = fr.data.end(); it != last; ++it, ++i )
		{
			const auto ns = skipSpaces< Trait >( 0, it->first.asString() );
			const auto gt = ( ns < it->first.length() ?
				( it->first[ ns ] == typename Trait::Char( '>' ) ? ns : -1 ) : -1 );

			if( gt > -1 )
			{
				it->first = it->first.sliced( gt + ( it->first.length() > gt + 1 ?
					( it->first[ gt + 1 ] == typename Trait::Char( ' ' ) ? 1 : 0 ) : 0 ) + 1 );
				bt = whatIsTheLine( it->first );
			}
			// Process lazyness...
			else
			{
				if( ns < 4 && isHorizontalLine< Trait >( it->first.asString().sliced( ns ) ) )
					break;

				const auto tmpBt = whatIsTheLine( it->first );

				if( bt == BlockType::Text )
				{
					if( isH1< Trait >( it->first.asString() ) )
					{
						const auto p = it->first.asString().indexOf( '=' );

						it->first.insert( p, '\\' );

						continue;
					}
					else if( isH2< Trait >( it->first.asString() ) )
					{
						const auto p = it->first.asString().indexOf( '-' );

						it->first.insert( p, '\\' );

						continue;
					}
				}

				if( ( bt == BlockType::Text || bt == BlockType::Blockquote || bt == BlockType::List )
					&& ( tmpBt == BlockType::Text || tmpBt == BlockType::CodeIndentedBySpaces ) )
				{
					continue;
				}
				else
					break;
			}
		}

		typename MdBlock< Trait >::Data tmp;

		for( ; j < i; ++j )
			tmp.push_back( fr.data.at( j ) );

		StringListStream< Trait > stream( tmp );

		std::shared_ptr< Blockquote< Trait > > bq( new Blockquote< Trait > );

		parse( stream, bq, doc, linksToParse, workingPath, fileName, collectRefLinks );

		if( !bq->isEmpty() && !collectRefLinks )
			parent->appendItem( bq );

		if( i < (long long int) fr.data.size() )
		{
			tmp.clear();

			std::copy( fr.data.cbegin() + i, fr.data.cend(), std::back_inserter( tmp ) );

			StringListStream< Trait > stream( tmp );

			parse( stream, parent, doc, linksToParse, workingPath, fileName,
				collectRefLinks );
		}
	}
}

template< class Trait >
inline bool
isListItemAndNotNested( const typename Trait::String & s, long long int indent )
{
	long long int p = skipSpaces< Trait >( 0, s );

	if( p >= indent || p == s.size() )
		return false;

	bool space = false;

	if( p + 1 >= s.size() )
		space = true;
	else
		space = s[ p + 1 ].isSpace();

	if( p < 4 )
	{
		if( s[ p ] == typename Trait::Char( '*' ) && space )
			return true;
		else if( s[ p ] == typename Trait::Char( '-' ) && space )
			return true;
		else if( s[ p ] == typename Trait::Char( '+' ) && space )
			return true;
		else
			return isOrderedList< Trait >( s );
	}
	else
		return false;
}

template< class Trait >
inline std::pair< long long int, long long int >
calculateIndent( const typename Trait::String & s, long long int p )
{
	return { 0, skipSpaces< Trait >( p, s ) };
}

template< class Trait >
inline std::tuple< bool, long long int, typename Trait::Char >
listItemData( const typename Trait::String & s )
{
	long long int p = skipSpaces< Trait >( 0, s );

	if( p == s.size() )
		return { false, 0, typename Trait::Char() };

	bool space = false;

	if( p + 1 >= s.size() )
		space = true;
	else
		space = s[ p + 1 ].isSpace();

	if( p < 4 )
	{
		if( s[ p ] == typename Trait::Char( '*' ) && space )
			return { true, p + 2, '*' };
		else if( s[ p ] == typename Trait::Char( '-' ) && space )
			return { true, p + 2, '-' };
		else if( s[ p ] == typename Trait::Char( '+' ) && space )
			return { true, p + 2, '+' };
		else
		{
			int d = 0, l = 0;
			typename Trait::Char c;

			if( isOrderedList< Trait >( s, &d, &l, &c ) )
				return { true, p + l + 2, c };
			else
				return { false, 0, typename Trait::Char() };
		}
	}
	else
		return { false, 0, typename Trait::Char() };
}

template< class Trait >
inline void
Parser< Trait >::parseList( MdBlock< Trait > & fr,
	std::shared_ptr< Block< Trait > > parent,
	std::shared_ptr< Document< Trait > > doc,
	typename Trait::StringList & linksToParse,
	const typename Trait::String & workingPath,
	const typename Trait::String & fileName,
	bool collectRefLinks, RawHtmlBlock< Trait > & html )
{
	for( auto it = fr.data.begin(), last = fr.data.end(); it != last; ++it )
		it->first.replace( typename Trait::Char( '\t' ), typename Trait::String( "    " ) );

	const auto p = skipSpaces< Trait >( 0, fr.data.front().first.asString() );

	if( p != fr.data.front().first.length() )
	{
		std::shared_ptr< List< Trait > > list( new List< Trait > );

		typename MdBlock< Trait >::Data listItem;
		auto it = fr.data.begin();
		listItem.push_back( *it );
		++it;

		bool ok = false;
		long long int indent = 0;
		typename Trait::Char marker;

		std::tie( ok, indent, marker ) = listItemData< Trait >( listItem.front().first.asString() );

		bool updateIndent = false;

		for( auto last = fr.data.end(); it != last; ++it )
		{
			if( updateIndent )
			{
				std::tie( ok, indent, marker ) = listItemData< Trait >( it->first.asString() );
				updateIndent = false;
			}

			const auto ns = skipSpaces< Trait >( 0, it->first.asString() );

			if( isHorizontalLine< Trait >( it->first.asString().sliced( ns ) ) && !listItem.empty() &&
				( ns == indent ? !isH2< Trait >( it->first.asString().sliced( ns ) ) : true ) )
			{
				updateIndent = true;

				MdBlock< Trait > block = { listItem, 0 };

				parseListItem( block, list, doc, linksToParse, workingPath, fileName,
					collectRefLinks, html );
				listItem.clear();

				if( !list->isEmpty() )
					parent->appendItem( list );

				list.reset( new List< Trait > );

				if( !collectRefLinks )
					doc->appendItem( std::shared_ptr< Item< Trait > > ( new HorizontalLine< Trait > ) );

				continue;
			}
			else if( isListItemAndNotNested< Trait >( it->first.asString(), indent ) && !listItem.empty() )
			{
				typename Trait::Char tmpMarker;
				std::tie( ok, indent, tmpMarker ) = listItemData< Trait >( it->first.asString() );

				MdBlock< Trait > block = { listItem, 0 };

				parseListItem( block, list, doc, linksToParse, workingPath, fileName,
					collectRefLinks, html );
				listItem.clear();

				if( tmpMarker != marker )
				{
					if( !list->isEmpty() )
						parent->appendItem( list );

					list.reset( new List< Trait > );

					marker = tmpMarker;
				}
			}

			listItem.push_back( *it );
		}

		if( !listItem.empty() )
		{
			MdBlock< Trait > block = { listItem, 0 };
			parseListItem( block, list, doc, linksToParse, workingPath, fileName,
				collectRefLinks, html );
		}

		if( !list->isEmpty() )
			parent->appendItem( list );
	}
}

template< class Trait >
inline void
Parser< Trait >::parseListItem( MdBlock< Trait > & fr,
	std::shared_ptr< Block< Trait > > parent,
	std::shared_ptr< Document< Trait > > doc,
	typename Trait::StringList & linksToParse,
	const typename Trait::String & workingPath,
	const typename Trait::String & fileName,
	bool collectRefLinks, RawHtmlBlock< Trait > & html )
{
	std::shared_ptr< ListItem< Trait > > item( new ListItem< Trait > );

	int i = 0;

	if( isOrderedList< Trait >( fr.data.front().first.asString(), &i ) )
	{
		item->setListType( ListItem< Trait >::Ordered );
		item->setStartNumber( i );
	}
	else
		item->setListType( ListItem< Trait >::Unordered );

	if( item->listType() == ListItem< Trait >::Ordered )
		item->setOrderedListPreState( i == 1 ?
			ListItem< Trait >::Start : ListItem< Trait >::Continue );

	typename MdBlock< Trait >::Data data;

	auto it = fr.data.begin();
	++it;

	int pos = 1;

	long long int indent = 0;
	bool ok = false;

	std::tie( ok, indent, std::ignore ) = listItemData< Trait >( fr.data.front().first.asString() );

	const auto firstNonSpacePos = calculateIndent< Trait >(
		fr.data.front().first.asString(), indent ).second;
	if( firstNonSpacePos - indent < 4 ) indent = firstNonSpacePos;

	if( indent < fr.data.front().first.length() )
		data.push_back( { fr.data.front().first.right( fr.data.front().first.length() - indent ),
			fr.data.front().second } );

	bool taskList = false;
	bool checked = false;

	if( !data.empty() )
	{
		auto p = skipSpaces< Trait >( 0, data.front().first.asString() );

		if( p < data.front().first.length() )
		{
			if( data.front().first[ p ] == typename Trait::Char( '[' ) )
			{
				++p;

				if( p < data.front().first.length() )
				{
					if( data.front().first[ p ] == typename Trait::Char( ' ' ) ||
						data.front().first[ p ].toLower() == typename Trait::Char( 'x' ) )
					{
						if( data.front().first[ p ].toLower() == typename Trait::Char( 'x' ) )
							checked = true;

						++p;

						if( p < data.front().first.length() )
						{
							if( data.front().first[ p ] == typename Trait::Char( ']' ) )
							{
								taskList = true;

								data[ 0 ].first = data[ 0 ].first.sliced( p + 1 );
							}
						}
					}
				}
			}
		}
	}

	if( taskList )
	{
		item->setTaskList();
		item->setChecked( checked );
	}

	for( auto last = fr.data.end(); it != last; ++it, ++pos )
	{
		std::tie( ok, std::ignore, std::ignore ) = listItemData< Trait >( it->first.asString() );

		if( ok )
		{
			StringListStream< Trait > stream( data );

			parse( stream, item, doc, linksToParse, workingPath, fileName,
				collectRefLinks );

			data.clear();

			typename MdBlock< Trait >::Data nestedList;
			nestedList.push_back( *it );
			++it;

			for( ; it != last; ++it )
			{
				const auto ns = skipSpaces< Trait >( 0, it->first.asString() );
				std::tie( ok, std::ignore, std::ignore ) = listItemData< Trait >(
					it->first.asString() );

				if( ok || ns > indent || ns == it->first.length() )
					nestedList.push_back( *it );
				else
					break;
			}

			for( auto it = nestedList.begin(), last = nestedList.end(); it != last; ++it )
			{
				if( it->first.asString().startsWith( typename Trait::String( indent, ' ' ) ) )
					it->first = it->first.sliced( indent );
			}

			MdBlock< Trait > block = { nestedList, 0 };

			parseList( block, item, doc, linksToParse, workingPath, fileName,
				collectRefLinks, html );

			for( ; it != last; ++it )
			{
				if( it->first.asString().startsWith( typename Trait::String( indent, ' ' ) ) )
					it->first = it->first.sliced( indent );

				data.push_back( *it );
			}

			break;
		}
		else
		{
			if( it->first.asString().startsWith( typename Trait::String( indent, ' ' ) ) )
				it->first = it->first.sliced( indent );

			data.push_back( *it );
		}
	}

	if( !data.empty() )
	{
		StringListStream< Trait > stream( data );

		parse( stream, item, doc, linksToParse, workingPath, fileName, collectRefLinks );
	}

	if( !item->isEmpty() && !collectRefLinks )
		parent->appendItem( item );
}

template< class Trait >
inline void
Parser< Trait >::parseCode( MdBlock< Trait > & fr,
	std::shared_ptr< Block< Trait > > parent,
	bool collectRefLinks, int indent )
{
	if( !collectRefLinks )
	{
		const auto i = skipSpaces< Trait >( 0, fr.data.front().first.asString() );

		if( i != fr.data.front().first.length() )
			indent += i;

		typename Trait::String syntax;
		isStartOfCode< Trait >( fr.data.front().first.asString(), &syntax );
		syntax = replaceEntity< Trait >( syntax );

		fr.data.erase( fr.data.cbegin() );
		fr.data.erase( std::prev( fr.data.cend() ) );

		if( syntax.toLower() == "math" )
		{
			typename Trait::String math;
			bool first = true;

			for( const auto & l : std::as_const( fr.data ) )
			{
				if( !first )
					math.push_back( typename Trait::Char( '\n' ) );

				math.push_back( l.first.asString() );

				first = false;
			}

			if( !collectRefLinks )
			{
				std::shared_ptr< Paragraph< Trait > > p( new Paragraph< Trait > );
				std::shared_ptr< Math< Trait > > m( new Math< Trait > );
				m->setInline( false );
				m->setExpr( math );
				p->appendItem( m );

				parent->appendItem( p );
			}
		}
		else
			parseCodeIndentedBySpaces( fr, parent, collectRefLinks, indent, syntax );
	}
}

template< class Trait >
inline void
Parser< Trait >::parseCodeIndentedBySpaces( MdBlock< Trait > & fr,
	std::shared_ptr< Block< Trait > > parent,
	bool collectRefLinks, int indent,
	const typename Trait::String & syntax )
{
	if( !collectRefLinks )
	{
		typename Trait::String code;

		for( const auto & l : std::as_const( fr.data ) )
		{
			const auto ns = skipSpaces< Trait >( 0, l.first.asString() );

			code.push_back( ( indent > 0 ? l.first.asString().right( l.first.length() -
					( ns < indent ? ns : indent ) ) + '\n' :
				l.first.asString() + '\n' ) );
		}

		if( !code.isEmpty() )
			code.remove( code.length() - 1, 1 );

		std::shared_ptr< Code< Trait > > codeItem( new Code< Trait >( code ) );
		codeItem->setSyntax( syntax );
		parent->appendItem( codeItem );
	}
}

} /* namespace MD */

#endif // MD4QT_MD_PARSER_HPP_INCLUDED
