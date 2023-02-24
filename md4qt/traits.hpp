
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

#ifndef MD4QT_MD_TRAITS_HPP_INCLUDED
#define MD4QT_MD_TRAITS_HPP_INCLUDED

#ifdef MD4QT_ICU_STL_SUPPORT

// C++ include.
#include <memory>
#include <vector>
#include <map>
#include <filesystem>
#include <cctype>
#include <string>
#include <algorithm>

// ICU include.
#include <unicode/unistr.h>
#include <unicode/uchar.h>

// uriparser include.
#include <uriparser/Uri.h>

#endif // MD4QT_ICU_STL_SUPPORT


#ifdef MD4QT_QT_SUPPORT

#ifndef MD4QT_ICU_STL_SUPPORT

// C++ include.
#include <memory>
#include <map>

#endif // MD4QT_ICU_STL_SUPPORT

// Qt include.
#include <QString>
#include <QVector>
#include <QTextStream>
#include <QStringList>
#include <QFileInfo>
#include <QUrl>

#endif // MD4QT_QT_SUPPORT


namespace MD {

template< class String >
class InternalStringT {
public:
	InternalStringT() {}
	InternalStringT( const String & s )
		:	str( s )
		,	startPos( 0 )
	{}

	String & asString() { return str; }
	const String & asString() const { return str; }

	long long int virginPos( long long int pos ) const
	{
		for( auto it = changedPos.crbegin(), last = changedPos.crend(); it != last; ++it )
			pos = virginPosImpl( pos, *it );

		return startPos + pos;
	}

	InternalStringT & replace( const String & what, const String & with )
	{
		String tmp;
		bool init = false;

		for( long long int i = 0; i < str.size(); )
		{
			long long int p = str.indexOf( what, i );

			if( p != -1 )
			{
				tmp.push_back( str.sliced( i, p - i ) );
				tmp.push_back( with );

				i = p + what.size();

				if( what.size() != with.size() )
				{
					if( !init )
					{
						changedPos.push_back( {} );
						init = true;
					}

					changedPos.back().push_back( { p, what.size(), with.size() } );
				}
			}
			else
			{
				tmp.push_back( str.sliced( i ) );

				i = str.size();
			}
		}

		std::swap( str, tmp );

		return *this;
	}

	InternalStringT & remove( long long int pos, long long int size )
	{
		str.remove( pos, size );

		changedPos.push_back( {} );
		changedPos.back().push_back( { pos, size, 0 } );

		return *this;
	}

private:
	String str;

	struct ChangedPos {
		long long int pos = -1;
		long long int oldLen = -1;
		long long int len = -1;
	};

	long long int startPos = -1;
	std::vector< std::vector< ChangedPos > > changedPos;

private:
	long long int virginPosImpl( long long int pos,
		const std::vector< ChangedPos > & changed ) const
	{
		long long int p = 0;

		for( const auto & c : changed )
		{
			if( c.pos <= pos + p )
			{
				if( c.pos + std::min( c.oldLen, c.len ) <= pos + p )
				{
					if( c.oldLen < c.len )
						p -= ( c.len - c.oldLen );
					else
						p += ( c.oldLen - c.len );
				}
				else
					break;
			}
			else
				break;
		}

		return pos + p;
	}
}; // class InternalString

#ifdef MD4QT_ICU_STL_SUPPORT

//
// UnicodeChar
//

//! Wrapper for UChar32 to be used with MD::Parser.
class UnicodeChar {
public:
	UnicodeChar()
		:	m_ch( 0 )
	{
	}

	UnicodeChar( UChar32 ch )
		:	m_ch( ch )
	{
	}

	operator UChar32 () const
	{
		return m_ch;
	}

	inline bool isSpace() const
	{
		bool unicodeSpace = false;

		const auto type = u_charType( m_ch );

		switch( type )
		{
			case U_SPACE_SEPARATOR :
			case U_LINE_SEPARATOR :
			case U_PARAGRAPH_SEPARATOR :
				unicodeSpace = true;
				break;

			default :
				break;
		}

		return m_ch == 0x20 || ( m_ch <= 0x0D && m_ch >= 0x09 ) ||
			( m_ch > 127 && ( m_ch == 0x85 || m_ch == 0xA0 || unicodeSpace ) );
	}

	inline bool isDigit() const
	{
		return ( u_charType( m_ch ) == U_DECIMAL_DIGIT_NUMBER );
	}

	inline bool isNull() const
	{
		return m_ch == 0;
	}

	inline UChar32 unicode() const
	{
		return m_ch;
	}

	inline bool isLetter() const
	{
		const auto type = u_charType( m_ch );

		switch( type )
		{
			case U_UPPERCASE_LETTER :
			case U_LOWERCASE_LETTER :
			case U_TITLECASE_LETTER :
			case U_MODIFIER_LETTER :
			case U_OTHER_LETTER :
				return true;

			default :
				return false;
		}
	}

	inline bool isLetterOrNumber() const
	{
		return isLetter() || isDigit();
	}

	inline bool isPunct() const
	{
		const auto type = u_charType( m_ch );

		switch( type )
		{
			case U_DASH_PUNCTUATION :
			case U_START_PUNCTUATION :
			case U_END_PUNCTUATION :
			case U_CONNECTOR_PUNCTUATION :
			case U_OTHER_PUNCTUATION :
			case U_INITIAL_PUNCTUATION :
			case U_FINAL_PUNCTUATION :
				return true;

			default :
				return false;
		}
	}

	UnicodeChar toLower() const
	{
		return icu::UnicodeString( 1, m_ch, 1 ).toLower().char32At( 0 );
	}

	bool operator == ( const UnicodeChar & other ) const
	{
		return m_ch == other.m_ch;
	}

	bool operator != ( const UnicodeChar & other ) const
	{
		return m_ch != other.m_ch;
	}

private:
	UChar32 m_ch;
}; // class UnicodeChar


//
// UnicodeString
//

//! Wrapper for icu::UnicodeString to be used with MD::Parser.
class UnicodeString final
	:	public icu::UnicodeString
{
public:
	UnicodeString()
	{
	}

	UnicodeString( const icu::UnicodeString & str )
		:	icu::UnicodeString( str )
	{
	}

	UnicodeString( char ch )
		:	icu::UnicodeString( (char16_t) ch )
	{
	}

	UnicodeString( const char16_t * str )
		:	icu::UnicodeString( str )
	{
	}

	UnicodeString( const UnicodeChar & ch )
		:	icu::UnicodeString( 1, (UChar32) ch, 1 )
	{
	}

	UnicodeString( const char * str )
		:	icu::UnicodeString( icu::UnicodeString::fromUTF8( str ) )
	{
	}

	UnicodeString( const std::string & str )
		:	icu::UnicodeString( icu::UnicodeString::fromUTF8( str ) )
	{
	}

	UnicodeString( long long int count, char ch )
		:	icu::UnicodeString( (int32_t) count, (UChar32) ch, (int32_t) count )
	{
	}

	~UnicodeString() override = default;

	UnicodeChar operator [] ( long long int position ) const
	{
		return UnicodeChar( char32At( (int32_t) position ) );
	}

	void push_back( const UnicodeChar & ch )
	{
		icu::UnicodeString::append( (UChar32) ch );
	}

	void push_back( const UnicodeString & str )
	{
		icu::UnicodeString::append( str );
	}

	int32_t size() const
	{
		return length();
	}

	int toInt( bool * ok = nullptr, int base = 10 ) const
	{
		try {
			std::string tmp;
			toUTF8String( tmp );
			const auto result = std::stoi( tmp, nullptr, base );
			if( ok ) *ok = true;
			return result;
		}
		catch( const std::invalid_argument & )
		{
			if( ok ) *ok = false;
		}
		catch( const std::out_of_range & )
		{
			if( ok ) *ok = false;
		}

		return 0;
	}

	bool contains( const UnicodeChar & ch ) const
	{
		return ( icu::UnicodeString::indexOf( (UChar32) ch ) != -1 );
	}

	bool contains( const UnicodeString & str ) const
	{
		return ( icu::UnicodeString::indexOf( str ) != -1 );
	}

	UnicodeString simplified() const
	{
		if( isEmpty() )
			return *this;

		UnicodeString result;
		int32_t i = 0;

		while( true )
		{
			while( i < length() && UnicodeChar( char32At( i ) ).isSpace() )
					++i;

			while( i != length() && !UnicodeChar( char32At( i ) ).isSpace() )
			{
				result.append( UnicodeChar( char32At( i ) ) );
				++i;
			}

			if( i == length() )
				break;

			result.append( UnicodeChar( ' ' ) );
		}

		if( !result.isEmpty() && result[ result.size() - 1 ] == UnicodeChar( ' ' ) )
			result.remove( result.size() - 1, 1 );

		return result;
	}

	std::vector< UnicodeString > split( const UnicodeChar & ch ) const
	{
		std::vector< UnicodeString > result;

		int32_t pos = 0;

		while( pos < length() )
		{
			const auto fpos = indexOf( (UChar32) ch, pos );

			if( fpos != -1 )
			{
				if( fpos - pos > 0 )
				{
					icu::UnicodeString tmp;
					extract( pos, fpos - pos, tmp );
					result.push_back( tmp );
				}

				pos = fpos + 1;
			}
			else if( pos < length() )
			{
				icu::UnicodeString tmp;
				extract( pos, length() - pos, tmp );
				result.push_back( tmp );

				break;
			}
		}

		return result;
	}

	std::vector< UnicodeString > split( char ch ) const
	{
		return split( UnicodeChar( ch ) );
	}

	UnicodeString & replace( const UnicodeChar & before, const UnicodeString & after )
	{
		for( int32_t pos = 0; ( pos = indexOf( before, pos ) ) != -1; pos += after.size() )
			icu::UnicodeString::replace( pos, 1, after );

		return *this;
	}

	UnicodeString & replace( const UnicodeString & before, const UnicodeString & after )
	{
		for( int32_t pos = 0; ( pos = indexOf( before, pos ) ) != -1; pos += after.size() )
			icu::UnicodeString::replace( pos, before.length(), after );

		return *this;
	}

	UnicodeString sliced( long long int pos, long long int len = -1 ) const
	{
		icu::UnicodeString tmp;
		extract( (int32_t) pos, (int32_t) ( len == -1 ? length() - pos : len ), tmp );

		return tmp;
	}

	UnicodeString right( long long int n ) const
	{
		icu::UnicodeString tmp;
		extract( length() - (int32_t) n, (int32_t) n, tmp );

		return tmp;
	}

	UnicodeString toCaseFolded() const
	{
		icu::UnicodeString tmp = *this;
		tmp.foldCase();

		return tmp;
	}

	UnicodeString toUpper() const
	{
		icu::UnicodeString tmp = *this;
		tmp.toUpper();

		return tmp;
	}

	UnicodeString toLower() const
	{
		icu::UnicodeString tmp = *this;
		tmp.toLower();

		return tmp;
	}

	void clear()
	{
		icu::UnicodeString::remove();
	}
}; // class StdString


//
// UrlUri
//

class UrlUri {
public:
	explicit UrlUri( const UnicodeString & uriStr )
		:	m_valid( false )
		,	m_relative( false )
	{
		UriUriA uri;
		std::string uriString;
		uriStr.toUTF8String( uriString );

		if( uriParseSingleUriA( &uri, uriString.c_str(), NULL ) == URI_SUCCESS )
		{
			m_valid = true;
			m_relative = !( uri.scheme.first && uri.scheme.afterLast );

			uriFreeUriMembersA( &uri );
		}
	}

	~UrlUri()
	{
	}

	bool isValid() const
	{
		return m_valid;
	}

	bool isRelative() const
	{
		return m_relative;
	}

private:
	bool m_valid;
	bool m_relative;
}; // class UrlUri


//
// UnicodeStringTrait
//

//! Trait to use this library with std::string.
struct UnicodeStringTrait {
	template< class T >
	using Vector = std::vector< T >;

	template< class T, class U >
	using Map = std::map< T, U >;

	using String = UnicodeString;

	using InternalString = InternalStringT< String >;

	using Char = UnicodeChar;

	using TextStream = std::istream;

	using StringList = std::vector< String >;

	using Url = UrlUri;

	//! Convert UTF-16 into trait's string.
	static String utf16ToString( const char16_t * u16 )
	{
		return UnicodeString( u16 );
	}

	//! \return Does file exist.
	static bool fileExists( const String & fileName, const String & workingPath )
	{
		std::string path;
		( workingPath + fileName ).toUTF8String( path );

		std::error_code er;

		const auto result = std::filesystem::exists( path, er );

		return ( er ? false : result );
	}

	//! \return Absolute file path.
	static String absoluteFilePath( const String & path )
	{
		std::string tmp;
		path.toUTF8String( tmp );
		std::error_code er;
		auto p = std::filesystem::canonical( tmp, er ).u8string();

		std::replace( p.begin(), p.end(), '\\', '/' );

		return ( er ? "" : UnicodeString::fromUTF8( p ) );
	}
}; // struct UnicodeStringTrait

#endif // MD4QT_ICU_STL_SUPPORT

#ifdef MD4QT_QT_SUPPORT

//
// QStringTrait
//

//! Trait to use this library with QString.
struct QStringTrait {
	template< class T >
	using Vector = QVector< T >;

	template< class T, class U >
	using Map = std::map< T, U >;

	using String = QString;

	using InternalString = InternalStringT< String >;

	using Char = QChar;

	using TextStream = QTextStream;

	using StringList = QStringList;

	using Url = QUrl;

	//! Convert UTF-16 into trait's string.
	static String utf16ToString( const char16_t * u16 )
	{
		return QString::fromUtf16( u16 );
	}

	//! \return Does file exist.
	static bool fileExists( const String & fileName, const String & workingPath )
	{
		return QFileInfo::exists( workingPath + fileName );
	}

	//! \return Absolute file path.
	static String absoluteFilePath( const String & path )
	{
		return QFileInfo( path ).absoluteFilePath();
	}
}; // struct QStringTrait

#endif // MD4QT_QT_SUPPORT

} /* namespace MD */

#endif // MD4QT_MD_TRAITS_HPP_INCLUDED
